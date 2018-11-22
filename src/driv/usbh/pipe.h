/*
 * pipe.h
 *
 *  Created on: 11 nov. 2017
 *      Author: SERAGUD
 */

#ifndef SRC_DRIV_USBH_PIPE_H_
#define SRC_DRIV_USBH_PIPE_H_

#include "./usbh_pstructs.h"

/// Initializes and opens a USB host pipe
static void priv_open_pipe(uint16_t const pipeNum) {
	usbh_pipe_t& pipe = me.pipes[pipeNum];

	pipe.open = TRUE;
	pipe.nextdata01 = FALSE;
	// TODO should these be here?
	pipe.currentCount = 0xFFFFU;
	pipe.numberPerUframe = 0;
	pipe.nakCount = Usbh_config_maxNak;

	// TODO set this based on type
	if (pipe.pipeType == Usbh_endpointType_isochronous) {
		pipe.interval = 0;	// TODO set correct value
	} else {
		pipe.interval = 0;	// TODO set correct value
	}
}

static bool_t priv_close_pipe(uint16_t const pipeNum) {
	usbh_pipe_t& pipe = me.pipes[pipeNum];

	if (pipe.open == 0) {
		return FALSE;
	}

	pipe.open = FALSE;
	// TODO release all transfers?

	return TRUE;;
}

static void priv_read_pipe(uint16_t const transferNum) {
	usbh_transfer_t& trans = &me.transfers[transferNum];
	usbh_pipe_t& pipe = me.pipes[trans.pipeNum];

	/* TODO
	 if ((transfer->transferLength & USB_MEM4_ALIGN_MASK) || ((uint32_t)transfer->transferBuffer & USB_MEM4_ALIGN_MASK))
	 {
	 if (usbHostPointer->khciSwapBufPointer == NULL)
	 {
	 return kStatus_USB_LackSwapBuffer;
	 }
	 if (pipePointer->maxPacketSize > USB_HOST_CONFIG_KHCI_DMA_ALIGN_BUFFER)
	 {
	 return kStatus_USB_LackSwapBuffer;
	 }
	 }
	 */

	trans.transferSofar = 0U;
	if (pipe.nakCount == 0U) {
		trans.nakTimeout = Usbh_config_timeoutDefault;
	} else {
		trans.nakTimeout = pipe.nakCount * Usbh_config_maxNakRetries;
	}
	trans.retry = Usbh_maxTransferRetries;
	trans.union2.frame = priv_get_longFrameNumber();

	// TODO _USB_HostKhciLinkTrRequestToList(controllerHandle, transfer);

	// TODO USB_OsaEventSet(usbHostPointer->khciEventPointer, USB_KHCI_EVENT_MSG);

	return status;
}

/// Write transfer with transfer id [transferNum] through specified pipe
static void priv_write_pipe(uint16_t const transferNum) {
	usbh_transfer_t& trans = &me.transfers[transferNum];
	usbh_pipe_t& pipe = me.pipes[trans.pipeNum];

	trans.retry = Usbh_maxTransferRetries;

	if (pipe.endpointAddr == 0) {
		/* TODO
		 if ((trans.dir == USB_IN) && (transfer->transferBuffer != NULL) &&
		 ((transfer->transferLength & USB_MEM4_ALIGN_MASK) ||
		 ((uint32_t)transfer->transferBuffer & USB_MEM4_ALIGN_MASK)))
		 {
		 if (usbHostPointer->khciSwapBufPointer == NULL)
		 {
		 return kStatus_USB_LackSwapBuffer;
		 }
		 if (pipePointer->maxPacketSize > USB_HOST_CONFIG_KHCI_DMA_ALIGN_BUFFER)
		 {
		 return kStatus_USB_LackSwapBuffer;
		 }
		 }
		 */
		trans.setupStatus = Usbh_setupStatus_0;

		if (trans.transferLength) {
			if (trans.dir == Usbh_pipeDir_in) {
				trans.nakTimeout = Usbh_config_timeoutToHost;
			} else {
				trans.nakTimeout = Usbh_config_timeoutToDevice;
			}
		} else {
			trans.nakTimeout = Usbh_config_timeoutNoData;
		}
	} else {
		if (pipe.nakCount == 0U) {
			trans.nakTimeout = Usbh_config_timeoutDefault;
		} else {
			trans.nakTimeout = pipe.nakCount * Usbh_config_maxNakRetries;
		}
	}

	trans.union2.frame = priv_get_longFrameNumber();

	// TODO _USB_HostKhciLinkTrRequestToList(controllerHandle, transfer);

	// TODO USB_OsaEventSet(usbHostPointer->khciEventPointer, USB_KHCI_EVENT_MSG);

	return status;
}

