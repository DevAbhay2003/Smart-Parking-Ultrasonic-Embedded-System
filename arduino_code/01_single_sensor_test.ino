/*
  PHASE 2-3: Single Ultrasonic Sensor Test
  Goal: Confirm one HC-SR04 sensor is wired correctly and
        prints a stable distance reading before adding complexity.

  Wiring:
    TRIG -> Pin 2
    ECHO -> Pin 3
    VCC  -> 5V
    GND  -> GND
*/

const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  long distance = duration * 0.0343 / 2;

  if (duration == 0) {
    Serial.println("No echo received (object out of range)");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(500);
}
