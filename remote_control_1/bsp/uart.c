#include "uart.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
//enable the DMA recieve port of uart1.
void usart1_tx_dma_init(void)
{
    //enable the DMA transfer for the receiver request
    
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);
}


void usart1_tx_dma_enable(uint8_t *data, uint16_t len)
{

    //disable DMA
 
    __HAL_DMA_DISABLE(&hdma_usart1_tx);
    while(hdma_usart1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_tx);
    }

    //clear flag

    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_HISR_TCIF7);
    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_HISR_HTIF7);

    //set data address

    hdma_usart1_tx.Instance->M0AR = (uint32_t)(data);
    //set data length
 
    hdma_usart1_tx.Instance->NDTR = len;

    //enable DMA
   
    __HAL_DMA_ENABLE(&hdma_usart1_tx);
}

