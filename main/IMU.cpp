#include "IMU.h"

IMU::IMU()
{
    Wire.begin();

    pinMode(G_LED_PORT, OUTPUT);
    pinMode(BUZZER1_PORT, OUTPUT);

    setWire(&Wire);
    beginAccel();
    beginGyro();
    beginMag();

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
    return sqrt(accData.X * accData.X + accData.Y * accData.Y + accData.Z * accData.Z) * 9.8;
}

float IMU::getPitch() { return pitchKalmanFilter.updateEstimate(atan2((-accData.X), sqrt(accData.Y * accData.Y + accData.Z * accData.Z)) * 57.3); }
float IMU::getRoll() { return atan2(accData.Y, accData.Z) * 57.3; }
float IMU::getYaw() { return atan(accData.Z / sqrt(accData.X * accData.X + accData.Z * accData.Z)) * 57.3; }

void IMU::updateSensors() {
    accelUpdate();
    gyroUpdate();

    accData = calibratedAccelData();
    gyroData = calibratedGyroData();
    speed = getSpeed();

    pitch = getPitch();
    roll = getRoll();
    yaw = getYaw();
}

void IMU::output()
{
    Serial.println("    ");
    Serial.println("Accel:");
    Serial.println(accData.X);
    Serial.println(accData.Y);
    Serial.println(accData.Z);
    Serial.println("Pitch: " + String(getPitch()) + "°");
    Serial.println("Yaw: " + String(getYaw()) + "°");
    Serial.println("Roll: " + String(getRoll()) + "°");
    Serial.println("SPEED " + String(speed) + "m/s");
    Serial.println("Gyro:");
    Serial.println(gyroData.X);
    Serial.println(gyroData.Y);
    Serial.println(gyroData.Z);

    magUpdate();
    Serial.println("MAG:");
    Serial.println(magX());
    Serial.println(magY());
    Serial.println(magZ());
    Serial.println(magHorizDirection());
}