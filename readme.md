<img width="750" height="644" alt="Screenshot 2026-11-07 at 10 35 29 AM" src="https://github.com/user-attachments/assets/0e66b8b9-5c54-4a6e-8d0a-a34a2a4b3cb4" />
<img width="1680" height="1050" alt="Screenshot 2026-11-07 at 10 35 01 AM" src="https://github.com/user-attachments/assets/2f9268bc-f8ca-42f1-a61f-4f49a1eb08f3" /># ESP32 <img width="750" height="644" alt="Board Top View" src="https://github.com/user-attachments/assets/0e66b8b9-5c54-4a6e-8d0a-a34a2a4b3cb4" />
<img width="1680" height="1050" alt="Schematic Overview" src="https://github.com/user-attachments/assets/2f9268bc-f8ca-42f1-a61f-4f49a1eb08f3" />

# ESP32 Robotics Carrier Board

A 2-layer robotics controller PCB designed in EasyEDA around the **ESP32-WROOM-32E**. I put this together to avoid messy jumper wire nests on small mobile robots—it combines a buck-boost motor rail, dual H-bridge drivers, USB-to-UART flashing, and level-shifted sensor headers onto one board.

<img width="884" height="761" alt="3D PCB Render" src="https://github.com/user-attachments/assets/043aadb5-ee1d-46d0-b955-ad3d439929c9" />

## Hardware Overview

* **MCU & USB:** ESP32-WROOM-32E (4MB Flash) paired with a CP2102N USB-to-UART bridge and auto-reset transistor circuit (`EN`/`IO0`) so you don't have to hold down boot buttons when flashing.
* **Motor Power (SEPIC):** Takes a $9\text{V} - 13\text{V}$ battery input (`B+`) and outputs a steady $12\text{V}$ rail (adjustable up to $24\text{V}$ via the $50\ \text{k}\Omega$ trimpot). Using a SEPIC buck-boost stage keeps the motor voltage from sagging when the battery drops under heavy stall loads.
* **Logic Rails:** Separate AMS1117 regulators step `B+` down to `EXT_5V` (for servos and 5V sensors) and `VDD33` (isolated for the ESP32 and CP2102N).
* **Motor Drivers:** Dual **L293DD** SMD H-bridge drivers with thermal stitching vias underneath.
* **Peripheral Headers:**
  * 2x RC Servo headers (5V power, hardware LEDC PWM)
  * I2C bus (`IO21` / `IO22`) for OLED displays or IMUs
  * 2x HC-SR04 ultrasonic headers with onboard 1k/2k voltage dividers on the Echo lines
  * 2x IR line/obstacle sensor headers
  * 2x ADC1 analog inputs (safe to read while Wi-Fi is active) + spare GPIO breakouts

---

## SEPIC Converter Notes

The motor rail (`Buck_12V`) is driven by an **XL6019E1** (5A, 400kHz) configured in a SEPIC topology:

* **Inductors (`L2`, `L3`):** $33\ \mu\text{H}$ shielded SMD
* **Coupling Cap (`C11`):** $10\ \mu\text{F}$ 50V MLCC
* **Filter Caps (`C12`, `C13`):** $220\ \mu\text{F}$ low-ESR electrolytics on input and output
* **Diode (`D5`):** SS54 (40V Schottky). *Note: If you plan to crank the trimpot up toward 24V, swap `D5` for a 60V or 100V diode so you have enough reverse-voltage margin.*
* **Feedback Divider:** $1.25\text{V}$ reference set by a $2\ \text{k}\Omega$ lower resistor (`R11`), $15\ \text{k}\Omega$ base resistor, and a $50\ \text{k}\Omega$ trimpot (`C118911`).

---

## Pinout

