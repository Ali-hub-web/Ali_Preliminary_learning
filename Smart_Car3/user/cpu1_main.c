/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

// **************************** �������� ****************************
/*struct nvision_gray_image base_image_struct[1];
uint8  new_image[MT9V03X_H][MT9V03X_W];
nvision_gray_t *threshold = 0;
*/
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
   // tft180_set_dir(TFT180_CROSSWISE);           // ��Ҫ�Ⱥ��� ��Ȼ��ʾ����
    //init();//ģ���ʼ���ѷ�װ
    //Motor_Init();

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
/*
        mid_line_list_find();
        memcpy(new_image[0] , mt9v03x_image[0] , MT9V03X_IMAGE_SIZE);
        nvision_image_gray_threshold(base_image_struct,img_threshold,NVISION_GRAY_THRESHOLD_MODE_OTSU);
        nvision_utils_edge_detect(base_image_struct,NVISION_EDGE_DETECT_LEFT,1,30,micro_path_left_struct,300);
        nvision_utils_edge_detect(base_image_struct,NVISION_EDGE_DETECT_RIGHT,1,30,micro_path_right_struct,300);
        img_threshold=nvision_gray_threshold_otsu(base_image_struct);
        set_image_twovalues(img_threshold);
        memcpy(base_image_struct->data, image[0], MT9V03X_IMAGE_SIZE);
        Speed_Set(PWM_L, GPIO_L, 2000, 0, 1);
        Speed_Set(PWM_R, GPIO_R, 2000, 0, 1);
        */

       // mid_line = mid_line_x();
        //Speed_Set(PWM_4, PWM_5, 1000);
        //Speed_Set(PWM_2, PWM_3, 1000);


        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
#pragma section all restore
// **************************** �������� ****************************
