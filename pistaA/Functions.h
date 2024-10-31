// Dana Elizabeth Torres Estrada ._.
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Sensors.h"

//* Variables para resolver la pista A
int currentMapSide = 1; // La pista se divide en 4 lados, esta variable es el lado de la pista actual
int baseSpeed = 255; // Velocidad
bool direction = true; // giros hacia la derecha
int pistaLength = 90; // cm
int step = 1; // aumentar currentMapSide para giros a la derecha
float sideThreshold = 5.0;
float frontThreshold = 10.0;


//* Funciones para resolver la pista A
// Función para rotar hacia la derecha o izquierda
void rotate(MotorPair &motorsLeft, MotorPair &motorsRight, SensorMPU6050 &mpu, bool direction, float targetAngle = 90.0, float tolerance = 5.0, int speed = baseSpeed) {
    motorsLeft.stop();
    motorsRight.stop();

    // Mover motores en función de la dirección
    if (direction) { // Gira a la derecha
        motorsLeft.moveForward(speed);
        motorsRight.moveForward(speed);
    } else { // Gira a la izquierda
        motorsLeft.moveForward(speed);
        motorsRight.moveForward(speed);
    }

    int16_t initialAngle, currentAngle, gx, gy, gz;
    mpu.readGyroscope(gx, gy, gz, true); // Leer ángulo inicial
    initialAngle = gz; // Suponiendo que estamos usando el eje Z para el giro //!sujeto a cambios nose como esta conectada ahorita

    do {
        mpu.readGyroscope(gx, gy, gz, true); // Leer ángulo actual
        currentAngle = gz - initialAngle; // Calcular el ángulo girado
        delay(100); // Pausa para evitar saturar la lectura
    } while (abs(currentAngle) < targetAngle - tolerance || abs(currentAngle) > targetAngle + tolerance);

    // Detener los motores después de la rotación
    motorsLeft.stop();
    motorsRight.stop();

    Serial.println("Rotation complete.");
}

// Función para invertir la dirección
void changeDirection(MotorPair &motorsLeft, MotorPair &motorsRight, SensorMPU6050 &mpu, bool &direction){
    direction = !direction;
    step *= -1;
    rotate(motorsLeft, motorsRight, mpu, direction, 180.0);
}

// Función para centrarse en la pista
void center(MotorPair &motorsLeft, MotorPair &motorsRight, UltrasonicHCSR04 &ultrasonic, SensorQTR8 &qtr, SensorMPU6050 &mpu, bool &direction, int speed = baseSpeed, float tolerance = 5.0){
    bool tooFar = ultrasonic.getDistance() > (pistaLength/2);
    bool tooClose = ultrasonic.getDistance() > (pistaLength/2) + tolerance;

    while(tooFar || tooClose){
        if(tooFar){
            motorsLeft.moveForward(speed);
            motorsRight.moveForward(speed);
        }
        if(tooClose){
            motorsLeft.moveBackward(speed);
            motorsRight.moveBackward(speed);
        }

        if(qtr.readAny()){
            motorsLeft.stop();
            motorsRight.stop();
            changeDirection(motorsLeft, motorsRight, mpu, direction);
            while(ultrasonic.getDistance() > frontThreshold){
                motorsLeft.moveForward(speed);
                motorsRight.moveForward(speed);
            }
            rotate(motorsLeft, motorsRight, mpu, direction);
            break;
        }
    }
    motorsLeft.stop();
    motorsRight.stop();
}

#endif // FUNCTIONS_H