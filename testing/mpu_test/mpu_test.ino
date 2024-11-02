// ._. || MPU test
#include "MPU.h"

SensorMPU6050 mpu("MPU");

void setup() {
  Serial.begin(9600);
  mpu.begin();
}

void loop() {
  // Probar esto primero
  mpu.read(true);
  
  // Luego esto para ver si giros a la derecha disminuyen o bajan el ángulo
  float numero = mpu.getAngleZ();
  Serial.print("Z = ");
  Serial.print(numero);
  Serial.println(" °");
}
