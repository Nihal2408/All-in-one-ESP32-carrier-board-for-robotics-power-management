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



## 📦 Bill of Materials (BOM)

### Visual BOM Reference
![Bill of Materials Preview](docs/images/bom_corrected_table.png)

### Complete Parts List

| Designator | Component / Value | Footprint | Qty | Manufacturer Part # | LCSC Part # | Notes |
| :--- | :--- | :--- | :---: | :--- | :--- | :--- |
| **U2** | ESP32-WROOM-32E (4MB) | `WIFI-SMD_ESP32-WROOM-32E` | 1 | `ESP32-WROOM-32E-N4` | `C701341` | Core SoC |
| **U4** | CP2102N-A01-GQFN28 | `QFN-28_L5.0-W5.0-P0.50-BL-EP3.3` | 1 | `CP2102N-A01-GQFN28R` | `C105167` | USB UART Bridge |
| **U3, U5** | L293DD | `SOIC-20_L12.8-W7.5-P1.27-LS10.3-TL` | 2 | `L293DD` | `C9900019186` | Motor Drivers |
| **U8** | XL6019E1 | `TO-263-5_L10.2-W9.9-P1.70-LS14.4-TL` | 1 | `XL6019E1` | `C73018` | SEPIC Controller |
| **U1** | AMS1117-3.3 | `SOT-223-3_L6.5-W3.4-P2.30-LS7.0-BR` | 1 | `AMS1117-3.3` | `C6186` | 3.3V Logic LDO |
| **U6, U7** | AMS1117-5.0RG | `SOT-223-3_L6.4-W3.5-P2.30-LS7.0-BR` | 2 | `AMS1117-5.0RG` | `C2977151` | 5.0V Peripheral LDO |
| **L2, L3** | 33uH Inductor | `IND-SMD_L12.0-W12.0_CDRH127NP` | 2 | `CDRH127NP-330MC` | `C167264` | Power Inductors |
| **D5** | SS54 Schottky Diode | `SMA_L4.4-W2.8-LS5.4-R-RD` | 1 | `SS54` | `C22452` | Power Rectifier |
| **D1** | BAT760-7 Schottky | `SOD-323_L1.7-W1.3-LS2.6-FD-1` | 1 | `BAT760-7` | `C27636120` | Reverse Protection |
| **D2, D3, D4** | LESD5D5.0CT1G | `SOD-523_L1.2-W0.8-LS1.6-BI` | 3 | `LESD5D5.0CT1G` | `C383211` | ESD Protection |
| **Q1, Q2** | S8050 NPN Transistor | `SOT-23-3_L3.0-W1.7-P0.95-LS2.9-BR` | 2 | `S8050 J3Y` | `C2146` | Auto-Program |
| **LED1** | KT-0603R Red LED | `LED-SMD_L1.6-W0.8-R-RD` | 1 | `KT-0603R` | `C2286` | Power Indicator |
| **C10** | 100uF Electrolytic | `CAP-SMD_BD6.3-L6.6-W6.6-FD` | 1 | `RVT1V101M0607` | `C18221518` | Bulk Cap |
| **C12, C13** | 220uF Electrolytic | `CAP-SMD_BD6.3-L6.6-W6.6-LS7.4-FD` | 2 | `RVT1E221M0607` | `C2918361` | SEPIC Input/Output |
| **C11** | 10uF Ceramic | `C1206` | 1 | `CL31A106KBHNNNE` | `C13585` | SEPIC Coupling |
| **C1, C3, C5** | 22uF Ceramic | `C0805` | 3 | `CL21A226MAQNNNE` | `C45783` | Decoupling |
| **C6** | 4.7uF Ceramic | `C0603` | 1 | `CL10A475KO8NNNC` | `C19666` | Decoupling |
| **C7, C8, C9** | 100nF Ceramic | `C0805` | 3 | `CC0805KRX7R9BB104` | `C49678` | Filter Cap |
| **C2, C4** | 100nF Ceramic | `C0603` | 2 | `CC0603KRX7R9BB104` | `C14663` | Filter Cap |
| **R1** | 10kΩ | `R0603` | 1 | `0603WAF1002T5E` | `C25804` | EN Pull-up |
| **R2** | 2kΩ | `R0603` | 1 | `0603WAF2001T5E` | `C22975` | LED Limiter |
| **R10** | 2kΩ | `R0805` | 1 | `0805W8F2001T5E` | `C17604` | Logic Divider |
| **R11** | 2kΩ | `R0603` | 1 | `0603WAF2001T5E` | `C22975` | FB Lower Resistor |
| **R13** | 18kΩ | `R0603` | 1 | `0603WAF1802T5E` | `C25810` | Divider Resistor |
| **R3, R4** | 0Ω | `R0805` | 2 | `0805W8F0000T5E` | `C17477` | Jumper Resistors |
| **R5, R6, R9** | 10kΩ | `R0805` | 3 | `0805W8F1002T5E` | `C17414` | Pull-ups |
| **R7** | 20.5kΩ | `R0805` | 1 | `0805W8F2052T5E` | `C17537` | Divider Resistor |
| **R8** | 47kΩ | `R0805` | 1 | `0805W8F4702T5E` | `C17713` | Divider Resistor |
| **USB1** | Micro-USB Connector | `MICRO-USB-SMD_U254-051T-4BH83-S1S` | 1 | `U254-051T-4BH83-S1S` | `C319160` | Programming Port |
| **SW1, SW2** | SMD Tact Switch | `SW-SMD_HX-3X6X2.5-X` | 2 | `HX-3x6x2.5-X SMD tact Switch` | `C49234125` | Reset / Boot |
| **MOTOR_A, MOTOR_B, BATTERY_INPUT** | HDR-F-2.54 1x2 | `HDR-F-2.54_1X2` | 3 | - | `C49661` | Power & Motors |
| **SENSOR, H10, SERVO_A, SERVO_B** | HDR-F-2.54 1x3 | `HDR-F-2.54_1X3` | 4 | - | `C146690` | Sensors & Servos |
| **H13, DISPLAY&I2C, ULTRA, H15** | HDR-F-2.54 1x4 | `HDR-F-2.54_1X4` | 4 | - | `C225501` | Displays & Sonar |
| **H7, POWER, GPIO** | HDR-F-2.54 1x5 | `HDR-F-2.54_1X5` | 3 | - | `C50950` | Power & Expansion |

---





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
