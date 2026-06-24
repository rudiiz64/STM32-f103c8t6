#include "stm32f10x.h"

/**
  * @brief  USART1 Configuration
  *         The UART interface is configured as the following: 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 720000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_IN                         = 8
  *            PLL_Mul                        = 9
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 2 WS
  * @param  None
  * @retval None
  */

/* Steps Taken
	1. Enable USART1 clock and GPIO clock
	2. Configure USART pins
*/
void usart1_config();

void usart1_tx(uint8_t data);

void usart1_rx();

/* USART TX
	1. Enable the USART by writing the UE = 1 in USART_CR1 
	2. Program the M bit in USART_CR1 to define word length
	3. Program the number of stop bits in USART_CR2
	4. Select DMA enable (DMAT) un USART_CR3 if Multi-Buffer Communication is to take place. Configure the DMA reg
	5. Select the desired baud rate in the USART_BRR reg
	6. Set the TE bit in USART_CR1 to send an idle frame as first transmission
	7. Write the data to send in the USART_DR reg (clears the TXE bit). Repeat for each data to be transmitted in case of single buffer
	8. After writing the last data in the USART_DR reg, wait until TC = 1 (means transmission of the last frame is done). This is required for when USART is disabled or enters Halt mode
*/


/* USART RX
	1. Enable USART by writing UE = 1 in USART_CR1.
	2. Program M bit in USART_CR1 to define the word length
	3. Program the number of stop bits in USART_CR2
	4. Select DMA enable (DMAR) in USART_CR3 if Multi-Buffer Communication is to take place. Configure the DMA reg
	5. Select the desired baud rate using baud rate reg USART_BRR
	6. Set the RE bit in USART_CR1. This enables receiver to look for a start bit.
*/