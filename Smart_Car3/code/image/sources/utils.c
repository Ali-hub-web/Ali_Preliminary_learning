/**
 * @file utils.c
 * @author Evan Xiao (1993996310@qq.com)
 * @brief 常用算法、工具函数
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "nvision.h"
#include "nvision_utils.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief 边缘提取 work-around 函数
 *
 */
typedef nvision_bool_t (*nvision_wa_func_t)(
    struct nvision_gray_image *gray_image, int8_t col_search_dir,
    int8_t row_search_dir, int16_t max_break_height, int16_t *x, int16_t *y);

/**
 * @brief 判断一个点是否被包裹
 *
 * @param gray_image 二值化图像
 * @param x
 * @param y
 * @return nvision_bool_t
 */
static nvision_bool_t
nvision_utils_is_surrounded(struct nvision_gray_image *gray_image,
                            nvision_size_t x, nvision_size_t y) {
    int16_t tmp_x, tmp_y;
    int8_t i;

    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            tmp_x = x + 1;
            tmp_y = y;
            break;
        case 1:
            tmp_x = x;
            tmp_y = y + 1;
            break;
        case 2:
            tmp_x = x;
            tmp_y = y - 1;
            break;
        case 3:
            tmp_x = x - 1;
            tmp_y = y;
            break;
        default:
            break;
        }

        // 超出边界也是为黑色
        if (tmp_x < 0 || tmp_x >= gray_image->width || tmp_y < 0 ||
            tmp_y >= gray_image->height) {
            continue;
        }

        if (gray_image->data[gray_image->width * tmp_y + tmp_x] != 0) {
            return NVISION_FALSE;
        }
    }

    return NVISION_TRUE;
}

/**
 * @brief 边缘提取 work-around 001: 在图像边缘处继续搜索
 *
 * @param gray_image
 * @param col_search_dir
 * @param row_search_dir
 * @param x
 * @param y
 * @return nvision_bool_t
 */
static nvision_bool_t edge_workaround_001(struct nvision_gray_image *gray_image,
                                          int8_t col_search_dir,
                                          int8_t row_search_dir,
                                          int16_t max_break_height, int16_t *x,
                                          int16_t *y) {
    int16_t tmp_x, tmp_y;
    int16_t searched_height = 0;
    tmp_x = *x;
    tmp_y = *y;

    if (tmp_x <= 2 || tmp_x >= gray_image->width - 2) {
        while (1) {
            // 沿着 y 方向搜索
            tmp_y += row_search_dir;
            // 判断是否超出图像范围
            if (tmp_y < 0 || tmp_y >= gray_image->height) {
                // 超出图像范围，搜索结束
                return NVISION_FALSE;
            }
            // 判断是否为边缘点
            if (gray_image->data[tmp_y * gray_image->width + tmp_x] == 0 &&
                !nvision_utils_is_surrounded(gray_image, tmp_x, tmp_y)) {
                // 是边缘点，保存该点
                *x = tmp_x;
                *y = tmp_y;
                return NVISION_TRUE;
            }
            searched_height++;
            if (searched_height >= max_break_height) {
                return NVISION_FALSE;
            }
        }
    }
    return NVISION_FALSE;
}

/**
 * @brief 判断一个点是否为孤立点(周围的点只有一个点为黑色)
 *
 * @param gray_image 二值化图像
 * @param x 坐标 X
 * @param y 坐标 Y
 * @return nvision_bool_t
 */
static nvision_bool_t
edge_is_standalone_pixel(struct nvision_gray_image *gray_image, int16_t x,
                         int16_t y) {
    int16_t tmp_x, tmp_y;
    int8_t i;
    int8_t black_count = 0;

    for (i = 0; i < 8; i++) {
        switch (i) {
        case 0:
            tmp_x = x + 1;
            tmp_y = y;
            break;
        case 1:
            tmp_x = x + 1;
            tmp_y = y + 1;
            break;
        case 2:
            tmp_x = x;
            tmp_y = y + 1;
            break;
        case 3:
            tmp_x = x - 1;
            tmp_y = y + 1;
            break;
        case 4:
            tmp_x = x - 1;
            tmp_y = y;
            break;
        case 5:
            tmp_x = x - 1;
            tmp_y = y - 1;
            break;
        case 6:
            tmp_x = x;
            tmp_y = y - 1;
            break;
        case 7:
            tmp_x = x + 1;
            tmp_y = y - 1;
            break;
        default:
            break;
        }

        // 超出边界也是为黑色
        if (tmp_x < 0 || tmp_x >= gray_image->width || tmp_y < 0 ||
            tmp_y >= gray_image->height) {
            black_count++;
            continue;
        }

        if (gray_image->data[gray_image->width * tmp_y + tmp_x] == 0) {
            black_count++;
        }
    }

    return black_count <= 1;
}

