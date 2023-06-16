#ifndef FLIGHT_COMPUTER_H
#define FLIGHT_COMPUTER_H

#include "IMU.h"
#include "TVC.h"

class IMU;
class TVC;

class FlightComputer{
  private:

  IMU* imu;
  TVC* tvc;
     
  static FlightComputer*
         
  instancePtr;
   
  FlightComputer();

  void setIMU(IMU* leImu);
  void setTVC(TVC* leTvc);
   
  public:
  static FlightComputer* getInstance();

  IMU* getIMU();
  TVC* getTVC();
};

#endif