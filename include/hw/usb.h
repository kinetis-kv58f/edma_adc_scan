/*
 * usb.h
 *
 *  Created on: 9 nov. 2017
 *      Author: SERAGUD
 */

#ifndef INCLUDE_HW_USB_H_
#define INCLUDE_HW_USB_H_

#include "types.h"
#include "common.h"

/// USB - Register Layout Typedef
typedef struct {
  __I  uint8_t PERID;                              /**< Peripheral ID register, offset: 0x0 */
       uint8_t RESERVED_0[3];
  __I  uint8_t IDCOMP;                             /**< Peripheral ID Complement register, offset: 0x4 */
       uint8_t RESERVED_1[3];
  __I  uint8_t REV;                                /**< Peripheral Revision register, offset: 0x8 */
       uint8_t RESERVED_2[3];
  __I  uint8_t ADDINFO;                            /**< Peripheral Additional Info register, offset: 0xC */
       uint8_t RESERVED_3[3];
  __IO uint8_t OTGISTAT;                           /**< OTG Interrupt Status register, offset: 0x10 */
       uint8_t RESERVED_4[3];
  __IO uint8_t OTGICR;                             /**< OTG Interrupt Control register, offset: 0x14 */
       uint8_t RESERVED_5[3];
  __IO uint8_t OTGSTAT;                            /**< OTG Status register, offset: 0x18 */
       uint8_t RESERVED_6[3];
  __IO uint8_t OTGCTL;                             /**< OTG Control register, offset: 0x1C */
       uint8_t RESERVED_7[99];
  __IO uint8_t ISTAT;                              /**< Interrupt Status register, offset: 0x80 */
       uint8_t RESERVED_8[3];
  __IO uint8_t INTEN;                              /**< Interrupt Enable register, offset: 0x84 */
       uint8_t RESERVED_9[3];
  __IO uint8_t ERRSTAT;                            /**< Error Interrupt Status register, offset: 0x88 */
       uint8_t RESERVED_10[3];
  __IO uint8_t ERREN;                              /**< Error Interrupt Enable register, offset: 0x8C */
       uint8_t RESERVED_11[3];
  __I  uint8_t STAT;                               /**< Status register, offset: 0x90 */
       uint8_t RESERVED_12[3];
  __IO uint8_t CTL;                                /**< Control register, offset: 0x94 */
       uint8_t RESERVED_13[3];
  __IO uint8_t ADDR;                               /**< Address register, offset: 0x98 */
       uint8_t RESERVED_14[3];
  __IO uint8_t BDTPAGE1;                           /**< BDT Page register 1, offset: 0x9C */
       uint8_t RESERVED_15[3];
  __IO uint8_t FRMNUML;                            /**< Frame Number register Low, offset: 0xA0 */
       uint8_t RESERVED_16[3];
  __IO uint8_t FRMNUMH;                            /**< Frame Number register High, offset: 0xA4 */
       uint8_t RESERVED_17[3];
  __IO uint8_t TOKEN;                              /**< Token register, offset: 0xA8 */
       uint8_t RESERVED_18[3];
  __IO uint8_t SOFTHLD;                            /**< SOF Threshold register, offset: 0xAC */
       uint8_t RESERVED_19[3];
  __IO uint8_t BDTPAGE2;                           /**< BDT Page Register 2, offset: 0xB0 */
       uint8_t RESERVED_20[3];
  __IO uint8_t BDTPAGE3;                           /**< BDT Page Register 3, offset: 0xB4 */
       uint8_t RESERVED_21[11];
  struct {                                         /* offset: 0xC0, array step: 0x4 */
    __IO uint8_t ENDPT;                              /**< Endpoint Control register, array offset: 0xC0, array step: 0x4 */
         uint8_t RESERVED_0[3];
  } ENDPOINT[16];
  __IO uint8_t USBCTRL;                            /**< USB Control register, offset: 0x100 */
       uint8_t RESERVED_22[3];
  __I  uint8_t OBSERVE;                            /**< USB OTG Observe register, offset: 0x104 */
       uint8_t RESERVED_23[3];
  __IO uint8_t CONTROL;                            /**< USB OTG Control register, offset: 0x108 */
       uint8_t RESERVED_24[3];
  __IO uint8_t USBTRC0;                            /**< USB Transceiver Control register 0, offset: 0x10C */
       uint8_t RESERVED_25[7];
  __IO uint8_t USBFRMADJUST;                       /**< Frame Adjust Register, offset: 0x114 */
} USB_Type;

