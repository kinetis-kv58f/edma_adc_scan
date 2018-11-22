// © 2011. Ravi Teja Gudapati. All rights reserved.

#ifndef INCLUDE_HW_NVIC_H_
#define INCLUDE_HW_NVIC_H_

#include "types.h"
#include "hw/common.h"

/// Low value = High prio. Range 0(highest) - 15(lowest)
enum {
	/// Debug UART
	NVIC_Prio_uart = 4,
	/// USB
	NVIC_Prio_usb = 5,
};

typedef enum IRQn {
	/* Auxiliary constants */
	NVIC_IRQn_NotAvail = -128, /**< Not available device specific interrupt */

	/* Core interrupts */
	NVIC_IRQn_NonMaskableInt = -14, /**< Non Maskable Interrupt */
	NVIC_IRQn_HardFault = -13, /**< Cortex-M7 SV Hard Fault Interrupt */
	NVIC_IRQn_MemoryManagement = -12, /**< Cortex-M7 Memory Management Interrupt */
	NVIC_IRQn_BusFault = -11, /**< Cortex-M7 Bus Fault Interrupt */
	NVIC_IRQn_UsageFault = -10, /**< Cortex-M7 Usage Fault Interrupt */
	NVIC_IRQn_SVCall = -5, /**< Cortex-M7 SV Call Interrupt */
	NVIC_IRQn_DebugMonitor = -4, /**< Cortex-M7 Debug Monitor Interrupt */
	NVIC_IRQn_PendSV = -2, /**< Cortex-M7 Pend SV Interrupt */
	NVIC_IRQn_SysTick = -1, /**< Cortex-M7 System Tick Interrupt */

	/* Device specific interrupts */
	NVIC_IRQn_DMA0 = 0, /**< DMA channel 0 transfer complete and error interrupt */
	NVIC_IRQn_DMA1 = 1, /**< DMA channel 1 transfer complete and error interrupt */
	NVIC_IRQn_DMA2 = 2, /**< DMA channel 2 transfer complete and error interrupt */
	NVIC_IRQn_DMA3 = 3, /**< DMA channel 3 transfer complete and error interrupt */
	NVIC_IRQn_Reserved20 = 4, /**< Reserved interrupt */
	NVIC_IRQn_FTFA = 5, /**< FTFA command complete and read collision */
	NVIC_IRQn_LVD_LVW = 6, /**< Low-voltage detect, low-voltage warning */
	NVIC_IRQn_LLWU = 7, /**< Low Leakage Wakeup */
	NVIC_IRQn_I2C0 = 8, /**< I2C0 interrupt */
	NVIC_IRQn_I2C1 = 9, /**< I2C1 interrupt */
	NVIC_IRQn_SPI0 = 10, /**< SPI0 single interrupt vector for all sources */
	NVIC_IRQn_SPI1 = 11, /**< SPI1 single interrupt vector for all sources */
	NVIC_IRQn_UART0 = 12, /**< UART0 status and error */
	NVIC_IRQn_UART1 = 13, /**< UART1 status and error */
	NVIC_IRQn_UART2 = 14, /**< UART2 status and error */
	NVIC_IRQn_ADC0 = 15, /**< ADC0 interrupt */
	NVIC_IRQn_CMP0 = 16, /**< CMP0 interrupt */
	NVIC_IRQn_TPM0 = 17, /**< TPM0 single interrupt vector for all sources */
	NVIC_IRQn_TPM1 = 18, /**< TPM1 single interrupt vector for all sources */
	NVIC_IRQn_TPM2 = 19, /**< TPM2 single interrupt vector for all sources */
	NVIC_IRQn_RTC = 20, /**< RTC alarm interrupt */
	NVIC_IRQn_RTC_Seconds = 21, /**< RTC seconds interrupt */
	NVIC_IRQn_PIT = 22, /**< PIT single interrupt vector for all channels */
	NVIC_IRQn_I2S0 = 23, /**< I2S0 Single interrupt vector for all sources */
	NVIC_IRQn_USB0 = 24, /**< USB0 OTG */
	NVIC_IRQn_DAC0 = 25, /**< DAC0 interrupt */
	NVIC_IRQn_TSI0 = 26, /**< TSI0 interrupt */
	NVIC_IRQn_MCG = 27, /**< MCG interrupt */
	NVIC_IRQn_LPTMR0 = 28, /**< LPTMR0 interrupt */
	NVIC_IRQn_Reserved45 = 29, /**< Reserved interrupt */
	NVIC_IRQn_PORTA = 30, /**< PORTA pin detect */
	NVIC_IRQn_PORTC_PORTD = 31 /**< Single interrupt vector for PORTC and PORTD pin detect */
} IRQn_Type;

typedef struct {
	/// Interrupt Set Enable Register
	__IOM uint32_t ISER[8U];
	uint32_t RESERVED0[24U];
	/// Interrupt Clear Enable Register
	__IOM uint32_t ICER[8U];
	uint32_t RSERVED1[24U];
	/// Interrupt Set Pending Register
	__IOM uint32_t ISPR[8U];
	uint32_t RESERVED2[24U];
	/// Interrupt Clear Pending Register
	__IOM uint32_t ICPR[8U];
	uint32_t RESERVED3[24U];
	/// Interrupt Active bit Register
	__IOM uint32_t IABR[8U];
	uint32_t RESERVED4[56U];
	/// Interrupt Priority Register (8Bit wide)
	__IOM uint8_t IP[240U];
	uint32_t RESERVED5[644U];
	/// Software Trigger Interrupt Register
	__OM uint32_t STIR;
} NVIC_Type;

enum {
	NVIC_IP_Prio_Shift = 4, NVIC_IP_Prio_Mask = 0xF << NVIC_IP_Prio_Shift,
} nvic_fields_t;

volatile NVIC_Type * volatile const hw_nvic = (NVIC_Type *) Memmap_NVIC_Addr;

#endif /* INCLUDE_HW_NVIC_H_ */
