/**
 * @file nvision_utils.h
 * @author Evan Xiao (1993996310@qq.com)
 * @brief 常用算法、工具函数
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __NVISION_UTILS_H__
#define __NVISION_UTILS_H__

#include "nvision.h"

#include <stdint.h>

/**
 * @brief 边缘检测结果，未知错误
 *
 */
#define NVISION_EDGE_DETECT_UNKNOWN 0
/**
 * @brief 边缘检测结果，未找到边缘曲线
 *
 */
#define NVISION_EDGE_DETECT_NOT_FOUND -1
/**
 * @brief 边缘检测结果,微路径数组已满
 *
 */
#define NVISION_EDGE_DETECT_PATH_FULL -2

/**
 * @brief 线段检测结果，未知错误
 *
 */
#define NVISION_LINE_DETECT_UNKNOWN 0

/**
 * @brief 线段检测结果，未找到线段
 *
 */
#define NVISION_LINE_DETECT_PATH_EMPTY -1

/**
 * @brief 线段检测结果，线段数组已满
 *
 */
#define NVISION_LINE_DETECT_LINE_FULL -2

/**
 * @brief
 * 微型路径的斜率，0-7分别表示0°、45°、90°、135°、180°、225°、270°、315°角度下的斜率
 *
 */
#define NVISION_PATH_SLOPE_0 0
#define NVISION_PATH_SLOPE_45 1
#define NVISION_PATH_SLOPE_90 2
#define NVISION_PATH_SLOPE_135 3
#define NVISION_PATH_SLOPE_180 4
#define NVISION_PATH_SLOPE_225 5
#define NVISION_PATH_SLOPE_270 6
#define NVISION_PATH_SLOPE_315 7
/**
 * @brief 微路径的斜率，未知，通常表示该点不在边缘曲线上
 *
 */
#define NVISION_PATH_SLOPE_UNKNOWN 8

/**
 * @brief 微路径，一个像素点的坐标和该点所在曲线的切线的斜率
 *
 */
struct nvision_micro_path {
    nvision_size_t x;
    nvision_size_t y;
    uint8_t slope;
};

/**
 * @brief 边缘检测方向，左边或右边
 *
 */
enum nvision_edge_detect_dir {
    NVISION_EDGE_DETECT_LEFT,
    NVISION_EDGE_DETECT_RIGHT
};

/**
 * @brief 线段，包含起点和终点
 *
 */
struct nvision_line {
    nvision_point_t start;
    nvision_point_t end;
};

/**
 * @brief 从二值化图像中提取边缘曲线的微路径。
 *
 * @param gray_image 二值化图像
 * @param detect_dir NVISION_EDGE_DETECT_LEFT 或 NVISION_EDGE_DETECT_RIGHT
 * 只会检测距离视野中心最近的边缘曲线。
 * @param start_row 从哪一行开始检测，若为负数，则从图像顶部开始检测
 * @param max_break_height 最大断裂高度，在开始寻找起点时，如果寻找高度已经超过 max_break_height，则认为没有找到边缘曲线。此外若路径消失在图像边缘处的高度超过 max_break_height，则认为边缘曲线已经结束
 * @param path 微路径数组
 * @param path_len 最多提取的微路径数量
 * @return int16_t 返回提取到的微路径数量，若返回值为负数，则表示提取失败
 */
int16_t nvision_utils_edge_detect(struct nvision_gray_image *gray_image,
                                  enum nvision_edge_detect_dir detect_dir,
                                  int8_t start_row,
                                  int16_t max_break_height,
                                  struct nvision_micro_path *path,
                                  uint16_t path_len,
                                  int8_t *line_grow);

/**
 * @brief 从微路径数组中提取直线段
 *
 * @param path 微路径数组
 * @param path_len 微路径数量
 * @param line 直线段数组
 * @param max_line_num 最多提取的直线段数量
 * @param max_slope_diff
 * 直线段的最大斜率差，若两个微路径的斜率差小于该值，则认为它们属于同一条直线段。请注意的一点是，这个斜率并非常规意义上的斜率，而是一个离散的值，具体取值见
 * NVISION_PATH_SLOPE_* 宏定义
 * @return int16_t 返回提取到的直线段数量，若返回值为负数，则表示提取失败
 */
int16_t nvision_utils_line_detect(struct nvision_micro_path *path,
                                  int16_t path_len, struct nvision_line *line,
                                  int16_t max_line_num, float max_slope_diff);

/**
 * @brief 从二值化图像中提取指定大小物体的坐标
 *
 * @param gray_image 二值化图像
 * @param[in] size 物体大小,
 * 以正方形边长表示，只有边长大于等于该值的物体才会被检测到
 * @param[out] location 物体坐标
 */
void nvision_utils_object_detect(struct nvision_gray_image *gray_image,
                                 nvision_size_t size,
                                 struct nvision_point *location);

#endif // __NVISION_UTILS_H__
