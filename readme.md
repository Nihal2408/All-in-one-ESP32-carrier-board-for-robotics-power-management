<img width="750" height="644" alt="Screenshot 2026-11-07 at 10 35 29 AM" src="https://github.com/user-attachments/assets/0e66b8b9-5c54-4a6e-8d0a-a34a2a4b3cb4" />
<img width="1680" height="1050" alt="Screenshot 2026-11-07 at 10 35 01 AM" src="https://github.com/user-attachments/assets/2f9268bc-f8ca-42f1-a61f-4f49a1eb08f3" /># ESP32 Robotics Carrier Board

An all-in-one, production-grade robotics controller and carrier board designed in **EasyEDA**. Built around the **ESP32-WROOM-32E** microcontroller, this board integrates an onboard automatic SEPIC buck-boost converter, linear logic power rails, an L293DD dual H-Bridge DC motor driver, and dedicated breakout interfaces for servos, sensors, displays, and analog telemetry.

---

## 🚀 Key Features<img width="884" height="761" alt="Screenshot 2026-10-11 at 3 20 58 PM" src="https://github.com/user-attachments/assets/043aadb5-ee1d-46d0-b955-ad3d439929c9" />


* **Core Processor:** ESP32-WROOM-32E (Dual-core Xtensa 32-bit LX6, 240 MHz, 4MB Flash) with Wi-Fi & Bluetooth LE.
* **Onboard USB-to-UART:** CP2102N bridge with auto-reset circuitry (`EN` / `IO0`) for seamless code flashing over USB.
* **Advanced Power Subsystem (SEPIC Architecture):**
  * Wide input battery support: $9\text{V} - 13\text{V}$ ($B+$).
  * Regulated motor output: Default $12\text{V}$ stabilized rail, adjustable up to $24\text{V}$ via an onboard $50\ \text{k}\Omega$ trimmer potentiometer (`C118911`).
  * Seamless buck-boost operation prevents MCU brownouts when battery voltage drops below target under high motor load.
* **Dedicated Logic Voltage Rails:**
  * `EXT_5V` rail powered via an AMS1117-5.0 linear regulator for 5V sensors and servos.
  * `VDD33` rail powered via an AMS1117-3.3 linear regulator dedicated strictly to ESP32 core logic and low-voltage ICs.
* **Motor Actuation:**
  * Integrated **L293DD** surface-mount dual H-Bridge motor driver with flyback protection and thermal land pattern.
* **Robotics Sensor & Actuator Interfaces:**
  * Dual RC Servo headers with dedicated hardware PWM.
  * I2C OLED / Sensor bus (`IO21` / `IO22`).
  * Dual HC-SR04 ultrasonic rangefinder interfaces with integrated 5V-to-3.3V resistor dividers on Echo pins.
  * Dual IR obstacle / line-tracking headers.
  * Dedicated ADC1 analog telemetry channels (Wi-Fi safe).
  * General-purpose digital expansion headers.

---

## ⚡ Power System Architecture

```
                      +-------------------+
                      |   Battery (B+)    | (9V - 13V)
                      +---------+---------+
                                |
               +----------------+----------------+
               |                                 |
               v                                 v
     +-------------------+             +-------------------+
     | XL6019E1 (SEPIC)  |             |   AMS1117-5.0     |
     | Buck-Boost Stage  |             +---------+---------+
     +---------+---------+                       |
               |                                 v
               v (12V - 24V Adj)             [ EXT_5V ] (Sensors, Servos)
      [ Buck_12V Rail ]                          |
               |                                 v
       (Motors & L293DD)               +-------------------+
                                       |   AMS1117-3.3     |
                                       +---------+---------+
                                                 |
                                                 v
                                             [ VDD33 ] (ESP32, CP2102N)
```

