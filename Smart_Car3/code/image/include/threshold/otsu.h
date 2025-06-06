/**
 * @file otsu.h
 * @author Evan Xiao (1993996310@qq.com)
 * @brief Otsu's Binarization
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GRAY_THRESHOLD_OTSU_H__
#define __GRAY_THRESHOLD_OTSU_H__

#include <stdint.h>

#include "nvision.h"

/**
 * @brief 大津法二值化
 * 
 * @param gray_image 灰度图像
 * @return nvision_gray_t 
*/
nvision_gray_t nvision_gray_threshold_otsu(struct nvision_gray_image *gray_image);



#endif // __GRAY_THRESHOLD_OTSU_H__