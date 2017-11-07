// � 2011. Ravi Teja Gudapati. All rights reserved.

#ifndef INCLUDE_HW_DMA_H_
#define INCLUDE_HW_DMA_H_

#include "types.h"
#include "hw/common.h"

typedef struct {
	__IO uint8_t CHCFG[32]; /**< Channel Configuration register, array offset: 0x0, array step: 0x1 */
} DMAMUX_Type;

enum {
	DMAMUX_CHCFG_ENBL_Shift = 7,
	DMAMUX_CHCFG_ENBL_Mask = 1 << DMAMUX_CHCFG_ENBL_Shift,
	DMAMUX_CHCFG_TRIG_Shift = 6,
	DMAMUX_CHCFG_TRIG_Mask = 1 << DMAMUX_CHCFG_TRIG_Shift,
	DMAMUX_CHCFG_SOURCE_Mask = 0x3F,

	DMAMUX_CHCFG_SOURCE_Group0_AlwaysOn0 = 60,
	DMAMUX_CHCFG_SOURCE_Group1_ADC0 = 45,
} dmamux_fields_t;

/// DMA - Register Layout Typedef
typedef struct {
  __IO uint32_t CR;                                /**< Control Register, offset: 0x0 */
  __I  uint32_t ES;                                /**< Error Status Register, offset: 0x4 */
       uint8_t RESERVED_0[4];
  __IO uint32_t ERQ;                               /**< Enable Request Register, offset: 0xC */
       uint8_t RESERVED_1[4];
  __IO uint32_t EEI;                               /**< Enable Error Interrupt Register, offset: 0x14 */
  __O  uint8_t CEEI;                               /**< Clear Enable Error Interrupt Register, offset: 0x18 */
  __O  uint8_t SEEI;                               /**< Set Enable Error Interrupt Register, offset: 0x19 */
  __O  uint8_t CERQ;                               /**< Clear Enable Request Register, offset: 0x1A */
  __O  uint8_t SERQ;                               /**< Set Enable Request Register, offset: 0x1B */
  __O  uint8_t CDNE;                               /**< Clear DONE Status Bit Register, offset: 0x1C */
  __O  uint8_t SSRT;                               /**< Set START Bit Register, offset: 0x1D */
  __O  uint8_t CERR;                               /**< Clear Error Register, offset: 0x1E */
  __O  uint8_t CINT;                               /**< Clear Interrupt Request Register, offset: 0x1F */
       uint8_t RESERVED_2[4];
  __IO uint32_t INT;                               /**< Interrupt Request Register, offset: 0x24 */
       uint8_t RESERVED_3[4];
  __IO uint32_t ERR;                               /**< Error Register, offset: 0x2C */
       uint8_t RESERVED_4[4];
  __I  uint32_t HRS;                               /**< Hardware Request Status Register, offset: 0x34 */
       uint8_t RESERVED_5[12];
  __IO uint32_t EARS;                              /**< Enable Asynchronous Request in Stop Register, offset: 0x44 */
       uint8_t RESERVED_6[184];
  __IO uint8_t DCHPRI3;                            /**< Channel n Priority Register, offset: 0x100 */
  __IO uint8_t DCHPRI2;                            /**< Channel n Priority Register, offset: 0x101 */
  __IO uint8_t DCHPRI1;                            /**< Channel n Priority Register, offset: 0x102 */
  __IO uint8_t DCHPRI0;                            /**< Channel n Priority Register, offset: 0x103 */
  __IO uint8_t DCHPRI7;                            /**< Channel n Priority Register, offset: 0x104 */
  __IO uint8_t DCHPRI6;                            /**< Channel n Priority Register, offset: 0x105 */
  __IO uint8_t DCHPRI5;                            /**< Channel n Priority Register, offset: 0x106 */
  __IO uint8_t DCHPRI4;                            /**< Channel n Priority Register, offset: 0x107 */
  __IO uint8_t DCHPRI11;                           /**< Channel n Priority Register, offset: 0x108 */
  __IO uint8_t DCHPRI10;                           /**< Channel n Priority Register, offset: 0x109 */
  __IO uint8_t DCHPRI9;                            /**< Channel n Priority Register, offset: 0x10A */
  __IO uint8_t DCHPRI8;                            /**< Channel n Priority Register, offset: 0x10B */
  __IO uint8_t DCHPRI15;                           /**< Channel n Priority Register, offset: 0x10C */
  __IO uint8_t DCHPRI14;                           /**< Channel n Priority Register, offset: 0x10D */
  __IO uint8_t DCHPRI13;                           /**< Channel n Priority Register, offset: 0x10E */
  __IO uint8_t DCHPRI12;                           /**< Channel n Priority Register, offset: 0x10F */
  __IO uint8_t DCHPRI19;                           /**< Channel n Priority Register, offset: 0x110 */
  __IO uint8_t DCHPRI18;                           /**< Channel n Priority Register, offset: 0x111 */
  __IO uint8_t DCHPRI17;                           /**< Channel n Priority Register, offset: 0x112 */
  __IO uint8_t DCHPRI16;                           /**< Channel n Priority Register, offset: 0x113 */
  __IO uint8_t DCHPRI23;                           /**< Channel n Priority Register, offset: 0x114 */
  __IO uint8_t DCHPRI22;                           /**< Channel n Priority Register, offset: 0x115 */
  __IO uint8_t DCHPRI21;                           /**< Channel n Priority Register, offset: 0x116 */
  __IO uint8_t DCHPRI20;                           /**< Channel n Priority Register, offset: 0x117 */
  __IO uint8_t DCHPRI27;                           /**< Channel n Priority Register, offset: 0x118 */
  __IO uint8_t DCHPRI26;                           /**< Channel n Priority Register, offset: 0x119 */
  __IO uint8_t DCHPRI25;                           /**< Channel n Priority Register, offset: 0x11A */
  __IO uint8_t DCHPRI24;                           /**< Channel n Priority Register, offset: 0x11B */
  __IO uint8_t DCHPRI31;                           /**< Channel n Priority Register, offset: 0x11C */
  __IO uint8_t DCHPRI30;                           /**< Channel n Priority Register, offset: 0x11D */
  __IO uint8_t DCHPRI29;                           /**< Channel n Priority Register, offset: 0x11E */
  __IO uint8_t DCHPRI28;                           /**< Channel n Priority Register, offset: 0x11F */
       uint8_t RESERVED_7[3808];
  struct {                                         /* offset: 0x1000, array step: 0x20 */
    __IO uint32_t SADDR;                             /**< TCD Source Address, array offset: 0x1000, array step: 0x20 */
    __IO uint16_t SOFF;                              /**< TCD Signed Source Address Offset, array offset: 0x1004, array step: 0x20 */
    __IO uint16_t ATTR;                              /**< TCD Transfer Attributes, array offset: 0x1006, array step: 0x20 */
    union {                                          /* offset: 0x1008, array step: 0x20 */
      __IO uint32_t NBYTES_MLNO;                       /**< TCD Minor Byte Count (Minor Loop Mapping Disabled), array offset: 0x1008, array step: 0x20 */
      __IO uint32_t NBYTES_MLOFFNO;                    /**< TCD Signed Minor Loop Offset (Minor Loop Mapping Enabled and Offset Disabled), array offset: 0x1008, array step: 0x20 */
      __IO uint32_t NBYTES_MLOFFYES;                   /**< TCD Signed Minor Loop Offset (Minor Loop Mapping and Offset Enabled), array offset: 0x1008, array step: 0x20 */
    };
    __IO uint32_t SLAST;                             /**< TCD Last Source Address Adjustment, array offset: 0x100C, array step: 0x20 */
    __IO uint32_t DADDR;                             /**< TCD Destination Address, array offset: 0x1010, array step: 0x20 */
    __IO uint16_t DOFF;                              /**< TCD Signed Destination Address Offset, array offset: 0x1014, array step: 0x20 */
    union {                                          /* offset: 0x1016, array step: 0x20 */
      __IO uint16_t CITER_ELINKNO;                     /**< TCD Current Minor Loop Link, Major Loop Count (Channel Linking Disabled), array offset: 0x1016, array step: 0x20 */
      __IO uint16_t CITER_ELINKYES;                    /**< TCD Current Minor Loop Link, Major Loop Count (Channel Linking Enabled), array offset: 0x1016, array step: 0x20 */
    };
    __IO uint32_t DLAST_SGA;                         /**< TCD Last Destination Address Adjustment/Scatter Gather Address, array offset: 0x1018, array step: 0x20 */
    __IO uint16_t CSR;                               /**< TCD Control and Status, array offset: 0x101C, array step: 0x20 */
    union {                                          /* offset: 0x101E, array step: 0x20 */
      __IO uint16_t BITER_ELINKNO;                     /**< TCD Beginning Minor Loop Link, Major Loop Count (Channel Linking Disabled), array offset: 0x101E, array step: 0x20 */
      __IO uint16_t BITER_ELINKYES;                    /**< TCD Beginning Minor Loop Link, Major Loop Count (Channel Linking Enabled), array offset: 0x101E, array step: 0x20 */
    };
  } TCD[32];
} DMA_Type;

