#pragma once
#include <Arduino.h>

void MadgwickQuaternionUpdateIMU2(float ax, float ay, float az, float gx, float gy, float gz, float deltat);
const float *getQ2();
const float *getR2();
void ResetQ();