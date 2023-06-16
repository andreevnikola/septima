#include "Wire.h"
#include <SimpleKalmanFilter.h>
#include "IMU.h"
#include "FlightComputer.h"
#include "D3.h"

// #define ROLL_CORRECTOR_PORT_PLUS 5
// #define ROLL_CORRECTOR_PORT_MINUS 6
#define YAW_CORRECTOR_PORT 9
#define PITCH_CORRECTOR_PORT 10
#define G_LED_PORT 8
#define BUZZER1_PORT 12

// MATH THINGS
const float EJECT_AT_FORCE = 2.5;
const float CAN_EJECT_AT = 3.0;
float highestSpeed = 0.0;

bool EJECTED = false;

class FlightComputer;
class IMU;
class D3;

FlightComputer* FlightComputer::instancePtr = NULL;

FlightComputer* flightComputer;

void setup() {

  // CONFIGURE PORTS
  pinMode(G_LED_PORT, OUTPUT);
  pinMode(BUZZER1_PORT, OUTPUT);
  // pinMode(ROLL_CORRECTOR_PORT_PLUS, OUTPUT);
  // pinMode(ROLL_CORRECTOR_PORT_MINUS, OUTPUT);

  // START SERIAL AND I2C CONNECTIONS
  Serial.begin(9600);

  tone(BUZZER1_PORT, 10000);
  digitalWrite(G_LED_PORT, HIGH);
  delay(500);
  noTone(BUZZER1_PORT);
  digitalWrite(G_LED_PORT, LOW);

  flightComputer = FlightComputer::getInstance();
}

void loop() {
  flightComputer->getIMU()->output();
  delay(500);
}