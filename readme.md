# ESP32 Robotics Carrier Board

A 2-layer robot controller board I designed in EasyEDA around the ESP32-WROOM-32E. I got tired of jumper-wire spaghetti on my small mobile robots, so I put the motor power, motor drivers, USB flashing and sensor headers all on one board.

<img width="884" height="761" alt="3D PCB Render" src="https://github.com/user-attachments/assets/043aadb5-ee1d-46d0-b955-ad3d439929c9" />

<img width="750" height="644" alt="Schematic" src="https://github.com/user-attachments/assets/0e66b8b9-5c54-4a6e-8d0a-a34a2a4b3cb4" />
<img width="1680" height="1050" alt="PCB layout" src="https://github.com/user-attachments/assets/2f9268bc-f8ca-42f1-a61f-4f49a1eb08f3" />

## What's on it

- **ESP32-WROOM-32E (4MB)** with a CP2102N USB-UART chip. There's an auto-reset circuit on EN/IO0, so flashing works without holding the boot button.
- **Motor rail:** XL6019E1 in a SEPIC setup. It takes 9-13V from the battery and gives a 12V rail. A trimpot lets you adjust it (up to about 24V, see the notes below).
- **Logic power:** AMS1117 regulators for 5V (servos, sensors) and 3.3V (ESP32 and CP2102N).
- **Motor drivers:** two L293DD H-bridges.
- **Headers:** 2 servo, I2C (IO21/IO22), 2 HC-SR04 ultrasonic (with dividers on the echo pins), 2 IR sensors, 2 analog inputs, and a few spare GPIOs.

## SEPIC section

- Inductors L2, L3: 33 uH shielded
- Coupling cap C11: 10 uF 50V
- Input/output caps C12, C13: 220 uF
- Diode D5: SS54 (40V)
- Feedback: 2k (R11) on the bottom, 15k on top, plus a 50k trimpot (C118911)

If you turn the trimpot up toward 24V, change D5 to a 60V or 100V Schottky. The SS54 doesn't leave enough margin.

## Pinout

| Function | Header | GPIO | Notes |
| :--- | :--- | :--- | :--- |
| Motor A PWM | onboard | 12 | strapping pin, must be LOW at boot |
| Motor B PWM | onboard | 26 | |
| Motor enable | onboard | 18 | |
| Servo 1 | H3 | 13 | 5V from EXT_5V |
| Servo 2 | H11 | 4 | 5V from EXT_5V |
| I2C SDA | H13/H14 | 21 | |
| I2C SCL | H13/H14 | 22 | |
| IR sensor 1 | H8 | 34 | input only |
| IR sensor 2 | H10 | 35 | input only |
| Ultrasonic 1 trig | sensor 1 | 27 | |
| Ultrasonic 1 echo | sensor 1 | 36 (VP) | divided down to 3.3V |
| Ultrasonic 2 trig | sensor 2 | 14 | |
| Ultrasonic 2 echo | sensor 2 | 39 (VN) | divided down to 3.3V |
| Analog in 1 | H15 | 32 | ADC1, works with Wi-Fi on |
| Analog in 2 | H15 | 33 | ADC1, works with Wi-Fi on |
| Spare | breakout | 16, 17, 19, 23, 5 | 16/17 = UART2, 19/23 = SPI, 5 is a strapping pin |

## Layout

2-layer, 1.6mm FR4, 1 oz copper. The bottom layer is mostly a ground pour. I kept the analog and I2C traces away from the SEPIC loop (L2, L3, D5, C11) so the switching noise doesn't show up in ADC readings. There are thermal vias under the XL6019 and the L293DDs, and the battery and motor traces are wide (over 1.5mm).

## Bill of materials

| Part | Designator | Qty | Price (USD) | LCSC |
| :--- | :--- | :---: | :---: | :--- |
| ESP32-WROOM-32E (4MB) | U2 | 1 | 2.35 | C701341 |
| CP2102N-A01-GQFN28 | U4 | 1 | 1.98 | C105167 |
| XL6019E1 | U8 | 1 | 0.58 | C73018 |
| L293DD | U3, U5 | 2 | 0.94 | C9900019186 |
| AMS1117-3.3 | U1 | 1 | 0.12 | C6186 |
| AMS1117-5.0 | U6, U7 | 2 | 0.11 | C2977151 |
| 33uH inductor | L2, L3 | 2 | 0.32 | C167264 |
| SS54 diode | D5 | 1 | 0.08 | C22452 |
| BAT760-7 diode | D1 | 1 | 0.15 | C27636120 |
| LESD5D5.0CT1G ESD diode | D2, D3, D4 | 3 | 0.04 | C383211 |
| S8050 NPN | Q1, Q2 | 2 | 0.02 | C2146 |
| KT-0603R red LED | LED1 | 1 | 0.01 | C2286 |
| 100uF electrolytic 35V | C10 | 1 | 0.14 | C18221518 |
| 220uF electrolytic | C12, C13 | 2 | 0.18 | C2918361 |
| 10uF 1206 50V | C11 | 1 | 0.09 | C13585 |
| 22uF 0805 | C1, C3, C5 | 3 | 0.05 | C45783 |
| 4.7uF 0603 | C6 | 1 | 0.03 | C19666 |
| 100nF 0805 | C7, C8, C9 | 3 | 0.01 | C49678 |
| 100nF 0603 | C2, C4 | 2 | 0.01 | C14663 |
| 10k 0603 | R1 | 1 | 0.01 | C25804 |
| 2k 0603 | R2, R11 | 2 | 0.01 | C22975 |
| 2k 0805 | R10 | 1 | 0.01 | C17604 |
| 18k 0603 | R13 | 1 | 0.01 | C25810 |
| 0 ohm 0805 | R3, R4 | 2 | 0.01 | C17477 |
| 10k 0805 | R5, R6, R9 | 3 | 0.01 | C17414 |
| 20.5k 0805 | R7 | 1 | 0.01 | C17537 |
| 47k 0805 | R8 | 1 | 0.01 | C17713 |
| Micro-USB | USB1 | 1 | 0.16 | C319160 |
| Tact switch 3x6x2.5 | SW1, SW2 | 2 | 0.04 | C49234125 |
| 2-pin female header | MOTOR_A, MOTOR_B, BATTERY_INPUT | 3 | 0.04 | C49661 |
| 3-pin female header | SENSOR, H10, SERVO_A, SERVO_B | 4 | 0.05 | C146690 |
| 4-pin female header | H13, DISPLAY&I2C, ULTRA, H15 | 4 | 0.06 | C225501 |
| 5-pin female header | H7, POWER, GPIO | 3 | 0.07 | C50950 |

## Bring-up

1. Power it up with nothing connected to the motor outputs.
2. Probe `Buck_12V` with a multimeter and turn the trimpot until you get the voltage you want.
3. Then plug in the motors.

If you hand-solder it, check GPIO 39 (SENSOR_VN). There's a ground pad right next to it that is easy to bridge.

## Known issues

- GPIO 12 is a strapping pin and I'm using it for motor PWM. If the driver input pulls it high at boot, the ESP32 won't start.
- The AMS1117s drop a lot of voltage from the battery, so they'll get warm if you load the 5V rail with servos.
- SS54 limits the motor rail to well under 24V (see above).

## License

MIT, see [LICENSE](LICENSE).
