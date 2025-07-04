/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       pid.c/h
  * @brief      pid实现函数，包括初始化，PID计算函数，
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
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
//电机转速限制
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
  * @param[out]     pid: PID结构数据指针
  * @param[in]      mode: PID_POSITION:普通PID位置式 PID概念 ：位置式 PID 控制是直接根据给定值与实际输出值的偏差，按照一定的 PID 算法来计算出控制量。它的控制量的大小代表了控制执行机构所处的位置
  *                 PID_DELTA: 差分PID增量式 PID 控制是根据当前时刻的偏差、相邻前两个时刻的偏差来计算出控制量的增量。它主要是对控制量的变化部分进行计算，控制量的更新是基于前一次的控制量加上这个增量
  * @param[in]      PID: 0: kp, 1: ki, 2:kd
  * @param[in]      max_out: pid最大输出
  * @param[in]      max_iout: pid最大积分输出
  * @retval         none
  * 
  * u(n) = Kpe(t) + KiT(e(0) + e(1) + ... + e(n)) + Kd{e(n) - e(n-1)} / T
  * ▲u(n) = Kp[e(n) - e(n-1)] + Kie(n) + Kd[e(n) - 2*e(n-1) + e(n-2)]
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
  * @brief          pid计算
  * @param[out]     pid: PID结构数据指针
  * @param[in]      ref: 反馈数据
  * @param[in]      set: 设定值
  * @retval         pid输出
  */
fp32 PID_calc(pid_type_def *pid, fp32 ref, fp32 set)
{
    if (pid == NULL)
    {
        return 0.0f;
    }

    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;						//目标值
    pid->fdb = ref;						//返回值
    pid->error[0] = set - ref;
    if (pid->mode == PID_POSITION)                //position PID:generally appears in position loop
    {
        pid->Pout = pid->Kp * pid->error[0];	//比例项
        pid->Iout += pid->Ki * pid->error[0];	//积分项
        pid->Dbuf[2] = pid->Dbuf[1];		
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);	//微分转差分
        pid->Dout = pid->Kd * pid->Dbuf[0];		//微分项
        LimitMax(pid->Iout, pid->max_iout);		//限制积分输出:avoid cover the max limit
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
        pid->out += pid->Pout + pid->Iout + pid->Dout;   //the sum of ▲u(n)
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
  * @brief          pid 输出清除
  * @param[out]     pid: PID结构数据指针
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
