/**
 * @file esp32_robotics_carrier_test.ino
 * @brief Hardware Bring-up & Diagnostic Firmware for ESP32 Robotics Carrier Board
 * @author Nihal (Nihal2408)
 * 
 * Hardware Test Coverage:
 *  - Dual H-Bridge Motor Control (L293DD)
 *  - Dual Ultrasonic Sensors (HC-SR04)
 *  - Dual Infrared Sensors
 *  - Hardware PWM RC Servos (LEDC peripheral)
 *  - I2C Bus Scanner & Display Readiness (GPIO 21/22)
 *  - ADC1 Analog Telemetry (GPIO 32, 34, 35)
 *  - Digital Expansion Header Test (GPIO 5, 16, 17, 19, 23)
 */

#include <Arduino.h>
#include <Wire.h>

// ==========================================
// 1. PIN DEFINITIONS (MAPPED FROM CARRIER SCHEMATIC)
// ==========================================

// Motor Driver (L293DD / U3 & U5)
#define PIN_MOTOR_A_DIR1   12
#define PIN_MOTOR_A_DIR2   18
#define PIN_MOTOR_B_DIR1   26
#define PIN_MOTOR_B_DIR2   14

// Ultrasonic Rangefinders (HC-SR04)
#define PIN_ULTRA1_TRIG    4
#define PIN_ULTRA1_ECHO    36  // SENSOR_VP (Level-shifted to 3.3V)
#define PIN_ULTRA2_TRIG    27
#define PIN_ULTRA2_ECHO    39  // SENSOR_VN (Level-shifted to 3.3V)

// Infrared Sensors (Digital Inputs)
#define PIN_IR_LEFT        34
#define PIN_IR_RIGHT       35

// RC Servo Hardware PWM Channels (LEDC)
#define PIN_SERVO_A        13
#define PIN_SERVO_B        25
#define SERVO_PWM_FREQ     50    // Standard 50Hz RC servo refresh
#define SERVO_PWM_RES      16    // 16-bit resolution
#define SERVO_CH_A         0
#define SERVO_CH_B         1

// I2C Bus
#define PIN_I2C_SDA        21
#define PIN_I2C_SCL        22

// Analog Telemetry (ADC1 - Safe with Wi-Fi)
#define PIN_ANALOG_VBAT    32

// Digital Expansion Breakouts
const uint8_t DIGITAL_EXP_PINS[] = {5, 16, 17, 19, 23};
const uint8_t NUM_DIGITAL_EXP = sizeof(DIGITAL_EXP_PINS) / sizeof(DIGITAL_EXP_PINS[0]);

// ==========================================
// 2. HELPER FUNCTIONS
// ==========================================

void setServoAngle(uint8_t channel, float angle) {
  // Map 0 - 180 degrees to pulse widths between 0.5ms and 2.5ms at 50Hz (20ms period)
  // 16-bit resolution: 20ms = 65535 counts -> 1ms ~= 3276.7 counts
  float pulse_ms = 0.5 + (angle / 180.0) * 2.0;
  uint32_t duty = (uint32_t)((pulse_ms / 20.0) * 65535.0);
  ledcWrite(channel, duty);
}

float readUltrasonicCM(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout (~5m max)
  if (duration == 0) return -1.0;                // Out of range / No echo
  return (duration * 0.0343) / 2.0;
}

void scanI2CBus() {
  Serial.println("\n[I2C] Scanning bus...");
  uint8_t devicesFound = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  -> Device found at address: 0x%02X\n", addr);
      devicesFound++;
    }
  }
  if (devicesFound == 0) {
    Serial.println("  -> No I2C devices acknowledged. Check pull-ups and power.");
  }
}

