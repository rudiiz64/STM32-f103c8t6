#include "stm32f10x.h"
#include <stdint.h>

#define MAX_FREQ 72
#define DIV_MS_US 1000

void TIM2_Config();

void delay_us(volatile uint16_t us);

void delay_ms(volatile uint16_t ms);