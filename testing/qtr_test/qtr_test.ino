/*
// ._. Testing QTR
#include "Sensors.h"

// Ejemplo usando todos los lectores del QTR

int qtrPins[] = {23, 25, 27, 29, 31, 33, 35, 37};
int irPin = 39;
int threshold = 500; // Umbral para detectar superficies oscuras

SensorQTR8 qtrSensor(qtrPins, 8, "QTR Sensor Array");

void setup() {
    Serial.begin(9600);
    //qtrSensor.calibrate();
}

void loop() {
    qtrSensor.readSensors(true);

    int position = qtrSensor.readLinePosition(true);

    if (qtrSensor.readAny()) {
        Serial.println("Black surface detected.");
    } else {
        Serial.println("No black surface detected.");
    }

    delay(500);
}

/*
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
}*/



#include <QTRSensors.h>
 
// create an object for your type of sensor (RC or Analog)
// in this example we have three sensors on analog inputs 0 - 2 (digital pins 14 - 16)
QTRSensors qtr((char[]) {23, 25, 27}, 3);
// QTRSensorsA qtr((char[]) {0, 1, 2}, 3);
 
void setup()
{
  // optional: wait for some input from the user, such as  a button press
 
  // then start calibration phase and move the sensors over both
  // reflectance extremes they will encounter in your application:
  int i;
  for (i = 0; i < 250; i++)  // make the calibration take about 5 seconds
  {
    qtr.calibrate();
    delay(20);
  }
 
  // optional: signal that the calibration phase is now over and wait for further
  // input from the user, such as a button press
}

void loop()
{
  unsigned int sensors[3];
  // get calibrated sensor values returned in the sensors array, along with the line
  // position, which will range from 0 to 2000, with 1000 corresponding to the line
  // over the middle sensor.
  int position = qtr.readLine(sensors);
 
  // if all three sensors see very low reflectance, take some appropriate action for this 
  // situation.
  if (sensors[0] > 750 && sensors[1] > 750 && sensors[2] > 750)
  {
    // do something.  Maybe this means we're at the edge of a course or about to fall off 
    // a table, in which case, we might want to stop moving, back up, and turn around.
    return;
  }
 
  // compute our "error" from the line position.  We will make it so that the error is
  // zero when the middle sensor is over the line, because this is our goal.  Error
  // will range from -1000 to +1000.  If we have sensor 0 on the left and sensor 2 on
  // the right,  a reading of -1000 means that we see the line on the left and a reading
  // of +1000 means we see the line on the right.
  int error = position - 1000;
 
  int leftMotorSpeed = 100;
  int rightMotorSpeed = 100;
  if (error < -500)  // the line is on the left
    leftMotorSpeed = 0;  // turn left
  if (error > 500)  // the line is on the right
    rightMotorSpeed = 0;  // turn right
 
  // set motor speeds using the two motor speed variables above
}