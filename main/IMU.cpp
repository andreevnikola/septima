#include "IMU.h"

IMU::IMU()
{
    Wire.begin();

    pinMode(G_LED_PORT, OUTPUT);
    pinMode(BUZZER1_PORT, OUTPUT);

    setWire(&Wire);
    beginAccel();
    beginGyro();

    accelUpdate();
    gyroUpdate();

    // CALIBRATE SENSORs
    Serial.println("PREPARE FOR CALIBRAION!");
    tone(BUZZER1_PORT, 10000);
    digitalWrite(G_LED_PORT, HIGH);
    delay(500);
    Serial.println("CALIBRATING SENSORS...");
    accelXCalibrationValue = accelX();
    accelYCalibrationValue = accelY();
    accelZCalibrationValue = accelZ();
    gyroXCalibrationValue = gyroX();
    gyroYCalibrationValue = gyroY();
    gyroZCalibrationValue = gyroZ();
    noTone(BUZZER1_PORT);
    digitalWrite(G_LED_PORT, LOW);
    Serial.println("ALL SENSORS CALIBRATED");
}

D3 IMU::calibratedAccelData(){
    D3 accelData(accelX() - accelXCalibrationValue, accelZ() - accelZCalibrationValue, -accelY() + accelYCalibrationValue);
    return accelData;
}

D3 IMU::calibratedGyroData(){
    D3 gyroData(gyroX() - gyroXCalibrationValue, gyroZ() - gyroZCalibrationValue, -gyroY() + gyroYCalibrationValue);
    return gyroData;
}

float IMU::getSpeed() {
    D3 accel = calibratedAccelData();
    return sqrt(accel.X * accel.X + accel.Y * accel.Y + accel.Z * accel.Z) * 9.8;
}

void IMU::output()
{
    accelUpdate();
    D3 accData = calibratedAccelData();
    gyroUpdate();
    D3 gyroData = calibratedGyroData();
    const float speed = getSpeed();

    // Accelerometer
    Serial.println("    ");
    Serial.println("Accel:");
    Serial.println(accData.X);
    Serial.println(accData.Y);
    Serial.println(accData.Z);
    Serial.println("SPEED " + String(speed) + "m/s");
    Serial.println("MAG:");
    Serial.println(gyroData.X);
    Serial.println(gyroData.Y);
    Serial.println(gyroData.Z);
}