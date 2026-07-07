#include "adc.h"
#include "delay_f103.h"

/* DMA Config 
	1. Set peripheral reg address in DMA_CPAR reg (data will be moved from/to this address to/from memory after peripheral event)
	2. Set mem address in DMA_CMAR reg (data will be written to or read from this mem after periph event)
	3. Configure total number of data to be transferred in the DMA_CNDTR reg (after each event, value is decremented)
	4. Configure channel priority using PL[1:0] bits in DMA_CCR reg
	5. Configure data transfer direction, circular mode, periph & mem incremented mode, periph & mem data size, and interrupt after half and/or full transfer 
		 in DMA_CCR reg
*/

void DMA_Init(){
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	// DMA CCR
	DMA1_Channel1->CCR &= ~(DMA_CCR1_DIR);									// DMA1 Channel 1 (ADC1) data transfer direction (read from periph)
	DMA1_Channel1->CCR |= DMA_CCR1_CIRC;										// Circular mode, since we are using SCAN in ADC
	DMA1_Channel1->CCR |= DMA_CCR1_MINC;										// Increment mem after read
	DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;									// Peripheral size: 16 bits (12 bit res)
	DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;	
}

void DMA_config(uint32_t srcAddr, uint32_t destAddr, uint8_t size){
	// DMA CNDTR
	DMA1_Channel1->CNDTR = size;
	
	// DMA CPAR & CMAR
	DMA1_Channel1->CPAR = srcAddr;												// DMA 1 Ch1 points to selected ADC DR
	DMA1_Channel1->CMAR = destAddr;												// Write peripheral data to some mem
	
	// DMA CCR
	DMA1_Channel1->CCR |= DMA_CCR1_EN;										// Enable DMA channel
}


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
	ADC1->SMPR2 |= (0 << 0);																	// Channel 1 will be sampled for 7.5 cycles
	ADC1->SMPR2 |= (0 << 3);																	// Channel 3 will be sampled for 7.5 cycles
	ADC1->SMPR1 |= (6 << 18);																	// Channel 16: 17.1 us = Sampling time + 12.5 / 4.5MHz -> 64.45 cycles
	
	// SQR1
	ADC1->SQR1 |= (2 << 20);																	// 3 channels, 3 conversions
	
	// SQR3
	ADC1->SQR3 |= (1 << 0);																		// SQ1 = Ch 1, bits correlate to what channel is sequenced
	ADC1->SQR3 |= (3 << 5);																		// SQ2 = Ch 3
	ADC1->SQR3 |= (16 << 10);																	// SQ3 = Ch 16 (temp sensor)
		
	// GPIO - PA0 & PA1
	GPIOA->CRL &= ~(GPIO_CRL_MODE1);													// PA1 Input mode
	GPIOA->CRL &= ~(GPIO_CRL_CNF1);														// PA1 Analog mode
	GPIOA->CRL &= ~(GPIO_CRL_MODE3);													// PA3 Input mode
	GPIOA->CRL &= ~(GPIO_CRL_CNF3);														// PA3 Analog mode
}

void adc_on(){
	ADC1->CR2 |= ADC_CR2_TSVREFE | ADC_CR2_ADON;						  // Enable ADC & temp sense Vref
	delay_us(1);																							// tstab = 1 us
	ADC1->CR2 |= ADC_CR2_ADON;																// Set again to start converion

}

void adc_start(){
	ADC1->SR = 0;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}	

/* Temperature Sensor
	1. Select ADCx_IN16
	2. Select sample time of 17.1 us
	3. Set TSVREFE bit in ADC_CR2
	4. Start ADC converstion by setting ADCON bit
	5. Read resulting Vsense data in ADC DR
	6. Obatin temp using the conversion: C_temp = [(V25-Vsense)/ avg_slope] + 25
		 V25 = 1.43V per datasheet
		 avg_slope = 4.3mV/C per datasheet
*/
double adc_tempRead(uint16_t *data){
	float temp = ((1.43 - ((float)(3.3*data[2]/(float) 4095))) / 0.0043) + 25; 		// temp in C = [(V25 - Vsense) / avg_slope] - 25, V25 & avg_slope in datasheet; Vsense = (VDD * adc_val) / resolution
	return temp;
}