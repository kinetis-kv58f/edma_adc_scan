// © 2011. Ravi Teja Gudapati. All rights reserved.

#include <driv/edma_adcScan/edma_adcScan.h>
#include <hw/dma.h>
#include "hw/sim.h"
#include "hw/port.h"
#include "hw/adc.h"
#include "driv/uart0/uart0.h"

static const uint8_t edmaAdcScanChannels[EdmaAdcScan_NumChannels] = { 6, 10, 11 };
uint16_t edmaAdcScanReadings[EdmaAdcScan_NumChannels] = { 0 };

enum {
	/// Channel number used to trigger ADC reading
	triggerChannel = 0,
	/// Channel number used to read ADC
	readerChannel = 16
} priv_EdmaAdcScan_consts_t;

static inline void priv_setup_pins(void) {
	// Initialize port B clock
	hw_sim->SCGC5 |= SIM_SCGC5_PORTB_Mask;
	// Initialize port E clock
	hw_sim->SCGC5 |= SIM_SCGC5_PORTE_Mask;

	// Set analog pin PTE19 (Channe6a)
	{
		uint32_t pcr = hw_portE->PCR[19];

		// Clear old Mux value
		pcr &= ~PortMuxMask;
		// Set Mux to GPIO functionality
		pcr |= ((uint32_t) Port_PinDisabledOrAnalog << PortMuxShift)
				& PortMuxMask;
		// Disable interrupt configuration
		pcr &= ~PortIrqCMask;
		// Disable pull resistor
		pcr &= ~PORT_PCR_PE_Mask;

		hw_portE->PCR[19] = pcr;
	}

	// Set analog pin PTE5 (Channel10)
	{
		uint32_t pcr = hw_portE->PCR[5];

		// Clear old Mux value
		pcr &= ~PortMuxMask;
		// Set Mux to GPIO functionality
		pcr |= ((uint32_t) Port_PinDisabledOrAnalog << PortMuxShift)
				& PortMuxMask;
		// Disable interrupt configuration
		pcr &= ~PortIrqCMask;
		// Disable pull resistor
		pcr &= ~PORT_PCR_PE_Mask;

		hw_portE->PCR[5] = pcr;
	}

	// Set analog pin PTE12 (Channel11)
	{
		uint32_t pcr = hw_portE->PCR[12];

		// Clear old Mux value
		pcr &= ~PortMuxMask;
		// Set Mux to GPIO functionality
		pcr |= ((uint32_t) Port_PinDisabledOrAnalog << PortMuxShift)
				& PortMuxMask;
		// Disable interrupt configuration
		pcr &= ~PortIrqCMask;
		// Disable pull resistor
		pcr &= ~PORT_PCR_PE_Mask;

		hw_portE->PCR[12] = pcr;
	}

	// TODO
}

void priv_adc_init(void) {
	hw_sim->SCGC6 |= SIM_SCGC6_ADC0_Mask;

	hw_adc0->SC1[0] = 0;
	// Disable interrupt
	// Single ended mode
	// Disable ADC
	hw_adc0->SC1[0] |= ADC_SC1_ADCH_Disable;

	hw_adc0->CFG1 = 0;
	// Low power configuration
	// No clock divide
	// Short sample time
	// 10-bit resolution
	// Bus  clock as input clock source
	hw_adc0->CFG1 |= ADC_CFG1_MODE_10bit;

	hw_adc0->CFG2 = 0;
	// Select 'a' channels in the mux
	// Disable asynchronous clock
	// Normal conversion speed selected
	// Longest sample time. 24 ADCK cycles
	hw_adc0->CFG2 |= 0;

	hw_adc0->SC2 = 0;
	// Software conversion trigger
	// Compare function range disable
	// Enable DMA
	// Default voltage reference pins
	hw_adc0->SC2 |= ADC_SC2_DMAEN_Mask;

	hw_adc0->SC3 = 0;
	// Continuous conversion disable
	// Hardware average disable
	hw_adc0->SC3 |= 0;

	priv_setup_pins();
}

