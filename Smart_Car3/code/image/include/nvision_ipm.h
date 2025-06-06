/**
 * @file nvision_ipm.h
 * @author Evan Xiao (1993996310@qq.com)
 * @brief (逆)透视变换。
 * @version 0.1
 * @date 2024-01-06
 *
 * @see https://zhuanlan.zhihu.com/p/637104222
 * @see https://www.ce.unipr.it/people/bertozzi/pap/cr/pbt.pdf
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __NVISION_IPM_H__
#define __NVISION_IPM_H__

#include "nvision.h"

/**
 * @brief 相机姿态
 *
 */
struct nvision_camera_pose {
    /**
     * @brief 相机高度
     *
     */
    float height;
    /**
     * @brief 相机与地面的夹角, 0为水平, 90为垂直, 方向为向下。
     *
     */
    float pitch;
    /**
     * @brief 相机横向视野, 单位为度。通过镜头的参数说明或者手动测量得到。
     *
     */
    float h_fov;
    /**
     * @brief 相机纵向视野, 单位为度。通过镜头的参数说明或者手动测量得到。
     *
     */
    float v_fov;
};

/**
 * @brief IPM 实例
 *
 */
struct nvision_ipm {
    struct nvision_camera_pose pose;
    nvision_size_t image_height;
    nvision_size_t image_width;
    float h_fov_div_width;
    float v_fov_div_height;
};

/**
 * @brief 真实坐标，单位为米。以相机在地面上的投影点为原点，相机前方为 x
 * 轴正方向，相机右侧为 y 轴正方向。
 *
 */
struct nvision_position {
    /**
     * @brief x 坐标
     *
     */
    float x;
    /**
     * @brief y 坐标
     *
     */
    float y;
};

typedef struct nvision_position nvision_position_t;

/**
 * @brief 初始化相机姿态
 *
 * @param pose 相机姿态
 * @param height 相机高度，单位为米
 * @param pitch 相机与地面的夹角, 0为水平, 90为垂直, 方向为向下。
 * @param fov 相机视野, 单位为度，Fov 请参考你的镜头说明书
 */
void nvision_camera_pose_init(struct nvision_camera_pose *pose, float height,
                              float pitch, float h_fov, float v_fov);

/**
 * @brief 初始化 IPM 实例
 *
 * @param ipm IPM 实例
 * @param pose 相机姿态
 * @param image_height 图片高度
 * @param image_width 图片宽度
 */
void nvision_ipm_init(struct nvision_ipm *ipm, struct nvision_camera_pose *pose,
                      nvision_size_t image_height, nvision_size_t image_width);

/**
 * @brief 将图像上的点投影到真实坐标
 *
 * @param ipm IPM 实例
 * @param point 图像上的点
 * @param position 真实坐标
 */
void nvision_ipm_to_world(struct nvision_ipm *ipm, nvision_point_t point,
                          nvision_position_t *position);


#endif /* __NVISION_IPM_H__ */