enum {
	EDMA_TCD_ATTR_SSIZE_Shift = 8,
	EDMA_TCD_ATTR_SSIZE_Mask = 0x7 << EDMA_TCD_ATTR_SSIZE_Shift,
	EDMA_TCD_ATTR_DSIZE_Mask = 0x7,
	EDMA_TCD_ATTR_SSIZE_8bit = 0 << EDMA_TCD_ATTR_SSIZE_Shift,
	EDMA_TCD_ATTR_SSIZE_16bit = 1 << EDMA_TCD_ATTR_SSIZE_Shift,
	EDMA_TCD_ATTR_SSIZE_32bit = 2 << EDMA_TCD_ATTR_SSIZE_Shift,
	EDMA_TCD_ATTR_SSIZE_16byteBurst = 4 << EDMA_TCD_ATTR_SSIZE_Shift,
	EDMA_TCD_ATTR_SSIZE_32byteBurst = 5 << EDMA_TCD_ATTR_SSIZE_Shift,
	EDMA_TCD_ATTR_DSIZE_8bit = 0,
	EDMA_TCD_ATTR_DSIZE_16bit = 1,
	EDMA_TCD_ATTR_DSIZE_32bit = 2,
	EDMA_TCD_ATTR_DSIZE_16byteBurst = 4,
	EDMA_TCD_ATTR_DSIZE_32byteBurst = 5,

