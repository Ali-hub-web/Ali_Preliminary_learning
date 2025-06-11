
#include "led_green.h"




/**
  * @brief          green led task
  * @param[in]      argument: NULL
  * @retval         none
  */

void led_green(void const * argument)
{

    while(1)
    {

        HAL_GPIO_WritePin(LED_green_GPIO_Port, LED_green_Pin, GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_green_GPIO_Port, LED_green_Pin, GPIO_PIN_SET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_green_GPIO_Port, LED_green_Pin, GPIO_PIN_RESET);
        osDelay(500);


    }
}


