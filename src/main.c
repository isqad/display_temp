#include "stm32f10x.h"

volatile uint32_t counter;

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	SystemInit();

	// Enable clock for C and B port
	// Port B is I2C:
	// B3 - SCK
	// B4 - SDA
	// Port C is for blinking diode
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN;
	// enable clock for timer
	// enable clock for I2C2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_I2C2EN;
	//RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;

	// GPIO configuration
	// 13 pin
	GPIOC->CRH |= GPIO_CRH_MODE13; // max speed 50Mhz (bits 11)
	GPIOC->CRH &= ~GPIO_CRH_CNF13; // Reset CNF13[1:0] bits (00) - push pull mode

	// prescaller 24000 - 1 = 1ms
	TIM2->PSC = (uint32_t)(24000 - 1);
	TIM2->ARR = (uint32_t)(3000 - 1);
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;

	NVIC_EnableIRQ(TIM2_IRQn);
	counter = 1;

	/* Infinite loop */
	while (1)
	{
	}

	return 0;
}

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;

	if (counter == 1)
	{
		GPIOC->BSRR |= GPIO_BSRR_BR13;
		counter = 0;
	} else {
		GPIOC->BSRR |= GPIO_BSRR_BS13;
		counter = 1;
	}
}
