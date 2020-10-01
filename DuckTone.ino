/*
   DuckTone by Jason
   arranged from the VL53L0X example
*/

#include <Wire.h>
#include <VL53L0X.h>
#include <toneAC.h>

const int input = A0;
int lastDistance = 0;

VL53L0X sensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}

void loop() {
  int volume = analogRead(input);
  int gain = map(volume, 0, 1023, 0, 10);

  Serial.print("gain: ");
  Serial.print(gain);
  Serial.print(" ");

  int distance = sensor.readRangeContinuousMillimeters();

  // smooth the distance change
  distance = (lastDistance + distance) / 2;
  lastDistance = distance;

  if (distance < 4000) {
    float frequency = distance2frequency(distance);
    toneAC(frequency, gain);
    delay(10);
    Serial.print("freqency: ");
    Serial.print(frequency);
  } else {
    toneAC(0);
    Serial.print("noTone");
  }

  Serial.println();
}

/*
   distance range from 80 to 500
   divide to 2 octave

   start @ E4: 329.63hz
*/
float distance2frequency(int distance) {

  float step = float(distance - 40.0) / (500 - 40) * 24;
  return 329.63 * pow(2, step / 12);
}
