/* System Defines */
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_exti.h"

#include "RccConfig.h"
#include "delay_f103.h"

/* RTOS Defines */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

/* Global Define */
volatile uint16_t LED_VAL = pdTRUE;
uint16_t *LED1 = (uint16_t *) GPIO_Pin_0;
int flag;
uint16_t counter = 0;

/* Struct */
typedef struct PH_STRUCT{
	// Struct will be modified later; placeholder def
	char structMem1;
	char structMem2;
} phStruct;
phStruct pinStruct;

void EXTI1_IRQHandler(){
	if (EXTI_GetFlagStatus(EXTI_Line1)){
		flag = 1;
		EXTI_ClearFlag(EXTI_Line1);
	}
}

/* MOVE THIS TO DIFF FILE */
void gpio_Config(){
	/* Enable APB2 Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
	
	/* Instantiating structs for GPIO config */
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitiStruct;
	
	/* Initializing pins PA0 (Output PP), PA1 (PU Input Button), PA15 (NSS1) */
	
	// GPIO Setting for PA0 LED (OUT PUSH/PULL)
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// GPIO Config for PA1 BTN (IN PULL UP)
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// EXTI config for PA1 INT
	EXTI_InitiStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitiStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitiStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitiStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitiStruct);
	
	// GPIO Config for PA15 SPI1_NSS1 (IN FLOAT)
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* Initializing pins B3 (SCK1), B4 (MISO1), B5 (MOSI1) for SPI */
	
	// GPIO Config for PB3 SPI1_SCK1 (ALT FUNCT OUT PUSH/PULL)
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// GPIO Config for PB4 SPI1_MISO1 (IN FLOAT)
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// GPIO Config for PB5 SPI1_MOSI1 (ALT FUNCT OUT PUSH/PULL)
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// NVIC Functions for EXTI1
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_EnableIRQ(EXTI1_IRQn);
}

/* Initial goal will be two define two tasks using FreeRTOS (priority define as 1 > 2):
		1. LED Control
		2. LCD Screen
	
	Second goal will be to add a third task, vControl, which will perform computations to adjust the PWM of the LED output resulting in the following prio:
		1. Control algorithm
		2. LED Control
		3. LCD Screen
*/

void ledControlTask(void* pvParam){
	uint16_t *pin = (uint16_t *) pvParam;																			// pin (0x080032AE) -> 0x20000000
	
	/* Toggle LED */
	if (LED_VAL){
		/* Writing only to PA0 for now */
		GPIO_WriteBit(GPIOA, *pin, LED_VAL);																		// pin = 0x20000000 (adr) -> 0x00000001 (val)
		LED_VAL = 0;
	}
	else {
		GPIO_WriteBit(GPIOA, *pin, LED_VAL);
		LED_VAL = 1;
	}	
}

void lcdControlTask(void *pvParam){
	/* Goal of function is to update an LCD with number of button presses */
	int *flag = (int *) pvParam;
	
	// Recast param from void* param to int *param
	// If param = 1, update LCD screen
	if (flag){
		/* Update LCD */
		counter++;
	}
	// If param = 0, do nothing
	
	/* THIS FUNCTION WILL BE UPDATED ONCE VERIFIED SIMPLE FUNCTION WORKS TO INCORPORATE PWM TO CONTROL BRIGHTNESS */
}

void ledPWMTask(){
	/* When BTN press occurs, change PWM of LCD and LED while also updating LCD drawing */
	// If LBTN
		/* 
				Reduce the PWM of both components
				Update LCD drawing to decrease value of "Brightness"
		*/
	
	// If RBTN
		/*
			Increase the PWM of both components
			Update LCD drawing to increase value of "Brightness"
		*/
	
	/* THIS FUNCTION OCCURS ONLY WHEN AN INTERRUPT IS TRIGGERED */
}

int main(void){
	SysClockConfig();
	gpio_Config();
	TIM2_Config();
	
	
	/* LED Task Function, LED Task, Normal stack size, pass PIN, HIGHEST Priority, No Handle */
	xTaskCreate(ledControlTask, "LED Task", configMINIMAL_STACK_SIZE, (void*) &LED1, 1, NULL);
	xTaskCreate(lcdControlTask, "LCD Task", configMINIMAL_STACK_SIZE, NULL &flag, 2, NULL);
	for(;;){
		ledControlTask(&LED1);											// We pass the address of LED1 which contains the value 0x00000001
		lcdControlTask(&flag);
		delay_ms(500);
	}
	return 0;
}
