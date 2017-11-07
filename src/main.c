// © 2011. Ravi Teja Gudapati. All rights reserved.

#include <driv/edma_adcScan/edma_adcScan.h>
#include <driv/uart0/uart0.h>
#include "driv/cpu/cpu.h"

int main(void) {
	cpu_init();

	uart0_init(Uart_Baudrate_4800);

	// Print start
	uart0_putc('s');
	uart0_print_newline();

	edmaAdcScan_prepare();

	uint16_t readings[EdmaAdcScan_NumChannels] = {0};

	{

		if(edmaAdcScan_read_blocking(6, &readings[0])) {
			uart0_print_hex_halfword(readings[0]);
		} else {
			// Print error
			uart0_putc('x');
			uart0_putc('3');
			uart0_print_newline();
		}
		uart0_putc(',');
		uart0_putc(' ');
		if(edmaAdcScan_read_blocking(10, &readings[1])) {
			uart0_print_hex_halfword(readings[1]);
		} else {
			// Print error
			uart0_putc('x');
			uart0_putc('4');
			uart0_print_newline();
		}
		uart0_putc(',');
		uart0_putc(' ');
		if(edmaAdcScan_read_blocking(11, &readings[2])) {
			uart0_print_hex_halfword(readings[2]);
		} else {
			// Print error
			uart0_putc('x');
			uart0_putc('5');
			uart0_print_newline();
		}
		uart0_print_newline();
	}

	if(edmaAdcScan_start()) {
		// Wait for completion
		while(edmaAdcScan_did_finish() == FALSE);

		edmaAdcScan_print_readings();

		for (uint32_t i = 0; i < EdmaAdcScan_NumChannels; i++) {
			if(edmaAdcScan_get_reading(i) != readings[i]) {
				// Print error
				uart0_putc('x');
				uart0_putc('1');
				uart0_print_newline();
				break;
			}
		}
	} else {
		// Print error
		uart0_putc('x');
		uart0_putc('2');
		uart0_print_newline();
	}

	// Print finish
	uart0_putc('k');
	uart0_print_newline();

	while(1);
}
