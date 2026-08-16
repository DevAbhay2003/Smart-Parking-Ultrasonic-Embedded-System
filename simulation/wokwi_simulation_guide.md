# Virtual Simulation Guide (Wokwi Recommended)

No hardware? This project can be fully built and demonstrated in [Wokwi](https://wokwi.com) (Arduino UNO + HC-SR04 + LCD are all natively supported) or Tinkercad Circuits. Wokwi is recommended because it lets you drag the distance slider on each ultrasonic sensor live during simulation.

## Step-by-Step

1. **Create project** — go to wokwi.com → New Project → Arduino Uno
2. **Add 4 ultrasonic sensors** — search "HC-SR04" in the parts panel, add four
3. **Add red and green LEDs** — 4 of each, plus 220Ω resistors
4. **Add LCD** — search "LCD1602" with the I2C variant
5. **Add buzzer** — search "Buzzer"
6. **Add servo (optional)** — search "Servo Motor" for the gate
7. **Wire all components** — follow `circuit_diagram/circuit_diagram.md` pin mapping exactly
8. **Paste code** — copy `arduino_code/smart_parking_complete.ino` into the sketch editor; add `LiquidCrystal_I2C` via Wokwi's library manager (or `wokwi-library.list`)
9. **Start simulation** — click the green Play button
10. **Set all sensor distances high** (drag each HC-SR04's distance slider to max) → **Verify:** all slots show FREE, all LEDs green, LCD shows `4 / 4`
11. **Reduce Sensor 1's distance** below 15cm → **Verify:** Slot 1 LED turns red, LCD count drops to `3 / 4`
12. **Reduce Sensor 2 and 3's distances** → **Verify:** available count drops to `1 / 4`
13. **Set all 4 sensors to occupied** (below threshold) → **Verify:** LCD shows "PARKING FULL", buzzer icon shows active, gate servo moves to closed position
14. **Increase one sensor's distance back above threshold** → **Verify:** buzzer stops, LCD shows `1 / 4`

## What Serial Monitor Should Show

```
---------------------------------
Slot 1: OCCUPIED
Slot 2: FREE
Slot 3: FREE
Slot 4: FREE
Available Slots: 3
```

## What the Display Should Show

- Normal: `Slots Available:` / `2 / 4`
- Full: `PARKING FULL` / `Please Wait...`

## Screenshots Students Should Capture

1. Full simulation circuit (all components wired)
2. All slots free state
3. One slot occupied state
4. Parking full state (buzzer + gate closed)
5. Serial Monitor output matching the LCD

## GitHub Proof to Upload

- A screenshot of the completed Wokwi circuit
- A screenshot of each state in step 10–14 above
- The Wokwi project link (if public) pasted into the README or `docs/` folder
- The exported `diagram.json` (Wokwi's circuit file) saved into `simulation/`
