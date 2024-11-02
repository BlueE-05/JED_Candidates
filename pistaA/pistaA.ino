// ._. || Pista A (Laberinto con la pelota)
#include "StateMachine.h"
#include "Functions.h"
#include <Servo.h>

//* ----------------------------------------------
//* Declaración de sensores
//* ----------------------------------------------
// 4 motores en el motor shield MH Electronics V1
MotorDC motorA1(1, "Motor Izq 1");
MotorDC motorA2(2, "Motor Izq 2");
MotorDC motorB1(3, "Motor Der 1");
MotorDC motorB2(4, "Motor Der 2");

// Instancia de pares de motores
MotorPair motorA(motorA1, motorA2);
MotorPair motorB(motorB1, motorB2);

// Sensores Ultrasónicos: Left, Front, Right
UltrasonicHCSR04 us_Left(22, 23, "Ultrasonic Izq");
UltrasonicHCSR04 us_Front(24, 25, "Ultrasonic Frente");
UltrasonicHCSR04 us_Right(26, 27, "Ultrasonic Der");

// Instancia del sensor MPU6050
SensorMPU6050 mpu("MPU");

// Sensor Color
ColorTCS3200 color_Sensor(28, 29, 30, 31, 32, "Sensor Color");

// Sensor QTR
int pinsQTR[] = {33, 34, 35, 36};
SensorQTR8 qtr_Sensor(pinsQTR, 4, "Linea QTR");

// Servos para la garra
Servo servo1;
Servo servo2;
int servo_val = 90; //ángulo en grados para levantar la garra
float dist_enter = 5.0; //distancia que debe avanzar para entrar al cuadrado interno

// Instanciar maquina de estados
StateMachine stateMachine;



//* ----------------------------------------------
//* Ejecución del código
//* ----------------------------------------------
void setup() {
    Serial.begin(9600);
    mpu.begin();
    servo1.attach(9);
    servo2.attach(10);
}

void loop() {
    stateMachine.run();
}



//* ----------------------------------------------
//* Definición de métodos de la máquina de estados
//* ----------------------------------------------
// Método para entrar a la pista
void StateMachine::phaseStart(){
    float startingDistance = us_Front.getDistance(); // leer distancia inicial para avanzar 30cm/1 cuadro
    while(us_Front.getDistance() > (startingDistance-25) || color_Sensor.isGreen()){ // Avanzar hasta aproximadamente 30cm para entrar a la pista
        motorA.moveForward(baseSpeed);
        motorB.moveForward(baseSpeed);
    }
    rotate(motorA, motorB, mpu, false); // Gira hacia la izquierda

    currentState = FETCH_BALL;
}

// Método para recorrer la pista buscando la pelota en el cuadrado interno
void StateMachine::phaseFetchBall(){
    bool blackLine = false;

    // Centrarse en el lado de la pista
    center(motorA, motorB, us_Front, qtr_Sensor, mpu, direction);

    // Verificar si la pelota se encuentra en ese lado de la pista
    if(direction){ // Dirección hacia la derecha, revisar el ultrasónico derecho
        if(us_Right.getDistance() > sideThreshold)
            currentState = GRAB_BALL;
    }else{ // Dirección hacia la izquierda, revisar el ultrasónico izquierdo
        if(us_Left.getDistance() > sideThreshold)
            currentState = GRAB_BALL;
    }

    // Si aun no se encuentra la pelota, avanzar al siguiente lado de la pista
    while(us_Front.getDistance() > frontThreshold && !blackLine){
        motorA.moveForward(baseSpeed);
        motorB.moveForward(baseSpeed);
        
        if(qtr_Sensor.readAny()){ // Si detecta una línea en el suelo, cambiar la dirección y continuar recorrido
            motorA.stop();
            motorB.stop();
            blackLine = true;
        }
    }

    // Girar en la esquina o girar 180 para cambiar de dirección
    if(blackLine)
        changeDirection(motorA, motorB, mpu, direction);
    else{
        rotate(motorA, motorB, mpu, direction);

        // Actualizar el lado de la pista en la que se encuentra el robot
        currentMapSide += step;
        if(currentMapSide < 1){
            currentMapSide = 4;
        }
        if(currentMapSide > 4){
            currentMapSide = 1;
        }
    }
}

// Método para tomar la pelota y sacarla del cuadrado interno
void StateMachine::phaseGrabBall(){
    rotate(motorA, motorB, mpu, direction); // Girar para quedar frente a la pelota
    // servos up
    servo1.write(servo_val);
    servo2.write(-servo_val);
    int initialDist = us_Front.getDistance();
    while(us_Front.getDistance() > (initialDist - dist_enter)){
        motorA.moveForward(baseSpeed);
        motorB.moveForward(baseSpeed);
    }
    // servos down
    servo1.write(0);
    servo2.write(0);
    while(us_Front.getDistance() < (initialDist - dist_enter)){
        motorA.moveBackward(baseSpeed);
        motorB.moveBackward(baseSpeed);
    }
    rotate(motorA, motorB, mpu, !direction); // regresar a la posición inicial

    // Preparaciones para la siguiente fase
    if(currentMapSide == 3){
        rotate(motorA, motorB, mpu, direction); // quedar frente a la salida
        currentState = FINISH;
    } else if(!direction || currentMapSide == 4){
        changeDirection(motorA, motorB, mpu, direction);
    }

    currentState = FETCH_EXIT;
}

// Método para recorrer las paredes para encontrar la salida
void StateMachine::phaseFetchExit(){
    bool blackLine = false;
    center(motorA, motorB, us_Front, qtr_Sensor, mpu, direction);
    if(currentMapSide == 4){
        rotate(motorA, motorB, mpu, direction);
        currentState = FINISH;
    }
    // Si aun no se encuentra en el lado 3
    while(us_Front.getDistance() > frontThreshold && !blackLine){
        motorA.moveForward(baseSpeed);
        motorB.moveForward(baseSpeed);
        
        if(qtr_Sensor.readAny()){ // Si detecta una línea en el suelo, cambiar la dirección y continuar recorrido
            motorA.stop();
            motorB.stop();
            blackLine = true;
        }
    }
    // Girar en la esquina o girar 180 para cambiar de dirección
    if(blackLine)
        changeDirection(motorA, motorB, mpu, direction);
    else{
        rotate(motorA, motorB, mpu, direction);

        // Actualizar el lado de la pista en la que se encuentra el robot
        currentMapSide += step;
        if(currentMapSide < 1){
            currentMapSide = 4;
        }
        if(currentMapSide > 4){
            currentMapSide = 1;
        }
    }
}

// Método para salir de la pista
void StateMachine::phaseFinish(){
    if( us_Front.getDistance() > frontThreshold || !color_Sensor.isRed()){
        motorA.moveForward(baseSpeed);
        motorB.moveForward(baseSpeed);
    }
    motorA.stop();
    motorB.stop();
}