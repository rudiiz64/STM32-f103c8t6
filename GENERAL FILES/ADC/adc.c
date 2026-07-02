#include "adc.h"
#include "delay_f103.h"

/* ADC Config
	1. Enable clock and GPIO
	2. Set parameters in CR1 (Scan Mode)
	3. Set parameters in CR2 (CONT, CAL)
	4. Set Sampling Time for channels in ADC_SMPR2
	5. Set Regular Channel sequence length in ADC_SQR1
	6. Set GPIO pins
*/

void adc_config(){
	// RCC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;												// Enable ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;												// Enable IO A bank
	
	// CR1
	ADC1->CR1 |= ADC_CR1_SCAN;																// If using multiple channels, scan them for input
	
	// C2
	ADC1->CR2 |= ADC_CR2_CONT;																// Continuous mode
	ADC1->CR2 |= ADC_CR2_DMA;																	// Set DMA since SCAN is enabled
	ADC1->CR2 &= ~(ADC_CR2_ALIGN);														// Set to right alignment
	
	// SMPR2
	ADC1->SMPR2 |= ADC_SMPR2_SMP1_0 | ADC_SMPR2_SMP0_0;				// Channel 0 & 1 will be sampled for 7.5 cycles
	
	// SQR1
	ADC1->SQR1 |= ADC_SQR1_L_0;																// 2 channels, 2 converions
	
	// GPIO - PA0 & PA1
	GPIOA->CRL &= ~(GPIO_CRL_MODE0);													// PA0 Input mode
	GPIOA->CRL &= ~(GPIO_CRL_CNF0);														// PA0 Analog mode
	GPIOA->CRL &= ~(GPIO_CRL_MODE1);													// PA1 Input mode
	GPIOA->CRL &= ~(GPIO_CRL_CNF1);														// PA1 Analog mode
}

void adc_on(){
	ADC1->CR2 |= ADC_CR2_ADON;																// Turn on ADC
	delay_us(1);																							// tstab = 1 us
	ADC1->CR2 |= ADC_CR2_ADON;																// Set again to start converion

}

void adc_read(int ch){
	ADC1->SQR3 = 0;																						// Reset SQR3 reg
	/* Need to utilize DMA */
	
	ADC1->CR2 |= ADC_CR2_SWSTART;
}	
	
void adc_tempConfig(){
	
}
	
void adc_tempRead(){
	
}