<img width="1680" height="1050" alt="Screenshot 2026-10-11 at 1 41 49 PM" src="https://github.com/user-attachments/assets/e110d744-82a8-44ad-a3ed-61f6d7a1b38b" />
# ESP32 Robotics Carrier Board

An all-in-one, production-grade robotics controller and carrier board designed in **EasyEDA**. Built around the **ESP32-WROOM-32E** microcontroller, this board integrates an onboard automatic SEPIC buck-boost converter, linear logic power rails, an L293DD dual H-Bridge DC motor driver, and dedicated breakout interfaces for servos, sensors, displays, and analog telemetry.

---

## 🚀 Key Features

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
