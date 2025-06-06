/*
 * Servo.c
 *
 *  Created on: 2025年3月2日
 *      Author: Alisoya
 */


#include "Servo.h"
uint16 pwm = SERVO_PWMMID;
uint16 real = SERVO_PWMMID;
/*
 * 舵机初始化
 * 改变50时，50*？-->SERVO_PWMMID也要*？，turn的系数也要*？
 */
void Servo_Init(void)
{
    pwm_init(duoji, 50, SERVO_PWMMID);//50一秒打5次
}
/*
 * 舵机控制
 */
void Servo_Ctrl(uint16 pwm)
{

    if(SERVO_PWMMAX < pwm)
    {
        real = SERVO_PWMMAX;
    }
    else if(SERVO_PWMMIN > pwm)
    {
        real = SERVO_PWMMIN;
    }
    else
    {
        real = pwm;
    }
    pwm_set_duty(duoji, real);
}

/*
 * 舵机开环转向
 * mid:图像中线值，k:开环转向系数
 * 将函数放入中断即可控制舵机开环转向
 */
void turn(uint16 mid, float k)
{
    int temp;
    temp = Limit_int(-turn_left_duoji_limit, k*(MID_W-mid), turn_right_duoji_limit);
    pwm_set_duty(duoji, duoji_mid+temp);
}

/*
 * 舵机闭环转向
 * mid:中线值
 * 将函数放入中断即可控制舵机闭环转向
 */
void servo(uint16 mid)
{
    int servo_value;//舵机打角值
    int temp1;
    servo_value = PID_Normal(&servo_pid, MID_W-mid, 0);//舵机打角值
    temp1 = duoji_mid-Limit_int(-turn_right_duoji_limit, servo_value, turn_left_duoji_limit);

}
