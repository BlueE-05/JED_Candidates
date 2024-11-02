// ._. Testing QTR
#include "Sensors.h"

/* Ejemplo usando todos los lectores del QTR

unsigned char qtrPins[] = {22, 23, 24, 25, 26, 27, 28, 29};
int threshold = 500; // Umbral para detectar superficies oscuras

SensorQTR8 qtrSensor(qtrPins, 8, "QTR Sensor Array");

void setup() {
    Serial.begin(9600);
    qtrSensor.calibrate();
}

void loop() {
    qtrSensor.readSensors(true);

    int position = qtrSensor.readLinePosition(true);

    if (qtrSensor.readAny(true)) {
        Serial.println("Black surface detected.");
    } else {
        Serial.println("No black surface detected.");
    }

    delay(500);
}
*/

unsigned char qtrPins[] = {2, 3, 4, 5}; // Solo algunos pines conectados
int threshold = 500; // Umbral para detectar superficies oscuras

SensorQTR8 qtrSensorPartial(qtrPins, 4, "Partial QTR Sensor Array");

void setup() {
    Serial.begin(9600);
    qtrSensorPartial.calibrate();
}

void loop() {
    qtrSensorPartial.readSensors(true);

    int position = qtrSensorPartial.readLinePosition(true);

    if (qtrSensorPartial.readAny(true)) {
        Serial.println("Black surface detected.");
    } else {
        Serial.println("No black surface detected.");
    }

    delay(500);
}