/*
 * Motor_Contrl.c
 *
 *  Created on: 2025��2��23��
 *      Author: Alisoya
 */

#include "Motor_Contrl.h"


/*route2*/
//�����ʼ������ת

void Motor_Init(void)
{
    //gpio_init(MotorL_turn, GPO, 1, GPO_PUSH_PULL);//1��low���������
    pwm_init(PWM_4, 17000, 0);
    pwm_init(PWM_5, 17000, 0);

    pwm_init(PWM_2, 17000, 0);
    pwm_init(PWM_3, 17000, 0);

    //gpio_init(MotorR_turn, GPO, 1, GPO_PUSH_PULL);

}
//��������
void Motor45_SetSpeed(int pwm)
{
    if(pwm >= 0)
    {
        pwm_set_duty(PWM_4, pwm);
        pwm_set_duty(PWM_5, 0);
    }else
    {
        pwm_set_duty(PWM_4, 0);
        pwm_set_duty(PWM_5, -pwm);
    }


}

//�ҵ������
void Motor23_SetSpeed(int pwm)
{
    if(pwm >= 0)
    {
        pwm_set_duty(PWM_2, pwm);
        pwm_set_duty(PWM_3, 0);
    }else
    {
        pwm_set_duty(PWM_2, 0);
        pwm_set_duty(PWM_3, -pwm);
    }
}


/*route3*/
//���������ṹ��
motor1 motor_l;
motor1 motor_r;


/*
 * ���õ�����ţ������ٶ�
 * ʵ�ֵ������ת
 * �͵�ƽ/ռ�ձȴ���������ת���ߵ�ƽ/ռ�ձ�С��������ת
 * �����Լ��ԡ��������ж�1.
 */
/*һ·pwm
 * void Speed_Set(pwm_channel_enum pin1, pwm_channel_enum pin2, int speed, uint8 just, uint8 lose)
{
    if(speed > 0)
    {
        pwm_set_duty(pin1, speed);
        pwm_set_duty(pin2, just);
    }
    else
    {
        pwm_set_duty(pin1, -speed);
        pwm_set_duty(pin2, lose);
    }
}
*/
/*��·pwm*/
void Speed_Set(pwm_channel_enum pin1, pwm_channel_enum pin2, int speed)
{
    if(speed > 0)
    {
        pwm_set_duty(pin1, speed);
        pwm_set_duty(pin2, 0);
    }
    else
    {
        pwm_set_duty(pin1, 0);
        pwm_set_duty(pin2, -speed);
    }
}
/*
 * �������Ƶ��������ֹͣ
 * �����������������
 * �������ú������ж�3.
 */
uint8 start = 0;

void car_start(void)
{
    if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
    {
        start++;
    }
    if(start!=0)
    {
        if(start%2==1)
        {
            Motor_Control(60, 60);//��ͨmodel
         //    Differential_Gear(80, 0.3, 20);//����ʹ�ã�����ʹ�����ת��Ҳ���������
        }
        else
        {
            Motor_Control(0, 0);
        }
    }

}

/*
void car_start(void)
{
    if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
    {
        start++;

        if(start!=0)
        {
            if(start%2==1)
            {
                Motor_Control(30, 30);//��ͨmodel
                //    Differential_Gear(80, 0.3, 20);//����ʹ�ã�����ʹ�����ת��Ҳ���������
            }
            else
            {
                Motor_Control(0, 0);
            }
        }
    }

}
*/
/*
 * ������ٿ���
 * ����ת��ʱ���ֿ죬������
 * c������,f������Ҳ��Ҫ
 * k:���ٱ���ϵ������Ҫ�Լ���������̻�е���
 *
 */

void Differential_Gear(int speed, float k, int limit)
{
    Motor_Control(Limit_int(speed-limit, speed-k*(MID_W-final_mid_line), speed+limit),
                  Limit_int(speed-limit, speed+k*(MID_W-final_mid_line), speed+limit));
}


/*
 * ����ջ�����
 * 5ms�жϵĻ�����һ��60/80speed,���speedָ���Ǳ������ٶ�
 * ʹ��ʱҪ�ŵ��ж���--�����ж�2.
 *
 */
void Motor_Control(int Speed_L, int Speed_R)
{
    motor_l.target_speed = Speed_L;
    motor_r.target_speed = Speed_R;

    motor_l.duty = Limit_int(-pid_limit, motor_l.duty+PID_Increase(&motor_pid_l, (float)motor_l.encoder_speed, (float)motor_l.target_speed), pid_limit);
    motor_r.duty = Limit_int(-pid_limit, motor_r.duty+PID_Increase(&motor_pid_r, (float)motor_r.encoder_speed, (float)motor_r.target_speed), pid_limit);
//ǰ��
    Speed_Set(PWM_2, PWM_3, motor_l.duty);//��
    Speed_Set(PWM_4, PWM_5, motor_r.duty);//�� motor_r.duty

//��
   // Speed_Set(PWM_3, PWM_4, motor_l.duty);
   // Speed_Set(PWM_2, PWM_5, motor_r.duty);

}
