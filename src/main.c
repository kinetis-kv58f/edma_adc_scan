// © 2011. Ravi Teja Gudapati. All rights reserved.

#include <driv/uart0/uart0.h>
#include "driv/cpu/cpu.h"
#include "driv/usbh/usbh.h"
#include "driv/usbh_msd/usbh_msd.h"

usbh_eventResp_t usbh_event_attached(void) {
	if (usbhMsd_event_attached() == Usbh_eventResp_used) {
		return Usbh_eventResp_used;
	}
	return Usbh_eventResp_noError;
}

usbh_eventResp_t usbh_event_enumerationDone(void) {
	if (usbhMsd_event_enumerationDone() == Usbh_eventResp_used) {
		return Usbh_eventResp_used;
	}
	return Usbh_eventResp_noError;
}

usbh_eventResp_t usbh_event_detached(void) {
	usbhMsd_event_detached();
	return Usbh_eventResp_noError;
}

int main(void) {
	cpu_init();

	uart0_init(Uart_Baudrate_4800);

	// Print start
	uart0_putc('s');
	uart0_print_newline();

	// TODO initialize USB host
	usbh_init();

	// TODO

	while (1)
		;
}
