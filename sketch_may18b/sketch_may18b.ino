#include "Wire.h"
#include <MPU9250_asukiaaa.h>
MPU9250_asukiaaa mySensor;
#include <Servo.h>
#include <SimpleKalmanFilter.h>

Servo yawCorrector;
Servo pitchCorrector;

#define ROLL_CORRECTOR_PORT_PLUS 5
#define ROLL_CORRECTOR_PORT_MINUS 6
#define YAW_CORRECTOR_PORT 9
#define PITCH_CORRECTOR_PORT 10
#define G_LED_PORT 8
#define BUZZER1_PORT 12

//MATH THINGS
const float EJECT_AT_FORCE = 2.5;
const float CAN_EJECT_AT = 3.0;
float highestSpeed = 0.0;
bool EJECTED = false;

SimpleKalmanFilter pitchKalmanFilter(2, 2, 0.01);
SimpleKalmanFilter yawKalmanFilter(2, 2, 0.01);
SimpleKalmanFilter rollKalmanFilter(2, 2, 0.05);
SimpleKalmanFilter forceKalmanFilter(2, 2, 0.5);

float PITCH_calculate(float x, float y, float z){
  return float(atan2((- x) , sqrt(y * y + z * z)) * 57.3);
}

float ROLL_calculate(float y, float z){
  return float(atan2(y , z) * 57.3);
}

float YAW_calculate(float x, float z){
  return float(atan(z/sqrt(x*x + z*z)) * 57.3);
}

int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return b - a;
}

void setup() {

  //CONFIGURE PORTS
  pinMode(G_LED_PORT, OUTPUT);
  pinMode(BUZZER1_PORT,OUTPUT);
  pinMode(ROLL_CORRECTOR_PORT_PLUS, OUTPUT);
  pinMode(ROLL_CORRECTOR_PORT_MINUS, OUTPUT);

  //ATTACH SEROVS
  yawCorrector.attach(YAW_CORRECTOR_PORT);
  pitchCorrector.attach(PITCH_CORRECTOR_PORT);

  //START SERIAL AND I2C CONNECTIONS
  Serial.begin(9600);
  Wire.begin();
  mySensor.setWire(&Wire);

  tone(BUZZER1_PORT,10000);
  digitalWrite(G_LED_PORT,HIGH);
  delay(500);
  noTone(BUZZER1_PORT);
  digitalWrite(G_LED_PORT,LOW);

  //START SENSOR READINGS
  mySensor.beginAccel();
  mySensor.beginGyro();
}

void loop() {
  mySensor.accelUpdate();
  const float aX = mySensor.accelX();
  const float aY = mySensor.accelY();
  const float aZ = mySensor.accelZ();
  const float force = forceKalmanFilter.updateEstimate(mySensor.accelSqrt());

  const float pitch = pitchKalmanFilter.updateEstimate(PITCH_calculate(aX, aY, aZ));
  const float yaw = yawKalmanFilter.updateEstimate(YAW_calculate(aX, aZ));
  
  mySensor.gyroUpdate();
  const float roll = rollKalmanFilter.updateEstimate(mySensor.gyroY());

  // FIXING ROLL
  if (roll > 0) {
    analogWrite(ROLL_CORRECTOR_PORT_PLUS, roll * 10);
    digitalWrite(ROLL_CORRECTOR_PORT_MINUS, LOW);
  } else {
    analogWrite(ROLL_CORRECTOR_PORT_MINUS, int(uint8_t(roll * 10)));
    digitalWrite(ROLL_CORRECTOR_PORT_PLUS, LOW);
  }

  //TRUST VECTOR CONTROLL
  yawCorrector.write(int(yaw / 2 + 90));
  pitchCorrector.write(int(pitch / 2 + 90));

  //EGECTION SYSTEM
  highestSpeed = max(highestSpeed, force);
  if (highestSpeed > CAN_EJECT_AT && force < EJECT_AT_FORCE && !EJECTED) {

    digitalWrite(G_LED_PORT,HIGH);
    tone(BUZZER1_PORT,10000);

    Serial.println("================================");
    Serial.println("=====EJECTING THE PARACHUTE=====");
    Serial.println("================================");
    EJECTED = true;

    delay(5000);
    digitalWrite(G_LED_PORT,LOW);
    noTone(BUZZER1_PORT);
  }

  Serial.print("PITCH : ");
  Serial.print(pitch);
  Serial.print("\tROLL : ");
  Serial.print(roll);
  Serial.print("\tYAW : ");
  Serial.println(yaw);
  Serial.print("\tFORCE : ");
  Serial.println(force);
}