/// Contains logic for USB enumeration

#ifndef SRC_DRIV_USBH_ENUMERATION_H_
#define SRC_DRIV_USBH_ENUMERATION_H_

#include "./usbh_pstructs.h"
#include "transmission.h"

/// Handles enumeration state
///
/// \note OriginalCode: USB_HostProcessState
static bool_t priv_handle_enumeration_stateMachine(void) {
	switch (me.dev.state) {
	case Usbh_enumState_getDes8:
	case Usbh_enumState_getDes: {
		// Issue get descriptor request

		// Build the transfer
		uint16_t const transferNum = priv_get_freeTransfer(
				me.dev.controlPipeNum);	// TODO check for no free transfer available
		usbh_transfer_t &trans = me.transfers[transferNum];
		if (me.dev.state == Usbh_enumState_getDes) {
			trans.transferLength = sizeof(usbh_device_descriptor_t);
		} else {
			trans.transferLength = 8;
		}

		// Get request of standard type with device as recipient
		trans.setupPacket.bmRequestType = Usbh_reqType_dir_in
				| Usbh_reqType_type_standard | Usbh_reqType_recipient_device;
		trans.setupPacket.bRequest = Usbh_standardReq_getDescriptor;
		// wValue = DescriptorType | index
		trans.setupPacket.wValue = uint16_make_from_bytes(Usbh_descType_device,
				0);
		trans.setupPacket.wIndex = 0;
		trans.setupPacket.wLength = trans.transferLength;

		// Send the transfer
		return priv_send_setupTransfer(transferNum);
	}
		break;
	case Usbh_enumState_setAddress: {
		// Issue set address request

		// Build the transfer
		uint16_t const transferNum = priv_get_freeTransfer(
				me.dev.controlPipeNum);	// TODO check for no free transfer available
		usbh_transfer_t &trans = me.transfers[transferNum];
		trans.transferLength = 0;

		// Set request of standard type with device as recipient
		trans.setupPacket.bmRequestType = Usbh_reqType_dir_out
				| Usbh_reqType_type_standard | Usbh_reqType_recipient_device;
		trans.setupPacket.bRequest = Usbh_standardReq_setAddress;
		trans.setupPacket.wValue = me.dev.allocatedAddress;
		trans.setupPacket.wIndex = 0;
		trans.setupPacket.wLength = 0;

		// Send the transfer
		return priv_send_setupTransfer(transferNum);
	}
		break;
	case Usbh_enumState_getCfg9: {
		// Issue get configuration request

		// Build the transfer
		uint16_t const transferNum = priv_get_freeTransfer(
				me.dev.controlPipeNum);	// TODO check for no free transfer available
		usbh_transfer_t &trans = me.transfers[transferNum];
		trans.transferLength = 9;

		// Get request of standard type with device as recipient
		trans.setupPacket.bmRequestType = Usbh_reqType_dir_in
				| Usbh_reqType_type_standard | Usbh_reqType_recipient_device;
		trans.setupPacket.bRequest = Usbh_standardReq_getDescriptor;
		// wValue = DescriptorType | ConfigurationSelected
		trans.setupPacket.wValue = uint16_make_from_bytes(
				Usbh_descType_configuration, me.dev.configurationValue);
		trans.setupPacket.wIndex = 0;
		trans.setupPacket.wLength = trans.transferLength;

		// Send the transfer
		return priv_send_setupTransfer(transferNum);
	}
		break;
	case Usbh_enumState_getCfg: {
		// Issue get configuration request

		// Build the transfer
		uint16_t const transferNum = priv_get_freeTransfer(
				me.dev.controlPipeNum);	// TODO check for no free transfer available
		usbh_transfer_t &trans = me.transfers[transferNum];
		trans.transferLength = me.dev.configurationLen;

		// Get request of standard type with device as recipient
		trans.setupPacket.bmRequestType = Usbh_reqType_dir_in
				| Usbh_reqType_type_standard | Usbh_reqType_recipient_device;
		trans.setupPacket.bRequest = Usbh_standardReq_getDescriptor;
		// wValue = DescriptorType | ConfigurationSelected
		trans.setupPacket.wValue = uint16_make_from_bytes(
				Usbh_descType_configuration, me.dev.configurationValue);
		trans.setupPacket.wIndex = 0;
		trans.setupPacket.wLength = trans.transferLength;

		// Send the transfer
		return priv_send_setupTransfer(transferNum);
	}
		break;
	case Usbh_enumState_setCfg: {
		// Issue set configuration request

		// Build the transfer
		uint16_t const transferNum = priv_get_freeTransfer(
				me.dev.controlPipeNum);	// TODO check for no free transfer available
		usbh_transfer_t &trans = me.transfers[transferNum];
		trans.transferLength = 0;

		// Set request of standard type with device as recipient
		trans.setupPacket.bmRequestType = Usbh_reqType_dir_out
				| Usbh_reqType_type_standard | Usbh_reqType_recipient_device;
		trans.setupPacket.bRequest = Usbh_standardReq_setConfiguration;
		trans.setupPacket.wValue =
				me.dev.configuration.configurationDesc.bConfigurationValue;
		trans.setupPacket.wIndex = 0;
		trans.setupPacket.wLength = 0;

		// Send the transfer
		return priv_send_setupTransfer(transferNum);
	}
		break;
	case Usbh_enumState_enumDone:
		// Notify that enumeration is done
		usbh_event_enumerationDone();
		me.dev.state = Usbh_enumState_appUsed;
		break;
	default:
		break;
	}
	return FALSE;
}

/// \param[in] status Status of the current transfer
/// \note OriginalCode: nextStep
static void priv_handle_enumeration_callback(
		usbh_transferStatus_t const status) {
	// TODO free existing transfer

	uint8_t nextStep = 0;

	if (status == Usbh_transferStatus_Success) {
		nextStep = 1;
	} else if (status == Usbh_transferStatus_TransferStall) {
		if (me.dev.stallRetries > 0) {
			// retry same transfer when stall
			me.dev.stallRetries--;
		} else {
			// process next state when all retries stall
			nextStep = 1;
		}
	} else if (status == Usbh_transferStatus_TransferCancel) {
		return;
	} else {
		if (me.dev.enumRetries > 0) {
			// Restart enumeration again
			me.dev.enumRetries--;
			me.dev.stallRetries = Usbh_config_maxStallRetries;
			me.dev.configurationValue = 0;
			me.dev.state = Usbh_enumState_getDes8;
		} else {
			// Bail
			return;
		}
	}

	// TODO Check if everything is alright
	if (nextStep == 1) {
		me.dev.stallRetries = Usbh_config_maxStallRetries;

		// TODO fix return type
		bool_t ok = priv_handle_enumeration_stateMachine();

		if (ok == Usbh_transferStatus_Success) {
			me.dev.state++;
		} else if (ok == Usbh_transferStatus_Retry) {
			// TODO update me.dev.state to retry state
		} else if (ok == Usbh_transferStatus_NotSupported) {
			// Bail
			return;
		} else {
			// process error, next retry
			if (me.dev.enumRetries > 0) {
				// Restart enumeration again
				me.dev.enumRetries--;
				me.dev.stallRetries = Usbh_config_maxStallRetries;
				me.dev.configurationValue = 0;
				me.dev.state = Usbh_enumState_getDes8;
			} else {
				// Bail
				return;
			}
		}
	}

	// process the new state
	if (priv_handle_enumeration_stateMachine() != Usbh_transferStatus_Success) {
		return;
	}
}

#endif /* SRC_DRIV_USBH_ENUMERATION_H_ */
