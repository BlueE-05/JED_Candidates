// ._. || MPU
#include <Wire.h>

class SensorMPU6050 {
private:
    const int MPU = 0x68; // Dirección I2C de la IMU
    const float A_R = 16384.0; // Ratio de aceleración
    const float G_R = 131.0; // Ratio de giroscopio
    #define RAD_TO_DEG 57.295779

    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ; // Valores RAW del sensor
    float Acc[2]; // Ángulos calculados a partir del acelerómetro
    float Gy[3]; // Velocidades angulares a partir del giroscopio
    float Angle[3]; // Ángulos resultantes
    long tiempo_prev; // Tiempo anterior para el cálculo de dt
    float dt;
    String name; // Nombre del sensor para debugging

public:
    // Constructor que inicializa el nombre del sensor
    SensorMPU6050(String sensorName) : name(sensorName), tiempo_prev(0) {
        for (int i = 0; i < 3; ++i) Angle[i] = 0;
    }

    // Método para inicializar el sensor
    void begin() {
        Wire.begin();
        Wire.beginTransmission(MPU);
        Wire.write(0x6B); // Registro para activar el MPU-6050
        Wire.write(0); // Activación
        Wire.endTransmission(true);
        tiempo_prev = millis();
    }

    // Método para leer datos del sensor y calcular ángulos
    void read(bool verbose = false) {
        // Leer los valores del Acelerómetro
        Wire.beginTransmission(MPU);
        Wire.write(0x3B); // Registro para AcX
        Wire.endTransmission(false);
        Wire.requestFrom(MPU, 6, true);
        AcX = Wire.read() << 8 | Wire.read();
        AcY = Wire.read() << 8 | Wire.read();
        AcZ = Wire.read() << 8 | Wire.read();

        // Calcular los ángulos a partir del acelerómetro
        Acc[1] = atan(-1 * (AcX / A_R) / sqrt(pow((AcY / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_TO_DEG;
        Acc[0] = atan((AcY / A_R) / sqrt(pow((AcX / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_TO_DEG;

        // Leer los valores del Giroscopio
        Wire.beginTransmission(MPU);
        Wire.write(0x43); // Registro para GyX
        Wire.endTransmission(false);
        Wire.requestFrom(MPU, 6, true);
        GyX = Wire.read() << 8 | Wire.read();
        GyY = Wire.read() << 8 | Wire.read();
        GyZ = Wire.read() << 8 | Wire.read();

        // Calcular velocidades angulares a partir del giroscopio
        Gy[0] = GyX / G_R;
        Gy[1] = GyY / G_R;
        Gy[2] = GyZ / G_R;

        // Calcular delta de tiempo
        dt = (millis() - tiempo_prev) / 1000.0;
        tiempo_prev = millis();

        // Filtro Complementario para los ángulos
        Angle[0] = 0.98 * (Angle[0] + Gy[0] * dt) + 0.02 * Acc[0];
        Angle[1] = 0.98 * (Angle[1] + Gy[1] * dt) + 0.02 * Acc[1];
        Angle[2] = Angle[2] + Gy[2] * dt; // Integración para YAW

        // Mostrar los valores por consola si verbose está activado
        if (verbose) {
            Serial.print(name + " -> X: ");
            Serial.print(Angle[0]);
            Serial.print(" Y: ");
            Serial.print(Angle[1]);
            Serial.print(" Z: ");
            Serial.println(Angle[2]);
        }
    }

    // Métodos para obtener los ángulos calculados
    float getAngleX(bool verbose=false) const {
        read(verbose);
        return Angle[0];
    }
    float getAngleY(bool verbose=false) const {
        read(verbose);
        return Angle[1];
    }
    float getAngleZ(bool verbose=false) const {
        read(verbose);
        return Angle[2];
    }
};