#include <stddef.h>

#include "stm32l0xx.h"
#include "board.h"

uint32_t _SysTick = 0;

#if defined(__GNUC__)
__attribute__((__always_inline__))
#endif /* __GNUC__ */
static inline void
configreGPIO(void)
{
	uint32_t val32;

	RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;

	/* GPIOA */
	val32 = GPIOA->AFR[1];
	val32 = (val32 & ~( 0xFF << (4 * (9 - 8)) )) | ( 0x04 << (4 * (9 - 8)) ); /* GPIOA_9: AF4 -- USART1_TX */
	GPIOA->AFR[1] = val32;

	val32 = GPIOA->MODER;
	val32 = (val32 & ~GPIO_MODER_MODE9) | GPIO_MODER_MODE9_1;
	GPIOA->MODER = val32;

	/* GPIOB */
	//val32 = GPIOB->PUPDR;
	val32 = 0; /* No pull-up, No pull-down*/
	val32 = (val32 & ~GPIO_PUPDR_PUPD15) | GPIO_PUPDR_PUPD15_0; /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD14) | GPIO_PUPDR_PUPD14_0; /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD13) | GPIO_PUPDR_PUPD13_0; /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD12) | GPIO_PUPDR_PUPD12_0; /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD11) | GPIO_PUPDR_PUPD11_0; /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD10) | GPIO_PUPDR_PUPD10_0; /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD9)  | GPIO_PUPDR_PUPD9_0;  /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD8)  | GPIO_PUPDR_PUPD8_0;  /* GPIOB_0: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD7)  | GPIO_PUPDR_PUPD7_0;  /* GPIOB_7: Pull-up -- I2C1_SDA */
	val32 = (val32 & ~GPIO_PUPDR_PUPD6)  | GPIO_PUPDR_PUPD6_0;  /* GPIOB_6: Pull-up -- I2C1_SCL */
	val32 = (val32 & ~GPIO_PUPDR_PUPD3)  | GPIO_PUPDR_PUPD3_0;  /* GPIOB_3: Pull-up -- PowerLED_MB (input) */
	val32 = (val32 & ~GPIO_PUPDR_PUPD2)  | GPIO_PUPDR_PUPD2_0;  /* GPIOB_2: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD1)  | GPIO_PUPDR_PUPD1_0;  /* GPIOB_1: Pull-up -- UNUSED pin */
	val32 = (val32 & ~GPIO_PUPDR_PUPD0)  | GPIO_PUPDR_PUPD0_0;  /* GPIOB_0: Pull-up -- UNUSED pin */
	GPIOB->PUPDR = val32;

	val32 = GPIOB->AFR[0];
	val32 = (val32 & ~( 0xFF << (4 * 7) )) | ( 0x01 << (4 * 7) ); /* GPIOB_7: AF1 -- I2C1_SDA */
	val32 = (val32 & ~( 0xFF << (4 * 6) )) | ( 0x01 << (4 * 6) ); /* GPIOB_6: AF1 -- I2C1_SCL */
	GPIOB->AFR[0] = val32;

	val32 = GPIOB->MODER;
	val32 = (val32 & ~GPIO_MODER_MODE7) | GPIO_MODER_MODE7_1; /* GPIOB_7: I2C1_SDA */
	val32 = (val32 & ~GPIO_MODER_MODE6) | GPIO_MODER_MODE6_1; /* GPIOB_6: I2C1_SCL */
	val32 = (val32 & ~GPIO_MODER_MODE5) | GPIO_MODER_MODE5_0; /* GPIOB_5: OutRedLED_Drv */
	val32 = (val32 & ~GPIO_MODER_MODE4) | GPIO_MODER_MODE4_0; /* GPIOB_4: OutGreenLED_Drv */
	val32 = (val32 & ~GPIO_MODER_MODE3); /* GPIOB_3: PowerLED_MB (input) */
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
#warning FIXME: hardcode for debug;
	SysTick_Config(2100000/1000); /* 1 ms */
	configreGPIO();
	configreUSART1withDMA();

	usart1_transmit_with_dma(test_msg, sizeof(test_msg));
	while (0 != (DMA1_Channel2->CCR & DMA_CCR_EN) ) {

	}
	usart1_transmit_with_dma(test_msg, sizeof(test_msg));
}

void
main_loop(void)
{
	uint32_t previousMillis = 0;
	uint32_t currentMillis;

	for (;;) {
		currentMillis = get_systick();

		if(currentMillis - previousMillis >= 200) {
			previousMillis = currentMillis;

			GPIOB->ODR ^= GPIO_ODR_OD5;
			GPIOB->ODR ^= GPIO_ODR_OD4;
		}

		uart_print(test_msg, sizeof(test_msg));
	}
}
