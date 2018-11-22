/// USBH Mass Storage Device driver interface

#ifndef INCLUDE_DRIV_USBH_MSD_USBH_MSD_H_
#define INCLUDE_DRIV_USBH_MSD_USBH_MSD_H_

#include "driv/usbh/usbh.h"

/// Shall be called when a USBD is attached
extern void usbhMsd_event_attached(usbh_configuration_t& configuration);

/// Shall be called when a USBD is detached
extern void usbhMsd_event_detached(void);

/// Shall be called when enumeration is done for a USBD
extern void usbhMsd_event_enumerationDone(void);

/// Must be periodically called to perform tasks
extern void usbhMsd_serve(void);

#endif /* INCLUDE_DRIV_USBH_MSD_USBH_MSD_H_ */
