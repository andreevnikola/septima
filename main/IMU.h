#ifndef IMU_H
#define  IMU_H

#include <MPU9250_asukiaaa.h>
#include "FlightComputer.h"
#include <Wire.h>
#include <SimpleKalmanFilter.h>

#include "D3.h"

class D3;

class IMU : public MPU9250_asukiaaa {
    private:
        D3 calibratedAccelData();
        float accelXCalibrationValue, accelYCalibrationValue, accelZCalibrationValue;
    public:
        IMU();
        void output();
};

#endif