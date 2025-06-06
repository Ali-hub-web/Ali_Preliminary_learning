/*
 * Motor_Contrl.h
 *
 *  Created on: 2025年2月23日
 *      Author: Alisoya
 */

#ifndef CODE_MOTOR_CONTRL_H_
#define CODE_MOTOR_CONTRL_H_
#include "zf_common_headfile.h"
//宏定义
#define PWM_4              (ATOM0_CH2_P21_4)
#define PWM_5              (ATOM0_CH3_P21_5)
#define PWM_2              (ATOM0_CH0_P21_2)
#define PWM_3              (ATOM0_CH1_P21_3)
/*
#define MotorL_pwm (ATOM0_CH2_P21_4)//左电机速度
#define MotorL_turn (P21_5)//左电机方向

#define MotorR_pwm (ATOM0_CH0_P21_2)//右电机速度
#define MotorR_turn (P21_3)//右电机方向
*/
//函数声明
/*route1*/
//void HIP4082_Init_D(void);
//void HIP4082_MOTOR_DRIVER_D(int L_SPEED,int R_SPEED);
/*route2*/
void Motor_Init(void);

void Motor23_SetSpeed(int pwm);//left motor
void Motor45_SetSpeed(int pwm);//right motor

/*route3*/
//宏定义
//#define MID_W 93            //赛道中值
//#define mid_line_limit 30   //中线值限幅
//结构体
typedef struct motor1
{
    int target_speed;
    int duty;
    int encoder_speed;
    int encoder_raw;
    int32 total_encoder;
}motor1;
//外用变量
extern struct motor1 motor_l;
extern struct motor1 motor_r;

//函数声明
void Differential_Gear(int speed, float k, int limit);
void car_start(void);
//void Speed_Set(pwm_channel_enum pin1, pwm_channel_enum pin2, int speed, uint8 just, uint8 lose);
void Speed_Set(pwm_channel_enum pin1, pwm_channel_enum pin2, int speed);

void Motor_Control(int Speed_L, int Speed_R);


#endif /* CODE_MOTOR_CONTRL_H_ */
