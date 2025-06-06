/**
 * @file ipm.c
 * @author Evan Xiao (1993996310@qq.com)
 * @brief (逆)透视变换
 * @version 0.1
 * @date 2024-01-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "nvision.h"
#include "nvision_ipm.h"

#include <math.h>

#include <stdio.h>

#define NVISION_DEG2RAD(x) ((x)*3.14159/ 180.0f)

void nvision_camera_pose_init(struct nvision_camera_pose *pose, float height,
                              float pitch, float h_fov, float v_fov) {
    pose->height = height;
    pose->pitch = NVISION_DEG2RAD(pitch);
    pose->h_fov = NVISION_DEG2RAD(h_fov);
    pose->v_fov = NVISION_DEG2RAD(v_fov);
}

void nvision_ipm_init(struct nvision_ipm *ipm, struct nvision_camera_pose *pose,
                      nvision_size_t image_height, nvision_size_t image_width) {
    ipm->pose = *pose;
    ipm->image_height = image_height;
    ipm->image_width = image_width;
    ipm->h_fov_div_width = ipm->pose.h_fov / (ipm->image_width - 1);
    ipm->v_fov_div_height = ipm->pose.v_fov / (ipm->image_height - 1);
}

static float nvision_cot(float x) { return 1 / tanf(x); }

void nvision_ipm_to_world(struct nvision_ipm *ipm, nvision_point_t point,
                          nvision_position_t *position) {
    position->x = ipm->pose.height *
                  nvision_cot((ipm->pose.pitch - (ipm->pose.v_fov / 2)) +
                              point.y * ipm->v_fov_div_height) *
                  cosf((-ipm->pose.h_fov / 2) + point.x * ipm->h_fov_div_width);

    position->y = ipm->pose.height *
                  nvision_cot((ipm->pose.pitch - (ipm->pose.v_fov / 2)) +
                              point.y * ipm->v_fov_div_height) *
                  sinf((-ipm->pose.h_fov / 2) + point.x * ipm->h_fov_div_width);
}
