#include "i2c.h"


/* I2C Configuration
	I2C_CR2 must have input clock of 2MHz (Sm) or 4 MHz (Fm)
	1. Program peripheral clock in I2C_CR2
	2. Configure clock control reg
	3. Configure rise time reg
	4. Program I2C_CR1 reg to enable peripheral
	5. Set START bit in I2C_CR1 to generate start condition

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
	
	I2C1->CR2 |= I2C_CR2_FREQ_5 | I2C_CR2_FREQ_2;		// APB clock = 36 MHz
	I2C1->CCR &= ~(I2C_CCR_FS);											// Sm mode
	I2C1->CCR |= (225 << 0);												// CCR = (tscl + tsclh) / tpclk1 = 225.22
	I2C1->CCR &= ~(I2C_CCR_DUTY);										// DUTY = 0 (using Sm)
	I2C1->TRISE |= (46 << 0);
	
	I2C1->CR1 |= I2C_CR1_PE;												// Enable peripheral
	I2C1->CR1 |= I2C_CR1_START;											// Generate START
}

/* I2C Setup 
	Operates in SLAVE mode by default, once START is detected mode changes to MASTER
	
	1. Wait for START -> set to MASTER mode
	2. ADDR is received from SDA line and sent to shift register then is compared with ADDR of interface (OAR1) with OAR2 (if ENDUAL = 1) or General Call Addr (ENGC = 1)
	3. If ADDR match, set ACK
	4. ADDR bit set, ITEVFEN set
	5. If ENDUAL = 1, read DUALF


*/

void I2C_tx(){
}

void I2C_rx(){
}