enum {
	USB_INTEN_STALLEN_Shift = 7,
	USB_INTEN_STALLEN_Mask = 1 << USB_INTEN_STALLEN_Shift,
	USB_INTEN_ATTACHEN_Shift = 6,
	USB_INTEN_ATTACHEN_Mask = 1 << USB_INTEN_ATTACHEN_Shift,
	USB_INTEN_RESUMEEN_Shift = 5,
	USB_INTEN_RESUMEEN_Mask = 1 << USB_INTEN_RESUMEEN_Shift,
	USB_INTEN_SLEEPEN_Shift = 4,
	USB_INTEN_SLEEPEN_Mask = 1 << USB_INTEN_SLEEPEN_Shift,
	USB_INTEN_TOKDNEEN_Shift = 3,
	USB_INTEN_TOKDNEEN_Mask = 1 << USB_INTEN_TOKDNEEN_Shift,
	USB_INTEN_SOFTOKEN_Shift = 2,
	USB_INTEN_SOFTOKEN_Mask = 1 << USB_INTEN_SOFTOKEN_Shift,
	USB_INTEN_ERROREN_Shift = 1,
	USB_INTEN_ERROREN_Mask = 1 << USB_INTEN_ERROREN_Shift,
	USB_INTEN_USBRSTEN_Mask = 1,

	USB_ISTAT_STALL_Shift = 7,
	USB_ISTAT_STALL_Mask = 1 << USB_ISTAT_STALL_Shift,
	USB_ISTAT_ATTACH_Shift = 6,
	USB_ISTAT_ATTACH_Mask = 1 << USB_ISTAT_ATTACH_Shift,
	USB_ISTAT_RESUME_Shift = 5,
	USB_ISTAT_RESUME_Mask = 1 << USB_ISTAT_RESUME_Shift,
	USB_ISTAT_SLEEP_Shift = 4,
	USB_ISTAT_SLEEP_Mask = 1 << USB_ISTAT_SLEEP_Shift,
	USB_ISTAT_TOKDNE_Shift = 3,
	USB_ISTAT_TOKDNE_Mask = 1 << USB_ISTAT_TOKDNE_Shift,
	USB_ISTAT_SOFTOK_Shift = 2,
	USB_ISTAT_SOFTOK_Mask = 1 << USB_ISTAT_SOFTOK_Shift,
	USB_ISTAT_ERROR_Shift = 1,
	USB_ISTAT_ERROR_Mask = 1 << USB_ISTAT_ERROR_Shift,
	USB_ISTAT_USBRST_Mask = 1,

	USB_CTL_JSTATE_Shift = 7,
	USB_CTL_JSTATE_Mask = 1 << USB_CTL_JSTATE_Shift,
	USB_CTL_SE0_Shift = 6,
	USB_CTL_SE0_Mask = 1 << USB_CTL_SE0_Shift,
	USB_CTL_TXSUSPENDTOKENBUSY_Shift = 5,
	USB_CTL_TXSUSPENDTOKENBUSY_Mask = 1 << USB_CTL_TXSUSPENDTOKENBUSY_Shift,
	USB_CTL_RESET_Shift = 4,
	USB_CTL_RESET_Mask = 1 << USB_CTL_RESET_Shift,
	USB_CTL_HOSTMODEEN_Shift = 3,
	USB_CTL_HOSTMODEEN_Mask = 1 << USB_CTL_HOSTMODEEN_Shift,
	USB_CTL_ODDRST_Shift = 1,
	USB_CTL_ODDRST_Mask = 1 << USB_CTL_ODDRST_Shift,
	USB_CTL_USBENSOFEN_Mask = 1,

