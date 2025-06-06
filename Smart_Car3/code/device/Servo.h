/*
 * Servo.h
 *
 *  Created on: 2025年3月2日
 *      Author: Alisoya
 */

#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_

#include "zf_common_headfile.h"
//宏定义
#define SERVO_PWMMID 350
#define SERVO_PWMMAX 445
#define SERVO_PWMMIN 225
#define turn_left_duoji_limit 60 //左舵机限幅
#define turn_right_duoji_limit 60  //右舵机限幅
#define duoji_mid 620    //舵机中值
#define duoji ATOM2_CH5_P33_13  //定义舵机引脚
//外用变量
extern uint16 pwm;
extern uint16 real;
//函数声明
void Servo_Init(void);
void Servo_Ctrl(uint16 pwm);
void turn(uint16 mid, float k);
void servo(uint16 mid);
#endif /* CODE_SERVO_H_ */
