# Tech Stack Options & Implementation Plan

## Hardware Options

### Option A — Easy
**Components:** Arduino UNO, 2 ultrasonic sensors, LEDs, Serial Monitor only.
**Difficulty:** Low. **Expected output:** Console-based FREE/OCCUPIED per slot with LED confirmation. **Hardware mandatory:** No — fully simulatable in Wokwi.

### Option B — Recommended
**Components:** Arduino UNO/ESP32, 4 ultrasonic sensors, LCD/OLED, red/green LEDs, buzzer, optional entry/exit sensor.
**Difficulty:** Medium. **Expected output:** Full display-driven demo matching this repo's `smart_parking_complete.ino`. **Hardware mandatory:** No — this is the version documented and simulated in this repo.

### Option C — Advanced
**Components:** ESP32, multiple sensors, servo barrier gate, Wi-Fi, cloud dashboard, mobile/web status page, optional RFID.
**Difficulty:** High. **Expected output:** Networked system with a remote-viewable dashboard. **Hardware mandatory:** Partially — Wi-Fi/cloud pieces need either real ESP32 hardware or a cloud simulator; core sensor logic is still simulatable.

**Best choice for students:** **Option B.** It covers every core embedded concept (sensing, thresholding, multi-peripheral output, display integration) without requiring networking infrastructure, and is fully achievable in simulation if hardware isn't available.

## Implementation Plan (14 Phases)

| Phase | Objective | Key Task | Expected Output | Common Mistake | Verify By |
|---|---|---|---|---|---|
| 1 | Environment setup | Install Arduino IDE + LiquidCrystal_I2C library | IDE compiles a blank sketch | Wrong board selected | Successful compile |
| 2 | Single sensor interfacing | Wire one HC-SR04, run `01_single_sensor_test.ino` | Serial prints raw distance | TRIG/ECHO swapped | Stable readings on Serial Monitor |
| 3 | Distance measurement | Validate distance formula against a ruler | Distance matches physical measurement ±1cm | Forgetting the `/2` | Compare to measured distance |
| 4 | Single slot detection | Add threshold logic + 1 LED pair, run `02_single_slot_detection.ino` | LED flips at threshold | Threshold too tight/loose | Move hand in/out of range |
| 5 | Multiple slot integration | Wire remaining 3 sensors | All 4 report independently | Pin/array index mismatch | Each slot responds only to its own sensor |
| 6 | Available slot counting | Sum FREE slots each loop | Correct count for any occupancy combo | Off-by-one in loop bounds | Manually occupy N slots, check count |
| 7 | LED status indication | Wire all red/green LED pairs | Correct LED per slot state | LEDs wired to wrong slot index | Visual per-slot check |
| 8 | LCD/OLED integration | Wire I2C LCD, call `lcd.print()` | Live available count on screen | Wrong I2C address | Scan I2C bus if blank |
| 9 | Parking full alert | Trigger buzzer when count = 0 | Buzzer ON only when full | Buzzer stuck on/off | Fill/free the last slot |
| 10 | Optional gate logic | Add servo, gate closes when full | Gate blocks entry only when full | Servo jitter from shared power rail | Watch gate on full/not-full |
| 11 | Full system integration | Merge all modules into one sketch | `smart_parking_complete.ino` runs end-to-end | Pin conflicts between modules | Run full test case table |
| 12 | Testing & calibration | Run all 13 test cases | All pass | Threshold not recalibrated for real mounting height | See `test_cases/test_cases.md` |
| 13 | Virtual simulation | Rebuild circuit in Wokwi/Tinkercad | Simulation matches hardware behavior | Missing library in simulator | Follow `simulation/wokwi_simulation_guide.md` |
| 14 | GitHub upload | Push repo with docs + screenshots | Public, well-documented repository | Committing everything in one giant commit | See `docs/github_upload_strategy.md` |
