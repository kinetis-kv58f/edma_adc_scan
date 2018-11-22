/*
 * usbh.c
 *
 *  Created on: 9 nov. 2017
 *      Author: SERAGUD
 */

#include "driv/usbh/usbh.h"
#include "hw/usb.h"
#include "./usbh_pstructs.h"
#include "enumeration.h"
#include "transfer.h"
#include "pipe.h"

/// Initializes the device after attach
static inline void priv_init_device_datastruct(void) {
	me.dev.allocatedAddress = 1;
	me.dev.setAddress = 0;

	// TODO set speed

	me.dev.stallRetries = Usbh_config_maxStallRetries;
	me.dev.enumRetries = Usbh_config_enumMaxRetries;

	me.dev.isAttached = TRUE;

	// Open control pipe
	me.dev.controlPipeNum = 0;

	me.pipes[0].endpointAddr = 0;
	me.pipes[0].pipeType = Usbh_endpointType_control;
	me.pipes[0].dir = Usbh_pipeDir_out;
	me.pipes[0].maxPacketSize = 8;
	priv_open_pipe(0);

	me.dev.state = Usbh_enumState_getDes8;
}

/// Handles attach
static inline bool_t priv_handle_attach(void) {
	// Initialize all BDT buffer ODD ping/pong fields
	hw_usb0->CTL |= USB_CTL_ODDRST_Mask;
	// Enable/Enter host mode
	hw_usb0->CTL = USB_CTL_HOSTMODEEN_Mask;
	hw_usb0->ADDR &= ~USD_ADDR_ADDR_Mask;
	hw_usb0->ADDR = 0;

	// TODO Wait 12ms to check line state

	uint8_t speed = 0;
	for (uint8_t trials = 0; trials < 3; trials++) {
		uint8_t const temp = hw_usb0->CTL & USB_CTL_JSTATE_Mask;
		// TODO Wait 60ms
		speed = hw_usb0->CTL & USB_CTL_JSTATE_Mask;

		if (temp != speed) {
			// Start listening for attach event again
			hw_usb0->INTEN |= USB_INTEN_ATTACHEN_Mask;
			return FALSE;
		}
	}

	if (hw_usb0->CTL & USB_CTL_SE0_Mask) {
		// Start listening for attach event again
		hw_usb0->INTEN |= USB_INTEN_ATTACHEN_Mask;
		return FALSE;
	}

	if (speed == 0) {
		hw_usb0->ADDR &= ~USB_ADDR_LSEN_Mask;
	} else {
		hw_usb0->ENDPOINT[0U].ENDPT = USB_ENDPT_HOSTWOHUB_Mask;
		hw_usb0->ADDR |= USB_ADDR_LSEN_Mask;
	}

	hw_usb0->CTL |= USB_CTL_USBENSOFEN_Mask;

	hw_usb0->ISTAT = 0xFFU;
	hw_usb0->INTEN &= ~(USB_INTEN_TOKDNEEN_Mask | USB_INTEN_USBRSTEN_Mask);

	// Do USB bus reset here
	hw_usb0->CTL |= USB_CTL_RESET_Mask;
	// TODO _USB_HostKhciDelay(usbHostPointer, 30U);
	hw_usb0->CTL &= ~USB_CTL_RESET_Mask;

	// TODO _USB_HostKhciDelay(usbHostPointer, 100U);

	hw_usb0->CONTROL &= ~USB_CONTROL_DPPULLUPNONOTG_Mask;
	hw_usb0->INTEN |= USB_INTEN_TOKDNEEN_Mask | USB_INTEN_USBRSTEN_Mask;
	me.deviceAttached = TRUE;

	// TODO Clear tok done event

	// Initialize data structures
	priv_init_device_datastruct();

	// TODO Open control pipe

	// Process enumeration state machine
	priv_handle_enumeration_stateMachine();

	// TODO usbHostPointer->txBd = 0U;
	// TODO usbHostPointer->rxBd = 0U;

	me.transferState = Usbh_transferState_getMsg;
}

