# Smart Parking System using Ultrasonic Sensors

An embedded system that detects whether parking slots are free or occupied using HC-SR04 ultrasonic sensors, counts available slots in real time, and displays the status on an LCD with LED and buzzer indication — plus an optional automatic gate. Built for both real hardware and virtual simulation (Wokwi/Tinkercad), so it can be completed and demonstrated with zero hardware cost.

## Overview

Drivers often waste time circling a lot looking for a free spot because there is no visibility into which slots are occupied. This project places one ultrasonic sensor above/beside each parking slot; a microcontroller reads the distance to whatever is below the sensor, decides FREE or OCCUPIED against a calibrated threshold, and reports the result through LEDs, an LCD, a buzzer, and (optionally) a servo-controlled gate.

## Problem Statement

Manual or unmonitored parking leads to wasted search time, uneven space utilization, and congestion at entrances when a lot is actually full but drivers don't know it. This project automates slot-level occupancy detection so that availability is known instantly and continuously.

## Objectives

- Detect occupancy of each parking slot using ultrasonic distance sensing
- Calculate and display the number of currently available slots
- Provide clear visual (LED, LCD) and audible (buzzer) feedback
- Trigger a "Parking Full" alert and optionally close a gate when no slots remain
- Demonstrate the design in simulation so it is reviewable without physical hardware

## Industry Relevance

The same slot-level occupancy pattern (sensor → microcontroller → decision → display) is used, at larger scale, in:

- **Shopping malls & airports** — entrance boards showing live free-spot counts per floor
- **Hospitals** — priority/visitor parking visibility to reduce delays
- **Office buildings & universities** — employee/student lot management
- **Smart cities** — municipal on-street parking sensors feeding a city dashboard
- **Residential societies** — visitor slot tracking
- **Railway stations & commercial lots** — high-turnover parking needing fast guidance

**Business value:** reduced search time, better space utilization, improved customer experience, less entrance congestion, automated (lower-manpower) monitoring, and real-time slot visibility for apps/dashboards.

## Features

- Real-time occupancy detection per slot via ultrasonic sensors
- Available slot counter
- Red/Green LED per slot
- 16x2 I2C LCD showing live availability
- Buzzer + "PARKING FULL" alert when all slots are occupied
- Optional servo gate that opens only when a slot is available
- Fully working virtual simulation (no hardware required)
- Modular, commented, beginner-friendly Embedded C/Arduino code

## Components Used

| Component | Purpose |
|---|---|
| Arduino UNO / ESP32 | Microcontroller — reads sensors, runs logic, drives outputs |
| HC-SR04 Ultrasonic Sensor (x4) | Measures distance to detect a vehicle in each slot |
| Green LED (x4) | Lit when a slot is FREE |
| Red LED (x4) | Lit when a slot is OCCUPIED |
| Buzzer | Sounds when parking is FULL |
| 16x2 I2C LCD | Displays live slot status / available count |
| Servo motor (optional) | Opens/closes an entry gate |
| Breadboard + jumper wires | Prototyping connections |
| 5V power supply / USB | Powers the board |

See `circuit_diagram/circuit_diagram.md` for full purpose/IO/behavior detail per component.

## Embedded Systems Concepts

Microcontroller & GPIO, Trigger/Echo timing, `pulseIn()` timer-based pulse measurement, distance calculation from time-of-flight, threshold-based decision logic, digital output control (LED/buzzer/servo), I2C serial communication (LCD), UART serial communication (debug output), and state tracking across a polling loop. See `docs/project_explanation.md` for why each concept is used.

## System Architecture

```
Parking Slot
   |
Ultrasonic Sensor (TRIG/ECHO)
   |
Distance Measurement  --> Distance = (Echo Time x Speed of Sound) / 2
   |
Microcontroller
   |
Occupied / Available Decision  --> distance < threshold ?
   |
Available Slot Count
   |
LCD / LED / Buzzer / Gate Output
```

## Circuit Diagram

Full wiring instructions, pin mapping table, and sensor placement notes are in [`circuit_diagram/circuit_diagram.md`](circuit_diagram/circuit_diagram.md).

