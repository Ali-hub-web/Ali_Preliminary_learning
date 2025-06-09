#ifndef USART_H
#define USART_H

#include "main.h"
#include "struct_typedef.h"


/// @brief init the dma trnsmint port of uart1.

extern void usart1_tx_dma_init(void);
extern void usart1_tx_dma_enable(uint8_t *data, uint16_t len);

#endif
