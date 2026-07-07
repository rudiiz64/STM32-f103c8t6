#include "stm32f10x.h"

#include "RccConfig.h"
#include "delay_f103.h"
#include "extINT.h"

int flag;
int counter;

int main(){
	SysClockConfig();
	gpio_config();
	interrupt_config();
	
	
	while(1){
		flag = IRQ_Handler();
		if (flag){
			delay_ms(100);
			counter++;
			flag = 0;
		}
	}
}