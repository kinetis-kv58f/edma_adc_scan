/// This file contains routines for transmitting USB packets.

#ifndef SRC_DRIV_USBH_TRANSMISSION_H_
#define SRC_DRIV_USBH_TRANSMISSION_H_

#include "./usbh_pstructs.h"

/// 16-bit frame number read from USB module
static inline uint16_t priv_get_frameNumber(void) {
	return uint16_make_from_bytes(hw_usb0->FRMNUMH, hw_usb0->FRMNUML);
}

/// Long 32-bit frame number
static inline uint32_t priv_get_longFrameNumber(void) {
	uint16_t const frameNumber = priv_get_frameNumber();
	if (frameNumber < me.oldFrameNumber) {
		me.longFrameNumber += 2048U;
	}
	me.oldFrameNumber = frameNumber;
	return me.longFrameNumber + frameNumber;
}

/// Utility function to find token type of current transfer
static usbh_transactionType_t priv_find_transfer_tokenType(void) {
	usbh_transfer_t& trans = &me.transfers[me.curTransfer];
	usbh_pipe_t & pipe = &me.pipes[trans.pipeNum];

	usbh_transactionType_t type = Usbh_transactionType_unknown;

	if (pipe.pipeType == Usbh_endpointType_control) {
		switch (trans.setupStatus) {
		case Usbh_setupStatus_0:
			type = Usbh_transactionType_ctrl;
			break;
		case Usbh_setupStatus_1:
			if (trans.transferLength) {
				if (trans.dir == Usbh_pipeDir_in) {
					type = Usbh_transactionType_in;
				} else {
					type = Usbh_transactionType_out;
				}
			} else {
				type = Usbh_transactionType_in;
			}
			break;
		case Usbh_setupStatus_2:
			if (trans.transferLength) {
				if (trans.dir == Usbh_pipeDir_in) {
					type = Usbh_transactionType_out;
				} else {
					type = Usbh_transactionType_in;
				}
			} else {
				type = Usbh_transactionType_in;
			}
			break;
		case Usbh_setupStatus_3:
			type = Usbh_transactionType_in;
			break;
		default:
			break;
		}
	} else {
		if (pipe.dir == Usbh_pipeDir_in) {
			type = Usbh_transactionType_in;
		} else {
			type = Usbh_transactionType_out;
		}
	}

	return type;
}

/// \note OriginalDriver: _USB_HostKhciAtomNonblockingTransaction
static int32_t priv_send_packet(uint8_t * const buf, uint32_t len) {
	usbh_transfer_t& trans = &me.transfers[me.curTransfer];
	usbh_pipe_t & pipe = &me.pipes[trans.pipeNum];

	if (len > pipe.maxPacketSize) {
		len = pipe.maxPacketSize;
	}

	// Set address
	if (me.dev.speed == 0) {
		hw_usb0->ADDR = USB_ADDR_LSEN_Mask | me.dev.setAddress;
	} else {
		hw_usb0->ADDR = me.dev.setAddress;
	}

	hw_usb0->ENDPOINT[0].ENDPT = USB_ENDPT_RETRYDIS_Mask | USB_ENDPT_EPTXEN_Mask
			| USB_ENDPT_EPRXEN_Mask;
	if (pipe.pipeType != Usbh_endpointType_isochronous) {
		hw_usb0->ENDPOINT[0].ENDPT |= USB_ENDPT_EPHSHK_Mask;
	}
	if (me.dev.level == 1) {		// TODO greater than 1?
		hw_usb0->ENDPOINT[0].ENDPT |= USB_ENDPT_HOSTWOHUB_Mask;
	}

	uint32_t counter = 0U;
	// Wait until USB controller is ready
	while (hw_usb0->CTL & USB_CTL_TXSUSPENDTOKENBUSY_Mask) {
		// TODO delay _USB_HostKhciDelay(usbHostPointer, 1U);

		// TODO break if tok done?

		counter++;
		if (counter >= 3) {
			return Usbh_packetTranStatus_crcError;
		}
	}

	hw_usb0->ERRSTAT = 0xffU;

	/* TODO DMA shit
	 if ((type == Usbh_transactionType_in) && ((len & USB_MEM4_ALIGN_MASK) || ((uint32_t)bufPointer & USB_MEM4_ALIGN_MASK)))
	 {
	 if ((usbHostPointer->khciSwapBufPointer != NULL)
	 && (len <= USB_HOST_CONFIG_KHCI_DMA_ALIGN_BUFFER)) {
	 buf = (uint8_t *)USB_MEM4_ALIGN((uint32_t)(usbHostPointer->khciSwapBufPointer + 4));
	 usbHostPointer->sXferSts.rxBuf = buf;
	 usbHostPointer->sXferSts.rxBufOrig = bufPointer;
	 usbHostPointer->sXferSts.rxLen = len;
	 usbHostPointer->sXferSts.isDmaAlign = 0U;
	 }
	 }
	 else {
	 usbHostPointer->sXferSts.isDmaAlign = 1U;
	 }
	 */

	// Find type of transaction
	usbh_transactionType_t const type = priv_find_transfer_tokenType();
	switch (type) {
	case Usbh_transactionType_ctrl: {
		uint16_t const bdtIndex = priv_compute_bdtIndex(1, me.txBD);
		bdt[bdtIndex].bufAddr = buf;
		// Byte count
		bdt[bdtIndex].config = len << USBH_BDT_BC_Shift;
		// Give ownership of the buffer to USB-FS
		bdt[bdtIndex].config |= USBH_BDT_OWN_Mask;
		hw_usb0->TOKEN = (Usbh_tokenPid_setup << USB_TOKEN_PID_Shift)
				| pipe.endpointAddr;
		me.txBD ^= 1;
	}
		break;
	case Usbh_transactionType_in: {
		uint16_t const bdtIndex = priv_compute_bdtIndex(0, me.rxBD);
		bdt[bdtIndex].bufAddr = buf;
		// Byte count
		bdt[bdtIndex].config = len << USBH_BDT_BC_Shift;
		bdt[bdtIndex].config |= pipe.nextdata01 << USBH_BDT_DATAX_Shift;
		// Give ownership of the buffer to USB-FS
		bdt[bdtIndex].config |= USBH_BDT_OWN_Mask;
		hw_usb0->TOKEN = (Usbh_tokenPid_in << USB_TOKEN_PID_Shift)
				| pipe.endpointAddr;
		me.rxBD ^= 1;
	}
		break;
	case Usbh_transactionType_out: {
		uint16_t const bdtIndex = priv_compute_bdtIndex(1, me.txBD);
		bdt[bdtIndex].bufAddr = buf;
		// Byte count
		bdt[bdtIndex].config = len << USBH_BDT_BC_Shift;
		bdt[bdtIndex].config |= pipe.nextdata01 << USBH_BDT_DATAX_Shift;
		// Give ownership of the buffer to USB-FS
		bdt[bdtIndex].config |= USBH_BDT_OWN_Mask;
		hw_usb0->TOKEN = (Usbh_tokenPid_out << USB_TOKEN_PID_Shift)
				| pipe.endpointAddr;
		me.txBD ^= 1;
	}
		break;

	default:
		break;
	}
	return 0;
}

