#pragma once
#include "MPU6050_6Axis_MotionApps20.h"

struct IMU_Config {
    int frames;
    float aThres;
    float omegaThres;
};

struct IMU_UpdateResult {
    bool hasData;
    float aMag;
    float omegaRate;
};

bool IMU_begin(const IMU_Config& cfg);
IMU_UpdateResult IMU_update();
bool IMU_isNoMotion(const float* aArr, const float* omegaArr);
