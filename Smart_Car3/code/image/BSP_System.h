/*
 * BSP_System.h
 *
 *  Created on: 2025锟斤拷2锟斤拷28锟斤拷
 *      Author: 25215
 */

#ifndef CODE_BSP_SYSTEM_H_
#define CODE_BSP_SYSTEM_H_

#include "zf_common_headfile.h"
#include "camera_ctrl.h"
#include <stdint.h>
#include "nvision.h"
#include "threshold/otsu.h"
#include "nvision_utils.h"


#define YUAN_Wdith 188
#define YUAN_Height 120

#define MAX_Wdith 90
#define MAX_Height 60

#define left_break 0
#define right_break 89

#define MT9V03X_W               (188)                                           // 图锟斤拷锟斤拷     锟斤拷围 [1-752]
#define MT9V03X_H               (120)   // 图锟斤拷叨锟�     锟斤拷围 [1-480]

extern uint8 picture[MAX_Height][MAX_Wdith];
extern uint8  new_image[MT9V03X_H][MT9V03X_W];
extern uint8_t img_threshold;
extern uint8_t search_end_line;

extern uint8 final_mid_line;
extern int mid_line_list[60];      //锟斤拷锟斤拷锟斤拷锟斤拷
extern int left_edge[60];
extern int right_edge[60];
extern uint8 MID_W;
extern uint8 track_width[60];
extern uint8 mid_weight_list[];

extern int valid_left, valid_right;    // 鏈夋晥琛屾暟(鏈夎竟鐣�)
extern int break_left_count, break_right_count;    //鏂嚎琛屾暟
extern int both_break; //鍚屾椂鏂嚎琛屾暟
extern nvision_gray_t *threshold;
extern struct nvision_gray_image base_image_struct[1];
#endif /* CODE_BSP_SYSTEM_H_ */
