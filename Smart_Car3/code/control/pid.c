/*
 * pid.c
 *
 *  Created on: 2025年2月26日
 *      Author: Alisoya
 */


#include <pid.h>

/*extra functions*/
int Limit_int(int a, int b, int c)
{
    if((b >= a) && (b <= c))
    {
        return b;
    }
    else if(b < a)
    {
        return a;
    }
    else if(b > c)
    {
        return c;
    }
    return 0;
}
/*变量定义*/
//舵机位置式pid初始参数
PID servo_pid = PID_CREATE(2, 0, 5, 0.8);
/*变量定义*/
//动态pid基础参数
float Kp_base = 0.7;
float Kd_base = 5;

//电机增量式pid参数
//调试：先调I，再调P
//PID motor_pid_l = PID_CREATE(20, 0.1, 0.1, 1);//暂时用不到最后一个参数--互补滤波参数，设1
//PID motor_pid_r = PID_CREATE(20, 0.1, 0.1, 1);
//PID motor_pid_l = PID_CREATE(20, 0.1, 0.1, 1);//暂时用不到最后一个参数--互补滤波参数，设1
PID motor_pid_l = PID_CREATE(58, 2, 1, 1);
PID motor_pid_r = PID_CREATE(58, 2, 1, 1);
//PID motor_pid_r = PID_CREATE(0, 0.1, 0, 1);
//kp:5
//ki:1.5
//good：42，3.2；45，3.5，5；25，3.2，0；28，3.2，1；40，4，2；45，4.5，4；20，0.1；15，0.15；20，0.15，0.1；21，0.2，0.1；
/*functions*/
/*
 * 动态pid参数赋值
 */
void dynamic_pid_value_set(void)
{
    //一次项Kp
    //servo_pid.Kp = Kp_base + my_abs-float(MID_W - final_mid_line)*0.045;//最大值2.05
    //二次项Kp
    servo_pid.Kp = Kp_base + (MID_W - final_mid_line)*(MID_W - final_mid_line)*0.0015;
    servo_pid.Ki = 0;
    servo_pid.Kd = Kd_base;
}

/*
 * 普通PD（位置式）
 * NowData:实际值
 * Point：目标值
 */
float PID_Normal(PID *PID, float NowData, float Point)
{
    PID->Error = Point - NowData;
    PID->Out_D = (PID->Error - PID->Out_P) * PID->LowPass + PID->Out_D * (1 - PID->LowPass);
    PID->Out_P = PID->Error;
    return (PID->Kp * PID ->Out_P + PID->Kd * PID->Out_D);//返回算出来的占空比
}

/*
 * 增量pid
 * NowData:实际值
 * Point：目标值
 */
float PID_Increase(PID *PID, float NowData, float Point)
{
    PID->Error = Point - NowData;//差量
    PID->Out_P = (PID->Error - PID->LastError);
    PID->Out_I = PID->Error;
    //PID->Out_I = Limit_float(-pid_out_I_limit, PID->Error, pid_out_I_limit);//增量式pid积分限幅
    PID->Out_D = (PID->Error - 2 * PID->LastError + PID->PrevError);

    PID->PrevError = 0.9 * PID->LastError + 0.1 * PID->PrevError;
    PID->LastError = 0.9 * PID->Error + 0.1 * PID->LastError;
    PID->LastData = NowData;
    return (PID->Kp * PID->Out_P + PID->Ki * PID->Out_I + PID->Kd * PID->Out_D);
}
