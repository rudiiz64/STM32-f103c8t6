#include "delay_f103.h"


void TIM2_Config(){
	
	// 1. Enable Timer clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	// 2. Set the prescalar and ARR
	TIM2->PSC |= MAX_FREQ - 1;					// CK_CNT = fpsc / (PSC + 1) -> 72 MHz / (TIM2->PSC[15:0] + 1)
	TIM2->ARR |= 0xFFFF - 1;		// Counter range 0 - ARR
	
	// 3. Enable the Timer and wait for the UPDATE FLAG to set
	TIM2->CR1 |= TIM_CR1_CEN;						// Enable the timer
	while (!(TIM2->SR & TIM_SR_UIF));		// While UIF is not set, stay here
}

void delay_us(volatile uint16_t us){
	// 1. Reset counter
	TIM2->CNT = 0;
	
	// 2. Wait for counter to reach entered value. Total wait time will be the required us delay
	while (TIM2->CNT < us);
}

void delay_ms(volatile uint16_t ms){
	for (uint16_t i = 0; i < ms; i++){
		delay_us(DIV_MS_US);		// us = ms / 1000
	}
	
}