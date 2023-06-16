#ifndef IMU_H
#define  IMU_H

#include <MPU9250_asukiaaa.h>
#include "FlightComputer.h"
#include <Wire.h>
#include <SimpleKalmanFilter.h>

#include "D3.h"

#define G_LED_PORT 8
#define BUZZER1_PORT 12

class D3;

class IMU : public MPU9250_asukiaaa {
    private:
        float accelXCalibrationValue, accelYCalibrationValue, accelZCalibrationValue;
        float gyroXCalibrationValue, gyroYCalibrationValue, gyroZCalibrationValue;
    public:
        IMU();
        D3 calibratedAccelData();
        D3 IMU::calibratedGyroData();
        float getSpeed();
        void output();
};

#endif