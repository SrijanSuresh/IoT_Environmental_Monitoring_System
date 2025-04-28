/*
Team Number: 19
Team Members:
- Nathan Thokkudubiyyapu (nthok2)
- Srijan Suresh (ssure8)
- Sripad Sirikonda (ssiri5)
- Sailesh Senthilkumar (ssent6)

Project Name: Smart Home Environment Monitoring System
Abstract: Our project aims to develop a Smart Home Environment Monitoring System using 
multiple Arduino Uno WiFi microcontrollers to track environmental conditions such as temperature, 
humidity, air quality, light levels, and sound levels within a home. The system will leverage 
WiFi-based communication to transmit sensor data to an IoT platform, enabling homeowners to 
monitor conditions remotely. Additionally, predictive analytics will be employed to optimize 
energy efficiency and enhance indoor comfort. The system will integrate with existing smart home 
devices, allowing automated HVAC and lighting adjustments for a seamless user experience. */

#include <WiFiS3.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// WiFi & Adafruit IO credentials
const char* ssid = "your_ssid";
const char* password = "your_password";
const char* mqtt_server   = "io.adafruit.com";
const int   mqtt_port     = 1883;
const char* mqtt_user     = "your_username";
const char* mqtt_password = "your_api_key";

WiFiClient net;
PubSubClient client(net);

// MQ-135 Sensor Pin
#define MQ135_PIN A0

// WS2812B LED Ring Config
#define LED_PIN    3
#define LED_COUNT  16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Air Quality Thresholds
const int AQ_GOOD     = 30;
const int AQ_MODERATE = 50;

// Timing variables
unsigned long previousMillis = 0;
const long interval = 60000;

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  Serial.println("Connecting to Adafruit IO...");
  while (!client.connected()) {
    if (client.connect("arduinoAirQuality", mqtt_user, mqtt_password)) {
      Serial.println("Connected to Adafruit IO!");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
    }
  }
}

void updateRing(int airVal) {
  uint32_t color;
  if (airVal < AQ_GOOD) {
    color = strip.Color(0, 150, 0);      // Green = Good
  } else if (airVal < AQ_MODERATE) {
    color = strip.Color(150, 150, 0);    // Yellow = Moderate
  } else {
    color = strip.Color(150, 0, 0);      // Red = Hazardous
  }

  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setup() {
  Serial.begin(9600);

  // Initialize LED Ring
  strip.begin();
  strip.show(); // All LEDs off

  // Connect WiFi & MQTT
  connectToWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int airVal = analogRead(MQ135_PIN);
    Serial.print("Air Quality (Raw): ");
    Serial.println(airVal);

    client.publish("SrijanSuresh/feeds/airquality", String(airVal).c_str());
    Serial.println("Published air quality to Adafruit IO.");

    updateRing(airVal);
  }
}