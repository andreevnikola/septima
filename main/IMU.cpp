#include "IMU.h"

IMU::IMU()
{
    Wire.begin();

    setWire(&Wire);
    beginGyro();
    beginAccel();

    accelUpdate();
    // CALIBRATE SENSORs
    Serial.println("PREPARE FOR CALIBRAION!");
    delay(500);
    Serial.println("CALIBRATING SENSORS...");
    accelXCalibrationValue = accelX();
    accelYCalibrationValue = accelY();
    accelZCalibrationValue = accelZ();
    Serial.println("ALL SENSORS CALIBRATED");
}

D3 IMU::calibratedAccelData(){
    D3 accelData(accelX() - accelXCalibrationValue, accelY() - accelYCalibrationValue, accelZ() - accelZCalibrationValue);
    return accelData;
}

void IMU::output()
{
    accelUpdate();
    D3 accData = calibratedAccelData();
    // const float aSqrt = accelSqrtCalibrated();

    // Accelerometer
    Serial.println("    ");
    Serial.println("Accel:");
    Serial.println(accData.X);
    Serial.println(accData.Y);
    Serial.println(accData.Z);
}