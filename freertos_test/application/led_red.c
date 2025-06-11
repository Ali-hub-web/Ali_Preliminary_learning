
#include "led_red.h"




/**
  * @brief          red led task
  * @param[in]      argument: NULL
  * @retval         none
  */

void led_red(void const * argument)
{

    while(1)
    {
        HAL_GPIO_WritePin(LED_red_GPIO_Port, LED_red_Pin, GPIO_PIN_SET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_red_GPIO_Port, LED_red_Pin, GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_red_GPIO_Port, LED_red_Pin, GPIO_PIN_RESET);
        osDelay(500);

    }
}


