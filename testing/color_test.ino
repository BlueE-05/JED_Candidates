// ._. Testing TCS3200
#include "Sensors.h"

const int s0 = 22;
const int s1 = 23;
const int s2 = 24;
const int s3 = 25;
const int outPin = 26;

ColorTCS3200 colorSensor(s0, s1, s2, s3, outPin, "Color Sensor");

void setup() {
    Serial.begin(9600);
    colorSensor.begin(true); // Inicializa el sensor y habilita el verbose
}

void loop() {
    int colorDetected = colorSensor.getColor(true);

    bool = isGreen(true);
    bool = isRed(true);

    delay(1000);
}