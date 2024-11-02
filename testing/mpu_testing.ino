// ._. Testing MPU6050
#include "Sensors.h"

SensorMPU6050 mpuSensor("MPU6050 Sensor");

void setup() {
    Serial.begin(9600);
    mpuSensor.begin();   // Inicializa el sensor
}

void loop() {
    float ax, ay, az;
    float gx, gy, gz;

    mpuSensor.readAcceleration(ax, ay, az, true);
    
    mpuSensor.readGyroscope(gx, gy, gz, true);
    
    delay(1000);
}