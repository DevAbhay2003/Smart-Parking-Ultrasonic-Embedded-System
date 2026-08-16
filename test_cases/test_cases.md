# Test Case Table

| # | Test Case | Input | Expected Output | Actual Output | Pass/Fail |
|---|---|---|---|---|---|
| 1 | All slots free | All sensors > threshold | All LEDs green, LCD `4/4`, buzzer off | | |
| 2 | One slot occupied | Slot 1 < threshold, rest free | Slot 1 red, LCD `3/4` | | |
| 3 | Multiple slots occupied | Slots 1 & 2 < threshold | Slots 1,2 red, LCD `2/4` | | |
| 4 | All slots occupied | All sensors < threshold | All red, LCD "PARKING FULL", buzzer ON | | |
| 5 | One vehicle leaves | From all-occupied, free Slot 1 | Slot 1 green, LCD `1/4`, buzzer OFF | | |
| 6 | Noisy sensor reading | Rapidly fluctuating distance near threshold | Stable enough decision each cycle (no LED flicker faster than loop rate) | | |
| 7 | Invalid echo reading | No object in range (echo timeout) | Slot classified FREE, no crash/hang | | |
| 8 | Parking full alert | Available slots = 0 | Buzzer ON, LCD shows "PARKING FULL" | | |
| 9 | LCD update | Any slot state change | LCD refreshes with new count within one loop cycle | | |
| 10 | LED indication | Any slot state change | Correct green/red pair for that slot only | | |
| 11 | Buzzer operation | Toggle between full and not-full | Buzzer follows state exactly, no stuck-on | | |
| 12 | Optional gate opening | Available slots > 0 after being full | Gate servo moves to open position | | |
| 13 | System reset | Power cycle the board | All slots re-evaluated fresh, no stale state | | |

**Pass/Fail criteria:** a case passes only if the Actual Output matches Expected Output for at least 3 consecutive polling cycles (to rule out a one-off sensor glitch).
