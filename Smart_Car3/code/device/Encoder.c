/*
 * Encoder.c
 *
 *  Created on: 2025年2月25日
 *      Author: Alisoya
 */
#include "Encoder.h"

//编码器初始化
//void Encoder_Init(void)
//{
    //左编码器
//    encoder_dir_init(TIM5_ENCODER, TIM5_ENCODER_CH2_P10_1, TIM5_ENCODER_CH2_P21_6);

    //右编码器
//    encoder_dir_init(TIM4_ENCODER, TIM4_ENCODER_CH2_P00_9, TIM4_ENCODER_CH2_P33_5);
//}

//编码器初始化
void Encoder_Init(void)
{
    //右编码器：B
    encoder_dir_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);

    //左编码器：A
    encoder_dir_init(TIM2_ENCODER, TIM2_ENCODER_CH2_P33_6, TIM2_ENCODER_CH1_P33_7);
}

//编码器数值获取
void Encoder_Data_Get(void)
{
    //前车
    //左电机encoder_speed
    motor_r.encoder_raw = encoder_get_count(TIM2_ENCODER);
    motor_r.encoder_speed = motor_r.encoder_speed*0.2+motor_r.encoder_raw*0.8;
    motor_r.total_encoder += motor_r.encoder_raw;
    encoder_clear_count(TIM2_ENCODER);

    //右电机
    motor_l.encoder_raw = -encoder_get_count(TIM5_ENCODER);
    motor_l.encoder_speed = motor_l.encoder_speed*0.2+motor_l.encoder_raw*0.8;
    motor_l.total_encoder += motor_l.encoder_raw;
    encoder_clear_count(TIM5_ENCODER);
}