/// Returns the number of a free transfer record
static inline uint16_t priv_get_freeTransfer(uint16_t const pipeNum) {
	if (pipeNum == 0xFFFF) {
		return 0xFFFF;
	}
	for (uint16_t i = 0; i < Usbh_numTransfersInModule; i++) {
		if (me.transfers[i].pipeNum == 0xFFFF) {
			me.transfers[i] = pipeNum;
			return i;
		}
	}
	return 0xFFFF;
}

/// Returns the number of a send-able transfer record
static inline uint16_t priv_find_sendableTransfer(void) {
	for (uint16_t i = 0; i < Usbh_numTransfersInModule; i++) {
		usbh_transfer_t& trans = &me.transfers[i];

		if (trans.pipeNum == 0xFFFF) {
			continue;
		}

		usbh_pipe_t& pipe = &me.pipes[trans.pipeNum];

		if (pipe.pipeType != Usbh_endpointType_interrupt) {
			continue;
		}

		return i;
	}
	return 0xFFFF;
}

/// Returns the number of a send-able interrupt transfer record
static inline uint16_t priv_find_sendableInterruptTransfer(void) {
	uint32_t const curFrameNum = priv_get_longFrameNumber();
	uint16_t selTrans = 0xFFFF;

	for (uint16_t i = 0; i < Usbh_numTransfersInModule; i++) {
		usbh_transfer_t& trans = &me.transfers[i];

		if (trans.pipeNum == 0xFFFF) {
			continue;
		}

		usbh_pipe_t& pipe = &me.pipes[trans.pipeNum];

		if (pipe.pipeType != Usbh_endpointType_interrupt) {
			continue;
		}

		if (pipe.currentCount == curFrameNum) {
			continue;
		}

		// TODO Can pipe.interval be 0?
		if ((curFrameNum - pipe.currentCount) > pipe.interval) {
			continue;
		}

		selTrans = i;
	}

	if (selTrans == 0xFFFF) {
		return 0xFFFF;
	}

	usbh_transfer_t& trans = &me.transfers[selTrans];
	usbh_pipe_t& pipe = &me.pipes[trans.pipeNum];
	pipe.currentCount = curFrameNum;
	priv_schedule_nextTransfer();
	return selTrans;
}

/// Returns the number of a send-able Isochronous transfer record
static inline uint16_t priv_find_sendableIsochronousTransfer(void) {
	uint32_t const curFrameNum = priv_get_longFrameNumber();
	for (uint16_t i = 0; i < Usbh_numTransfersInModule; i++) {
		usbh_transfer_t& trans = &me.transfers[i];

		if (trans.pipeNum == 0xFFFF) {
			continue;
		}

		usbh_pipe_t& pipe = &me.pipes[trans.pipeNum];

		if (pipe.pipeType != Usbh_endpointType_isochronous) {
			continue;
		}

		// TODO Can pipe.interval be 0?
		if ((curFrameNum - pipe.currentCount) > pipe.interval) {
			continue;
		}

		usbh_transfer_t& trans = &me.transfers[i];
		usbh_pipe_t& pipe = &me.pipes[trans.pipeNum];
		pipe.currentCount = curFrameNum;
		return i;
	}
	return 0xFFFF;
}

#endif /* SRC_DRIV_USBH_PIPE_H_ */
