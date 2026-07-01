#include "stm32f10x.h"


void spi_config();
void spi_en();
void spi_disable();
void spi_tx(uint8_t *data, int size);
void spi_rx(uint8_t *data, int size);