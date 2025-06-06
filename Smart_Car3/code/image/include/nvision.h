/**
 * @file nvision.h
 * @author Evan Xiao (1993996310@qq.com)
 * @brief Nano Vision Library
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __NVISION_H__
#define __NVISION_H__

#include <stdint.h>


/**
 * @brief 布尔值
 * 
*/
#define NVISION_FALSE (0)
#define NVISION_TRUE (1)

typedef uint8_t nvision_bool_t;

/**
 * @brief 灰度二值化模式: 大于阈值为 1，小于阈值为 0。
 *
 */
#define NVISION_GRAY_THRESHOLD_MODE_BINARY (0x01 << 0)
/**
 * @brief 灰度二值化模式: 大于阈值为 0，小于阈值为 1。
 *
 */
#define NVISION_GRAY_THRESHOLD_MODE_BINARY_INV (0x01 << 1)

/**
 * @brief 灰度二值化模式: 大津法模式，该模式下阈值参数无效。
 *
 */
#define NVISION_GRAY_THRESHOLD_MODE_OTSU (0x01 << 8)

/**
 * @brief HSL 二值化模式: 满足阈值范围为 1，不满足阈值范围为 0。
 *
 */
#define NVISION_HSL_THRESHOLD_MODE_BINARY (0)
/**
 * @brief HSL 二值化模式: 满足阈值范围为 0，不满足阈值范围为 1。
 *
 */
#define NVISION_HSL_THRESHOLD_MODE_BINARY_INV (1)

/**
 * @brief HSL 颜色阈值: 该通道不参与阈值判断
 *
 */
#define NVISION_HSL_DONT_CARE (0)

/**
 * @brief 二值化颜色: 黑色
 *
 */
#define NVISION_BINARY_BLACK (0)

/**
 * @brief 二值化颜色: 白色
 *
 */
#define NVISION_BINARY_WHITE (255)

/**
 * @brief RGB888 颜色结构
 *
 */