// work-around-002: 解决由于孤立点造成的盲路径现象
static nvision_bool_t edge_workaround_002(struct nvision_gray_image *gray_image,
                                          int8_t col_search_dir,
                                          int8_t row_search_dir,
                                          int16_t max_break_height, int16_t *x,
                                          int16_t *y) {
    int16_t tmp_x, tmp_y;
    tmp_y = *y;
    // 判断是否为孤立点，只有孤立点才会出现盲路径
    if (!edge_is_standalone_pixel(gray_image, *x, *y)) {
        return NVISION_FALSE;
    }
    // 某些情况下，盲路径是水平方向的，因此也需要移动 y 坐标
    for (int8_t j = 0; j < 3; j++) {
        tmp_y += row_search_dir;
        // 根据搜索方向，某侧横向查找
        for (int8_t i = 1; i < 3; i++) {
            // 横向查找
            tmp_x = *x + i * col_search_dir;
            // 像素点是否为黑色
            if (gray_image->data[tmp_y * gray_image->width + tmp_x] == 0 &&
                !nvision_utils_is_surrounded(gray_image, tmp_x, tmp_y)) {
                // 是边缘点，保存该点
                // 更新 x 和 y
                *x = tmp_x;
                *y = tmp_y;
                return NVISION_TRUE;
            }
        }
    }

    return NVISION_FALSE;
}

static nvision_wa_func_t workarounds[] = {
    edge_workaround_001,
    edge_workaround_002,
};

int16_t nvision_utils_edge_detect(struct nvision_gray_image *gray_image,
                                  enum nvision_edge_detect_dir detect_dir,
                                  int8_t start_row, int16_t max_break_height,
                                  struct nvision_micro_path *path,
                                  uint16_t path_len,
                                  int8_t *line_grow) {
    int8_t col_search_dir = 0;
    int8_t row_search_dir = 0;
    int16_t searched_height = 0;

    int16_t x = 0, y = 0,grow=0;
    int16_t tmp_x = 0, tmp_y = 0,tmp_grow=0;

    int8_t slope = 0;

    int32_t path_index = 0;

    if (detect_dir == NVISION_EDGE_DETECT_LEFT) {
        col_search_dir = -1;
    } else if (detect_dir == NVISION_EDGE_DETECT_RIGHT) {
        col_search_dir = 1;
    } else {
        return NVISION_EDGE_DETECT_UNKNOWN;
    }

    // 根据 start_row 的正负确定搜索方向
    if (start_row >= 0) {
        // 从下往上搜索
        row_search_dir = -1;
        y = gray_image->height - start_row - 1;
    } else {
        row_search_dir = 1;
        y = -start_row;
    }

    // 设置搜索起始点, x 从图像中心开始搜索
    x = gray_image->width / 2;

    // 先向左或向右搜索，直到找到边缘点，找不到则换行寻找
    while (y >= 0 && y <= gray_image->height) {
        while (x > 0 && x < gray_image->width) {
            if (gray_image->data[y * gray_image->width + x] == 0) {
                // 找到边缘点
                break;
            }
            x += col_search_dir;
        }
        if (gray_image->data[y * gray_image->width + x] == 0) {
            // 找到边缘点, 跳出外层循环
            break;
        }
        x = gray_image->width / 2;
        y += row_search_dir;

        searched_height++;
        if (searched_height >= max_break_height) {
            return NVISION_EDGE_DETECT_PATH_FULL;
        }
    }
    // 保存起始点
    path[path_index].x = x;
    path[path_index].y = y;
    path[path_index].slope = NVISION_PATH_SLOPE_UNKNOWN;
    // 8 邻域搜索边缘曲线
    while (1) {
        uint8_t found_edge = 0;
        int8_t i = 0;
        // 起始搜索斜率，会随着搜索过程变化
        int8_t start_slope = 0;
        // 搜索 8 邻域
        for (i = 0; i < 8; i++) {
            // 根据斜率计算下一个点的坐标，优先尝试 45 度
            switch ((i + start_slope) % 8) {
            case NVISION_PATH_SLOPE_0:
                tmp_x = x + 1;
                tmp_y = y;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_45:
                tmp_x = x + 1;
                tmp_y = y - 1;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_90:
                tmp_x = x;
                tmp_y = y - 1;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_135:
                tmp_x = x - 1;
                tmp_y = y - 1;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_180:
                tmp_x = x - 1;
                tmp_y = y;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_225:
                tmp_x = x - 1;
                tmp_y = y + 1;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_270:
                tmp_x = x;
                tmp_y = y + 1;
                tmp_grow=i;
                break;
            case NVISION_PATH_SLOPE_315:
                tmp_x = x + 1;
                tmp_y = y + 1;
                tmp_grow=i;
                break;
            default:
                break;
            }
            // 判断下一个点是否在图像范围内
            if (tmp_x < 0 || tmp_x >= gray_image->width || tmp_y < 0 ||
                tmp_y >= gray_image->height) {
                // 不在图像范围内，跳过
                continue;
            }
            // 判断下一个点是否为边缘点
            if (gray_image->data[tmp_y * gray_image->width + tmp_x] == 0) {
                // 判断这个点是否被包裹，如果被包裹，则不是边缘
                if (nvision_utils_is_surrounded(gray_image, tmp_x, tmp_y)) {
                    continue;
                }
                uint8_t is_duplicate = 0;
                // 确保跟近 5 个点不重复
                for (int8_t i = 1; i < 5; i++) {
                    if ((path_index - i) > 0 &&
                        path[path_index - i].x == tmp_x &&
                        path[path_index - i].y == tmp_y) {
                        is_duplicate = 1;
                        break;
                    }
                }
                if (is_duplicate) {
                    continue;
                }

                break;
            }
        }
        // 判断是否找到边缘点
        if (i == 8) {
            // 没有找到边缘点，尝试 work-around
            for (int8_t i = 0; i < sizeof(workarounds) / sizeof(workarounds[0]);
                 i++) {
                if (workarounds[i](gray_image, col_search_dir, row_search_dir,
                                   max_break_height, &x, &y)) {
                    found_edge = 1;
                    break;
                }
            }

            if (!found_edge) {
                break;
            }

            // Y 已经处于
        } else {
            // 找到边缘点，保存该点
            // 更新 x 和 y
            x = tmp_x;
            y = tmp_y;
            found_edge = 1;
            grow=tmp_grow;

        }

        // 保存找到的边缘点
        if (found_edge) {
            path_index++;
            if (path_index >= path_len) {
                return NVISION_EDGE_DETECT_PATH_FULL;
            }
            path[path_index].x = x;
            path[path_index].y = y;
            path[path_index].slope = (i + start_slope) % 8;
            line_grow[path_index]=grow;
            // 下次搜索的起始斜率
            start_slope = (i + start_slope) % 8;
            if (detect_dir == NVISION_EDGE_DETECT_LEFT) {
                start_slope -= 1;
            } else if (detect_dir == NVISION_EDGE_DETECT_RIGHT) {
                start_slope += 1;
            }

            found_edge = 0;
        }
    }

    return path_index + 1;
}

