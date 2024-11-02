// ._. || Sensor Color

class ColorTCS3200 {
private:
    int s0, s1, s2, s3, out; // Pines del sensor
    String name;             // Nombre del sensor para debug
    int red, green, blue;
    int threshold;
    const int redMin = 0, redMax = 255; // Valores mínimos y máximos de red para calibración
    const int greenMin = 0, greenMax = 255; // Valores mínimos y máximos de green para calibración
    const int blueMin = 0, blueMax = 255; // Valores mínimos y máximos de blue para calibración

    // Método para leer los valores de color
    void readRaw(bool verbose = false) {
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
    ColorTCS3200(int s0Pin, int s1Pin, int s2Pin, int s3Pin, int outPin, String sensorName, int colorThreshold = 150)
        : s0(s0Pin), s1(s1Pin), s2(s2Pin), s3(s3Pin), out(outPin), name(sensorName), threshold(colorThreshold) {
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
        readRaw();
        rgb[0] = map(red, greenMin, greenMax, 255, 0);
        rgb[1] = map(green, greenMin, greenMax, 255, 0);
        rgb[2] = map(blue, greenMin, greenMax, 255, 0);
        if (verbose) {
            Serial.print(name);
            Serial.print(" - Red: ");
            Serial.print(rgb[0]);
            Serial.print(", Green: ");
            Serial.print(rgb[1]);
            Serial.print(", Blue: ");
            Serial.println(rgb[2]);
        }
    }

    // Método para verificar si el color detectado es verde
    bool isGreen(bool verbose = false) {
        int rgb[3];
        getRGB(rgb, verbose);
        if (rgb[1] > rgb[0] && rgb[1] > rgb[2]) {
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
        int rgb[3];
        getRGB(rgb, verbose);
        if (rgb[0] > rgb[1] && rgb[0] > rgb[2]) {
            if (verbose) {
                Serial.print(name);
                Serial.println(" detected RED color");
            }
            return true;
        }
        return false;
    }
};