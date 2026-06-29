#include "stm32f10x.h"

#include "i2c.h"
#include "RccConfig.h"
#include "delay_f103.h"

#define i2c_addr 0x4E

int main(void){
	SysClockConfig();
	TIM2_Config();
	I2C_Config();
	I2C_Start();
	
	while (1){
		I2C_Addr(i2c_addr);
		I2C_tx(0x01);
		I2C_Stop();
	}

}
	