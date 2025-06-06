/*
 * Motor_Contrl.h
 *
 *  Created on: 2025��2��23��
 *      Author: Alisoya
 */

#ifndef CODE_MOTOR_CONTRL_H_
#define CODE_MOTOR_CONTRL_H_
#include "zf_common_headfile.h"
//�궨��
#define PWM_4              (ATOM0_CH2_P21_4)
#define PWM_5              (ATOM0_CH3_P21_5)
#define PWM_2              (ATOM0_CH0_P21_2)
#define PWM_3              (ATOM0_CH1_P21_3)
/*
#define MotorL_pwm (ATOM0_CH2_P21_4)//�����ٶ�
#define MotorL_turn (P21_5)//��������

#define MotorR_pwm (ATOM0_CH0_P21_2)//�ҵ���ٶ�
#define MotorR_turn (P21_3)//�ҵ������
*/
//��������
/*route1*/
//void HIP4082_Init_D(void);
//void HIP4082_MOTOR_DRIVER_D(int L_SPEED,int R_SPEED);
/*route2*/
void Motor_Init(void);

void Motor23_SetSpeed(int pwm);//left motor
void Motor45_SetSpeed(int pwm);//right motor

/*route3*/
//�궨��
//#define MID_W 93            //������ֵ
//#define mid_line_limit 30   //����ֵ�޷�
//�ṹ��
typedef struct motor1
{
    int target_speed;
    int duty;
    int encoder_speed;
    int encoder_raw;
    int32 total_encoder;
}motor1;
//���ñ���
extern struct motor1 motor_l;
extern struct motor1 motor_r;

//��������
void Differential_Gear(int speed, float k, int limit);
void car_start(void);
//void Speed_Set(pwm_channel_enum pin1, pwm_channel_enum pin2, int speed, uint8 just, uint8 lose);
void Speed_Set(pwm_channel_enum pin1, pwm_channel_enum pin2, int speed);

void Motor_Control(int Speed_L, int Speed_R);


#endif /* CODE_MOTOR_CONTRL_H_ */
