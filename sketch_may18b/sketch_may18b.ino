#include <MPU9250_asukiaaa.h>
MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt;
float gX ,gY ,gZ;

void setup() {
  //I2C connections
  Wire.begin();
  Serial.begin(9600);
  mySensor.setWire(&Wire);
  
  //DECLARE THE SENSORS YOU ARE GOING TO USE!
  //MUST DO!
  mySensor.beginGyro();
  mySensor.beginAccel();
}

double PITCH_calculate(float x, float y, float z){
  return atan2((- x) , sqrt(y * y + z * z)) * 57.3;
}

double ROLL_calculate(float y, float z){
  return atan2(y , z) * 57.3;
}

void loop() {
  
  //Accelerometer data connections (I2C)
  mySensor.accelUpdate();
  aX = mySensor.accelX();
  aY = mySensor.accelY();
  aZ = mySensor.accelZ();
  aSqrt = mySensor.accelSqrt();
  
  //Gyroscope data connections (I2C)
  mySensor.gyroUpdate();
  gX = mySensor.gyroX();
  gY = mySensor.gyroY();
  gZ = mySensor.gyroZ();
  
  //Accelerometer
  Serial.println("    ");
  Serial.println("Accel");
  Serial.println(aX);
  Serial.println(aY);
  Serial.println(aZ);
  
  //Gyroscope
  Serial.println("    ");
  Serial.println("Gyro");
  Serial.println(gX);
  Serial.println(gY);
  Serial.println(gZ);

  // Converted data to degrees
  Serial.println("    ");
  Serial.println("Degrees");
  Serial.println("PITCH " + String(PITCH_calculate(aX, aY, aZ)));
  Serial.println("ROLL " + String(ROLL_calculate(aY, aZ)));

  delay(1000);
}