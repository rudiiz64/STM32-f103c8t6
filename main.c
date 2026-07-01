#include "stm32f10x.h"

// Board config
#include "RccConfig.h"
#include "delay_f103.h"

// Peripherals
#include "i2c.h"
#include "usartconfig.h"

int main(){
	SysClockConfig();
	
	return 0;
}