| Function | Header | GPIO | Net | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **Motor A PWM** | Onboard | `GPIO 12` | `IO12` | **Strapping pin:** Must stay LOW during boot |
| **Motor B PWM** | Onboard | `GPIO 26` | `IO26` | General I/O |
| **Motor Enable** | Onboard | `GPIO 18` | `IO18` | General I/O |
| **Servo 1** | `H3` | `GPIO 13` | `IO13` | Powered by `EXT_5V` |
| **Servo 2** | `H11` | `GPIO 4` | `IO4` | Powered by `EXT_5V` |
| **I2C SDA** | `H13` / `H14` | `GPIO 21` | `IO21` | Hardware I2C |
| **I2C SCL** | `H13` / `H14` | `GPIO 22` | `IO22` | Hardware I2C |
| **IR Sensor 1** | `H8` | `GPIO 34` | `IO34` | Input-only (ADC1_CH6) |
| **IR Sensor 2** | `H10` | `GPIO 35` | `IO35` | Input-only (ADC1_CH7) |
| **Ultrasonic 1 Trig** | Sensor 1 | `GPIO 27` | `IO27` | 10 µs trigger pulse |
| **Ultrasonic 1 Echo** | Sensor 1 | `GPIO 36` | `SENSOR_VP` | 5V-to-3.3V divider onboard |
| **Ultrasonic 2 Trig** | Sensor 2 | `GPIO 14` | `IO14` | 10 µs trigger pulse |
| **Ultrasonic 2 Echo** | Sensor 2 | `GPIO 39` | `SENSOR_VN` | 5V-to-3.3V divider onboard |
| **Analog In 1** | `H15` | `GPIO 32` | `IO32` | ADC1_CH4 (Wi-Fi safe) |
| **Analog In 2** | `H15` | `GPIO 33` | `IO33` | ADC1_CH5 (Wi-Fi safe) |
| **GPIO Breakout** | Breakout | `GPIO 16` | `IO16` | UART2 RX |
| **GPIO Breakout** | Breakout | `GPIO 17` | `IO17` | UART2 TX |
| **GPIO Breakout** | Breakout | `GPIO 19` | `IO19` | VSPI MISO |
| **GPIO Breakout** | Breakout | `GPIO 23` | `IO23` | VSPI MOSI |
| **GPIO Breakout** | Breakout | `GPIO 5` | `IO5` | Strapping pin (pulled HIGH at boot) |

---

## Layout Notes

* **Stackup:** 2-layer FR4, $1.6\text{mm}$, 1 oz copper.
* **Grounding & Noise:** The bottom layer is a solid ground pour. Analog inputs and I2C traces are routed away from the XL6019 switching loop (`L2`, `L3`, `D5`, `C11`) to keep switching noise out of the ADC readings.
* **Thermals:** Thermal vias sit directly under the XL6019 and L293DD packages, with $>1.5\text{mm}$ traces on the battery and motor power paths.

---

## Bill of Materials

