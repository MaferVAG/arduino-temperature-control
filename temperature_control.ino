/**
 * Automated Temperature Control System
 * =====================================
 * Author: María Fernanda Vásquez — UAM Azcapotzalco
 * Course: Microelectronics (Microelectrónica)
 * Date:   December 2025
 *
 * Description:
 * Reads ambient temperature via an LM35 sensor, averages 5 readings
 * to eliminate noise, and activates a relay-controlled heating/cooling
 * circuit when temperature drops to or below 15°C.
 *
 * Hardware:
 *   - Arduino UNO
 *   - LM35 Precision Temperature Sensor → Analog Pin A0
 *   - 5V/10A Relay → Digital Pin 8
 *   - LCD 16x2 with I2C interface (address 0x27)
 *   - External 12V/12A heating (Nichrome wire) + cooling (fan) circuit
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// ── Constants ──────────────────────────────────────────────
const int   RELAY_PIN        = 8;
const int   SENSOR_PIN       = A0;
const int   NUM_READINGS     = 5;
const int   READING_DELAY_MS = 500;
const float TEMP_THRESHOLD   = 15.0;  // °C — activates relay below this value
const float ADC_VOLTAGE      = 5000.0; // mV reference voltage
const float ADC_RESOLUTION   = 1023.0;
const float LM35_MV_PER_DEG  = 10.0;  // LM35 outputs 10mV per °C

// ── LCD Setup ──────────────────────────────────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── Function Declarations ──────────────────────────────────
float readAverageTemperature();
void  updateDisplay(float temperature);
void  controlRelay(float temperature);

// ──────────────────────────────────────────────────────────
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // ensure relay starts OFF
}

void loop() {
  float avgTemp = readAverageTemperature();
  controlRelay(avgTemp);
  updateDisplay(avgTemp);
}

// ── Reads sensor 5 times and returns the average temperature
float readAverageTemperature() {
  float sum = 0.0;
  for (int i = 0; i < NUM_READINGS; i++) {
    int   raw  = analogRead(SENSOR_PIN);
    float temp = (raw * ADC_VOLTAGE / ADC_RESOLUTION) / LM35_MV_PER_DEG;
    sum += temp;
    delay(READING_DELAY_MS);
  }
  return sum / NUM_READINGS;
}

// ── Activates relay (heating) if temperature is at or below threshold
void controlRelay(float temperature) {
  if (temperature <= TEMP_THRESHOLD) {
    digitalWrite(RELAY_PIN, HIGH); // activate heating circuit
  } else {
    digitalWrite(RELAY_PIN, LOW);  // deactivate
  }
}

// ── Updates LCD with current temperature reading
void updateDisplay(float temperature) {
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print(" C  "); // trailing spaces clear previous digits
}