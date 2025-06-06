/*
 * init.c
 *
 *  Created on: 2025年2月26日
 *      Author: Alisoya
 */

#include <init.h>

bool flag = 1;
int time = 0;

void init(void)
{
    //舵机初始化
    Servo_Init();

    //pwm初始化
    //前车45为右脚，23为左脚
    //后车25为右脚，34为左脚
    pwm_init(PWM_4, 17000, 0);
    pwm_init(PWM_5, 17000, 0);

    pwm_init(PWM_2, 17000, 0);
    pwm_init(PWM_3, 17000, 0);

    //显示屏初始化
    tft180_set_dir(TFT180_CROSSWISE);           // 需要先横屏 不然显示不下
    tft180_init();
    mt9v03x_init();
    tft180_show_string(0, 0, "mt9v03x init.");
    //编码器初始化
    Encoder_Init();
    //按键初始化
    key_init(5);
    //定时器初始化
    pit_ms_init(CCU60_CH0, 20);//定时器中断20s
    pit_ms_init(CCU60_CH1, 5);//定时器中断5s

    gpio_init(P33_13, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // 初始化 LED2 输出 默认高电平 推挽输出模式

}
