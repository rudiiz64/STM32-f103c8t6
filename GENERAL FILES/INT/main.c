#include "stm32f10x.h"

#include "RccConfig.h"
#include "delay_f103.h"
#include "extINT.h"

int flag;
int counter;

void EXTI0_IRQHandler(){
	if (EXTI->PR & (1 << 0)){												// If interrupt triggers
		flag = 1;
		EXTI->PR |= (0 << 0);													// Clear EXTI->PR bit
		
	}
}

int main(){
	SystemInit();
	gpio_config();
	TIM2_Config();
	interrupt_config();
	
	
	while(1){
		if (flag){
			delay_ms(100);
			counter++;
			flag = 0;
		}
	}
}

