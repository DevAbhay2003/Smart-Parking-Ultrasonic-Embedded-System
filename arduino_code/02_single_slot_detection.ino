/*
  PHASE 4: Single Parking Slot Detection
  Goal: Turn a raw distance reading into a FREE/OCCUPIED decision
        for one slot, and show it on one Green + one Red LED.

  Wiring:
    TRIG -> Pin 2      ECHO -> Pin 3
    GREEN LED -> Pin A0 (with 220ohm resistor)
    RED LED   -> Pin 10 (with 220ohm resistor)
*/

const int TRIG_PIN = 2;
const int ECHO_PIN = 3;
const int GREEN_LED = A0;
const int RED_LED = 10;

const int THRESHOLD_CM = 15; // below this = occupied

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) return -1;
  return duration * 0.0343 / 2;
}

void loop() {
  long distance = readDistanceCM();
  bool occupied = (distance != -1 && distance < THRESHOLD_CM);

  digitalWrite(GREEN_LED, occupied ? LOW : HIGH);
  digitalWrite(RED_LED, occupied ? HIGH : LOW);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Slot: ");
  Serial.println(occupied ? "OCCUPIED" : "FREE");

  delay(500);
}
