#ifndef FLIGHT_COMPUTER_H
#define FLIGHT_COMPUTER_H

#include "IMU.h"

class IMU;

class FlightComputer{
  private:

  IMU* imu;
     
  static FlightComputer*
         
  instancePtr;
   
  FlightComputer();
   
  public:
  static FlightComputer* getInstance();

  IMU* getIMU();
};

#endif