#include "TVC.h"

TVC::TVC() {
    fc = FlightComputer::getInstance();
    imu = fc->getIMU();

    yawCorrector.attach(YAW_CORRECTOR_PORT);
    pitchCorrector.attach(PITCH_CORRECTOR_PORT);
}

void TVC::correct() {
    yawCorrector.write(int(imu->getYaw() / 2 + 90));
    pitchCorrector.write(int(imu->getPitch() / 2 + 90));

    Serial.println("   ");
}