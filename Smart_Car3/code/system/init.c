/*
 * init.c
 *
 *  Created on: 2025��2��26��
 *      Author: Alisoya
 */

#include <init.h>

bool flag = 1;
int time = 0;

void init(void)
{
    //�����ʼ��
    Servo_Init();

    //pwm��ʼ��
    //ǰ��45Ϊ�ҽţ�23Ϊ���
    //��25Ϊ�ҽţ�34Ϊ���
    pwm_init(PWM_4, 17000, 0);
    pwm_init(PWM_5, 17000, 0);

    pwm_init(PWM_2, 17000, 0);
    pwm_init(PWM_3, 17000, 0);

    //��ʾ����ʼ��
    tft180_set_dir(TFT180_CROSSWISE);           // ��Ҫ�Ⱥ��� ��Ȼ��ʾ����
    tft180_init();
    mt9v03x_init();
    tft180_show_string(0, 0, "mt9v03x init.");
    //��������ʼ��
    Encoder_Init();
    //������ʼ��
    key_init(5);
    //��ʱ����ʼ��
    pit_ms_init(CCU60_CH0, 20);//��ʱ���ж�20s
    pit_ms_init(CCU60_CH1, 5);//��ʱ���ж�5s

    gpio_init(P33_13, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED2 ��� Ĭ�ϸߵ�ƽ �������ģʽ

}