| Component | Designator | Qty | Unit Price (USD) | LCSC Part |
| :--- | :--- | :---: | :---: | :--- |
| **ESP32-WROOM-32E (4MB)** | `U2` | 1 | $2.35 | [C701341](https://www.lcsc.com/product-detail/WiFi-Modules_Espressif-Systems-ESP32-WROOM-32E-N4_C701341.html) |
| **CP2102N-A01-GQFN28** USB Bridge | `U4` | 1 | $1.98 | [C105167](https://www.lcsc.com/product-detail/USB-ICs_Silicon-Labs-CP2102N-A01-GQFN28R_C105167.html) |
| **XL6019E1** 5A SEPIC Controller | `U8` | 1 | $0.58 | [C73018](https://www.lcsc.com/product-detail/DC-DC-Converters_XLSEMI-XL6019E1_C73018.html) |
| **L293DD** Dual Motor Driver | `U3, U5` | 2 | $0.94 | [C9900019186](https://www.lcsc.com/search?q=C9900019186) |
| **AMS1117-3.3** LDO | `U1` | 1 | $0.12 | [C6186](https://www.lcsc.com/product-detail/Linear-Voltage-Regulators-LDO_AMS1117-3-3_C6186.html) |
| **AMS1117-5.0RG** LDO | `U6, U7` | 2 | $0.11 | [C2977151](https://www.lcsc.com/product-detail/Linear-Voltage-Regulators-LDO_AMS1117-5-0RG_C2977151.html) |
| **33uH** Shielded Inductor | `L2, L3` | 2 | $0.32 | [C167264](https://www.lcsc.com/product-detail/Inductors-SMD_Sumida-CDRH127NP-330MC_C167264.html) |
| **SS54** 5A 40V Schottky Diode | `D5` | 1 | $0.08 | [C22452](https://www.lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_SS54_C22452.html) |
| **BAT760-7** 30V 1A Schottky Diode | `D1` | 1 | $0.15 | [C27636120](https://www.lcsc.com/search?q=C27636120) |
| **LESD5D5.0CT1G** ESD Diode | `D2, D3, D4` | 3 | $0.04 | [C383211](https://www.lcsc.com/product-detail/ESD-and-Surge-Protection-TVS_LESD5D5-0CT1G_C383211.html) |
| **S8050** NPN Transistor (SOT-23) | `Q1, Q2` | 2 | $0.02 | [C2146](https://www.lcsc.com/product-detail/Bipolar-Transistors-BJT_Jiangsu-Changjing-Electronics-Technology-Co-Ltd-S8050_C2146.html) |
| **KT-0603R** Red SMD LED | `LED1` | 1 | $0.01 | [C2286](https://www.lcsc.com/product-detail/Light-Emitting-Diodes-LED_Hubei-Kento-Elec-KT-0603R_C2286.html) |
| **100uF** Electrolytic Cap (35V) | `C10` | 1 | $0.14 | [C18221518](https://www.lcsc.com/search?q=C18221518) |
| **220uF** Electrolytic Cap (25V/35V) | `C12, C13` | 2 | $0.18 | [C2918361](https://www.lcsc.com/search?q=C2918361) |
| **10uF** MLCC (1206, 50V) | `C11` | 1 | $0.09 | [C13585](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL31A106KBHNNNE_C13585.html) |
| **22uF** MLCC (0805) | `C1, C3, C5` | 3 | $0.05 | [C45783](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL21A226MAQNNNE_C45783.html) |
| **4.7uF** MLCC (0603) | `C6` | 1 | $0.03 | [C19666](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10A475KO8NNNC_C19666.html) |
| **100nF** MLCC (0805) | `C7, C8, C9` | 3 | $0.01 | [C49678](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_YAGEO-CC0805KRX7R9BB104_C49678.html) |
| **100nF** MLCC (0603) | `C2, C4` | 2 | $0.01 | [C14663](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_YAGEO-CC0603KRX7R9BB104_C14663.html) |
| **10kΩ** Resistor (0603) | `R1` | 1 | $0.01 | [C25804](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1002T5E_C25804.html) |
| **2kΩ** Resistor (0603) | `R2, R11` | 2 | $0.01 | [C22975](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2001T5E_C22975.html) |
| **2kΩ** Resistor (0805) | `R10` | 1 | $0.01 | [C17604](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F2001T5E_C17604.html) |
| **18kΩ** Resistor (0603) | `R13` | 1 | $0.01 | [C25810](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1802T5E_C25810.html) |
| **0Ω** Jumper (0805) | `R3, R4` | 2 | $0.01 | [C17477](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F0000T5E_C17477.html) |
| **10kΩ** Resistor (0805) | `R5, R6, R9` | 3 | $0.01 | [C17414](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F1002T5E_C17414.html) |
| **20.5kΩ** Resistor (0805) | `R7` | 1 | $0.01 | [C17537](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F2052T5E_C17537.html) |
| **47kΩ** Resistor (0805) | `R8` | 1 | $0.01 | [C17713](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F4702T5E_C17713.html) |
| **Micro-USB Receptacle** | `USB1` | 1 | $0.16 | [C319160](https://www.lcsc.com/product-detail/USB-Connectors_Korean-HRO-Elec-U254-051T-4BH83-S1S_C319160.html) |
| **3x6x2.5mm Tact Switch** | `SW1, SW2` | 2 | $0.04 | [C49234125](https://www.lcsc.com/search?q=C49234125) |
| **2-Pin 2.54mm Female Header** | `MOTOR_A, MOTOR_B, BATTERY_INPUT` | 3 | $0.04 | [C49661](https://www.lcsc.com/product-detail/Female-Headers_BOOMELE-Boom-Precision-Elec-2-54-1-2P_C49661.html) |
| **3-Pin 2.54mm Female Header** | `SENSOR, H10, SERVO_A, SERVO_B` | 4 | $0.05 | [C146690](https://www.lcsc.com/search?q=C146690) |
| **4-Pin 2.54mm Female Header** | `H13, DISPLAY&I2C, ULTRA, H15` | 4 | $0.06 | [C225501](https://www.lcsc.com/search?q=C225501) |
| **5-Pin 2.54mm Female Header** | `H7, POWER, GPIO` | 3 | $0.07 | [C50950](https://www.lcsc.com/product-detail/Female-Headers_BOOMELE-Boom-Precision-Elec-2-54-1-5P_C50950.html) |

![Bill of Materials](docs/images/bom_corrected_table.png)

---

## Bring-Up & Assembly Notes

If you are ordering boards from JLCPCB or hand-soldering a prototype:

* **Set the motor rail before connecting loads:** Power up the board and probe `Buck_12V` with a multimeter while adjusting trimpot `C118911` to your target voltage before plugging in any motors.
* **Watch out for `IO39` (`SENSOR_VN`):** Inspect the solder joints around `GPIO 39` carefully—there is a ground pad right next to it that can bridge easily when soldering by hand.
* **High-voltage operation:** Swap `D5` (SS54, 40V) for a higher-voltage Schottky if you intend to run the motor rail close to 24V.

## License

MIT License — see [LICENSE](LICENSE) for details.
---

## 📄 License
This project is open-source hardware licensed under the [MIT License](LICENSE).
