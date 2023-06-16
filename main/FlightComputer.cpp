#include "FlightComputer.h"

FlightComputer::FlightComputer()
{
}

static FlightComputer *FlightComputer::getInstance()
{
  if (instancePtr == NULL)
  {
    instancePtr = new FlightComputer();

    instancePtr->setIMU(new IMU());
    instancePtr->setTVC(new TVC());

    return instancePtr;
  }
  else
  {
    return instancePtr;
  }
}

IMU *FlightComputer::getIMU()
{
  return this->imu;
}

TVC *FlightComputer::getTVC()
{
  return this->tvc;
}

void FlightComputer::setIMU(IMU *leImu)
{
  imu = leImu;
}

void FlightComputer::setTVC(TVC *leTVC)
{
  tvc = leTVC;
}