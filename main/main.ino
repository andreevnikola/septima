#include "Wire.h"
#include <MPU9250_asukiaaa.h>
#include <Servo.h>
#include <SimpleKalmanFilter.h>

Servo yawCorrector;
Servo pitchCorrector;

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

class MySensor : public MPU9250_asukiaaa {
public:
  float magZOffset, gyroXOffset, gyroYOffset, gyroZOffset, accelXOffset, accelYOffset, accelZOffset;
  SimpleKalmanFilter pitchKalmanFilter = SimpleKalmanFilter(2, 2, 0.1);
  SimpleKalmanFilter yawKalmanFilter = SimpleKalmanFilter(2, 2, 0.1);
  SimpleKalmanFilter rollKalmanFilter = SimpleKalmanFilter(2, 2, 0.5);
  SimpleKalmanFilter forceKalmanFilter = SimpleKalmanFilter(2, 2, 0.5);
  SimpleKalmanFilter gyroYKalmanFilter = SimpleKalmanFilter(2, 2, 1);
  SimpleKalmanFilter gyroXKalmanFilter = SimpleKalmanFilter(2, 2, 1);
  SimpleKalmanFilter gyroZKalmanFilter = SimpleKalmanFilter(2, 2, 1);

  float accelY() {
    return MPU9250_asukiaaa::accelY() - accelYOffset;
  }

  float accelX() {
    return MPU9250_asukiaaa::accelX() - accelXOffset;
  }

  float accelZ() {
    return MPU9250_asukiaaa::accelZ() - accelZOffset;
  }

  float pitch() {
    return pitchKalmanFilter.updateEstimate(PITCH_calculate(accelX(), accelY(), accelZ()));
  }

  float yaw() {
    return yawKalmanFilter.updateEstimate(YAW_calculate(accelX(), accelZ()));
  }

  float roll() {
    return rollKalmanFilter.updateEstimate(gyroY());
  }

  float force() {
    return forceKalmanFilter.updateEstimate(MPU9250_asukiaaa::accelSqrt());
  }

  float gyroY() {
    return MPU9250_asukiaaa::gyroY() - gyroYOffset;
  }
  float gyroX() {
    return MPU9250_asukiaaa::gyroX() - gyroXOffset;
  }
  float gyroZ() {
    return MPU9250_asukiaaa::gyroZ() - gyroZOffset;
  }
  float magZ() {
    return MPU9250_asukiaaa::magZ() - magZOffset;
  }

  float *rotation() {
    float rotation[3] = {
      gyroXKalmanFilter.updateEstimate(90 - atan(magZ() / magY()) * 180 / M_PI),
      gyroYKalmanFilter.updateEstimate(90 - atan(magZ() / magX()) * 180 / M_PI),
      gyroZKalmanFilter.updateEstimate(90 - atan(magY() / magX()) * 180 / M_PI),
    };
    return rotation;
  }



  float PITCH_calculate(float x, float y, float z) {
    return float(atan2((-x), sqrt(y * y + z * z)) * 57.3);
  }

  float ROLL_calculate(float y, float z) {
    return float(atan2(y, z) * 57.3);
  }

  float YAW_calculate(float x, float z) {
    return float(atan(z / sqrt(x * x + z * z)) * 57.3);
  }
};

MySensor mySensor;

int sort_desc(const void *cmp1, const void *cmp2) {
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return b - a;
}

void setup() {

  // CONFIGURE PORTS
  pinMode(G_LED_PORT, OUTPUT);
  pinMode(BUZZER1_PORT, OUTPUT);
  // pinMode(ROLL_CORRECTOR_PORT_PLUS, OUTPUT);
  // pinMode(ROLL_CORRECTOR_PORT_MINUS, OUTPUT);

  // ATTACH SEROVS
  yawCorrector.attach(YAW_CORRECTOR_PORT);
  pitchCorrector.attach(PITCH_CORRECTOR_PORT);

  // START SERIAL AND I2C CONNECTIONS
  Serial.begin(9600);
  Wire.begin();
  mySensor.setWire(&Wire);

  tone(BUZZER1_PORT, 10000);
  digitalWrite(G_LED_PORT, HIGH);
  delay(500);
  noTone(BUZZER1_PORT);
  digitalWrite(G_LED_PORT, LOW);

  // START SENSOR READINGS
  mySensor.beginAccel();
  mySensor.beginMag();
  mySensor.beginGyro();

  // CALIBRATE SENSORs
  Serial.println("PREPARE FOR CALIBRAION!");
  delay(500);
  Serial.println("CALIBRATING SENSORS...");
  mySensor.magUpdate();
  mySensor.accelUpdate();
  mySensor.gyroUpdate();
  mySensor.magXOffset = mySensor.magX();
  mySensor.magYOffset = mySensor.magY();
  mySensor.magZOffset = mySensor.magZ();
  mySensor.gyroXOffset = mySensor.gyroX();
  mySensor.gyroYOffset = mySensor.gyroY();
  mySensor.gyroZOffset = mySensor.gyroZ();
  mySensor.accelXOffset = mySensor.accelX();
  mySensor.accelYOffset = mySensor.accelY();
  mySensor.accelZOffset = mySensor.accelZ();
  Serial.println("ALL SENSORS CALIBRATED");
}

void loop() {
  mySensor.accelUpdate();
  const float force = mySensor.force();

  const float pitch = mySensor.pitch();
  const float yaw = mySensor.yaw();

  mySensor.gyroUpdate();
  const float roll = mySensor.roll();

  // // FIXING ROLL
  // if (roll > 0) {
  //   analogWrite(ROLL_CORRECTOR_PORT_PLUS, roll * 10);
  //   digitalWrite(ROLL_CORRECTOR_PORT_MINUS, LOW);
  // } else {
  //   analogWrite(ROLL_CORRECTOR_PORT_MINUS, int(uint8_t(roll * 10)));
  //   digitalWrite(ROLL_CORRECTOR_PORT_PLUS, LOW);
  // }

  // TRUST VECTOR CONTROLL
  yawCorrector.write(int(yaw / 2 + 90));
  pitchCorrector.write(int(pitch / 2 + 90));

  const float speed = force * 9.8;

  mySensor.magUpdate();
  const float *rotation = mySensor.rotation();

  // EGECTION SYSTEM
  highestSpeed = max(highestSpeed, force);
  if (highestSpeed > CAN_EJECT_AT && !EJECTED && (force < EJECT_AT_FORCE || (rotation[2] > 50 && -rotation[2] > 50))) {

    digitalWrite(G_LED_PORT, HIGH);
    tone(BUZZER1_PORT, 10000);

    Serial.println("================================");
    Serial.println("=====EJECTING THE PARACHUTE=====");
    Serial.println("================================");
    EJECTED = true;

    delay(5000);
    digitalWrite(G_LED_PORT, LOW);
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
  Serial.print("\tSPEED : ");
  Serial.println(speed);
  Serial.print("\tXRORATION : ");
  Serial.println(rotation[0]);
  Serial.print("\tYRORATION  : ");
  Serial.println(rotation[1]);
  Serial.print("\tZROTATION : ");
  Serial.println(rotation[2]);
}