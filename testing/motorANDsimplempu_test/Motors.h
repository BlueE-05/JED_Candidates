// ._. || motores
#include <AFMotor.h>
#include "MPU.h"

class MotorDC {
private:
    AF_DCMotor motor;
    String name;  // Almacena el nombre para debug
    int pin;      // Almacena el número del pin

public:
    // Constructor: inicializa el motor en el puerto especificado y le da un nombre
    MotorDC(int port, String motorName) : motor(port), name(motorName), pin(port) {
        motor.setSpeed(255);  // Velocidad máxima predeterminada
    }

    // Método para avanzar
    void moveForward(int speed, bool verbose = false) {
        motor.setSpeed(constrain(speed, 0, 255)); // Limita la velocidad entre 0 y 255 para evitar fallos
        motor.run(FORWARD);
        if (verbose) {
            Serial.print(name);
            Serial.println(" is moving forward");
        }
    }

    // Método para retroceder
    void moveBackward(int speed, bool verbose = false) {
        motor.setSpeed(constrain(speed, 0, 255)); // Limita la velocidad entre 0 y 255
        motor.run(BACKWARD);
        if (verbose) {
            Serial.print(name);
            Serial.println(" is moving backward");
        }
    }

    // Método para parar
    void stop(bool verbose = false) {
        motor.run(RELEASE);
        
        if(verbose){
            Serial.print(name);
            Serial.println(" has stopped");
        }
    }
};

class MotorPair {
private:
    MotorDC& motor1; // Referencia al primer motor
    MotorDC& motor2; // Referencia al segundo motor

public:
    // Constructor: inicializa el par de motores
    MotorPair(MotorDC& m1, MotorDC& m2) : motor1(m1), motor2(m2) {}

    // Método para mover ambos motores hacia adelante
    void moveForward(int speed, bool verbose = false) {
        motor1.moveForward(speed, verbose);
        motor2.moveForward(speed, verbose);
    }

    // Método para mover ambos motores hacia atrás
    void moveBackward(int speed, bool verbose = false) {
        motor1.moveBackward(speed, verbose);
        motor2.moveBackward(speed, verbose);
    }

    // Método para detener ambos motores
    void stop(bool verbose = false) {
        motor1.stop(verbose);
        motor2.stop(verbose);
    }
};


// Función para rotar hacia la derecha o izquierda
void rotate(MotorPair &motorsLeft, MotorPair &motorsRight, SensorMPU6050 &mpu, bool direction, float targetAngle = 90.0, float tolerance = 5.0, int speed = baseSpeed) {
    // Detener los motores antes de comenzar la rotación
    motorsLeft.stop();
    motorsRight.stop();

    float initialAngle = mpu.getAngleZ(); // Leer ángulo actual
    float desiredAngle = direction ? (initialAngle + targetAngle) : (initialAngle - targetAngle);
    float currentAngle = mpu.getAngleZ();
    
    // Determinar la dirección de rotación y establecer el límite
    do {
        currentAngle = mpu.getAngleZ(true);

        // Mover motores en función de la dirección
        if (direction) { // Gira a la derecha
            motorsLeft.moveForward(speed);
            motorsRight.moveBackward(speed);
        } else { // Gira a la izquierda
            motorsLeft.moveBackward(speed);
            motorsRight.moveForward(speed);
        }
    } while ((direction && currentAngle <= desiredAngle - tolerance) || (!direction && currentAngle >= desiredAngle + tolerance));
    motorsLeft.stop();
    motorsRight.stop();

    Serial.println("Rotation complete.");
}