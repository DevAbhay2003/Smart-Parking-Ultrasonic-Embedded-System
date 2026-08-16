# Circuit Diagram & Wiring Instructions

## Text-Based Wiring Diagram

```
                         +-----------------------+
   HC-SR04 (Slot 1)      |                       |
     VCC ---------------> 5V                     |
     GND ---------------> GND                     |
     TRIG --------------> D2                       Arduino UNO
     ECHO --------------> D3                       (or ESP32,
                          |                        pins remapped)
   HC-SR04 (Slot 2)       |
     TRIG --------------> D4                       |
     ECHO --------------> D5                       |
                          |                       |
   HC-SR04 (Slot 3)       |                       |
     TRIG --------------> D6                       |
     ECHO --------------> D7                       |
                          |                       |
   HC-SR04 (Slot 4)       |                       |
     TRIG --------------> D8                       |
     ECHO --------------> D9                       |
                          |                       |
   Green LEDs (x4) ------> A0, A1, A2, A3          |
   Red LEDs (x4) --------> D10, D11, D12, D13      |
   Buzzer (+) -----------> A4                      |
   Servo signal ---------> A5 (optional gate)      |
   LCD SDA --------------> A4 (I2C, ESP32: GPIO21) |
   LCD SCL --------------> A5 (I2C, ESP32: GPIO22) |
                         +-----------------------+
```

> Note: On UNO, A4/A5 are shared between I2C (LCD) and the buzzer/servo pin
> assignments shown in the code comments — if using the LCD, move the
> buzzer/servo to spare digital pins in your actual wiring and update the
> `#define` constants in the code to match.

## Full Pin Mapping Table

| Component | Pin on Component | Connects To |
|---|---|---|
| Ultrasonic Sensor 1 | VCC | 5V |
| | GND | GND |
| | TRIG | D2 |
| | ECHO | D3 |
| Ultrasonic Sensor 2 | TRIG / ECHO | D4 / D5 |
| Ultrasonic Sensor 3 | TRIG / ECHO | D6 / D7 |
| Ultrasonic Sensor 4 | TRIG / ECHO | D8 / D9 |
| Green LED 1–4 | Anode (+) via 220Ω resistor | A0, A1, A2, A3 |
| Red LED 1–4 | Anode (+) via 220Ω resistor | D10, D11, D12, D13 |
| All LEDs | Cathode (−) | GND |
| Buzzer | + | A4 (or spare digital pin) |
| Buzzer | − | GND |
| LCD (I2C backpack) | VCC / GND | 5V / GND |
| | SDA / SCL | A4 / A5 (UNO) |
| Servo (optional gate) | Signal | Spare digital pin (e.g. D13 if LEDs remapped) |
| | VCC / GND | 5V / GND |

## Safe Wiring Notes

- Always connect grounds of all components together (common ground)
- Use a 220Ω resistor in series with every LED
- Do not power a servo motor directly from the Arduino 5V pin if using more than one — use an external 5V supply with shared ground
- Double-check TRIG vs ECHO — swapping them is the most common wiring mistake
- If using ESP32, HC-SR04 ECHO pins output 5V logic; use a voltage divider (or a 3.3V-tolerant ultrasonic sensor) to protect the ESP32 GPIO

## Sensor Placement

Mount each ultrasonic sensor directly above the center of its parking slot (ceiling-mounted, facing down) or at bumper height facing into the slot. Measure the sensor-to-ground (or sensor-to-empty-slot-floor) distance once installed — this becomes your reference for calibrating `SLOT_THRESHOLD_CM` in the code (see README → Distance Calculation).
