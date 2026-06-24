
#include "delay_f103.h"
#include "RccConfig.h"

int main(){
	SysClockConfig();
	TIM2_Config();
	
	while(1){
	}
}