/* System Defines */
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

#include "RccConfig.h"
#include "delay_f103.h"

/* RTOS Defines */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

/* Global Define */
volatile uint16_t LED_VAL = pdTRUE;
uint16_t LED1 = GPIO_Pin_1;

/* Struct */
typedef struct PH_STRUCT{
	// Struct will be modified later; placeholder def
	uint16_t currPin;
	char structMem2;
	
} phStruct;
phStruct pinStruct;

void gpio_Config(){
	/* Enable APB2 Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* Instantiating structs for GPIO config */
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Initializing pins PA0 (Output PP), PA15 (NSS1) */
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* Initializing pins B3 (SCK1), B4 (MISO1), B5 (MOSI1) for SPI */
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* Initial goal will be two define two tasks using FreeRTOS (priority define as 1 > 2):
		1. LED Control
		2. LCD Screen
	
	Second goal will be to add a third task, vControl, which will perform computations to adjust the PWM of the LED output resulting in the following prio:
		1. Control algorithm
		2. LED Control
		3. LCD Screen
*/

void ledControlTask(){
	//uint16_t *pin = (uint16_t *) pvParam;
	/* Toggle LED */
	if (LED_VAL){
		/* Writing only to PA0 for now */
		//GPIO_WriteBit(GPIOA, pinStruct.currPin, LED_VAL);
		GPIO_SetBits(GPIOA, pinStruct.currPin);
		//GPIO_Write(GPIOA, LED_VAL);
		//GPIOA->BSRR |= (LED_VAL << 0);
		LED_VAL = 0;
	}
	else {
		GPIO_WriteBit(GPIOA, pinStruct.currPin, LED_VAL);
		//GPIO_Write(GPIOA, LED_VAL);
		//GPIOA->BSRR |= ~(LED_VAL << 0) << 16;
		LED_VAL = 1;
	}	
}

void lcdControlTask(){
	
}

void ledPWMTask(){

}

int main(void){
	SysClockConfig();
	gpio_Config();
	TIM2_Config();
	
	phStruct pinStruct;
	pinStruct.currPin = GPIO_Pin_0;
	
	/* LED Task Function, LED Task, Normal stack size, pass PIN, HIGHEST Priority, No Handle */
	xTaskCreate(ledControlTask, "LED Task", configMINIMAL_STACK_SIZE, (void*) &LED1, 1, NULL);
	for(;;){
		ledControlTask();
		delay_ms(500);
	}
	return 0;
}
