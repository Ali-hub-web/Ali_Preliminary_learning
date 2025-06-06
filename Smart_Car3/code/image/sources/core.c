/**
 * @file core.c
 * @author Evan Xiao (1993996310@qq.com)
 * @brief Nano Vision 的功能实现
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "nvision.h"
#include "threshold/otsu.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define nvision_is_between(x, a, b)                                            \
    (((x) >= (a) && (x) <= (b)) || ((x) >= (b) && (x) <= (a)))

void nvision_image_create_rgb_from_buffer(struct nvision_rgb888_image *image,
                                          uint8_t *buffer, nvision_size_t width,
                                          nvision_size_t height,
                                          uint16_t mode) {
    image->data = (nvision_rgb888_t *)buffer;
    image->width = width;
    image->height = height;
}

void nvision_image_create_hsl_from_buffer(struct nvision_hsl_image *image,
                                          uint8_t *buffer, nvision_size_t width,
                                          nvision_size_t height,
                                          uint16_t mode) {
    image->data = (nvision_hsl_t *)buffer;
    image->width = width;
    image->height = height;
}

void nvision_image_create_gray_from_buffer(struct nvision_gray_image *image,
                                           uint8_t *buffer,
                                           nvision_size_t width,
                                           nvision_size_t height,
                                           uint16_t mode) {
    image->data = (nvision_gray_t *)buffer;
    image->width = width;
    image->height = height;
}

void nvision_color_rgb_to_hsl(nvision_rgb888_t *rgb, nvision_hsl_t *hsl) {
    float r = rgb->red / 255.0f;
    float g = rgb->green / 255.0f;
    float b = rgb->blue / 255.0f;

    float max = fmaxf(r, fmaxf(g, b));
    float min = fminf(r, fminf(g, b));
    float delta = max - min;

    float hue = 0;

    if (r == max) {
        hue = (g - b) / delta;
    } else if (g == max) {
        hue = 2 + (b - r) / delta;
    } else if (b == max) {
        hue = 4 + (r - g) / delta;
    }

    hue *= 60;
    if (hue < 0) {
        hue += 360;
    }

    hsl->hue = hue;
    hsl->saturation = (max == 0) ? 0 : (delta / max);
    hsl->lightness = (max + min) / 2;
}

void nvision_color_rgb_to_gray(nvision_rgb888_t *rgb, nvision_gray_t *gray) {
    (*gray) = (rgb->red + rgb->green + rgb->blue) / 3;
}

uint8_t nvision_color_hsl_in_range(nvision_hsl_t *hsl, nvision_hsl_t *start_hsl,
                                   nvision_hsl_t *end_hsl) {
    if (start_hsl->hue != NVISION_HSL_DONT_CARE) {
        if (!nvision_is_between(hsl->hue, start_hsl->hue, end_hsl->hue)) {
            return 0;
        }
    }

    if (start_hsl->saturation != NVISION_HSL_DONT_CARE) {
        if (!nvision_is_between(hsl->saturation, start_hsl->saturation,
                                end_hsl->saturation)) {
            return 0;
        }
    }

    if (start_hsl->lightness != NVISION_HSL_DONT_CARE) {
        if (!nvision_is_between(hsl->lightness, start_hsl->lightness,
                                end_hsl->lightness)) {
            return 0;
        }
    }

    return 1;
}

void nvision_image_rgb_to_hsl(struct nvision_rgb888_image *rgb_image,
                              struct nvision_hsl_image *hsl_image) {
    uint16_t i;
    for (i = 0; i < rgb_image->width * rgb_image->height; i++) {
        nvision_color_rgb_to_hsl(&rgb_image->data[i], &hsl_image->data[i]);
    }
}

void nvision_image_rgb_to_gray(struct nvision_rgb888_image *rgb_image,
                               struct nvision_gray_image *gray_image) {
    uint16_t i;
    for (i = 0; i < rgb_image->width * rgb_image->height; i++) {
        nvision_color_rgb_to_gray(&rgb_image->data[i], &gray_image->data[i]);
    }
}

void nvision_image_gray_threshold(struct nvision_gray_image *gray_image,
                                  nvision_gray_t threshold, uint16_t mode) {
    uint16_t i;
    nvision_gray_t *data = nvision_image_gray_raw(gray_image);
    nvision_gray_t pixel_white = NVISION_BINARY_WHITE;
    nvision_gray_t pixel_black = NVISION_BINARY_BLACK;

    if (mode & NVISION_GRAY_THRESHOLD_MODE_BINARY_INV) {
        pixel_white = NVISION_BINARY_BLACK;
        pixel_black = NVISION_BINARY_WHITE;
    }

    // 判断使用何种模式
    if (mode & NVISION_GRAY_THRESHOLD_MODE_OTSU) {
        // 使用 Otsu 算法计算阈值
        threshold = nvision_gray_threshold_otsu(gray_image);
 //       printf("%d  ",threshold);
    }

    for (i = 0; i < gray_image->width * gray_image->height; i++) {
        if (data[i] > threshold) {
            data[i] = pixel_white;
        } else {
            data[i] = pixel_black;
        }
    }
}

void nvision_image_hsl_threshold(struct nvision_hsl_image *hsl_image,
                                 struct nvision_gray_image *binary_image,
                                 nvision_hsl_t start_hsl, nvision_hsl_t end_hsl,
                                 uint16_t mode) {
    uint16_t i;
    nvision_gray_t *data = nvision_image_gray_raw(binary_image);
    for (i = 0; i < hsl_image->width * hsl_image->height; i++) {
        if (nvision_color_hsl_in_range(&hsl_image->data[i], &start_hsl,
                                       &end_hsl)) {
            data[i] = (mode == NVISION_HSL_THRESHOLD_MODE_BINARY);
        } else {
            data[i] = !(mode == NVISION_HSL_THRESHOLD_MODE_BINARY);
        }
    }
}

nvision_rgb888_t *nvision_image_rgb_raw(struct nvision_rgb888_image *image) {
    return image->data;
}

nvision_hsl_t *nvision_image_hsl_raw(struct nvision_hsl_image *image) {
    return image->data;
}

nvision_gray_t *nvision_image_gray_raw(struct nvision_gray_image *image) {
    return image->data;
}
