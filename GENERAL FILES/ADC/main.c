#include "stm32f10x.h"

#include "RccConfig.h"
#include "delay_f103.h"
#include "adc.h"

uint16_t adcData[3];										// Array of two halfwords
float temp;

int main(){
	SysClockConfig();
	TIM2_Config();
	adc_config();
	adc_on();
	DMA_Init();
	DMA_config((uint32_t ) &ADC1->DR, (uint32_t ) adcData, 2);				// CPAR & CMAR 32-bit reg, cast 32-bit 
	adc_start();
	
	while(1){
		temp = ((1.43 - (float)(3.3*adcData[2]/(float) 4095)) / 0.0043) + 25;
	}
}
