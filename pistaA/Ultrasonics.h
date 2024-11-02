// ._. || Ultrasonicos

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