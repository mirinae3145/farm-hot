#include "IMU.h"

static IMU_Config _cfg;

static MPU6050 mpu(0x68);
static uint8_t FIFOBuffer[64];

static Quaternion q;
static VectorInt16 aaRaw, aaReal, gyroRaw;
static VectorFloat gravity;

bool IMU_begin(const IMU_Config& cfg) {
    _cfg = cfg;
    mpu.initialize();
    if(!mpu.testConnection()) return false;
    if(mpu.dmpInitialize()) return false;

    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);

    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);

    return true;
}

IMU_UpdateResult IMU_update() {
    IMU_UpdateResult res{};
    if(!mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) {
        res.hasData = false;
        return res;
    }

    mpu.dmpGetQuaternion(&q, FIFOBuffer);
    mpu.dmpGetAccel(&aaRaw,FIFOBuffer);
    mpu.dmpGetGyro(&gyroRaw,FIFOBuffer);

    mpu.dmpGetGravity(&gravity,&q);
    mpu.dmpGetLinearAccel(&aaReal,&aaRaw,&gravity);

    float ax = aaReal.x / 16384.0f;
    float ay = aaReal.y / 16384.0f;
    float az = aaReal.z / 16384.0f;
    res.aMag = sqrt(ax*ax + ay*ay + az*az);

    float gx = gyroRaw.x / 65.5f;
    float gy = gyroRaw.y / 65.5f;
    float gz = gyroRaw.z / 65.5f;
    res.omegaRate = sqrt(gx*gx + gy*gy + gz*gz);

    res.hasData = true;
    return res;
}

bool IMU_isNoMotion(const float* aArr, const float* omegaArr) {
    float aSum = 0;
    float omegaSum = 0;

    for (size_t i = 0; i < _cfg.frames; i++) {
        aSum += aArr[i];
        omegaSum += omegaArr[i];
    }

    return (aSum / _cfg.frames < _cfg.aThres) && (omegaSum / _cfg.frames < _cfg.omegaThres);
}
