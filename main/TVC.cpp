#include "TVC.h"

TVC::TVC() {
    fc = FlightComputer::getInstance();
    imu = fc->getIMU();
}

void TVC::correct() {
    Serial.println("TVC correct");
}