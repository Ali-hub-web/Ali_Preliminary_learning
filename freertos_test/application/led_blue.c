
#include "led_blue.h"




/**
  * @brief          blue led task
  * @param[in]      argument: NULL
  * @retval         none
  */

void led_blue(void const * argument)
{

    while(1)
    {

        HAL_GPIO_WritePin(LED_blue_GPIO_Port, LED_blue_Pin, GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_blue_GPIO_Port, LED_blue_Pin, GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_blue_GPIO_Port, LED_blue_Pin, GPIO_PIN_SET);
        osDelay(500);
    }
}