	USB_USBCTRL_SUSP_Shift = 7,
	USB_USBCTRL_SUSP_Mask = 1 << USB_USBCTRL_SUSP_Shift,
	USB_USBCTRL_PDE_Shift = 6,
	USB_USBCTRL_PDE_Mask = 1 << USB_USBCTRL_PDE_Shift,

	USB_ADDR_LSEN_Shift = 7,
	USB_ADDR_LSEN_Mask = 1 << USB_ADDR_LSEN_Shift,
	USD_ADDR_ADDR_Mask = 0x7F,

	USB_TOKEN_PID_Shift = 4,
	USB_TOKEN_PID_Mask = 0xF << USB_TOKEN_PID_Shift,
	USB_TOKEN_ENDPT_Mask = 0xF,

	USB_ENDPT_HOSTWOHUB_Shift = 7,
	USB_ENDPT_HOSTWOHUB_Mask = 1 << USB_ENDPT_HOSTWOHUB_Shift,
	USB_ENDPT_RETRYDIS_Shift = 6,
	USB_ENDPT_RETRYDIS_Mask = 1 << USB_ENDPT_RETRYDIS_Shift,
	USB_ENDPT_EPCTLDIS_Shift = 4,
	USB_ENDPT_EPCTLDIS_Mask = 1 << USB_ENDPT_EPCTLDIS_Shift,
	USB_ENDPT_EPRXEN_Shift = 3,
	USB_ENDPT_EPRXEN_Mask = 1 << USB_ENDPT_EPRXEN_Shift,
	USB_ENDPT_EPTXEN_Shift = 2,
	USB_ENDPT_EPTXEN_Mask = 1 << USB_ENDPT_EPTXEN_Shift,
	USB_ENDPT_EPSTALL_Shift = 1,
	USB_ENDPT_EPSTALL_Mask = 1 << USB_ENDPT_EPSTALL_Shift,
	USB_ENDPT_EPHSHK_Mask = 1,

	USB_CONTROL_DPPULLUPNONOTG_Shift = 4,
	USB_CONTROL_DPPULLUPNONOTG_Mask = 1 << USB_CONTROL_DPPULLUPNONOTG_Shift,
} usb_fields_t;

/// Reference manual Table 35-3
typedef struct {
	/// Config
	uint32_t config;

	/// 32-bit buffer address
	///
	/// Address of the buffer where TX and RX data are written
	uint32_t bufAddr;
} bdt_t;

typedef enum {
	USBH_BDT_TOKPID_Shift = 2,
	USBH_BDT_TOKPID_Mask = 0xF << USBH_BDT_TOKPID_Shift,
	USBH_BDT_BDTSTALL_Shift = 2,
	USBH_BDT_BDTSTALL_Mask = 1 << USBH_BDT_BDTSTALL_Shift,
	USBH_BDT_DTS_Shift = 3,
	USBH_BDT_DTS_Mask = 1 << USBH_BDT_DTS_Shift,
	USBH_BDT_NINC_Shift = 4,
	USBH_BDT_NINC_Mask = 1 << USBH_BDT_NINC_Shift,
	USBH_BDT_KEEP_Shift = 5,
	USBH_BDT_KEEP_Mask = 1 << USBH_BDT_KEEP_Shift,
	USBH_BDT_DATAX_Shift = 6,
	USBH_BDT_DATAX_Mask = 1 << USBH_BDT_DATAX_Shift,
	USBH_BDT_OWN_Shift = 7,
	USBH_BDT_OWN_Mask = 1 << USBH_BDT_OWN_Shift,
	USBH_BDT_BC_Shift = 16,
	USBH_BDT_BC_Mask = 0x3FF << USBH_BDT_BC_Shift,
} usbh_bdt_fields_t;

// TODO set address
volatile USB_Type * const volatile hw_usb0;

#endif /* INCLUDE_HW_USB_H_ */
