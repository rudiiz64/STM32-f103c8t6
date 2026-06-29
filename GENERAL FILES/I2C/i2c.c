#include "i2c.h"


/* I2C Configuration
	I2C_CR2 must have input clock of 2MHz (Sm) or 4 MHz (Fm)
	1. Reset I2C
	2. Program peripheral clock in I2C_CR2
	3. Configure clock control reg
	4. Configure rise time reg
	5. Program I2C_CR1 reg to enable peripheral
*/
void I2C_Config(){
	// Enable I2C clock and GPIO clock (Port B)
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; 
	
	// Configure the GPIO pins
	// SCL (Alt Funct output open drain) - PB6
	GPIOB->CRL |= GPIO_CRL_MODE6_0;									// Output 10MHz
	GPIOB->CRL |= GPIO_CRL_CNF6;										// Alt funct output open drain
	
	// SDA (Alt Funct output open drain) - PB7
	GPIOB->CRL |= GPIO_CRL_MODE7_0;									// Output 10MHz (01)
	GPIOB->CRL |= GPIO_CRL_CNF7;										// Alt funct output open drain (11)
	
	
	I2C1->CR1 |= I2C_CR1_SWRST;											// Enable SWRST (bit 15)
	I2C1->CR1 &= ~(I2C_CR1_SWRST);									// Disable SWRST
	I2C1->CR2 |= (36 << 0);													// APB clock = 36 MHz
	I2C1->CCR &= ~(I2C_CCR_FS);											// Sm mode
	I2C1->CCR |= (205 << 0);												// CCR = (tscl + tsclh) / tpclk1 = 205.206
	I2C1->CCR &= ~(I2C_CCR_DUTY);										// DUTY = 0 (using Sm)
	I2C1->TRISE |= (37 << 0);												// TRISE = [trise(scl)/tpclk1] + 1 = [1000 ns / 27.77] + 1 
	
	I2C1->CR1 |= I2C_CR1_PE;												// Enable peripheral
}

void I2C_Start(){
	I2C1->CR1 |= I2C_CR1_START;													// Generate START
	while (!(I2C1->SR1 & I2C_SR1_SB));									// Wait for SB bit to set in SR1
}


void I2C_Stop(){
	I2C1->CR1 |= I2C_CR1_STOP;													// Set STOP bit 
}

void I2C_Addr(uint8_t addr){
	I2C1->DR = addr;																		// Send slave addr to DR
	while (!(I2C1->SR1 & I2C_SR1_ADDR));								// Wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;								// Read both SR1 and SR2 to clear ADDR
}

void I2C_tx(uint8_t data){
	while (!(I2C1->SR1 & I2C_SR1_TXE));									// Wait for TxE to set
	I2C1->DR = data;																		// Put data in DR
	while (!(I2C1->SR1 & I2C_SR1_BTF));									// Wait for BTF to set (program stop request)
}

/* I2C Receive



*/
void I2C_rx(){
	
}

void I2C_multitx(uint8_t *data, uint8_t size){
	while (!(I2C1->SR1 & I2C_SR1_TXE));								// Wait for TxE to set
	while (size){																			// While size of data > 0
		while (!(I2C1->SR1 & I2C_SR1_TXE));							// Wait for TxE to set (loop)
		I2C1->DR = (volatile uint8_t) *data++;					// Put data in DR then increment mem addr
		size--;																					// Decrement size
	}
	while (!(I2C1->SR1 & I2C_SR1_BTF));								// Wait for BTF to set
}