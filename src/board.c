#include <stddef.h>

#include "stm32l0xx.h"

#if defined(__GNUC__)
__attribute__((__always_inline__))
#endif /* __GNUC__ */
static inline void
configreGPIO(void)
{
	uint32_t val32;

	RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;

	/* GPIOA*/
	val32 = GPIOA->AFR[1];
	val32 = (val32 & ~( 0xFF << (4 * (9 - 8)) )) | ( 0x04 << (4 * (9 - 8)) ); /* GPIOA_9: AF4 -- USART1_TX */
	GPIOA->AFR[1] = val32;

	val32 = GPIOA->MODER;
	val32 = (val32 & ~GPIO_MODER_MODE9) | GPIO_MODER_MODE9_1;
	GPIOA->MODER = val32;

	val32 = GPIOB->MODER;
	val32 = (val32 & ~GPIO_MODER_MODE5) | GPIO_MODER_MODE5_0; /* GPIOB_5: OutRedLED_Drv */
	GPIOB->MODER = val32;
}

#if defined(__GNUC__)
__attribute__((__always_inline__))
#endif /* __GNUC__ */
static inline void
configreUSART1withDMA(void)
{
	uint32_t val32;

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; /* Enable the peripheral clock USART1 */
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; /* Enable the peripheral clock DMA1 */

	USART1->CR1 = 0; /* Disable USART for configure */

	USART1->BRR = 2100000 / 9600;

	USART1->CR3 = USART_CR3_DMAT; /* Enable DMA in transmission */

	val32 = 0;
	val32 |= USART_CR1_TE;
	val32 |= USART_CR1_UE;
	USART1->CR1 = val32;

	while( 0 == (USART1->ISR & USART_ISR_TC) ) { /* polling idle frame Transmission */
		/* add time out here for a robust application */
	}

//	__NOP();
//	USART1->TDR = 'z';

	/* DMA1 Channel2 USART1_TX config */
	/* (1)  Map USART1_TX DMA channel */
	/* (2)  Peripheral address */
	/* (3)  Memory address */
	/* (4)  Memory increment */
	/*      Memory to peripheral */
	/*      8-bit transfer */
	/*      Transfer complete IT */
	DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C2S) | (3 << (1 * 4)); /* (1) */
	DMA1_Channel2->CPAR = (uint32_t)&(USART1->TDR); /* (2) */
	//DMA1_Channel2->CMAR = (uint32_t)stringtosend; /* (3) */
	DMA1_Channel2->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE; /* (4) */

	/* Configure IT */
	/* (10) Set priority for DMA1_Channel2_3_IRQn */
	/* (11) Enable DMA1_Channel2_3_IRQn */
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0); /* (10) */
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn); /* (11) */
}

#if defined(__GNUC__)
__attribute__((__always_inline__))
#endif /* __GNUC__ */
static inline void
usart1_transmit_with_dma(const void *data, const uint16_t len)
{
	DMA1_Channel2->CMAR = (uint32_t)data;
	DMA1_Channel2->CNDTR = len;
	DMA1_Channel2->CCR |= DMA_CCR_EN;
}

int
uart_print(const void *data, const uint16_t len)
{
	if(NULL == data)
		return -1;

	if(0 == len)
		return 0;

	usart1_transmit_with_dma(data, len);
	return 0;
}

static const char test_msg[] = "Hello, World!!!\r\n";
void
peripherals_init(void)
{
	//SystemCoreClockUpdate();
	configreGPIO();
	configreUSART1withDMA();

	usart1_transmit_with_dma(test_msg, sizeof(test_msg));
	while (0 != (DMA1_Channel2->CCR & DMA_CCR_EN) ) {

	}
	usart1_transmit_with_dma(test_msg, sizeof(test_msg));
}

static void
delay(void)
{
    const uint32_t cnt = 100000;

    volatile uint32_t i = cnt;
    while (i > 0) {
        --i;
    }
}

void
main_loop(void)
{
	for (;;) {
		delay();
		GPIOB->ODR ^= GPIO_ODR_OD5;
		//GPIOB->ODR ^= GPIO_ODR_OD4;
		uart_print(test_msg, sizeof(test_msg));
	}
}
