#pragma once
#include "utility/MPU9250.h"
#include "quaternionFilters2.h"

MPU9250 IMU;

class ARlikeMotion
{
public:
    static void Init()
    {
        Wire.begin();
        IMU.initMPU9250();
    }

    static void GetRotation(float rotation[][3]) // float[3][3]
    {
        float r[3][3] = {{*getR2(), *(getR2() + 3), *(getR2() + 6)}, {*(getR2() + 1), *(getR2() + 4), *(getR2() + 7)}, {*(getR2() + 2), *(getR2() + 5), *(getR2() + 8)}};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                rotation[i][j] = r[i][j];
    }

    static void ReviseIMU(float deltat)
    {
        if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
        {
            IMU.readAccelData(IMU.accelCount);
            IMU.getAres();

            IMU.ax = (float)IMU.accelCount[0] * IMU.aRes;
            IMU.ay = (float)IMU.accelCount[1] * IMU.aRes;
            IMU.az = (float)IMU.accelCount[2] * IMU.aRes;

            IMU.readGyroData(IMU.gyroCount);
            IMU.getGres();

            IMU.gx = (float)IMU.gyroCount[0] * IMU.gRes;
            IMU.gy = (float)IMU.gyroCount[1] * IMU.gRes;
            IMU.gz = (float)IMU.gyroCount[2] * IMU.gRes;
        }
        IMU.updateTime();
        // Don't use mag since it's too noisy indoor
        MadgwickQuaternionUpdateIMU2(IMU.ax, IMU.ay, IMU.az, IMU.gx * DEG_TO_RAD, IMU.gy * DEG_TO_RAD, IMU.gz * DEG_TO_RAD, deltat);
    }
};
