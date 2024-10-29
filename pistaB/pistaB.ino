// Dana Elizabeth Torres Estrada ._. || Pista B (Seguidor de Línea)
#include "Sensors.h"
#include "StateMachine.h"

// 4 motores en el motor shield MH Electronics V1
MotorDC motorA1(1, "Motor Izq 1");
MotorDC motorA2(2, "Motor Izq 2");
MotorDC motorB1(3, "Motor Der 1");
MotorDC motorB2(4, "Motor Der 2");

// Sensores Ultrasónicos: Left, Front, Right
UltrasonicHCSR04 us_Left(22, 23, "Ultrasonic Izq");
UltrasonicHCSR04 us_Front(24, 25, "Ultrasonic Frente");
UltrasonicHCSR04 us_Right(26, 27, "Ultrasonic Der");

// Instancia del sensor MPU6050
SensorMPU6050 sensorMPU;

// Sensor Color
ColorTCS3200 color_Sensor(28, 29, 30, 31, 32, "Sensor Color");

// Sensor QTR
int pinsQTR[] = {};
SensorQTR8 qtr_Sensor(pins, 4, "Linea QTR");

void setup() {
    Serial.begin(9600);
    sensorMPU.begin();
    stateMachine.setup();
}

void loop() {
    stateMachine.loop();
}