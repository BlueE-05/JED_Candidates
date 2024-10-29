// ._. Testing Ultrasónicos
#include "Sensors.h"

UltrasonicHCSR04 ultrasonic_Left(22, 23, "Ultrasonic Sensor Front");
UltrasonicHCSR04 ultrasonic_Front(24, 25, "Ultrasonic Sensor Front");
UltrasonicHCSR04 ultrasonic_Right(26, 27, "Ultrasonic Sensor Front");

void setup() {
    Serial.begin(9600);
}

void loop() {
    long distance = ultrasonic.getDistance(true);
    delay(1000);
}