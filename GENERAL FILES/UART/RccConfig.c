/**
  * @brief  System Clock Configuration
  *         The System Clock is configured as the following: 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 720000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_IN                         = 8
  *            PLL_Mul                        = 9
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 2 WS
  * @param  None
  * @retval None
  */

#include "RccConfig.h"

/* SysClockConfig Setup:
	1. Enable HSE (High-Speed External) and wait for HSE to be ready
	2. Set the POWER ENABLE CLOCK and voltage regulator
	3. Configure the FLASH PREFETCH and the LATENCY related settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL (Phase lock loop)
	6. Enable the PPL and wait for it to become ready
	7. Select the Clock Source and wait for it be set
*/

void SysClockConfig(void){
	RCC->CR |= RCC_CR_HSEON;																	// Turn HSE ON
	while (!(RCC->CR & RCC_CR_HSERDY));												// Wait until HSE is ready
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;												// Enable PWR CLK
	// Voltage regulator EN by default
	
	FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;			// Flash settings
	
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE; 											// PREDIV1  = /1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;													// AHB,  PS = /1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;													// APB1, PS = /2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;													// APB2, PS = /1
	
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9;			// Set PLL = HSE, PLLMUL = 9
	
	RCC->CR |= RCC_CR_PLLON;																	// Enable PLL
	while (!(RCC->CR & RCC_CR_PLLRDY));												// Wait until PLL is ready
	
	RCC->CFGR |= RCC_CFGR_SW_PLL;															// Select PLL as system clock source
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);		// Wait for system clock switch status to be set
	
}