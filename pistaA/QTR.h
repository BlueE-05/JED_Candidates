// ._. || Sensor QTR8
#include <QTRSensors.h>

class SensorQTR8 {
private:
    QTRSensors qtr;
    int sensorPins[8]; // Pines de los sensores
    String name;             // Nombre del sensor para debug
    int numSensors;          // Número de sensores QTR utilizados

    unsigned int sensorValues[8]; // Arreglo para almacenar los valores de cada sensor
    int threshold;

public:
    // Constructor que inicializa los pines y el nombre del sensor
    SensorQTR8(int pins[], int num, String sensorName) : numSensors(num), name(sensorName) {
        for (int i = 0; i < numSensors; i++) { // Copiar los pines a sensorPins
            sensorPins[i] = pins[i];
        }
        //qtr.init(sensorPins, numSensors); // Inicializar el sensor con los pines dados
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
        qtr.read(sensorValues); // Llamada al método que lee los sensores y almacena los valores en el arreglo sensorValues

        // Procesar los valores de los sensores para calcular la posición de la línea
        int weightedSum = 0;
        int totalValue = 0;

        for (int i = 0; i < numSensors; i++) {
            weightedSum += sensorValues[i] * i; // Multiplicamos el valor por el índice como peso
            totalValue += sensorValues[i];
        }

        int position;
        if (totalValue > 0) {
            // Calcula la posición ponderada de la línea
            position = weightedSum / totalValue;
        } else {
            // Si no hay señal, devuelve un valor fuera del rango
            position = -1;
        }

        if (verbose) {
            Serial.print("Posición de la línea: ");
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