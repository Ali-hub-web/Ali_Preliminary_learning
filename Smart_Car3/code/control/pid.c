/*
 * pid.c
 *
 *  Created on: 2025��2��26��
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
/*��������*/
//���λ��ʽpid��ʼ����
PID servo_pid = PID_CREATE(2, 0, 5, 0.8);
/*��������*/
//��̬pid��������
float Kp_base = 0.7;
float Kd_base = 5;

//�������ʽpid����
//���ԣ��ȵ�I���ٵ�P
//PID motor_pid_l = PID_CREATE(20, 0.1, 0.1, 1);//��ʱ�ò������һ������--�����˲���������1
//PID motor_pid_r = PID_CREATE(20, 0.1, 0.1, 1);
//PID motor_pid_l = PID_CREATE(20, 0.1, 0.1, 1);//��ʱ�ò������һ������--�����˲���������1
PID motor_pid_l = PID_CREATE(58, 2, 1, 1);
PID motor_pid_r = PID_CREATE(58, 2, 1, 1);
//PID motor_pid_r = PID_CREATE(0, 0.1, 0, 1);
//kp:5
//ki:1.5
//good��42��3.2��45��3.5��5��25��3.2��0��28��3.2��1��40��4��2��45��4.5��4��20��0.1��15��0.15��20��0.15��0.1��21��0.2��0.1��
/*functions*/
/*
 * ��̬pid������ֵ
 */
void dynamic_pid_value_set(void)
{
    //һ����Kp
    //servo_pid.Kp = Kp_base + my_abs-float(MID_W - final_mid_line)*0.045;//���ֵ2.05
    //������Kp
    servo_pid.Kp = Kp_base + (MID_W - final_mid_line)*(MID_W - final_mid_line)*0.0015;
    servo_pid.Ki = 0;
    servo_pid.Kd = Kd_base;
}

/*
 * ��ͨPD��λ��ʽ��
 * NowData:ʵ��ֵ
 * Point��Ŀ��ֵ
 */
float PID_Normal(PID *PID, float NowData, float Point)
{
    PID->Error = Point - NowData;
    PID->Out_D = (PID->Error - PID->Out_P) * PID->LowPass + PID->Out_D * (1 - PID->LowPass);
    PID->Out_P = PID->Error;
    return (PID->Kp * PID ->Out_P + PID->Kd * PID->Out_D);//�����������ռ�ձ�
}

/*
 * ����pid
 * NowData:ʵ��ֵ
 * Point��Ŀ��ֵ
 */
float PID_Increase(PID *PID, float NowData, float Point)
{
    PID->Error = Point - NowData;//����
    PID->Out_P = (PID->Error - PID->LastError);
    PID->Out_I = PID->Error;
    //PID->Out_I = Limit_float(-pid_out_I_limit, PID->Error, pid_out_I_limit);//����ʽpid�����޷�
    PID->Out_D = (PID->Error - 2 * PID->LastError + PID->PrevError);

    PID->PrevError = 0.9 * PID->LastError + 0.1 * PID->PrevError;
    PID->LastError = 0.9 * PID->Error + 0.1 * PID->LastError;
    PID->LastData = NowData;
    return (PID->Kp * PID->Out_P + PID->Ki * PID->Out_I + PID->Kd * PID->Out_D);
}
