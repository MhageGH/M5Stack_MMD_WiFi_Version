#pragma once
#include <Arduino.h>

class Madgwick {
  float GyroMeasError = PI * (10.0f / 180.0f);
  float GyroMeasDrift = PI * (0.01f / 180.0f);
  float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // Compute beta
  float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;
  float eInt[3] = {0.0f, 0.0f, 0.0f};
  float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
  float r[9] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  float invSqrt(float x);

 public:
  void Update(float ax, float ay, float az, float gx, float gy, float gz,
              float deltat);
  const float *getR2();
};