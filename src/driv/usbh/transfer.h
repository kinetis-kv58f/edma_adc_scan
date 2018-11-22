/*
 * transfer.h
 *
 *  Created on: 11 nov. 2017
 *      Author: SERAGUD
 */

#ifndef SRC_DRIV_USBH_TRANSFER_H_
#define SRC_DRIV_USBH_TRANSFER_H_

#include "./usbh_pstructs.h"
#include "./transmission.h"

/// Schedules next transfer. Must be called after a transfer completes
/// and the bus is free.
/// TODO Needs name change?
/// \note OriginalCode: USB_KHCI_EVENT_MSG event
static void priv_schedule_nextTransfer(void) {
	// TODO USB_OsaEventSet(usbHostPointer->khciEventPointer, USB_KHCI_EVENT_MSG);

	// TODO
}

/// \note OriginalCode: _USB_HostKhciTransferStateMachine
static void priv_handle_transferStateMachine(void) {
	switch (me.transferState) {
	case Usbh_transferState_getMsg: {
		// Find next transfer to send from list of transfers

		uint16_t transNum = priv_find_sendableInterruptTransfer();

		if (transNum == 0xFFFF) {
			transNum = priv_find_sendableTransfer();
		}

		if (transNum != 0xFFFF) {
			me.curTransfer = transNum;
			me.transferState = priv_commit_transfer();
		}
	}
		break;

	case Usbh_transferState_getIsoMsg: {
		me.curTransfer = priv_find_sendableIsochronousTransfer();
		if (me.curTransfer != 0xFFFF) {
			// Start the current transfer
			me.transferState = priv_commit_transfer();
		} else {
			me.transferState = Usbh_transferState_getMsg;
		}
	}
		break;

	case Usbh_transferState_startTransmit:
		if (me.curTransfer != 0xFFFF) {
			// Start the current transfer
			me.transferState = priv_commit_transfer();
		}
		break;

	case Usbh_transferState_transmiting: {
		// Handle timeout of a transfer
		// Check if current transfer is taking too long. If the timeout is exceeded cancel the transfer
		if (me.curTransfer != 0xFFFF) {
			usbh_transfer_t& trans = me.transfers[me.curTransfer];
			if ((priv_get_longFrameNumber() - trans.union2.frame)
					> Usbh_config_timeoutOther) {
				usbh_pipe_t& pipe = me.pipes[trans.pipeNum];
				if ((pipe.pipeType == Usbh_endpointType_control)
						|| (pipe.pipeType == Usbh_endpointType_bulk)) {
					// TODO clear current bdt status
					// TODO _USB_HostKhciTransactionDone(usbHostPointer, transfer);
					me.transferState = Usbh_transferState_transmitDone;
					trans.union1.transferResult = Usbh_packetTranStatus_timeout;
				}
			}
		}
	}
		break;

	case Usbh_transferState_transmitDone: {
		// TODO Set transfer as done. Clean up transfer data structure

		// TODO Call appropriate process callbacks

		me.transferState = Usbh_transferState_getMsg;
		priv_schedule_nextTransfer();
	}
		break;

	default:
		break;
	}
}

/// Time to send a SOF token. If we have an isochronous transfer, we must send it.
///
/// If there is another transfer ongoing, it must be paused and isochronous transfer
/// must be sent first!
static void priv_handle_sofTok(void) {
	if (me.transferState != Usbh_transferState_getMsg) {
		// If a transfer is already being sent, bail.
		return;
	}

	me.transferState = Usbh_transferState_getIsoMsg;
}

/// Finds the status of the completed transaction
///
/// \retval Positive Number of bytes transmitted/received
/// \retval Negative Error status of last transaction
static int32_t priv_find_doneTokenStatus(void) {
	usbh_pipe_t pipe;	// TODO get current transfer pipe
	usbh_transfer_t trans;	// TODO get current transfer

	usbh_transactionType_t type = priv_find_transfer_tokenType();

	uint16_t bdtIndex = -1;

	// Find the BD being used
	switch (type) {
	case Usbh_transactionType_ctrl:
	case Usbh_transactionType_out: {
		uint8_t const oddness = (me.txBD ^ 1) & 1;
		bdtIndex = priv_compute_bdtIndex(1, oddness);
	}
		break;
	case Usbh_transactionType_in: {
		uint8_t const oddness = (me.rxBD ^ 1) & 1;
		bdtIndex = priv_compute_bdtIndex(0, oddness);
	}
		break;
	default:
		break;
	}

	if (bdtIndex < 0) {
		return -1;
	}

	// TODO if there are USB errors, then bail
	// TODO check if the current BD is owned
	// TODO compute transfer result
	// TODO DMA thing
	// TODO
}

/// Must be called after a transaction has been sent.
///
/// If there are still some bytes to be sent, transfer state is set to new transaction.
/// If all transactions in a transfer are sent, the bus is freed for other transfers.
static int32_t priv_handle_transactionDone(void) {
	// TODO check that me.curTransfer is valid
	usbh_transfer_t& trans = me.transfers[me.curTransfer];
	usbh_pipe_t& pipe = me.pipes[trans.pipeNum];

	int32_t const transferResult = priv_find_doneTokenStatus();
	trans.union1.transferResult = transferResult;

	// Compute the next transmit state

	if (transferResult >= 0) {
		if (pipe.pipeType == Usbh_endpointType_control) {
			switch (trans.setupStatus) {
			case Usbh_setupStatus_2:
			case Usbh_setupStatus_3:
				me.transferState = Usbh_transferState_transmitDone;
				break;
			case Usbh_setupStatus_1:
				me.transferState = Usbh_transferState_startTransmit;
				trans.transferSofar += transferResult;
				if (((trans.transferLength - trans.transferSofar) <= 0U)
						|| (transferResult < pipe.maxPacketSize)) {
					trans.setupStatus++;
				}
				break;
			case Usbh_setupStatus_0:
				me.transferState = Usbh_transferState_startTransmit;
				trans.setupStatus++;	// Next state
				break;
			case Usbh_setupStatus_idle:
				// TODO
				break;
			default:
				break;
			}
		} else {
			trans.transferSofar += transferResult;
			if (((trans.transferLength - trans.transferSofar) <= 0U)
					|| (transferResult < pipe.maxPacketSize)) {
				me.transferState = Usbh_transferState_transmitDone;
			} else {
				me.transferState = Usbh_transferState_startTransmit;
			}
		}
	} else {
		if ((transferResult == Usbh_packetTranStatus_nak)) {
			if (pipe.pipeType == Usbh_endpointType_interrupt) {
				// Ignore error
				me.transferState = Usbh_transferState_getMsg;
				priv_schedule_nextTransfer();
			} else {
				if ((priv_get_longFrameNumber() - trans.union2.frame)
						> trans.nakTimeout) {
					me.transferState = Usbh_transferState_transmitDone;
					trans.union1.transferResult = Usbh_packetTranStatus_timeout;
				} else {
					me.transferState = Usbh_transferState_getMsg;
					priv_schedule_nextTransfer();
				}
			}
		} else {
			me.transferState = Usbh_transferState_transmitDone;
		}
	}

	return me.transferState;
}

#endif /* SRC_DRIV_USBH_TRANSFER_H_ */