// USB0 interrupt handler
void USB0_IRQHandler(void) {
	while (TRUE) {
		// Interrupt status
		const uint8_t status = hw_usb0->ISTAT & hw_usb0->INTEN;

		// No interrupts? Finish.
		if (status == 0)
			break;

		// Clear interrupts
		hw_usb0->ISTAT = status;

		// Handle SOF token
		if (status & USB_ISTAT_SOFTOK_Mask) {
			// TODO handle SOF token
		}

		// Handle attach
		if (status & USB_ISTAT_ATTACH_Mask) {
			// Disable attach interrupt
			hw_usb0->INTEN &= ~USB_INTEN_ATTACHEN_Mask;

			priv_handle_attach();
		}

		// Handle token done
		if (status & USB_ISTAT_TOKDNE_Mask) {
			// TODO Handle token done
			priv_handle_tokenDone();
		}

		// Handle USB reset
		if (status & USB_ISTAT_USBRST_Mask) {
			// TODO Handle USB reset
		}
	}
}

bool_t usbh_init(void) {
	// Configure clocks in SIM
	// TODO CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);

	// Send 'Disconnect' line state. TODO Is this right?
	// Set internal register pull down
	hw_usb0->CTL = USB_CTL_SE0_Mask;
	hw_usb0->CTL = 0;

	// Clear all outstanding interrupts
	hw_usb0->ISTAT = 0xFF;
	// Enable week pull-downs, useful for detecting detach (effectively bus discharge)
	hw_usb0->USBCTRL |= USB_USBCTRL_PDE_Mask;
	// Remove suspend state
	hw_usb0->USBCTRL &= ~USB_USBCTRL_SUSP_Mask;
	// Initialize all BDT buffer ODD ping/pong fields
	hw_usb0->CTL |= USB_CTL_ODDRST_Mask;

	// Setup BDT page addresses
	hw_usb0->BDTPAGE1 = uint32_get_byte1((uint32_t) &bdt);
	hw_usb0->BDTPAGE2 = uint32_get_byte2((uint32_t) &bdt);
	hw_usb0->BDTPAGE3 = uint32_get_byte3((uint32_t) &bdt);

	// Set SOF threshold
	hw_usb0->SOFTHLD = 255;
	// Disable all error interrupts
	hw_usb0->ERREN = 0x00U;
	// Enable/Enter host mode
	hw_usb0->CTL = USB_CTL_HOSTMODEEN_Mask;
	// Wait for attach interrupt
	hw_usb0->INTEN |= USB_INTEN_ATTACHEN_Mask | USB_INTEN_SOFTOKEN_Mask;

	// TODO

	// Enable NVIC interrupts
	hw_nvic->ISER[NVIC_IRQn_USB0 >> 5] = (uint32_t) 0x1
			<< (NVIC_IRQn_USB0 & 0x1F);

	return TRUE;
}

void usbh_off(void) {
	// TODO Initialize clock

	// Disable all interrupts
	hw_usb0->INTEN = 0;
	// Clear all outstanding interrupts
	hw_usb0->ISTAT = 0xFFU;

	hw_usb0->ADDR &= ~USD_ADDR_ADDR_Mask;
	hw_usb0->CTL = 0;
	hw_usb0->USBCTRL = USB_USBCTRL_SUSP_Mask | USB_USBCTRL_PDE_Mask;
	hw_usb0->ADDR = 0;
}

static void priv_busControl_reset_bus(void) {
	while ((hw_usb0->CTL) & USB_CTL_TXSUSPENDTOKENBUSY_Mask)
		;

	hw_usb0->CTL |= USB_CTL_RESET_Mask;
	// Wait for 30 milliseconds (2.5 is minimum for reset, 10 recommended)
	// TODO delay _USB_HostKhciDelay(usbHostPointer, 30U);
	hw_usb0->CTL &= (~USB_CTL_RESET_Mask);
	hw_usb0->CTL |= USB_CTL_ODDRST_Mask;
	hw_usb0->CTL = USB_CTL_HOSTMODEEN_Mask;

	// TODO usbHostPointer->txBd = 0U;
	// TODO usbHostPointer->rxBd = 0U;
}

static void priv_busControl_restart_bus(void) {
	// TODO clear attached info

	// Enter host mode
	hw_usb0->CTL = USB_CTL_HOSTMODEEN_Mask;
	// Clear all outstanding interrupts
	hw_usb0->ISTAT = 0xFFU;
	// Enable only USB interrupt attach for host mode
	hw_usb0->INTEN |= USB_INTEN_ATTACHEN_Mask;
}

/// Serve routine that must be periodically called to handle
void usbh_serve(void) {
	// TODO

	if(me.deviceAttached) {
		priv_handle_transferStateMachine();
	} else {
		// TODO
	}

	// TODO
}
