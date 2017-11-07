// © 2011. Ravi Teja Gudapati. All rights reserved.

#ifndef INCLUDE_DRIV_EDMA_ADCSCAN_EDMA_ADCSCAN_H_
#define INCLUDE_DRIV_EDMA_ADCSCAN_EDMA_ADCSCAN_H_

#include "types.h"

enum {
	EdmaAdcScan_NumChannels = 3,
} EdmaAdcScan_consts_t;

void edmaAdcScan_prepare(void);

bool_t edmaAdcScan_start(void);

bool_t edmaAdcScan_did_finish(void);

void edmaAdcScan_clear_readings(void);

void edmaAdcScan_print_readings(void);

int16_t edmaAdcScan_get_reading(uint8_t const index);

bool_t edmaAdcScan_read_blocking(uint8_t const ch, uint16_t * const ret);

#endif /* INCLUDE_DRIV_EDMA_ADCSCAN_EDMA_ADCSCAN_H_ */
