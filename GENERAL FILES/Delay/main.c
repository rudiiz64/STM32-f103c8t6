#include "stm32f10x.h"
#include "delay_f103.h"


void GPIO_Config(void){
	// 1. Enable GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 2. Set output
	GPIOA->CRL |= GPIO_CRL_MODE0_0;		// Enable PA0 output at 10 MHz
}	

int main (){
	//SystemInit();
	GPIO_Config();
	TIM2_Config();
	
	while (1){
		GPIOA->BSRR = GPIO_BSRR_BS0;		// Enable PA0
		delay_ms(1000);									// Delay 1 sec
		GPIOA->BSRR = GPIO_BSRR_BR0;		// Disable PA0
		delay_ms(1000);									// Delay 1 sec
	}
	
}