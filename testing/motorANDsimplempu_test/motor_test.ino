// ._. Testing Motores
#include "MPU.h"

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
    //delay(2000);
    
    Serial.println();


    motor_L1.stop(true);
    motor_L1.stop(true);
    motor_L1.stop(true);
    motor_L1.stop(true);
    //delay(3000);
    
    //rotate(motors_left, motors_right, mpu, true);
    delay(1000);
}