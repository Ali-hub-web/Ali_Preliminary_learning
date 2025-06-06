/*
 * Servo.c
 *
 *  Created on: 2025��3��2��
 *      Author: Alisoya
 */


#include "Servo.h"
uint16 pwm = SERVO_PWMMID;
uint16 real = SERVO_PWMMID;
/*
 * �����ʼ��
 * �ı�50ʱ��50*��-->SERVO_PWMMIDҲҪ*����turn��ϵ��ҲҪ*��
 */
void Servo_Init(void)
{
    pwm_init(duoji, 50, SERVO_PWMMID);//50һ���5��
}
/*
 * �������
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
 * �������ת��
 * mid:ͼ������ֵ��k:����ת��ϵ��
 * �����������жϼ��ɿ��ƶ������ת��
 */
void turn(uint16 mid, float k)
{
    int temp;
    temp = Limit_int(-turn_left_duoji_limit, k*(MID_W-mid), turn_right_duoji_limit);
    pwm_set_duty(duoji, duoji_mid+temp);
}

/*
 * ����ջ�ת��
 * mid:����ֵ
 * �����������жϼ��ɿ��ƶ���ջ�ת��
 */
void servo(uint16 mid)
{
    int servo_value;//������ֵ
    int temp1;
    servo_value = PID_Normal(&servo_pid, MID_W-mid, 0);//������ֵ
    temp1 = duoji_mid-Limit_int(-turn_right_duoji_limit, servo_value, turn_left_duoji_limit);

}
