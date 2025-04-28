# IoT Environmental Monitoring System

This repository contains the code and documentation for a **distributed IoT environmental monitoring system** built with Arduino Uno R4, Adafruit IO, and MQTT. The system measures temperature, humidity, air quality, light intensity, and noise levels, and controls actuators like servos and buzzers based on sensor data.

## 🛠️ Hardware Overview

1. **Arduino 1 (Uno R4)**
   - **Inputs:** DHT11 (Temperature & Humidity)
   - **Output:** 16x2 LCD Display

2. **Arduino 2 (Uno R4)**
   - **Input:** MQ-135 (Air Quality)
   - **Output:** WS2812B 8-LED NeoPixel Ring

3. **Arduino 3 (Uno R4)**
   - **Input:** Photoresistor (Light Intensity)
   - **Output:** Servo Motor (Vent Control)

4. **Arduino 4 (Uno R4)**
   - **Input:** KY-038 (Noise Level)
   - **Output:** Buzzer


![Hardware Setup](https://github.com/user-attachments/assets/830cf27d-a25a-48e5-bb4f-de20a16c874b)

*Figure 1: Hardware setup of the 4 Arduinos and sensors.*

![Adafruit IO Dashboard](https://github.com/user-attachments/assets/c6ce798d-5b1c-4c18-8a2d-95d22d86c457)
*Figure 2: Live dashboard showing sensor feeds.*

## 🔧 Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/YourUsername/Environmental-IoT-System.git
   cd Environmental-IoT-System
   ```
2. Install required Arduino libraries via the Arduino IDE:
   - WiFiS3
   - PubSubClient
   - LiquidCrystal
   - DHT sensor library (Adafruit)
   - Adafruit NeoPixel
   - SoftwareSerial
3. Update `ssid`, `password`, `mqtt_user`, and `mqtt_password` variables in each Arduino sketch with your WiFi and Adafruit IO credentials.

## 🚀 Usage

1. Upload the respective sketch to each Arduino board:
   - `arduino1_dht_lcd.ino` → Arduino 1
   - `arduino2_airquality_led.ino` → Arduino 2
   - `arduino3_light_servo.ino` → Arduino 3
   - `arduino4_noise_buzzer.ino` → Arduino 4
2. Open the Serial Monitor at **9600 baud** to view debug logs.
3. Monitor live data on your Adafruit IO dashboard linked via MQTT.

## 📋 Code Structure

- **`lcd.ino`**: Reads DHT11, publishes `temp` and `humidity`, displays on LCD.
- **`led.ino`**: Reads MQ-135, publishes `airquality`, updates NeoPixel ring.
- **`light.ino`**: Reads LDR, publishes `lightintensity`, controls servo, receives noise over Serial.
- **`noise.ino`**: Reads KY-038, publishes `noiselevel` to Arduino 3 via Serial, controls buzzer.

## ⚙️ Configuration

- **Publish Interval:** Default is once per minute (`60000 ms`) to conserve Adafruit IO quota.
- **Thresholds:** Adjust `AQ_GOOD`, `AQ_MODERATE`, light and noise thresholds in code as needed.

---

© 2025 Srijan Suresh. All rights reserved.

