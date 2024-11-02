// Dana Elizabeth Torres Estrada ._.
#ifndef SENSORS_H
#define SENSORS_H

// verificar q librerias se requieren
#include <AFMotor.h>
#include <Wire.h>
#include <MPU6050.h>
#include <QTRSensors.h>

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

class UltrasonicHCSR04 {
private:
    int triggerPin;  // Pin de trigger
    int echoPin;     // Pin de echo
    String name;     // Nombre del sensor para debug

public:
    // Constructor: inicializa los pines y el nombre del sensor
    UltrasonicHCSR04(int trigger, int echo, String sensorName) : triggerPin(trigger), echoPin(echo), name(sensorName) {
        pinMode(triggerPin, OUTPUT); // Establecer el pin de trigger como salida
        pinMode(echoPin, INPUT);     // Establecer el pin de echo como entrada
    }

    // Método para obtener la distancia en centímetros
    long getDistance(bool verbose = false) {
        // Enviar un pulso de 10 microsegundos al pin de trigger
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);

        long duration = pulseIn(echoPin, HIGH); // Leer el tiempo que dura el pulso

        long distance = duration * 0.034 / 2; // Calcular la distancia en centímetros

        if (verbose) {
            Serial.print(name);
            Serial.print(" distance: ");
            Serial.print(distance);
            Serial.println(" cm");
        }

        return distance;
    }
};

class SensorMPU6050 {
private:
    MPU6050 mpu; // Objeto de la clase MPU6050
    String name; // Nombre del sensor para debugging

public:
    // Constructor que inicializa el nombre del sensor
    SensorMPU6050(String sensorName) : name(sensorName) {}

    // Método para inicializar el sensor
    void begin() {
        Wire.begin();  // Inicia la comunicación I2C
        mpu.initialize(); // Inicializa el MPU6050
        if (!mpu.testConnection()) {
            Serial.print(name);
            Serial.println(" connection failed!");
        } else {
            Serial.print(name);
            Serial.println(" initialized successfully.");
        }
    }

    // Método para leer datos de aceleración
    void readAcceleration(float &ax, float &ay, float &az, bool verbose = false) {
        mpu.getAcceleration(&ax, &ay, &az); // Leer datos de aceleración
        if (verbose) {
            Serial.print(name);
            Serial.print(" Acceleration: ");
            Serial.print("X: ");
            Serial.print(ax);
            Serial.print(", Y: ");
            Serial.print(ay);
            Serial.print(", Z: ");
            Serial.println(az);
        }
    }

    // Método para leer datos de giroscopio
    void readGyroscope(float &gx, float &gy, float &gz, bool verbose = false) {
        mpu.getRotation(&gx, &gy, &gz); // Leer datos de giroscopio
        if (verbose) {
            Serial.print(name);
            Serial.print(" Gyroscope: ");
            Serial.print("X: ");
            Serial.print(gx);
            Serial.print(", Y: ");
            Serial.print(gy);
            Serial.print(", Z: ");
            Serial.println(gz);
        }
    }
};

class SensorQTR8 {
private:
    QTRSensorsRC qtr;
    unsigned char sensorPins[8]; // Pines de los sensores
    String name;             // Nombre del sensor para debug
    int numSensors;          // Número de sensores QTR utilizados

    unsigned int sensorValues[8]; // Arreglo para almacenar los valores de cada sensor
    int threshold;

public:
    // Constructor que inicializa los pines y el nombre del sensor
    SensorQTR8(unsigned char pins[], int num, String sensorName) : numSensors(num), name(sensorName) {
        for (int i = 0; i < numSensors; i++) { // Copiar los pines a sensorPins
            sensorPins[i] = pins[i];
        }
        qtr.init(sensorPins, numSensors); // Inicializar el sensor con los pines dados
    }

    // Método para calibrar el sensor
    void calibrate() {
        Serial.print(name);
        Serial.println(" calibrating... Move the robot so that all sensor readers pass through black and white surfaces");

        for (int i = 0; i < 400; i++) { // Calibración con 400 muestras
            qtr.calibrate();
            delay(20);
        }

        Serial.print(name);
        Serial.println(" calibration completed.");
    }

    // Método para leer los valores de los sensores
    void readSensors(bool verbose = false) {
        qtr.readCalibrated(sensorValues); // Leer los valores de los sensores

        if (verbose) {
            Serial.print(name);
            Serial.print(" sensor values: ");
            for (int i = 0; i < numSensors; i++) {
                Serial.print(sensorValues[i]);
                Serial.print(" ");
            }
            Serial.println();
        }
    }

    // Método para obtener la posición de la línea (de 0 a 7000)
    int readLinePosition(bool verbose = false) {
        int position = qtr.readLine(sensorValues); // Leer la posición de la línea, indica si esta más a la izquierda o más a la derecha

        if (verbose) {
            Serial.print(name);
            Serial.print(" line position: ");
            Serial.println(position);
        }
        return position;
    }

