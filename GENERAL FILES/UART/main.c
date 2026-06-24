
#include "delay_f103.h"
#include "RccConfig.h"
#include "usartconfig.h"

int main(){
	SysClockConfig();
	TIM2_Config();
	
	while(1){

	}
}