/// \note OriginalDriver: _USB_HostKhciStartTranfer
static usbh_transferState_t priv_commit_transfer(void) {
	usbh_transfer_t& trans = me.transfers[me.curTransfer];
	usbh_pipe_t & pipe = me.pipes[trans.pipeNum];

	int32_t transferResult;

	if (pipe.pipeType == Usbh_endpointType_control) {
		switch (trans.setupStatus) {
		case Usbh_setupStatus_0:
			transferResult = priv_send_packet((uint8_t *) &trans.setupPacket,
					8);
			break;
		case Usbh_setupStatus_1:
			if (trans.transferLength != 0) {
				uint8_t * const buf = &trans.transferBuffer[trans.transferSofar];
				uint32_t const bytesLeft = trans.transferLength
						- trans.transferSofar;
				transferResult = priv_send_packet(buf, bytesLeft);
			} else {
				pipe.nextdata01 = 1;
				trans.setupStatus = Usbh_setupStatus_3;
				transferResult = priv_send_packet(&trans.transferBuffer[0], 0);
			}
			break;
		case Usbh_setupStatus_2:
			if (trans.transferLength != 0) {
				pipe.nextdata01 = 1;
				transferResult = priv_send_packet(&trans.transferBuffer[0], 0);
			} else {
				pipe.nextdata01 = 1;
				transferResult = priv_send_packet(&trans.transferBuffer[0], 0);
			}
			break;
		case Usbh_setupStatus_3: {
			pipe.nextdata01 = 1;
			transferResult = priv_send_packet(&trans.transferBuffer[0], 0);
		}
			break;
		default:
			break;
		}
	} else {
		uint8_t * const buf = &trans.transferBuffer[trans.transferSofar];
		uint32_t const bytesLeft = trans.transferLength - trans.transferSofar;
		transferResult = priv_send_packet(buf, bytesLeft);
	}

	trans.union1.transferResult = transferResult;

	// Update transfer state
	if (trans.union1.transferResult == 0U) {
		me.transferState = Usbh_transferState_transmiting;
	} else {
		me.transferState = Usbh_transferState_transmitDone;
	}
	return me.transferState;
}

/// Sends the given transfer as setup packet
static bool_t priv_send_setupTransfer(uint16_t const transferNum) {
	usbh_transfer_t& trans = me.transfers[transferNum];

	// Initialize transfer structure
	trans.transferSofar = 0;
	trans.setupStatus = 0;

	// Setup direction
	if ((trans.setupPacket.bmRequestType & Usbh_reqType_dir_Mask)
			== Usbh_reqType_dir_in) {
		trans.dir = Usbh_pipeDir_in;
	} else {
		trans.dir = Usbh_pipeDir_out;
	}

	return priv_write_pipe(transferNum);
}

#endif /* SRC_DRIV_USBH_TRANSMISSION_H_ */
