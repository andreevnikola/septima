#ifndef TVC_H
#define  TVC_H

#include "FlightComputer.h"
#include "IMU.h"
#include <Servo.h>

#define YAW_CORRECTOR_PORT 9
#define PITCH_CORRECTOR_PORT 10

class FlightComputer;
class IMU;

class TVC {
    private:
        Servo yawCorrector;
        Servo pitchCorrector;

        FlightComputer* fc;
        IMU* imu;
    public:
        TVC();
        void correct();
};

#endif