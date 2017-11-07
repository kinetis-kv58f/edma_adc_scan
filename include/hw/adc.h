/*
 * adc.h
 *
 *  Created on: 2 nov. 2017
 *      Author: SERAGUD
 */

#ifndef INCLUDE_HW_ADC_H_
#define INCLUDE_HW_ADC_H_

#include "hw/common.h"
#include "types.h"

/// ADC - Register Layout Typedef
typedef struct {
	__IO uint32_t SC1[2]; /**< ADC Status and Control Registers 1, array offset: 0x0, array step: 0x4 */
	__IO uint32_t CFG1; /**< ADC Configuration Register 1, offset: 0x8 */
	__IO uint32_t CFG2; /**< ADC Configuration Register 2, offset: 0xC */
	__I uint32_t R[2]; /**< ADC Data Result Register, array offset: 0x10, array step: 0x4 */
	__IO uint32_t CV1; /**< Compare Value Registers, offset: 0x18 */
	__IO uint32_t CV2; /**< Compare Value Registers, offset: 0x1C */
	__IO uint32_t SC2; /**< Status and Control Register 2, offset: 0x20 */
	__IO uint32_t SC3; /**< Status and Control Register 3, offset: 0x24 */
	__IO uint32_t OFS; /**< ADC Offset Correction Register, offset: 0x28 */
	__IO uint32_t PG; /**< ADC Plus-Side Gain Register, offset: 0x2C */
	__IO uint32_t MG; /**< ADC Minus-Side Gain Register, offset: 0x30 */
	__IO uint32_t CLPD; /**< ADC Plus-Side General Calibration Value Register, offset: 0x34 */
	__IO uint32_t CLPS; /**< ADC Plus-Side General Calibration Value Register, offset: 0x38 */
	__IO uint32_t CLP4; /**< ADC Plus-Side General Calibration Value Register, offset: 0x3C */
	__IO uint32_t CLP3; /**< ADC Plus-Side General Calibration Value Register, offset: 0x40 */
	__IO uint32_t CLP2; /**< ADC Plus-Side General Calibration Value Register, offset: 0x44 */
	__IO uint32_t CLP1; /**< ADC Plus-Side General Calibration Value Register, offset: 0x48 */
	__IO uint32_t CLP0; /**< ADC Plus-Side General Calibration Value Register, offset: 0x4C */
	uint8_t RESERVED_0[4];__IO uint32_t CLMD; /**< ADC Minus-Side General Calibration Value Register, offset: 0x54 */
	__IO uint32_t CLMS; /**< ADC Minus-Side General Calibration Value Register, offset: 0x58 */
	__IO uint32_t CLM4; /**< ADC Minus-Side General Calibration Value Register, offset: 0x5C */
	__IO uint32_t CLM3; /**< ADC Minus-Side General Calibration Value Register, offset: 0x60 */
	__IO uint32_t CLM2; /**< ADC Minus-Side General Calibration Value Register, offset: 0x64 */
	__IO uint32_t CLM1; /**< ADC Minus-Side General Calibration Value Register, offset: 0x68 */
	__IO uint32_t CLM0; /**< ADC Minus-Side General Calibration Value Register, offset: 0x6C */
} ADC_Type;

enum {
	ADC_SC1_COCO_Shift = 7,
	ADC_SC1_COCO_Mask = 1 << ADC_SC1_COCO_Shift,
	ADC_SC1_ADCH_Mask = 0x1F,
	ADC_SC1_ADCH_Disable = 0x1F,

	ADC_CFG1_ADLPC_Shift = 7,
	ADC_CFG1_ADLPC_Mask = 1 << ADC_CFG1_ADLPC_Shift,
	ADC_CFG1_ADIV_Shift = 5,
	ADC_CFG1_ADIV_Mask = 3 << ADC_CFG1_ADIV_Shift,
	ADC_CFG1_ADIV_1div = 0 << ADC_CFG1_ADIV_Shift,
	ADC_CFG1_ADIV_2div = 1 << ADC_CFG1_ADIV_Shift,
	ADC_CFG1_ADIV_4div = 2 << ADC_CFG1_ADIV_Shift,
	ADC_CFG1_ADIV_8div = 3 << ADC_CFG1_ADIV_Shift,
	ADC_CFG1_ADLSMP_Shift = 4,
	ADC_CFG1_ADLSMP_Mask = 1 << ADC_CFG1_ADLSMP_Shift,
	ADC_CFG1_MODE_Shift = 2,
	ADC_CFG1_MODE_Mask = 3 << ADC_CFG1_MODE_Shift,
	ADC_CFG1_MODE_8bit = 0 << ADC_CFG1_MODE_Shift,
	ADC_CFG1_MODE_12bit = 1 << ADC_CFG1_MODE_Shift,
	ADC_CFG1_MODE_10bit = 2 << ADC_CFG1_MODE_Shift,
	ADC_CFG1_MODE_16bit = 3 << ADC_CFG1_MODE_Shift,

	ADC_SC2_DMAEN_Shift = 2,
	ADC_SC2_DMAEN_Mask = 1 << ADC_SC2_DMAEN_Shift,
} adc_fields_t;

volatile ADC_Type * const volatile hw_adc0 = (ADC_Type *) Memmap_ADC0_Addr;

#endif /* INCLUDE_HW_ADC_H_ */
