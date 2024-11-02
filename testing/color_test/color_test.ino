#include "Colors.h"
ColorTCS3200 colorSensor(2, 3, 4, 5, 6, "Color Sensor");

void setup() {
  Serial.begin(9600);
}

void loop() {
  colorSensor.readRaw(true);
  // despues de calibrar aka modificar el .h mins y maxs
  //int rgb[3];
  //colorSensor.getRGB(rgb, true);
  
  //otra prueba necesaria
  //colorSensor.isGreen(true);
  //colorSensor.isRed(true);
  delay(500);
}