    //Método que detecta si cualquier lector de qtr ha pasado por una superficie oscura
    bool readAny() {
        qtr.readCalibrated(sensorValues);
        for (int i = 0; i < numSensors; i++) {
            if (sensorValues[i] < threshold)
                return true; // Si cualquier sensor detecta oscuridad
        }
        return false; // Si ninguno detecta oscuridad
    }
};

class ColorTCS3200 {
private:
    int s0, s1, s2, s3, out; // Pines del sensor
    String name;             // Nombre del sensor para debug
    int red, green, blue;
    int threshold;

    // Método para leer los valores de color
    void readColor(bool verbose = false) {
        // Leer valor de rojo
        digitalWrite(s2, LOW);
        digitalWrite(s3, LOW);
        delay(100); // Tiempo de estabilización
        red = pulseIn(out, digitalRead(out) == HIGH ? HIGH : LOW);

        // Leer valor de verde
        digitalWrite(s2, HIGH);
        digitalWrite(s3, HIGH);
        delay(100);
        green = pulseIn(out, digitalRead(out) == HIGH ? HIGH : LOW);

        // Leer valor de azul
        digitalWrite(s2, LOW);
        digitalWrite(s3, HIGH);
        delay(100);
        blue = pulseIn(out, digitalRead(out) == HIGH ? HIGH : LOW);

        if (verbose) {
            Serial.print(name);
            Serial.print(" - Red: ");
            Serial.print(red);
            Serial.print(", Green: ");
            Serial.print(green);
            Serial.print(", Blue: ");
            Serial.println(blue);
        }
    }

public:
    // Constructor: inicializa los pines y el nombre del sensor
    ColorTCS3200(int s0Pin, int s1Pin, int s2Pin, int s3Pin, int outPin, String sensorName, int colorThreshold=150) : s0(s0Pin), s1(s1Pin), s2(s2Pin), s3(s3Pin), out(outPin), name(sensorName), threshold(colorThreshold) {
        pinMode(s0, OUTPUT);
        pinMode(s1, OUTPUT);
        pinMode(s2, OUTPUT);
        pinMode(s3, OUTPUT);
        pinMode(out, INPUT);

        digitalWrite(s0, HIGH);
        digitalWrite(s1, LOW);
    }
    
    // Método que regresa el color detectado en un array en formato RGB
    void getRGB(int (&rgb)[3], bool verbose = false) {
        readColor(verbose);
        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;
    }

    // Método para obtener el color a partir de la lista de colores de la pista: Rojo, Verde, Azul, Amarillo, Morado, Magenta, Negro y Blanco
    void getColor(bool verbose = false) { //! Para que este método funcione de buena manera, lo ideal es crear un método para calibrar el sensor en lugar de poner un valor de threshold para todos
        readColor(verbose);

        // Determina el color basado en los valores RGB
        if (red > threshold && green < threshold && blue < threshold) {
            if (verbose) Serial.println("Red");
            return 1; // Código para rojo
        } else if (green > threshold && red < threshold && blue < threshold) {
            if (verbose) Serial.println("Green");
            return 2; // Código para verde
        } else if (blue > threshold && red < threshold && green < threshold) {
            if (verbose) Serial.println("Blue");
            return 3; // Código para azul
        } else if (red > threshold && green > threshold && blue < threshold) {
            if (verbose) Serial.println("Yellow");
            return 4; // Código para amarillo
        } else if (red > threshold && green < threshold && blue > threshold) {
            if (verbose) Serial.println("Magenta");
            return 5; // Código para magenta
        } else if (red < threshold && green > threshold && blue > threshold) {
            if (verbose) Serial.println("Cyan");
            return 6; // Código para cian
        } else if (red < threshold && green < threshold && blue < threshold) {
            if (verbose) Serial.println("Black");
            return 7; // Código para negro
        } else {
            if (verbose) Serial.println("White");
            return 0; // Código para blanco
        }
    }

    // Método para verificar si el color detectado es verde
    bool isGreen(bool verbose = false) {
        readColor(verbose);
        if (green > red && green > blue) {
            if (verbose) {
                Serial.print(name);
                Serial.println(" detected GREEN color");
            }
            return true;
        }
        return false;
    }

    // Método para verificar si el color detectado es rojo
    bool isRed(bool verbose = false) {
        readColor(verbose);
        if (red > green && red > blue) {
            if (verbose) {
                Serial.print(name);
                Serial.println(" detected RED color");
            }
            return true;
        }
        return false;
    }
};

class ServoMotor {}; // esta la opcion de conectarlo al motor shield

#endif // SENSORS_H