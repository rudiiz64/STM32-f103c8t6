#include "stm32f10x.h"
#include <stdio.h>

/* Setting up necessary clocks (RCC, GPIO) */
void SysClockConfig(void){
	// Set MCO to be HSE for debugging
	RCC->CFGR |= RCC_CFGR_MCO_HSE;
	
	// 1. Enable HSE (High-Speed External) and wait for the HSE to become ready
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));
	
	// 2. Set the POWER ENABLE CLOCK and voltage regulator
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	// 3. Configure the FLASH PREFETCH and the LATENCY related settings
	FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
	
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE; // PREDIV1  = /1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;		// AHB,  PS = /1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;		// APB1, PS = /2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;		// APB2, PS = /1
	
	// 5. Configure the MAIN PLL (Phase lock loop)
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9;
	
	// 6. Enable the PPL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	
	// 7. Select the Clock Source and wait for it be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
}

/* Testing clocks utilizing GPIO test */
void GPIO_config (void){
	// 1. Enable the GPIO Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		// Enable clock for Port A IO bank
	
	// 2. Set the Pin as OUTPUT
	GPIOA->CRL |= GPIO_CRL_MODE0_1;				// Enable PA0

	// 3. Configure the OUTPUT mode
	
}

/* Simple delay function to allow blinking of LED */
void delay(volatile uint32_t time){			// Compiler was optimizing function, volatile necessary to run function
	while (time-- > 0);
}

int main (){
	SysClockConfig();
	GPIO_config();
	
	while (1){
		GPIOA->BSRR = GPIO_BSRR_BS0;		// Enable BSRR PA0 (1)
		delay(1000000);
		GPIOA->BSRR = GPIO_BSRR_BR0;		// Disable BSRR PA0 (0)
		delay(1000000);
	}
	
}