#include "Wire.h"
#include <SimpleKalmanFilter.h>
#include "IMU.h"
#include "FlightComputer.h"
#include "D3.h"
#include "TVC.h"

// #define ROLL_CORRECTOR_PORT_PLUS 5
// #define ROLL_CORRECTOR_PORT_MINUS 6
#define YAW_CORRECTOR_PORT 9
#define PITCH_CORRECTOR_PORT 10

// MATH THINGS
const float EJECT_AT_FORCE = 2.5;
const float CAN_EJECT_AT = 3.0;
float highestSpeed = 0.0;

bool EJECTED = false;

class FlightComputer;
class IMU;
class D3;
class TVC;

FlightComputer* FlightComputer::instancePtr = NULL;

FlightComputer* flightComputer;

void setup() {
  // START SERIAL AND I2C CONNECTIONS
  Serial.begin(9600);

  flightComputer = FlightComputer::getInstance();
}

void loop() {
  flightComputer->getIMU()->output();
  flightComputer->getTVC()->correct();
  delay(350);
}