## Pin Configuration (Arduino UNO)

| Slot | TRIG | ECHO | Green LED | Red LED |
|---|---|---|---|---|
| 1 | D2 | D3 | A0 | D10 |
| 2 | D4 | D5 | A1 | D11 |
| 3 | D6 | D7 | A2 | D12 |
| 4 | D8 | D9 | A3 | D13 |

Buzzer → A4 · Servo (optional) → A5 · LCD (I2C) → SDA/SCL

## Distance Calculation

```
Distance = (Echo Time x Speed of Sound) / 2
```

The `/2` accounts for the pulse traveling to the object **and back** — the echo time measures the full round trip, not the one-way distance.

## Parking Slot Logic

```
IF Distance < SLOT_THRESHOLD_CM
    Slot = OCCUPIED
ELSE
    Slot = FREE
```

Example (threshold = 15 cm): 5 cm → Occupied, 10 cm → Occupied, 30 cm → Free.
Calibrate `SLOT_THRESHOLD_CM` to roughly the empty-slot distance from your sensor's mounting point minus a small margin.

## Folder Structure

```
Smart-Parking-Ultrasonic-Embedded-System/
├── src/                  Reusable/shared logic notes
├── arduino_code/         All .ino sketches (phase sketches + full integrated code)
├── simulation/           Wokwi/Tinkercad simulation guide
├── circuit_diagram/      Wiring instructions and pin tables
├── data/                 Sample sensor/test data
├── test_cases/           Test case table with pass/fail criteria
├── outputs/              Sample Serial Monitor / LCD output logs
├── screenshots/          Proof screenshots (see checklist)
├── reports/              Full project report
├── docs/                 Explanations, implementation plan, interview prep, GitHub strategy
├── README.md
└── .gitignore
```

## Installation

1. Install the [Arduino IDE](https://www.arduino.cc/en/software)
2. Install the library: **LiquidCrystal_I2C** (Tools → Manage Libraries)
3. Open `arduino_code/smart_parking_complete.ino`
4. Select your board and COM port

## How to Run

**Real hardware:**
1. Wire the components per `circuit_diagram/circuit_diagram.md`
2. Upload `smart_parking_complete.ino`
3. Open Serial Monitor at 9600 baud
4. Place an object within ~10 cm of a sensor to simulate a parked car
5. Observe LED, LCD, buzzer, and Serial Monitor update

**Virtual simulation:** see [`simulation/wokwi_simulation_guide.md`](simulation/wokwi_simulation_guide.md).

## Test Cases

See [`test_cases/test_cases.md`](test_cases/test_cases.md) for the full input/expected-output/pass-fail table (13 cases).

## Sample Output

```
---------------------------------
Slot 1: OCCUPIED
Slot 2: FREE
Slot 3: OCCUPIED
Slot 4: FREE
Available Slots: 2
```

LCD: `Slots Available: 2 / 4`

## Screenshots

See [`docs/screenshots_checklist.md`](docs/screenshots_checklist.md) for the exact list and suggested filenames.

## Results

The system correctly classifies slot occupancy against the configured threshold, updates the available-slot count within one polling cycle (~0.8s), and reliably triggers the PARKING FULL alert (buzzer + LCD message + gate closed) only when all monitored slots are occupied.

## Limitations

- Ultrasonic sensors can misread on soft/angled/absorptive surfaces
- A single fixed threshold assumes consistent sensor mounting height
- No persistent logging or networked dashboard in the base version
- Polling-based (not interrupt-driven), so response time is bounded by the loop delay

## Future Improvements

- Add Wi-Fi (ESP32) + cloud dashboard / mobile app for remote slot viewing
- Add RFID or camera-based entry/exit logging
- Add per-slot logging with timestamps for occupancy analytics
- Replace polling with interrupt-driven echo capture for lower latency

## Learning Outcomes

Practical experience with GPIO-level sensor interfacing, timing-based distance measurement, threshold-based decision logic, multi-peripheral integration (LED/buzzer/LCD/servo) on one microcontroller, and structuring an embedded project for both hardware and simulated demonstration.

## Author

_Add your name, course, and institution here._
