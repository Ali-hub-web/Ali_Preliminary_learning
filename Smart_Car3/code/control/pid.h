/*
 * pid.h
 *
 *  Created on: 2025��2��26��
 *      Author: Alisoya
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_
#include "zf_common_headfile.h"

/*�궨��*/

#define pid_limit 4000   //���ֵ������8000���
typedef struct PID
{
    float Kp;
    float Ki;
    float Kd;
    float LowPass;//�����˲�ϵ��

    float Out_P;
    float Out_I;
    float Out_D;

    float PrevError;
    float LastError;
    float Error;
    float LastData;
}PID;

#define PID_CREATE(_kp, _ki, _kd, _low_pass)\
{                                           \
    .Kp=_kp,                                \
    .Ki = _ki,                              \
    .Kd = _kd,                              \
    .LowPass = _low_pass,                   \
    .Out_P = 0,                             \
    .Out_I = 0,                             \
    .Out_D = 0,                             \
}
#define pid_out_I_limit 30//����ʽpid�������޷�3 ��ֹ���ͳ�

/*��������*/
extern struct PID motor_pid_l;
extern struct PID motor_pid_r;
extern struct PID servo_pid;
/*��������*/
int Limit_int(int a, int b, int c);
float PID_Normal(PID *PID, float NowData, float Point);
float PID_Increase(PID *PID, float NowData, float Point);
void dynamic_pid_value_set(void);

#endif /* CODE_PID_H_ */