	EDMA_TCD_CSR_MAJORLINKCH_Shift = 8,
	EDMA_TCD_CSR_MAJORLINKCH_Mask = 0x1F << EDMA_TCD_CSR_MAJORLINKCH_Shift,
	EDMA_TCD_CSR_DONE_Shift = 7,
	EDMA_TCD_CSR_DONE_Mask = 0x1 << EDMA_TCD_CSR_DONE_Shift,
	EDMA_TCD_CSR_ACTIVE_Shift = 6,
	EDMA_TCD_CSR_ACTIVE_Mask = 0x1 << EDMA_TCD_CSR_ACTIVE_Shift,
	EDMA_TCD_CSR_MAJORELINK_Shift = 5,
	EDMA_TCD_CSR_MAJORELINK_Mask = 0x1 << EDMA_TCD_CSR_MAJORELINK_Shift,
	EDMA_TCD_CSR_ESG_Shift = 4,
	EDMA_TCD_CSR_ESG_Mask = 0x1 << EDMA_TCD_CSR_ESG_Shift,
	EDMA_TCD_CSR_DREQ_Shift = 3,
	EDMA_TCD_CSR_DREQ_Mask = 0x1 << EDMA_TCD_CSR_DREQ_Shift,
	EDMA_TCD_CSR_INTFLAG_Shift = 2,
	EDMA_TCD_CSR_INTFLAG_Mask = 0x1 << EDMA_TCD_CSR_INTFLAG_Shift,
	EDMA_TCD_CSR_INTMAJOR_Shift = 1,
	EDMA_TCD_CSR_INTMAJOR_Mask = 0x1 << EDMA_TCD_CSR_INTMAJOR_Shift,
	EDMA_TCD_CSR_START_Shift = 0,
	EDMA_TCD_CSR_START_Mask = 0x1 << EDMA_TCD_CSR_START_Shift,

	EMA_TCD_BITER_ELINK_Shift = 15,
	EMA_TCD_BITER_ELINK_Mask = 1 << EMA_TCD_BITER_ELINK_Shift,
	EMA_TCD_BITER_ELINKYES_LINKCH_Shift = 9,
	EMA_TCD_BITER_ELINKYES_LINKCH_Mask = 0x1F << EMA_TCD_BITER_ELINKYES_LINKCH_Shift,
	EMA_TCD_BITER_ELINKYES_BITER_Mask = 0x1FF,
} edma_fields_t;

volatile DMAMUX_Type * const volatile hw_dmamux =
		(DMAMUX_Type *) Memmap_DMAMUX_Addr;

volatile DMA_Type * const volatile hw_dma = (DMA_Type *) Memmap_DMA0_Addr;

#endif /* INCLUDE_HW_DMA_H_ */
