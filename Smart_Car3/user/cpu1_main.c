/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.10.2
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

// **************************** 代码区域 ****************************
/*struct nvision_gray_image base_image_struct[1];
uint8  new_image[MT9V03X_H][MT9V03X_W];
nvision_gray_t *threshold = 0;
*/
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等
   // tft180_set_dir(TFT180_CROSSWISE);           // 需要先横屏 不然显示不下
    //init();//模块初始化已封装
    //Motor_Init();

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
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


        // 此处编写需要循环执行的代码
    }
}
#pragma section all restore
// **************************** 代码区域 ****************************