### SEPIC Converter Bill of Materials:
* **Controller IC (`U8`):** XL6019E1 (5A 400kHz switching regulator)
* **Power Inductors (`L2`, `L3`):** $33\ \mu\text{H}$ SMD high-current power inductors
* **Coupling Capacitor (`C11`):** $10\ \mu\text{F}$ 50V Low-ESR ceramic capacitor
* **Rectifier Diode (`D5`):** SS54 Schottky diode (recommended upgrade to 60V–100V for >12V operations)
* **Filter Capacitors:** $220\ \mu\text{F}$ Low-ESR electrolytic capacitors at input (`C13`) and output (`C12`)
* **Feedback Network:** $1.25\text{V}$ reference divider comprising a $2\ \text{k}\Omega$ lower resistor (`R11`), a $15\ \text{k}\Omega$ base resistor, and a $50\ \text{k}\Omega$ trimpot (`C118911`)

---

## 📌 Pinout & Peripheral Assignment

| Peripheral Block | Header Ref | ESP32 GPIO | Net Label | Direction / Subsystem | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Motor A Control** | Onboard | `GPIO 12` | `IO12` | Output (PWM) | Strapping pin: Keep LOW during boot |
| **Motor B Control** | Onboard | `GPIO 26` | `IO26` | Output (PWM) | Safe General I/O |
| **Motor Enable** | Onboard | `GPIO 18` | `IO18` | Output (DIGITAL) | Safe General I/O |
| **Servo 1** | `H3` | `GPIO 13` | `IO13` | Output (LEDC PWM) | 5V Power (`EXT_5V`) |
| **Servo 2** | `H11` | `GPIO 4` | `IO4` | Output (LEDC PWM) | 5V Power (`EXT_5V`) |
| **I2C Display (SDA)**| `H13` / `H14` | `GPIO 21` | `IO21` | Bi-directional | Hardware I2C SDA |
| **I2C Display (SCL)**| `H13` / `H14` | `GPIO 22` | `IO22` | Output | Hardware I2C SCL |
| **IR Sensor 1** | `H8` | `GPIO 34` | `IO34` | Input Only | ADC1_CH6 / Digital read |
| **IR Sensor 2** | `H10` | `GPIO 35` | `IO35` | Input Only | ADC1_CH7 / Digital read |
| **Ultrasonic 1 Trig**| Sensor 1 | `GPIO 27` | `IO27` | Output | 10 µs pulse output |
| **Ultrasonic 1 Echo**| Sensor 1 | `GPIO 36` | `SENSOR_VP`| Input Only | Scaled via 1k/2k divider |
| **Ultrasonic 2 Trig**| Sensor 2 | `GPIO 14` | `IO14` | Output | 10 µs pulse output |
| **Ultrasonic 2 Echo**| Sensor 2 | `GPIO 39` | `SENSOR_VN`| Input Only | Scaled via 1k/2k divider |
| **Analog In / Telemetry**| `H15` | `GPIO 32` | `IO32` | Input (ADC1_CH4) | Safe with active Wi-Fi |
| **Analog In / Aux** | `H15` | `GPIO 33` | `IO33` | Input (ADC1_CH5) | Safe with active Wi-Fi |
| **Digital Expansion** | Breakout | `GPIO 16` | `IO16` | In / Out | UART2 RX / General I/O |
| **Digital Expansion** | Breakout | `GPIO 17` | `IO17` | In / Out | UART2 TX / General I/O |
| **Digital Expansion** | Breakout | `GPIO 19` | `IO19` | In / Out | VSPI MISO / General I/O |
| **Digital Expansion** | Breakout | `GPIO 23` | `IO23` | In / Out | VSPI MOSI / General I/O |
| **Digital Expansion** | Breakout | `GPIO 5` | `IO5` | In / Out | General I/O (Default HIGH) |

---

## 🛠️ PCB Layout & Design Highlights

* **Layer Stackup:** 2-Layer FR4 ($1.6\text{mm}$, 1 oz Copper).
* **Ground Planes:** Solid ground pour across bottom layer; analog inputs and sensitive digital buses are kept clear of high-frequency switching loops.
* **Thermal Management:**
  * Exposed copper thermal stitching vias underneath the **XL6019** regulator and **L293DD** motor driver.
  * Wide traces ($>1.5\text{mm}$) on the SEPIC power loop and motor lines to minimize $I^2R$ resistive heating and voltage drops.
* **Signal Isolation:**
  * High $\frac{di}{dt}$ switching current loops (inductors, Schottky diode, coupling capacitor) are placed tightly to eliminate EMI emission.
  * Feedback traces are routed far from magnetic fields of `L2` and `L3`.

