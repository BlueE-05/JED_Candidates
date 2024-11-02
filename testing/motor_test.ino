// ._. Testing Motores
#include "Sensors.h"

MotorDC motor_L1(1, "Motor Izquierda 1");
MotorDC motor_L2(1, "Motor Izquierda 2");
MotorDC motor_R1(1, "Motor Derecha 1");
MotorDC motor_R2(1, "Motor Derecha 2");

void setup() {
    Serial.begin(9600);
}

void loop() {
    motor_L1.moveForward(200, true);
    motor_L2.moveForward(200, true);
    motor_R1.moveForward(200, true);
    motor_R2.moveForward(200, true);
    delay(2000);

    motor_L1.moveBackward(150, true);
    motor_L2.moveBackward(150, true);
    motor_R1.moveBackward(150, true);
    motor_R2.moveBackward(150, true);
    delay(2000);

    motor_L1.stop(true);
    motor_L1.stop(true);
    motor_L1.stop(true);
    motor_L1.stop(true);
    delay(1000);
}