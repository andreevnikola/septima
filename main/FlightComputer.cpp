#include "FlightComputer.h"

  FlightComputer::FlightComputer()
  {
    imu = new IMU();
  }

  static FlightComputer* FlightComputer::getInstance()
  {
    if (instancePtr == NULL)
    {
      instancePtr = new FlightComputer();
       
      return instancePtr;
    }
    else
    {
      return instancePtr;
    }
  }

  IMU* FlightComputer::getIMU() {
    return this->imu;
  }