/*
 * usbh_msd.cpp
 *
 *  Created on: 12 nov. 2017
 *      Author: SERAGUD
 */

#include <memory.h>
#include "driv/usbh_msd/usbh_msd.h"

/// host app device attach/detach status
typedef enum {
	/// There is no device attach/detach
	Usbh_appState_idle = 0,
	/// Device is attached
	Usbh_appState_attached,
	/// Device is detached
	Usbh_appState_detached,
} usbh_appState_t;

typedef struct {
	/// State of the attached device
	usbh_appState_t state;

	/// Selected configuration number.
	uint16_t configNum;

	/// Selected interface
	uint16_t interfaceNum;
} usbhMsd_mod_t;

static usbhMsd_mod_t me;

typedef enum {
	Usbh_msdSubclass_ufi = 4, Usbh_msdSubclass_scsi = 6,
} usbh_msdSubclass_t;

typedef enum {
	Usbh_msdProtocol_bulk = 0x50,
} usbh_msdProtocol_t;

void usbhMsd_init(void) {
	memset(&me, 0, sizeof(me));
	me.state = Usbh_appState_idle;
	me.configNum = 0xFFFF;
	me.interfaceNum = 0xFFFF;
}

/// \note OriginalCode: USB_HostMsdEvent
usbh_eventResp_t usbhMsd_event_attached(usbh_configuration_t& configuration) {
	if (me.state != Usbh_appState_idle) {
		return Usbh_eventResp_noError;
	}

	for (uint16_t interfaceIndex = 0;
			interfaceIndex < configuration.interfaceCount; ++interfaceIndex) {
		usbh_interface_t& interface =
				&configuration.interfaceList[interfaceIndex];

		// Check that interface belongs to MSD class
		if (interface.interfaceDesc.bInterfaceClass != Usbh_class_msd)
			continue;

		// Check for valid subclass
		if ((interface.interfaceDesc.bInterfaceSubClass != Usbh_msdSubclass_ufi)
				&& (interface.interfaceDesc.bInterfaceSubClass
						!= Usbh_msdSubclass_scsi)) {
			continue;
		}

		if (interface.interfaceDesc.bInterfaceProtocol
				!= Usbh_msdProtocol_bulk) {
			continue;
		}

		// The interface is supported by the application
		me.interfaceNum = interfaceIndex;
		me.configNum = configuration.configurationDesc.iConfiguration;
		return Usbh_eventResp_used;
	}

	return Usbh_eventResp_noError;
}

/// \note OriginalCode: USB_HostMsdEvent
usbh_eventResp_t usbhMsd_event_enumerationDone(uint16_t const configNum) {
	if (me.configNum != configNum) {
		return Usbh_eventResp_noError;
	}

	if (me.state != Usbh_appState_idle) {
		// TODO what do?
		return Usbh_eventResp_noError;
	}

	me.state = Usbh_appState_attached;
	return Usbh_eventResp_used;
}

/// \note OriginalCode: USB_HostMsdEvent
usbh_eventResp_t usbhMsd_event_detached(uint16_t const configNum) {
	if (me.configNum != configNum) {
		return Usbh_eventResp_noError;
	}

	me.configNum = 0xFFFF;
	me.interfaceNum = 0xFFFF;

	if (me.state != Usbh_appState_idle) {
		me.state = Usbh_appState_detached;
	}
	return Usbh_eventResp_used;
}

void usbhMsd_serve(void) {
	// TODO handle state change

	// TODO handle runState
}
