#include "stm32f10x.h"

void DMA_Init();
void DMA_config(uint32_t srcAddr, uint32_t destAddr, uint8_t size);

void adc_config();
void adc_on();
void adc_start();
double adc_tempRead(uint16_t *data);