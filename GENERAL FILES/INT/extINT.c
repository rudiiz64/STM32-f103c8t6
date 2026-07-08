#include "extINT.h"

void gpio_config(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0xf << 0);
	GPIOA->CRL |= (8 << 0);
	GPIOA->ODR |= (1 << 0);
}

/* Interrupt Configuration
	1. Configure mask bits of 20 interrupt lines (EXTI_IMR)
	2. Configure the Trigger Selection bits of Interrupt lines (EXTI_RTSR & EXTI_FSTR)
	3. Configure the enable and mask bits that control the NVIC IRQ channel mapped to EXTI
	4. Configure edge trigger
	5. Set interrupt priority
	6. Enable interrupt

*/
void interrupt_config(){
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
									
	AFIO->EXTICR[0] &= ~(0xf << 0);									// Setting control for MUX to EXTI0 to select PA0
	
	EXTI->IMR |= (1 << 0);													// Enable interrupt request
	
	EXTI->RTSR |= (1 << 0);													// Set to run on rising edge
	EXTI->FTSR &= ~(1 << 0);												// Disable falling edge detection
	
	NVIC_SetPriority(EXTI0_IRQn, 1);								// Setting EXTI0 interrupt request as HIGH priority
	NVIC_EnableIRQ(EXTI0_IRQn);											// Enable the interrupt on EXTI0
}

