# Interview Preparation

**1. Explain your project.**
I built a Smart Parking System that uses ultrasonic sensors to detect whether each parking slot is occupied or free. Each sensor sits above a slot; a microcontroller measures the distance to whatever's below it, compares that against a threshold I calibrated, and decides the slot's status. It aggregates that into an available-slot count, shows it on an LCD, lights a red or green LED per slot, and sounds a buzzer with a "PARKING FULL" message once every slot is occupied. I built and tested it in Wokwi simulation and documented the equivalent real-hardware wiring.

**2. Why did you choose a microcontroller instead of a full computer/Raspberry Pi for this?**
The task is simple, deterministic I/O — read a few digital timing pulses, run threshold comparisons, drive some outputs. A microcontroller like the Arduino UNO is cheaper, lower power, boots instantly, and is the standard choice for this kind of dedicated, always-on sensing loop, whereas a full OS adds overhead I don't need here.

**3. How does the ultrasonic sensor actually measure distance?**
I trigger the HC-SR04 with a short pulse on TRIG, it emits a 40kHz ultrasonic burst, and its ECHO pin stays HIGH for exactly the time the pulse takes to travel to the nearest object and bounce back. I capture that duration with `pulseIn()` and convert it to distance using the speed of sound, dividing by two because the timed duration covers the round trip, not just one way.

**4. Walk me through the distance formula.**
Distance = (Echo Time × Speed of Sound) / 2. Speed of sound in air is about 0.0343 cm per microsecond. If the echo pulse lasts, say, 600 microseconds, that's 600 × 0.0343 = ~20.58cm round trip, so the actual one-way distance is about 10.3cm.

**5. What GPIO pins did you use and why do TRIG and ECHO need to be separate?**
TRIG is an output pin — the microcontroller sends the trigger pulse out on it. ECHO is an input pin — the sensor drives it and the microcontroller reads the pulse width. They have to be separate because one is a command going out and the other is a measurement coming in; you can't multiplex a timed pulse-width read with an outgoing trigger on the same pin without added complexity.

**6. How did you decide the occupancy threshold, and what happens if it's wrong?**
I based it on the sensor's mounting height above an empty slot, with a margin below that as the cutoff. If it's too high, a large vehicle far from the sensor could read as occupied when it isn't (false positive); if it's too low, a low-profile vehicle might not trip the threshold (false negative). It needs to be recalibrated any time the physical mounting height changes.

**7. How did you handle multiple sensors without interference or timing conflicts?**
I trigger and read each sensor sequentially in the main loop rather than in parallel, with a small delay between full cycles. That avoids one sensor's ultrasonic burst being picked up by a neighboring sensor's echo detection, which can happen with truly simultaneous triggering.

**8. What does the LCD/display logic actually do, and how is it connected?**
The LCD is I2C-connected (just SDA/SCL plus power), so it only needs two data pins regardless of how much I display. Each loop cycle, I clear and rewrite it with either the live available count or a "PARKING FULL" message, driven off the same `availableSlots` variable the LEDs and buzzer use — one source of truth for all outputs.

**9. How did you debug a sensor that wasn't giving reliable readings?**
I isolated it — ran the single-sensor test sketch on that one sensor alone with Serial Monitor open, checked TRIG/ECHO weren't swapped, confirmed the wiring against the pin table, and used `pulseIn()`'s timeout parameter so an out-of-range reading returns cleanly instead of hanging the loop, then treated that as a "no object" (free) case rather than a crash.

**10. What would you improve if you kept working on this?**
I'd move from polling to interrupt-driven echo capture to cut latency, add Wi-Fi on an ESP32 to push slot status to a small dashboard, and log occupancy over time so the data could support trend analysis, not just live status.
