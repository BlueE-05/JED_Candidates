// ._. Testing Motores
#include "Motors.h"

MotorDC motor_L1(1, "Motor Izquierda 1");
MotorDC motor_L2(2, "Motor Izquierda 2");
MotorDC motor_R1(3, "Motor Derecha 1");
MotorDC motor_R2(4, "Motor Derecha 2");

MotorPair motors_left(motor_L1, motor_L2);
MotorPair motors_right(motor_R1, motor_R2);

SensorMPU6050 mpu("MPU");

int baseSpeed = 255;

void setup() {
    Serial.begin(9600);
}

void loop() {
    motor_L1.moveForward(255, true);
    motor_L2.moveForward(255, true);
    motor_R1.moveForward(255, true);
    motor_R2.moveForward(255, true);
    delay(1000);
    Serial.println();

    motor_L1.moveBackward(255, true);
    motor_L2.moveBackward(255, true);
    motor_R1.moveBackward(255, true);
    motor_R2.moveBackward(255, true);
    delay(1000);
    Serial.println();

    motor_L1.stop(true);
    motor_L1.stop(true);
    motor_L1.stop(true);
    motor_L1.stop(true);
    delay(1000);
    Serial.println();
}