// ==========================================
// 3. SETUP & INITIALIZATION
// ==========================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n================================================");
  Serial.println("  ESP32 ROBOTICS CARRIER BOARD - HARDWARE TEST  ");
  Serial.println("================================================");

  // 1. Initialize Motors
  pinMode(PIN_MOTOR_A_DIR1, OUTPUT);
  pinMode(PIN_MOTOR_A_DIR2, OUTPUT);
  pinMode(PIN_MOTOR_B_DIR1, OUTPUT);
  pinMode(PIN_MOTOR_B_DIR2, OUTPUT);
  digitalWrite(PIN_MOTOR_A_DIR1, LOW);
  digitalWrite(PIN_MOTOR_A_DIR2, LOW);
  digitalWrite(PIN_MOTOR_B_DIR1, LOW);
  digitalWrite(PIN_MOTOR_B_DIR2, LOW);
  Serial.println("[OK] Motor driver pins initialized (L293DD).");

  // 2. Initialize Ultrasonics
  pinMode(PIN_ULTRA1_TRIG, OUTPUT);
  pinMode(PIN_ULTRA1_ECHO, INPUT);
  pinMode(PIN_ULTRA2_TRIG, OUTPUT);
  pinMode(PIN_ULTRA2_ECHO, INPUT);
  Serial.println("[OK] Ultrasonic sensors initialized.");

  // 3. Initialize IR Sensors (GPI Pins 34 & 35)
  pinMode(PIN_IR_LEFT, INPUT);
  pinMode(PIN_IR_RIGHT, INPUT);
  Serial.println("[OK] Infrared sensor pins configured.");

  // 4. Initialize Hardware PWM Servos
  ledcSetup(SERVO_CH_A, SERVO_PWM_FREQ, SERVO_PWM_RES);
  ledcAttachPin(PIN_SERVO_A, SERVO_CH_A);
  ledcSetup(SERVO_CH_B, SERVO_PWM_FREQ, SERVO_PWM_RES);
  ledcAttachPin(PIN_SERVO_B, SERVO_CH_B);
  setServoAngle(SERVO_CH_A, 90);
  setServoAngle(SERVO_CH_B, 90);
  Serial.println("[OK] LEDC hardware PWM servos initialized at 90 deg.");

  // 5. Initialize I2C Bus
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  scanI2CBus();

  // 6. Initialize Digital Expansion Breakouts
  for (uint8_t i = 0; i < NUM_DIGITAL_EXP; i++) {
    pinMode(DIGITAL_EXP_PINS[i], OUTPUT);
    digitalWrite(DIGITAL_EXP_PINS[i], LOW);
  }
  Serial.println("[OK] Digital GPIO breakout headers initialized.");

  // 7. ADC Resolution Configuration
  analogReadResolution(12); // 0 - 4095
  Serial.println("[OK] ADC1 telemetry configured.");
  Serial.println("================================================\n");
}

// ==========================================
// 4. MAIN DIAGNOSTIC LOOP
// ==========================================

void loop() {
  Serial.println("--- [DIAGNOSTIC CYCLE START] ---");

  // 1. Read Analog Battery / Telemetry
  int rawAdc = analogRead(PIN_ANALOG_VBAT);
  float sensedVoltage = (rawAdc / 4095.0) * 3.3;
  Serial.printf("[ADC Telemetry] GPIO 32 Raw: %d | Voltage: %.2f V\n", rawAdc, sensedVoltage);

  // 2. Read IR Obstacle Sensors
  int irLeft = digitalRead(PIN_IR_LEFT);
  int irRight = digitalRead(PIN_IR_RIGHT);
  Serial.printf("[IR Sensors] Left (IO34): %s | Right (IO35): %s\n",
                irLeft == LOW ? "DETECTED" : "CLEAR",
                irRight == LOW ? "DETECTED" : "CLEAR");

  // 3. Read Ultrasonic Rangefinders
  float dist1 = readUltrasonicCM(PIN_ULTRA1_TRIG, PIN_ULTRA1_ECHO);
  float dist2 = readUltrasonicCM(PIN_ULTRA2_TRIG, PIN_ULTRA2_ECHO);
  Serial.printf("[Ultrasonic 1] Dist: %.1f cm | [Ultrasonic 2] Dist: %.1f cm\n", dist1, dist2);

  // 4. Pulse Digital Expansion Lines
  for (uint8_t i = 0; i < NUM_DIGITAL_EXP; i++) {
    digitalWrite(DIGITAL_EXP_PINS[i], HIGH);
  }
  delay(100);
  for (uint8_t i = 0; i < NUM_DIGITAL_EXP; i++) {
    digitalWrite(DIGITAL_EXP_PINS[i], LOW);
  }
  Serial.println("[Expansion GPIO] Cycled HIGH -> LOW test pulses.");

  // 5. Sweep Servos
  Serial.println("[Servos] Sweeping to 45 degrees...");
  setServoAngle(SERVO_CH_A, 45);
  setServoAngle(SERVO_CH_B, 135);
  delay(500);

  Serial.println("[Servos] Sweeping back to 135 degrees...");
  setServoAngle(SERVO_CH_A, 135);
  setServoAngle(SERVO_CH_B, 45);
  delay(500);

  setServoAngle(SERVO_CH_A, 90);
  setServoAngle(SERVO_CH_B, 90);

  // 6. Safe Low-Duty Motor Spin Test (0.5s Forward pulse)
  Serial.println("[Motors] Testing forward jog (0.5s)...");
  digitalWrite(PIN_MOTOR_A_DIR1, HIGH);
  digitalWrite(PIN_MOTOR_A_DIR2, LOW);
  digitalWrite(PIN_MOTOR_B_DIR1, HIGH);
  digitalWrite(PIN_MOTOR_B_DIR2, LOW);
  delay(500);

  digitalWrite(PIN_MOTOR_A_DIR1, LOW);
  digitalWrite(PIN_MOTOR_A_DIR2, LOW);
  digitalWrite(PIN_MOTOR_B_DIR1, LOW);
  digitalWrite(PIN_MOTOR_B_DIR2, LOW);
  Serial.println("[Motors] Stopped.");

  Serial.println("--- [CYCLE COMPLETE - WAITING 3 SECONDS] ---\n");
  delay(3000);
}