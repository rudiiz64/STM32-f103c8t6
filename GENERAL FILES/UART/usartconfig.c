#include "usartconfig.h"

/* Steps Taken
	1. Enable USART1 clock and GPIO clock
	2. Configure USART pins
	3. Enable the USART by writing the UE = 1 in USART_CR1 
	4. Program the M bit in USART_CR1 to define word length
	5. Program the number of stop bits in USART_CR2
	6. Select DMA enable (DMAT) in USART_CR3 if Multi-Buffer Communication is to take place. Configure the DMA reg
	7. Select the desired baud rate in the USART_BRR reg
	8. Set the TE bit in USART_CR1 to send an idle frame as first transmission
*/

void usart1_config(){
	// 1. Enable USART1 clock and GPIO clock
	RCC->APB2ENR = RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
	
	// 2. Configure USART pins to be alternate functions for PA9 PA10
	/* PA9 */
	GPIOB->CRH |= GPIO_CRH_MODE9_0;							// Output 10 MHz
	GPIOB->CRH |= GPIO_CRH_CNF9_1;							// Alternate Function output
	
	/* PA10 */
	GPIOA->CRH &= ~(GPIO_CRH_MODE10);						// Input
	GPIOA->CRH |= GPIO_CRH_CNF10_0;							// Input push pull
	
	USART1->CR1 = 0x00;													// Reset CR1
	USART1->CR1 |= USART_CR1_UE;								// Enable USART1
	USART1->CR1 &= ~(USART_CR1_M);							// 8 bits
	USART1->CR2 &= ~(1 << 12);									// 1 stop bit
	
	// Baud rate, refer to p. 798
	USART1->BRR |= (39 << 4) | (1 << 0);				// USARTDIV = 72 MHz / (16*115200) -> 39.0625
	USART1->CR1 |= USART_CR1_TE;								// Transmit enable
	USART1->CR1 |= USART_CR1_RE;								// Receive enable
}

/********************************************************  USART TX ******************************************************** 
	1. Write the data to send in the USART_DR reg (clears the TXE bit). Repeat for each data to be transmitted in case of single buffer
	2. After writing the last data in the USART_DR reg, wait until TC = 1 (means transmission of the last frame is done). This is required for when USART is disabled or enters Halt mode
*****************************************************************************************************************************/
void usart1_txChar(uint8_t data){
	USART1->DR = data;													// Load data into data reg
	while(!(USART1->DR & USART_SR_TC));					// Wait for TC to set
	
}

void usart2_txString(char *string){
	while (*string){
		usart1_txChar(*string++);
	}
}

/********************************************************  USART RX ******************************************************** 
	1. Set the RE bit in USART_CR1. This enables receiver to look for a start bit.
*****************************************************************************************************************************/
uint8_t usart1_rxChar(){
	uint8_t temp;
	while (!(USART1->CR1 & USART_SR_RXNE));			// Wait for RXNE to be ready
	temp = USART1->DR;													// Transfer data from reg to temp
	return temp;																// Return data
}