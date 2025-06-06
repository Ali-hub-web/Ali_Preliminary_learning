/**
 * @file otsu.c
 * @author Evan Xiao (1993996310@qq.com)
 * @brief 澶ф触娉曚簩鍊煎寲鐨勫疄鐜�
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "threshold/otsu.h"
#include <stdint.h>


nvision_gray_t
nvision_gray_threshold_otsu(struct nvision_gray_image *gray_image) {
    nvision_size_t width = gray_image->width;
    nvision_size_t height = gray_image->height;
    nvision_gray_t *data = gray_image->data;

    uint16_t hist[256] = {0};
    uint16_t total = width * height;

    uint16_t i = 0;

    for (i = 0; i < total; i++) {
        hist[data[i]]++;
    }

    float sum = 0;
    for (i = 0; i < 256; i++) {
        sum += i * hist[i];
    }

    float sumB = 0;
    uint16_t wB = 0;
    uint16_t wF = 0;

    float varMax = 0;
    nvision_gray_t threshold = 0;

    for (i = 0; i < 256; i++) {
        wB += hist[i];
        if (wB == 0) {
            continue;
        }

        wF = total - wB;
        if (wF == 0) {
            break;
        }

        sumB += i * hist[i];

        float mB = sumB / wB;
        float mF = (sum - sumB) / wF;

        float varBetween = wB * wF * (mB - mF) * (mB - mF);

        if (varBetween > varMax) {
            varMax = varBetween;
            threshold = i;
 //           printf("%d",i);
        }
    }

    return threshold;
}
