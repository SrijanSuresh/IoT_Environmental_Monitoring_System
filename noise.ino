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

// WiFi & Adafruit IO credentials
const char* ssid = "your_ssid";
const char* password = "your_password";
const char* mqtt_server   = "io.adafruit.com";
const int   mqtt_port     = 1883;
const char* mqtt_user     = "your_username";
const char* mqtt_password = "your_api_key";

WiFiClient net;
PubSubClient client(net);

#define NOISE_PIN A0
#define BUZZER_PIN 9

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
    if (client.connect("arduinoNoiseBuzzer", mqtt_user, mqtt_password)) {
      Serial.println("Connected to Adafruit IO!");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  connectToWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) connectToMQTT();
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int noiseVal = analogRead(NOISE_PIN);
    Serial.print("Noise Level: ");
    Serial.println(noiseVal);

    client.publish("SrijanSuresh/feeds/noiselevel", String(noiseVal).c_str());
    Serial.println("Published noise level to Adafruit IO.");

    if (noiseVal > 30) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("Buzzer ON (Loud Noise).");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Buzzer OFF (Quiet).");
    }
  }
}