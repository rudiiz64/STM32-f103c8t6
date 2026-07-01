#include "spi.h"


/* Procedure for MOSI = data out, MISO = data in
	1. Enable clock
	2. Select BR[2:0] bits to define baud rate in SPI_CR1
	3. Select CPOL and CPHA bits
	4. Set DFF for 8-bit frame
	5. Configure LSBFIRST bit in SPI_CR1 to define frame format
	6. Input mode: NSS to HI (hw) or set SSM & SSI in SPI_CR1 (sw)
		 Output mode: Set SSOE
	7. MSTR and SPE bits must be set
*/
void spi_config(){
	/* SPI CONFIGURATION */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	// CPOL & CPHA depend on slave devices
	SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;										// 72MHz / 16 = 4.5 MHz speed
	
	SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	SPI1->CR1 &= ~(SPI_CR1_RXONLY);
	SPI1->CR1 &= ~(SPI_CR1_DFF);
	
	SPI1->CR2 = 0;
	
	/* GPIO CONFIGURATION */
	RCC->APB1ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB1ENR |= RCC_APB2ENR_IOPBEN;
	
	// SPI CLK PB3 (Master) -> Alt Funt Push-Pull
	GPIOB->CRL |= GPIO_CRL_MODE3;
	GPIOB->CRL |= GPIO_CRL_CNF3_1;
	
	// SPI MOSI PB5 (Full, master) -> Alt Funct Push-pull
	GPIOB->CRL |= GPIO_CRL_MODE5;
	GPIOB->CRL |= GPIO_CRL_CNF5_1;
	
	// SPI MISO PB4 (Full, master) -> Input floating / input pull-up
	GPIOB->CRL &= ~(GPIO_CRL_MODE4);
	GPIOB->CRL |= GPIO_CRL_CNF4_0;
	
	// SPI NSS PA15 (HW Master) -> Alt Funct Push-pull
	GPIOA->CRH |= GPIO_CRH_MODE15;
	GPIOA->CRH |= GPIO_CRH_CNF15_1;
}

void spi_en(){
	SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_disable(){
	SPI1->CR1 &= ~(SPI_CR1_SPE);
}

/* Transmit procedure
	1. Tx begins when byte is written in Tx buffer
	2. TXE bit set on transfer of data from Tx buffer to shift reg
	2. Depending on LSBFIRST bit in SPI_CR1, data either shifts data out of buffer MSB first or LSB first
*/
void spi_tx(uint8_t *data, int size){
	int i = 0;
	while (size > i){														// Determine if we are at end of data
		while (!(SPI1->SR & SPI_SR_TXE));					// Wait for TXE to set
		SPI1->DR = data[i];												// Data to DR
		i++;
	}
	/* Transmit & Reception
	while (!(SPI1->SR & SPI_SR_RXNE));
	SPI1->DR = data[i];
	while (!(SPI1->SR & (SPI_SR_TXE | SPI_SR_BSY)));
	*/
	
	/* Transmit only */
	while (!(SPI1->SR & SPI_SR_TXE));						// Wait for TXE = 1
	while (SPI1->SR & SPI_SR_BSY);							// Wait for BSY = 0
	uint8_t temp = SPI1->DR;										// Put discarded data in temp
	temp = SPI1->SR;														// Read SR to clear OVR flag
}

/* Receive procedure
	1. RXNE set, data in shift reg transferred to buffer
	2. Interrupt generated if RXNEIE bit set in SPI_CR2
	3. Last sampling clk edge RXNE is set and copies data byte received in shift reg to Rx buffer
	4. When SPI_DR is read, SPI peripheral returns previous
*/
void spi_rx(uint8_t *data, int size){
	while(size){																// Size check
		while(SPI1->SR & SPI_SR_BSY);							// Wait until BSY is low to ensure communication is not in progress
		SPI1->DR = 0;															// Dummy data
		while(!(SPI1->SR & SPI_SR_RXNE));					// Wait until RXNE sets
		*data++ = SPI1->DR;												// Put data from RX buffer into data address
		size--;																		// Reduce size
	}
}