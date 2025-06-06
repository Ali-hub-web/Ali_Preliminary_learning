/*
 * camera_ctrl.h
 *
 *  Created on: 2025年4月16日
 *      Author: 25215
 */

#ifndef CODE_IMAGE_CAMERA_CTRL_H_
#define CODE_IMAGE_CAMERA_CTRL_H_


#include "zf_common_headfile.h"

extern int c_flag;

void image_Handler(void);
void Find_line(uint8 Image[60][90]);
uint8 find_mid_line_weight(void);
void edge_detect(uint8 Image[60][90],int *line_left_edge,int *line_right_edge);
void cross_judge(void);
void cross_process(void);
void cross_pass_judge(void);

void round_judge(void);
void round_process(void);
void roundabout(void);

void draw_line(void);

void total_judge(void);


#endif /* CODE_IMAGE_CAMERA_CTRL_H_ */
