/*
  ============================================================
  SMART PARKING SYSTEM USING ULTRASONIC SENSORS
  Complete Integrated Code (4 Slots)
  Board: Arduino UNO (also works on ESP32 with pin remap)
  ============================================================
  Modules:
    A. Ultrasonic distance measurement
    B. Parking slot free/occupied detection
    C. Available slot counter
    D. Red/Green LED indication
    E. Buzzer + "PARKING FULL" alert
    F. 16x2 I2C LCD display
    G. Optional servo gate
  ------------------------------------------------------------
  Library required: LiquidCrystal_I2C (by Frank de Brabander)
  Install via Arduino IDE -> Tools -> Manage Libraries
  ============================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ---------------- CONFIG ----------------
#define NUM_SLOTS 4
const int TRIG_PIN[NUM_SLOTS]  = {2, 4, 6, 8};
const int ECHO_PIN[NUM_SLOTS]  = {3, 5, 7, 9};
const int GREEN_LED[NUM_SLOTS] = {A0, A1, A2, A3};
const int RED_LED[NUM_SLOTS]   = {10, 11, 12, 13};

const int BUZZER_PIN = A4;
const bool USE_SERVO_GATE = true;
const int SERVO_PIN = A5;

// Distance threshold (cm). Below this = OCCUPIED.
// Calibrate based on your sensor mounting height (see README).
const int SLOT_THRESHOLD_CM = 15;

// Invalid reading guard (HC-SR04 max reliable range ~400cm)
const int MAX_VALID_DISTANCE = 400;

LiquidCrystal_I2C lcd(0x27, 16, 2); // change 0x27 if your I2C scanner shows a different address
Servo gateServo;

bool slotOccupied[NUM_SLOTS];
int availableSlots = 0;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_SLOTS; i++) {
    pinMode(TRIG_PIN[i], OUTPUT);
    pinMode(ECHO_PIN[i], INPUT);
    pinMode(GREEN_LED[i], OUTPUT);
    pinMode(RED_LED[i], OUTPUT);
  }
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  if (USE_SERVO_GATE) {
    gateServo.attach(SERVO_PIN);
    gateServo.write(0); // closed
  }

  delay(1500);
  lcd.clear();
}

// ---------------- MODULE A: DISTANCE MEASUREMENT ----------------
long readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 25ms timeout (~430cm max) avoids blocking forever when there's no echo
  long duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) {
    return -1; // invalid / nothing detected in range
  }

  // Distance = (Echo Time x Speed of Sound) / 2
  // Speed of sound ~ 0.0343 cm/microsecond
  // Divide by 2 because the pulse travels TO the object AND back
  long distance = duration * 0.0343 / 2;

  if (distance > MAX_VALID_DISTANCE || distance <= 0) {
    return -1;
  }
  return distance;
}

// ---------------- MODULE B: SLOT DETECTION ----------------
bool isSlotOccupied(long distanceCM) {
  if (distanceCM == -1) {
    // No echo usually means nothing is close -> treat as FREE
    return false;
  }
  return (distanceCM < SLOT_THRESHOLD_CM);
}

// ---------------- MODULE D: LED INDICATION ----------------
void updateLED(int slotIndex, bool occupied) {
  digitalWrite(GREEN_LED[slotIndex], occupied ? LOW : HIGH);
  digitalWrite(RED_LED[slotIndex], occupied ? HIGH : LOW);
}

// ---------------- MODULE E: BUZZER / ALERT ----------------
void updateAlert(bool parkingFull) {
  digitalWrite(BUZZER_PIN, parkingFull ? HIGH : LOW);
}

// ---------------- MODULE G: GATE CONTROL ----------------
void updateGate(bool parkingFull) {
  if (!USE_SERVO_GATE) return;
  gateServo.write(parkingFull ? 0 : 90); // 0 = closed, 90 = open
}

// ---------------- MODULE F: LCD DISPLAY ----------------
void updateLCD(bool parkingFull) {
  lcd.clear();
  if (parkingFull) {
    lcd.setCursor(0, 0);
    lcd.print("PARKING FULL");
    lcd.setCursor(0, 1);
    lcd.print("Please Wait...");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Slots Available:");
    lcd.setCursor(0, 1);
    lcd.print(availableSlots);
    lcd.print(" / ");
    lcd.print(NUM_SLOTS);
  }
}

// ---------------- SERIAL DEBUG OUTPUT ----------------
void printSerialStatus() {
  Serial.println("---------------------------------");
  for (int i = 0; i < NUM_SLOTS; i++) {
    Serial.print("Slot ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(slotOccupied[i] ? "OCCUPIED" : "FREE");
  }
  Serial.print("Available Slots: ");
  Serial.println(availableSlots);
  if (availableSlots == 0) {
    Serial.println("STATUS: PARKING FULL - Buzzer ON, Gate CLOSED");
  }
}

// ---------------- MAIN LOOP ----------------
void loop() {
  availableSlots = 0;

  for (int i = 0; i < NUM_SLOTS; i++) {
    long distance = readDistanceCM(TRIG_PIN[i], ECHO_PIN[i]);
    slotOccupied[i] = isSlotOccupied(distance);
    updateLED(i, slotOccupied[i]);
    if (!slotOccupied[i]) availableSlots++;
  }

  bool parkingFull = (availableSlots == 0);
  updateAlert(parkingFull);
  updateGate(parkingFull);
  updateLCD(parkingFull);
  printSerialStatus();

  delay(800); // fast enough to feel real-time, slow enough to avoid sensor cross-talk
}
