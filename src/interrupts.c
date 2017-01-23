#include "stm32l0xx.h"

void DMA1_Channel2_3_IRQHandler(void)
{
	if( 0 != (DMA1->ISR & DMA_ISR_TCIF2) ) {
		DMA1->IFCR = DMA_IFCR_CTCIF2;/* Clear TC flag */
		DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	}
}