void edmaAdcScan_prepare(void) {
	edmaAdcScan_clear_readings();

	priv_adc_init();

	// Enable clock for DMAMUX hardware
	hw_sim->SCGC6 |= SIM_SCGC6_DMAMUX_Mask;
	// Enable clock for DMA hardware
	hw_sim->SCGC7 |= SIM_SCGC6_DMA_Mask;

	// Always enabled source. No trigger.
	hw_dmamux->CHCFG[triggerChannel] = DMAMUX_CHCFG_ENBL_Mask
			| DMAMUX_CHCFG_SOURCE_Group0_AlwaysOn0;
	// Source address. Constant buffer in RAM holding channels to read.
	hw_dma->TCD[triggerChannel].SADDR = (uint32_t) &edmaAdcScanChannels[0];
	// Source address increment. Data is 1 byte.
	hw_dma->TCD[triggerChannel].SOFF = 0x01;
	// Source address reload after major loop finish.
	hw_dma->TCD[triggerChannel].SLAST = 0x00;
	// Destination address. ADC0 control register.
	hw_dma->TCD[triggerChannel].DADDR = (uint32_t) &hw_adc0->SC1[0];
	// Destination address increment. No increment.
	hw_dma->TCD[triggerChannel].DOFF = 0x00;
	// Destination address reload after major loop finish. No address reload needed.
	hw_dma->TCD[triggerChannel].DLAST_SGA = 0x00;
	// Number of bytes for minor loop (one data transfer).
	hw_dma->TCD[triggerChannel].NBYTES_MLNO = 0x01;
	// Channel linking and major loop setting.
	// No linking after minor loop.
	// Number of minor loops in a major loop: Number of ADC channels to read.
	hw_dma->TCD[triggerChannel].BITER_ELINKNO = 1;
	// Channel linking and major loop setting reload value after major loop finish.
	// No linking after minor loop.
	// Number of minor loops in a major loop: Number of ADC channels to read.
	hw_dma->TCD[triggerChannel].CITER_ELINKNO = 1;
	// Source and destination data width specification.
	// No circular buffer.
	// Both source and destination sizes are 8-bit.
	hw_dma->TCD[triggerChannel].ATTR = 0;
	// Common channel setting.
	// No bandwidth control. No linking after major loop.
	// Disable channel after major loop completion.
	// No IRQ request enable.
	hw_dma->TCD[triggerChannel].CSR = EDMA_TCD_CSR_DREQ_Mask;

	// ADC source. No trigger.
	hw_dmamux->CHCFG[readerChannel] = DMAMUX_CHCFG_ENBL_Mask
			| DMAMUX_CHCFG_SOURCE_Group1_ADC0;
	// Source address. ADC read value.
	hw_dma->TCD[readerChannel].SADDR = (uint32_t) &hw_adc0->R[0];
	// Source address increment. Each reading is read from same register.
	hw_dma->TCD[readerChannel].SOFF = 0x00;
	// Source address reload after major loop finish.
	hw_dma->TCD[readerChannel].SLAST = 0;
	// Destination address. ADC0 control register.
	hw_dma->TCD[readerChannel].DADDR = (uint32_t) edmaAdcScanReadings;
	// Destination address increment. Each reading is 12-bits or 2-bytes.
	hw_dma->TCD[readerChannel].DOFF = 0x02;
	// Destination address reload after major loop finish.
	hw_dma->TCD[readerChannel].DLAST_SGA = -2 * EdmaAdcScan_NumChannels;
	// Number of bytes for minor loop (one data transfer).
	// Each reading is 12-bits or 2-bytes.
	hw_dma->TCD[readerChannel].NBYTES_MLNO = 0x02;
	// Channel linking and major loop setting.
	// Minor loop linking to trigger next reading.
	// Number of minor loops in a major loop: Number of ADC channels to read.
	hw_dma->TCD[readerChannel].BITER_ELINKNO = EMA_TCD_BITER_ELINK_Mask | EdmaAdcScan_NumChannels;
	// Channel linking and major loop setting reload value after major loop finish.
	// Minor loop linking to trigger next reading.
	// Number of minor loops in a major loop: Number of ADC channels to read.
	hw_dma->TCD[readerChannel].CITER_ELINKNO = EMA_TCD_BITER_ELINK_Mask | EdmaAdcScan_NumChannels;
	// Source and destination data width specification.
	// No circular buffer.
	// Both source and destination sizes are 16-bit.
	hw_dma->TCD[readerChannel].ATTR = EDMA_TCD_ATTR_DSIZE_16bit
			| EDMA_TCD_ATTR_SSIZE_16bit;
	// Common channel setting.
	// No bandwidth control. No major loop linking.
	// No IRQ request enable.
	hw_dma->TCD[readerChannel].CSR = EDMA_TCD_CSR_DREQ_Mask;
}

bool_t edmaAdcScan_start(void) {
	if (hw_dma->TCD[readerChannel].CSR & EDMA_TCD_CSR_ACTIVE_Mask) {
		return FALSE;
	}

	// Enable request on DMA channels 0 and 16
	// ((uint32_t) 1 << triggerChannel) |
	hw_dma->ERQ |= ((uint32_t) 1 << readerChannel);

	// TODO hw_adc0->SC1[0] = 6;
	hw_dma->TCD[triggerChannel].CSR |= EDMA_TCD_CSR_START_Mask;

	return TRUE;
}

bool_t edmaAdcScan_did_finish(void) {
	return (hw_dma->TCD[readerChannel].CSR & EDMA_TCD_CSR_DONE_Mask)
			&& (hw_dma->TCD[triggerChannel].CSR & EDMA_TCD_CSR_DONE_Mask);
}

void edmaAdcScan_clear_readings(void) {
	for (uint32_t i = 0; i < EdmaAdcScan_NumChannels; i++) {
		edmaAdcScanReadings[i] = 0;
	}
}

void edmaAdcScan_print_readings(void) {
	for (uint32_t i = 0; i < EdmaAdcScan_NumChannels; i++) {
		uart0_print_hex_halfword(edmaAdcScanReadings[i]);
		uart0_putc(',');
	}
}

int16_t edmaAdcScan_get_reading(uint8_t const index) {
	if (index >= EdmaAdcScan_NumChannels)
		return -1;
	return edmaAdcScanReadings[index];
}

bool_t edmaAdcScan_read_blocking(uint8_t const ch, uint16_t * const ret) {
	// TODO If DMA is in operation, return false

	// Start conversion
	hw_adc0->SC1[0] = (hw_adc0->SC1[0] & ~ADC_SC1_ADCH_Mask) | ch;

	uint32_t const sampleTimeUs = 2000
			* 100 /* TODO = delays[ADC_SAMPLE_TIME] * 2000*/;

	for (volatile uint32_t waitUs = 0; waitUs < sampleTimeUs; waitUs++) {
		if (hw_adc0->SC1[0] & ADC_SC1_COCO_Mask) {
			*ret = hw_adc0->R[0];
			hw_adc0->SC1[0] |= ADC_SC1_ADCH_Disable;
			return TRUE;
		}
	}

	// Timed out. Abort
	hw_adc0->SC1[0] |= ADC_SC1_ADCH_Disable;

	return FALSE;
}
