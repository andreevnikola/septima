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
        SimpleKalmanFilter pitchKalmanFilter = SimpleKalmanFilter(2, 2, 0.01);
        float yaw = 0;
        float roll = 0;
        float pitch = 0;
        D3 accData;
        D3 gyroData;
        float speed;

        float NULISH_TREASHHOLD = 0.1;
    public:
        IMU();
        D3 calibratedAccelData();
        D3 IMU::calibratedGyroData();
        float getSpeed();
        float getPitch();
        float getYaw();
        float getRoll();
        void updateSensors();
        void output();
};

#endif