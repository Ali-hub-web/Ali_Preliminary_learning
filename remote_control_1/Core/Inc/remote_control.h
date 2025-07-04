#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "struct_typedef.h"
#include "main.h"
/// @brief To init the dma recieve port of uart3.
/// @param rx1_buf 
/// @param rx2_buf 
/// @param dma_buf_num 
extern void RC_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

#endif

