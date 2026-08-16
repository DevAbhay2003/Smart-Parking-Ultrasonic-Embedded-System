# Project Report: Smart Parking System using Ultrasonic Sensors

## Abstract
This project presents an embedded system for automated parking slot occupancy detection using HC-SR04 ultrasonic sensors and a microcontroller. Each slot's distance reading is compared against a calibrated threshold to classify it as free or occupied; the system aggregates this into a live available-slot count and communicates it through an LCD, per-slot LEDs, a buzzer, and an optional servo-controlled gate. Both a real-hardware build and a fully equivalent virtual simulation are provided, making the project completable and demonstrable without physical components.

## Introduction
Manual observation of parking availability does not scale well and creates avoidable congestion at lot entrances. Embedded sensing offers a low-cost way to automate this at the level of individual slots.

## Problem Statement
There is no automated, real-time way to know which individual parking slots are free without either walking the lot or relying on a human attendant, leading to wasted time and uneven space utilization.

## Objectives
Detect per-slot occupancy; compute and display available slot count; provide LED/LCD/buzzer feedback; trigger a full-lot alert; support both hardware and simulated demonstration.

## Existing System
Traditional lots rely on visible signage updated manually, barrier-gate ticketing without slot-level detail, or no system at all — none of which give a driver per-slot guidance.

## Proposed System
A distributed set of ultrasonic sensors, one per slot, feeding a central microcontroller that runs a sense-decide-act loop and drives a shared LCD/LED/buzzer/gate output layer.

## Hardware Requirements
Arduino UNO or ESP32; 4x HC-SR04 ultrasonic sensors; 4x red LED, 4x green LED; buzzer; 16x2 I2C LCD; optional servo motor; breadboard, jumper wires, 5V supply.

## Software Requirements
Arduino IDE; `LiquidCrystal_I2C` library; (for simulation) a Wokwi or Tinkercad account.

## System Architecture
See the workflow diagram in `docs/project_explanation.md` — sensor input flows through distance measurement, threshold-based decision logic, slot counting, and finally to the output layer (LCD/LED/buzzer/gate).

## Circuit Design
Full wiring and pin mapping are documented in `circuit_diagram/circuit_diagram.md`.

## Working Principle
Each ultrasonic sensor is triggered in sequence; the microcontroller times the returned echo, converts that time to a distance, and classifies the slot against a threshold. All four results are summed into an available count each loop cycle, and that count drives every output peripheral.

## Ultrasonic Sensor Working
The HC-SR04 emits a 40kHz ultrasonic burst when triggered and holds its ECHO line high for exactly the round-trip travel time of that pulse, which the microcontroller measures with `pulseIn()`.

## Distance Calculation
`Distance = (Echo Time x Speed of Sound) / 2`, dividing by two because the measured time covers travel to the object and back.

## Algorithm
1. For each slot, trigger the sensor and read the echo duration
2. Convert duration to distance in cm
3. If distance < threshold, mark OCCUPIED, else FREE
4. Update that slot's LED
5. Sum FREE slots into `availableSlots`
6. If `availableSlots == 0`, trigger buzzer, show "PARKING FULL", close gate
7. Otherwise update LCD with the live count and open the gate if it was closed
8. Print status to Serial Monitor
9. Repeat

## Flowchart
```
Start -> Trigger Sensor -> Measure Echo -> Calculate Distance
  -> Distance < Threshold? --Yes--> Slot OCCUPIED --> Red LED
                            --No---> Slot FREE     --> Green LED
  -> Repeat for all slots -> Count Free Slots
  -> Count == 0? --Yes--> Buzzer ON, "PARKING FULL", Gate Closed
                 --No---> Buzzer OFF, Show Count, Gate Open
  -> Loop
```

## Source Code Explanation
See inline comments in `arduino_code/smart_parking_complete.ino`; the code is split into clearly labeled modules (distance measurement, slot detection, LED, buzzer/alert, gate, LCD) matching this report's algorithm section one-to-one.

## Simulation
Documented step-by-step in `simulation/wokwi_simulation_guide.md`, including exact verification steps for each occupancy scenario.

## Testing
13 test cases covering normal operation, edge cases (invalid echo, noisy readings), and the full/empty boundary conditions — see `test_cases/test_cases.md`.

## Results
The system reliably distinguishes occupied from free slots at the calibrated threshold and correctly gates the "PARKING FULL" condition to only the all-occupied case, both on hardware and in simulation.

## Applications
Malls, airports, hospitals, offices, smart cities, residential societies, railway stations, universities, and commercial parking facilities (see README → Industry Relevance).

## Advantages
Low cost, easy to interface, no camera/vision processing needed, straightforward to scale by adding more sensor/LED pairs, fully demonstrable without hardware.

## Limitations
Fixed-height threshold assumes consistent mounting; ultrasonic reflections can be affected by irregular vehicle surfaces; polling loop adds a small latency bound; no persistent/networked logging in the base version.

## Future Scope
Wi-Fi/cloud dashboard (ESP32), mobile app integration, RFID or camera-based entry/exit logging, interrupt-driven sensing for lower latency, occupancy analytics over time.

## Conclusion
This project demonstrates a complete, low-cost embedded pipeline — from raw sensor timing to a human-facing display and alert system — that mirrors real commercial parking-guidance systems while remaining approachable for a student to build, test, and document end-to-end.
