// Dana Elizabeth Torres Estrada ._. || Pista B (Seguidor de Línea)
#include "Sensors.h"

// 4 motores en el motor shield MH Electronics V1
MotorDC motorA1(1, "Motor Izq 1");
MotorDC motorA2(2, "Motor Izq 2");
MotorDC motorB1(3, "Motor Der 1");
MotorDC motorB2(4, "Motor Der 2");

// Sensor Color
ColorTCS3200 color_Sensor(28, 29, 30, 31, 32, "Sensor Color");

// Sensor QTR
int pinsQTR[] = {};
SensorQTR8 qtr_Sensor(pinsQTR, 4, "Linea QTR");

void setup() {
    Serial.begin(9600);
}

void loop() {
}