---




## 🛒 Bill of Materials & Component Sourcing

### Parts & Material Buying Table

| Part / Material | Designator | Qty | Unit Price (USD) | Buying Link |
| :--- | :--- | :---: | :---: | :--- |
| **ESP32-WROOM-32E (4MB)** | `U2` | 1 | $2.35 | [LCSC C701341](https://www.lcsc.com/product-detail/WiFi-Modules_Espressif-Systems-ESP32-WROOM-32E-N4_C701341.html) |
| **CP2102N-A01-GQFN28** USB Bridge | `U4` | 1 | $1.98 | [LCSC C105167](https://www.lcsc.com/product-detail/USB-ICs_Silicon-Labs-CP2102N-A01-GQFN28R_C105167.html) |
| **XL6019E1** 5A SEPIC Controller | `U8` | 1 | $0.58 | [LCSC C73018](https://www.lcsc.com/product-detail/DC-DC-Converters_XLSEMI-XL6019E1_C73018.html) |
| **L293DD** Dual Motor Driver | `U3, U5` | 2 | $0.94 | [LCSC C9900019186](https://www.lcsc.com/search?q=C9900019186) |
| **AMS1117-3.3** Linear Regulator | `U1` | 1 | $0.12 | [LCSC C6186](https://www.lcsc.com/product-detail/Linear-Voltage-Regulators-LDO_AMS1117-3-3_C6186.html) |
| **AMS1117-5.0RG** Linear Regulator | `U6, U7` | 2 | $0.11 | [LCSC C2977151](https://www.lcsc.com/product-detail/Linear-Voltage-Regulators-LDO_AMS1117-5-0RG_C2977151.html) |
| **33uH** Shielded Power Inductor | `L2, L3` | 2 | $0.32 | [LCSC C167264](https://www.lcsc.com/product-detail/Inductors-SMD_Sumida-CDRH127NP-330MC_C167264.html) |
| **SS54** 5A 40V Schottky Diode | `D5` | 1 | $0.08 | [LCSC C22452](https://www.lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_SS54_C22452.html) |
| **BAT760-7** 30V 1A Schottky Diode | `D1` | 1 | $0.15 | [LCSC C27636120](https://www.lcsc.com/search?q=C27636120) |
| **LESD5D5.0CT1G** ESD Protection | `D2, D3, D4` | 3 | $0.04 | [LCSC C383211](https://www.lcsc.com/product-detail/ESD-and-Surge-Protection-TVS_LESD5D5-0CT1G_C383211.html) |
| **S8050** NPN Transistor (SOT-23) | `Q1, Q2` | 2 | $0.02 | [LCSC C2146](https://www.lcsc.com/product-detail/Bipolar-Transistors-BJT_Jiangsu-Changjing-Electronics-Technology-Co-Ltd-S8050_C2146.html) |
| **KT-0603R** Red SMD LED | `LED1` | 1 | $0.01 | [LCSC C2286](https://www.lcsc.com/product-detail/Light-Emitting-Diodes-LED_Hubei-Kento-Elec-KT-0603R_C2286.html) |
| **100uF** SMD Electrolytic (35V) | `C10` | 1 | $0.14 | [LCSC C18221518](https://www.lcsc.com/search?q=C18221518) |
| **220uF** SMD Electrolytic (25V/35V) | `C12, C13` | 2 | $0.18 | [LCSC C2918361](https://www.lcsc.com/search?q=C2918361) |
| **10uF** MLCC Ceramic (1206, 50V) | `C11` | 1 | $0.09 | [LCSC C13585](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL31A106KBHNNNE_C13585.html) |
| **22uF** MLCC Ceramic (0805) | `C1, C3, C5` | 3 | $0.05 | [LCSC C45783](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL21A226MAQNNNE_C45783.html) |
| **4.7uF** MLCC Ceramic (0603) | `C6` | 1 | $0.03 | [LCSC C19666](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Samsung-Electro-Mechanics-CL10A475KO8NNNC_C19666.html) |
| **100nF** MLCC Ceramic (0805) | `C7, C8, C9` | 3 | $0.01 | [LCSC C49678](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_YAGEO-CC0805KRX7R9BB104_C49678.html) |
| **100nF** MLCC Ceramic (0603) | `C2, C4` | 2 | $0.01 | [LCSC C14663](https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_YAGEO-CC0603KRX7R9BB104_C14663.html) |
| **10kΩ** SMD Resistor (0603) | `R1` | 1 | $0.01 | [LCSC C25804](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1002T5E_C25804.html) |
| **2kΩ** SMD Resistor (0603) | `R2, R11` | 2 | $0.01 | [LCSC C22975](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2001T5E_C22975.html) |
| **2kΩ** SMD Resistor (0805) | `R10` | 1 | $0.01 | [LCSC C17604](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F2001T5E_C17604.html) |
| **18kΩ** SMD Resistor (0603) | `R13` | 1 | $0.01 | [LCSC C25810](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF1802T5E_C25810.html) |
| **0Ω** Jumper Resistor (0805) | `R3, R4` | 2 | $0.01 | [LCSC C17477](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F0000T5E_C17477.html) |
| **10kΩ** SMD Resistor (0805) | `R5, R6, R9` | 3 | $0.01 | [LCSC C17414](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F1002T5E_C17414.html) |
| **20.5kΩ** SMD Resistor (0805) | `R7` | 1 | $0.01 | [LCSC C17537](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F2052T5E_C17537.html) |
| **47kΩ** SMD Resistor (0805) | `R8` | 1 | $0.01 | [LCSC C17713](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F4702T5E_C17713.html) |
| **Micro-USB Female Receptacle** | `USB1` | 1 | $0.16 | [LCSC C319160](https://www.lcsc.com/product-detail/USB-Connectors_Korean-HRO-Elec-U254-051T-4BH83-S1S_C319160.html) |
| **3x6x2.5mm SMD Tact Switch** | `SW1, SW2` | 2 | $0.04 | [LCSC C49234125](https://www.lcsc.com/search?q=C49234125) |
| **2-Pin 2.54mm Female Header** | `MOTOR_A, MOTOR_B, BATTERY_INPUT` | 3 | $0.04 | [LCSC C49661](https://www.lcsc.com/product-detail/Female-Headers_BOOMELE-Boom-Precision-Elec-2-54-1-2P_C49661.html) |
| **3-Pin 2.54mm Female Header** | `SENSOR, H10, SERVO_A, SERVO_B` | 4 | $0.05 | [LCSC C146690](https://www.lcsc.com/search?q=C146690) |
| **4-Pin 2.54mm Female Header** | `H13, DISPLAY&I2C, ULTRA, H15` | 4 | $0.06 | [LCSC C225501](https://www.lcsc.com/search?q=C225501) |
| **5-Pin 2.54mm Female Header** | `H7, POWER, GPIO` | 3 | $0.07 | [LCSC C50950](https://www.lcsc.com/product-detail/Female-Headers_BOOMELE-Boom-Precision-Elec-2-54-1-5P_C50950.html) |

### Visual BOM Reference

![Bill of Materials](docs/images/bom_corrected_table.png)




## 📋 Manufacturing & Assembly Checklist

When ordering boards from **JLCPCB** or similar fabrication houses:

1. **Design Rule Check (DRC):**
   * Run the DRC tool in EasyEDA to ensure zero clearance, unrouted nets, or overlapping pad errors.
2. **Gerber Generation:**
   * Go to `Fabrication` -> `PCB Fabrication File (Gerber)` in EasyEDA.
   * Verify top/bottom copper, silkscreen, solder mask, and drill layer outputs.
3. **Bill of Materials (BOM) & CPL:**
   * Export BOM and Pick-and-Place (CPL) files if using SMT assembly services.
   * Verify diode `D5` voltage rating matches target output ($20\text{V}$ margin above maximum trimmer setpoint).
4. **Assembly Cautions:**
   * Ensure `IO39` (`SENSOR_VN`) has zero connection or solder bridges to adjacent Ground pads.
   * Adjust trimpot `C118911` with a multimeter across the `Buck_12V` output **before** connecting motors or external loads.

---

## 📄 License
This project is open-source hardware licensed under the [MIT License](LICENSE).
