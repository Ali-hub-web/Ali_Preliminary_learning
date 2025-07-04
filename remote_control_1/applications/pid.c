/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       pid.c/h
  * @brief      pidʵ�ֺ�����������ʼ����PID���㺯����
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "pid.h"
#include "main.h"
#include "stdio.h"
//���ת������
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

/**
  * @brief          pid struct data init
  * @param[out]     pid: PID struct data point
  * @param[in]      mode: PID_POSITION: normal pid
  *                 PID_DELTA: delta pid
  * @param[in]      PID: 0: kp, 1: ki, 2:kd
  * @param[in]      max_out: pid max out
  * @param[in]      max_iout: pid max iout
  * @retval         none
  */
/**
  * @brief          pid struct data init
  * @param[out]     pid: PID�ṹ����ָ��
  * @param[in]      mode: PID_POSITION:��ͨPIDλ��ʽ PID���� ��λ��ʽ PID ������ֱ�Ӹ��ݸ���ֵ��ʵ�����ֵ��ƫ�����һ���� PID �㷨������������������Ŀ������Ĵ�С�����˿���ִ�л���������λ��
  *                 PID_DELTA: ���PID����ʽ PID �����Ǹ��ݵ�ǰʱ�̵�ƫ�����ǰ����ʱ�̵�ƫ���������������������������Ҫ�ǶԿ������ı仯���ֽ��м��㣬�������ĸ����ǻ���ǰһ�εĿ����������������
  * @param[in]      PID: 0: kp, 1: ki, 2:kd
  * @param[in]      max_out: pid������
  * @param[in]      max_iout: pid���������
  * @retval         none
  * 
  * u(n) = Kpe(t) + KiT(e(0) + e(1) + ... + e(n)) + Kd{e(n) - e(n-1)} / T
  * ��u(n) = Kp[e(n) - e(n-1)] + Kie(n) + Kd[e(n) - 2*e(n-1) + e(n-2)]
  */
void PID_init(pid_type_def *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout)
{
    if (pid == NULL || PID == NULL)
    {
        return;
    }
    pid->mode = mode;
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}

/**
  * @brief          pid calculate 
  * @param[out]     pid: PID struct data point
  * @param[in]      ref: feedback data 
  * @param[in]      set: set point
  * @retval         pid out
  */
/**
  * @brief          pid����
  * @param[out]     pid: PID�ṹ����ָ��
  * @param[in]      ref: ��������
  * @param[in]      set: �趨ֵ
  * @retval         pid���
  */
fp32 PID_calc(pid_type_def *pid, fp32 ref, fp32 set)
{
    if (pid == NULL)
    {
        return 0.0f;
    }

    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;						//Ŀ��ֵ
    pid->fdb = ref;						//����ֵ
    pid->error[0] = set - ref;
    if (pid->mode == PID_POSITION)                //position PID:generally appears in position loop
    {
        pid->Pout = pid->Kp * pid->error[0];	//������
        pid->Iout += pid->Ki * pid->error[0];	//������
        pid->Dbuf[2] = pid->Dbuf[1];		
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);	//΢��ת���
        pid->Dout = pid->Kd * pid->Dbuf[0];		//΢����
        LimitMax(pid->Iout, pid->max_iout);		//���ƻ������:avoid cover the max limit
        pid->out = pid->Pout + pid->Iout + pid->Dout;  //plus together 
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DELTA)               //Incremental PID: generally used in speed ring
    {
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        pid->Iout = pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        pid->out += pid->Pout + pid->Iout + pid->Dout;   //the sum of ��u(n)
        LimitMax(pid->out, pid->max_out);
    }
    return pid->out;
//		printf("My PID:%f,%d\n",ref,500);

}

/**
  * @brief          pid out clear
  * @param[out]     pid: PID struct data point
  * @retval         none
  */
/**
  * @brief          pid ������
  * @param[out]     pid: PID�ṹ����ָ��
  * @retval         none
  */
void PID_clear(pid_type_def *pid)
{
    if (pid == NULL)
    {
        return;
    }

    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->fdb = pid->set = 0.0f;
}
