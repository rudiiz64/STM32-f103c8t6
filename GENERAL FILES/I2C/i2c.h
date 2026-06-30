#include "stm32f10x.h"

void I2C_Config();

void I2C_Start();

void I2C_Stop();

void I2C_tx(uint8_t data, uint8_t addr);

void I2C_rx(uint8_t addr, uint8_t *buf, uint8_t size);