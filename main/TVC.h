#ifndef TVC_H
#define  TVC_H

#include "FlightComputer.h"
#include "IMU.h"

class FlightComputer;
class IMU;

class TVC {
    private:
        FlightComputer* fc;
        IMU* imu;
    public:
        TVC();
        void correct();
};

#endif