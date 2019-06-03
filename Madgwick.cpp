#include "Madgwick.h"

#define Kp 2.0f * 5.0f
#define Ki 0.0f

float Madgwick::invSqrt(float x) {
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  y = y * (1.5f - (halfx * y * y));
  return y;
}

void Madgwick::Update(float ax, float ay, float az, float gx, float gy,
                      float gz, float deltat) {
  float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2,
      q3q3;

  // Rate of change of quaternion from gyroscope
  qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
  qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
  qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
  qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

  // Compute feedback only if accelerometer measurement valid (avoids NaN in
  // accelerometer normalisation)
  if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    // Auxiliary variables to avoid repeated arithmetic
    _2q0 = 2.0f * q0;
    _2q1 = 2.0f * q1;
    _2q2 = 2.0f * q2;
    _2q3 = 2.0f * q3;
    _4q0 = 4.0f * q0;
    _4q1 = 4.0f * q1;
    _4q2 = 4.0f * q2;
    _8q1 = 8.0f * q1;
    _8q2 = 8.0f * q2;
    q0q0 = q0 * q0;
    q1q1 = q1 * q1;
    q2q2 = q2 * q2;
    q3q3 = q3 * q3;

    // Gradient decent algorithm corrective step
    s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
    s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 +
         _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
    s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 +
         _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
    s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
    recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 +
                        s3 * s3);  // normalise step magnitude
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    // Apply feedback step
    qDot1 -= beta * s0;
    qDot2 -= beta * s1;
    qDot3 -= beta * s2;
    qDot4 -= beta * s3;
  }

  // Integrate rate of change of quaternion to yield quaternion
  q0 += qDot1 * deltat;
  q1 += qDot2 * deltat;
  q2 += qDot3 * deltat;
  q3 += qDot4 * deltat;

  // Normalise quaternion
  recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;

  // 補正
  q0 += 0.000021;
  q1 += 0.000018;
  q2 += 0.000152;
  q3 += 0.000177;

  q[0] = q0;
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;

  float w = q[0];
  float x = q[1];
  float y = q[2];
  float z = q[3];
  r[0] = w * w + x * x - y * y - z * z;
  r[1] = 2.0 * x * y - 2.0 * w * z;
  r[2] = 2.0 * x * z + 2.0 * w * y;
  r[3] = 2.0 * x * y + 2.0 * w * z;
  r[4] = w * w - x * x + y * y - z * z;
  r[5] = 2.0 * y * z - 2.0 * w * x;
  r[6] = 2.0 * x * z - 2.0 * w * y;
  r[7] = 2.0 * y * z + 2.0 * w * x;
  r[8] = w * w - x * x - y * y + z * z;
  // for (int i = 0; i < 4; ++i) Serial.printf("q[%d] = %f\r\n", i, q[i]);
}

const float* Madgwick::getR2() { return r; }