void nvision_utils_object_detect(struct nvision_gray_image *gray_image,
                                 nvision_size_t size,
                                 struct nvision_point *location) {
    // TODO: 未完成
}

int16_t nvision_utils_line_detect(struct nvision_micro_path *path,
                                  int16_t path_len, struct nvision_line *line,
                                  int16_t max_line_num, float max_slope_diff) {
    int16_t line_index = 0;
    int16_t path_index = 0;

    int16_t tmp_x, tmp_y;

    uint8_t is_first_line = 1;

    float current_line_slope = -1;
    float current_path_slope = 0;

    // 如果路径长度小于 5，直接返回

    if (path_len < 5) {
        return NVISION_LINE_DETECT_PATH_EMPTY;
    }
    while (1) {
        // 每次算4个点的平均 slope
        // 用于记录已经算入了多少个点
        int16_t path_count = 0;
        while (1) {
            // 斜率 UNKOWN 时，跳过
            if (path[path_index].slope == NVISION_PATH_SLOPE_UNKNOWN) {
                path_index++;
                continue;
            }
            // 计算斜率
            current_path_slope += path[path_index].slope;
            path_count++;
            path_index++;
            // 如果已经计算了 5 个点，则跳出循环
            if (path_count >= 5) {
                break;
            }
            if (path_index >= path_len) {
                return line_index + 1;
            }
        }
        // 计算平均斜率
        current_path_slope /= path_count;
        // 跟最后一次计算的斜率比较，如果差值大于
        // max_slope_diff，则认为是新的一条线
        float slope_diff = fabsf(current_path_slope - current_line_slope);

        // 由于角度特点，当斜率从 0 到 7 时，差值会很大，因此需要特殊处理
        if (slope_diff > 4) {
            slope_diff = 8 - slope_diff;
        }

        if (slope_diff > max_slope_diff) {
            // 新的一条线，更新上一条线的 end 坐标，并保存新的一条线的 start
            // 坐标
            if (!is_first_line) {
                line[line_index].end.x = path[path_index - 1 - 5].x;
                line[line_index].end.y = path[path_index - 1 - 5].y;
                line_index++;
            }
            is_first_line = 0;

            // 如果超出最大线条数量，则返回
            if (line_index >= max_line_num) {
                return NVISION_LINE_DETECT_LINE_FULL;
            }

            line[line_index].start.x = path[path_index - 1 - 5].x;
            line[line_index].start.y = path[path_index - 1 - 5].y;

        } else {
            // 更新 end 坐标
            line[line_index].end.x = path[path_index - 1].x;
            line[line_index].end.y = path[path_index - 1].y;
        }
        current_line_slope = current_path_slope;
    }

    return line_index + 1;
}