struct nvision_rgb888 {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

typedef struct nvision_rgb888 nvision_rgb888_t;

/**
 * @brief HSL 颜色结构
 *
 */
struct nvision_hsl {
    float hue;
    float saturation;
    float lightness;
};

typedef struct nvision_hsl nvision_hsl_t;

/**
 * @brief 灰度值数据类型
 *
 */
typedef uint8_t nvision_gray_t;

/**
 * @brief 图像尺寸数据类型, 一般用于图像的宽度和高度
 *
 */
typedef int16_t nvision_size_t;

/**
 * @brief RGB 图像
 *
 */
struct nvision_rgb888_image {
    nvision_size_t width;
    nvision_size_t height;
    nvision_rgb888_t *data;
};

/**
 * @brief HSL 图像
 *
 */
struct nvision_hsl_image {
    nvision_size_t width;
    nvision_size_t height;
    nvision_hsl_t *data;
};

/**
 * @brief 灰度图像
 *
 */
struct nvision_gray_image {
    nvision_size_t width;
    nvision_size_t height;
    nvision_gray_t *data;
};

/**
 * @brief 点坐标
 * 
*/
struct nvision_point {
    nvision_size_t x;
    nvision_size_t y;
};

typedef struct nvision_point nvision_point_t;

/**
 * @brief 从指定的缓冲区创建 RGB 图像实例
 *
 * @param image 图像实例
 * @param buffer 缓冲区
 * @param width 图像宽度
 * @param height 图像高度
 * @param mode 模式(当前版本暂无模式，填 0 即可)
 */
void nvision_image_create_rgb_from_buffer(struct nvision_rgb888_image *image,
                                          uint8_t *buffer, nvision_size_t width,
                                          nvision_size_t height, uint16_t mode);

/**
 * @brief 从指定的缓冲区创建 HSL 图像实例
 *
 * @param image 图像实例
 * @param buffer 缓冲区
 * @param width 图像宽度
 * @param height 图像高度
 * @param mode 模式(当前版本暂无模式，填 0 即可)
 */
void nvision_image_create_hsl_from_buffer(struct nvision_hsl_image *image,
                                          uint8_t *buffer, nvision_size_t width,
                                          nvision_size_t height, uint16_t mode);

/**
 * @brief 从指定的缓冲区创建灰度图像实例
 *
 * @param image 图像实例
 * @param buffer 缓冲区
 * @param width 图像宽度
 * @param height 图像高度
 * @param mode 模式(当前版本暂无模式，填 0 即可)
 */
void nvision_image_create_gray_from_buffer(struct nvision_gray_image *image,
                                           uint8_t *buffer,
                                           nvision_size_t width,
                                           nvision_size_t height,
                                           uint16_t mode);

/**
 * @brief 获取 RGB888 图像的原始数据, 适合性能要求较高的场景
 *
 * @param image 图像实例
 * @return nvision_rgb888_t* RGB888 图像原始数据
 */
nvision_rgb888_t *nvision_image_rgb_raw(struct nvision_rgb888_image *image);

/**
 * @brief 获取 HSL 图像的原始数据, 适合性能要求较高的场景
 *
 * @param image 图像实例
 * @return nvision_gray_t* HSL 图像原始数据
 */
nvision_hsl_t *nvision_image_hsl_raw(struct nvision_hsl_image *image);

/**
 * @brief 获取灰度图像的原始数据, 适合性能要求较高的场景
 *
 * @param image 图像实例
 * @return nvision_gray_t* 灰度图像原始数据
 */
nvision_gray_t *nvision_image_gray_raw(struct nvision_gray_image *image);

/**
 * @brief 将 RGB888 颜色转换为 HSL 颜色
 *
 * @param rgb RGB888 颜色
 * @param hsl HSL 颜色
 */
void nvision_color_rgb_to_hsl(nvision_rgb888_t *rgb, nvision_hsl_t *hsl);

/**
 * @brief 将 RGB888 颜色转换为灰度值
 *
 * @param rgb RGB888 颜色
 * @param gray 灰度值
 */
void nvision_color_rgb_to_gray(nvision_rgb888_t *rgb, nvision_gray_t *gray);

/**
 * @brief 判断 HSL 颜色是否在指定的范围内
 *
 * @param hsl HSL 颜色
 * @param start_hsl 开始 HSL 颜色。当对应通道为 NVISION_HSL_DONT_CARE
 * 时，该通道不参与阈值判断
 * @param end_hsl 结束 HSL 颜色
 * @return uint8_t 1: 在范围内 0: 不在范围内
 */
uint8_t nvision_color_hsl_in_range(nvision_hsl_t *hsl, nvision_hsl_t *start_hsl,
                                   nvision_hsl_t *end_hsl);

/**
 * @brief 将灰度值转换为 RGB888 颜色
 *
 * @param rgb_image rgb 图像
 * @param hsl_image hsl 图像
 */
void nvision_image_rgb_to_hsl(struct nvision_rgb888_image *rgb_image,
                              struct nvision_hsl_image *hsl_image);

/**
 * @brief 将 RGB888 图像转换为灰度图像
 *
 * @param rgb_image rgb 图像
 * @param gray_image 灰度图像
 */
void nvision_image_rgb_to_gray(struct nvision_rgb888_image *rgb_image,
                               struct nvision_gray_image *gray_image);

/**
 * @brief 将灰度图像转换为二值化图像
 *
 * @param gray_image 灰度图像
 * @param threshold 阈值
 * @param mode 模式, 例如 NVISION_GRAY_THRESHOLD_MODE_BINARY | NVISION_GRAY_THRESHOLD_MODE_OTSU 表示使用大津法进行二值化
 * @see NVISION_GRAY_THRESHOLD_MODE_BINARY
 * @see NVISION_GRAY_THRESHOLD_MODE_BINARY_INV
 * @see NVISION_GRAY_THRESHOLD_MODE_OTSU
 */
void nvision_image_gray_threshold(struct nvision_gray_image *gray_image,
                                  nvision_gray_t threshold, uint16_t mode);

/**
 * @brief 将 HSL 图像转换为二值化图像
 *
 * @param hsl_image HSL 图像
 * @param binary_image 二值图像
 * @param start_hsl HSL 开始阈值, 设置对应通道为 NVISION_HSL_DONT_CARE
 * 时，该通道不参与阈值判断
 * @param end_hsl HSL 结束阈值
 * @param mode 模式 @see NVISION_HSL_THRESHOLD_MODE_BINARY @see
 * NVISION_HSL_THRESHOLD_MODE_BINARY_INV
 */
void nvision_image_hsl_threshold(struct nvision_hsl_image *hsl_image,
                                 struct nvision_gray_image *binary_image,
                                 nvision_hsl_t start_hsl, nvision_hsl_t end_hsl,
                                 uint16_t mode);

#endif // __NVISION_H__
