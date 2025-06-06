/*
 * Motor_Contrl.c
 *
 *  Created on: 2025年2月23日
 *      Author: Alisoya
 */

#include "Motor_Contrl.h"


/*route2*/
//电机初始化，正转

void Motor_Init(void)
{
    //gpio_init(MotorL_turn, GPO, 1, GPO_PUSH_PULL);//1，low，推挽输出
    pwm_init(PWM_4, 17000, 0);
    pwm_init(PWM_5, 17000, 0);

    pwm_init(PWM_2, 17000, 0);
    pwm_init(PWM_3, 17000, 0);

    //gpio_init(MotorR_turn, GPO, 1, GPO_PUSH_PULL);

}
//左电机控制
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

//右电机控制
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
//定义两个结构体
motor1 motor_l;
motor1 motor_r;


/*
 * 设置电机引脚，方向，速度
 * 实现电机正反转
 * 低电平/占空比大于零电机正转，高电平/占空比小于零电机反转
 * 还需自己试——》见中断1.
 */
/*一路pwm
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
/*二路pwm*/
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
 * 按键控制电机启动和停止
 * 方便调车，看舵机打脚
 * 具体启用函数看中断3.
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
            Motor_Control(60, 60);//普通model
         //    Differential_Gear(80, 0.3, 20);//差速使用，不会使电机疯转，也不会减到零
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
                Motor_Control(30, 30);//普通model
                //    Differential_Gear(80, 0.3, 20);//差速使用，不会使电机疯转，也不会减到零
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
 * 电机差速控制
 * 由于转弯时外轮快，内轮慢
 * c车四轮,f车三轮也需要
 * k:差速比例系数，需要自己量或者请教机械组别
 *
 */

void Differential_Gear(int speed, float k, int limit)
{
    Motor_Control(Limit_int(speed-limit, speed-k*(MID_W-final_mid_line), speed+limit),
                  Limit_int(speed-limit, speed+k*(MID_W-final_mid_line), speed+limit));
}


/*
 * 电机闭环控制
 * 5ms中断的话，设一个60/80speed,这个speed指的是编码器速度
 * 使用时要放到中断里--》见中断2.
 *
 */
void Motor_Control(int Speed_L, int Speed_R)
{
    motor_l.target_speed = Speed_L;
    motor_r.target_speed = Speed_R;

    motor_l.duty = Limit_int(-pid_limit, motor_l.duty+PID_Increase(&motor_pid_l, (float)motor_l.encoder_speed, (float)motor_l.target_speed), pid_limit);
    motor_r.duty = Limit_int(-pid_limit, motor_r.duty+PID_Increase(&motor_pid_r, (float)motor_r.encoder_speed, (float)motor_r.target_speed), pid_limit);
//前车
    Speed_Set(PWM_2, PWM_3, motor_l.duty);//左
    Speed_Set(PWM_4, PWM_5, motor_r.duty);//右 motor_r.duty

//后车
   // Speed_Set(PWM_3, PWM_4, motor_l.duty);
   // Speed_Set(PWM_2, PWM_5, motor_r.duty);

}
