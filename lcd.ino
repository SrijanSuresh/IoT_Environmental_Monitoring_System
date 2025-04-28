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
#include <LiquidCrystal.h>
#include <DHT.h>

// WiFi & Adafruit IO credentials
const char* ssid = "your_ssid";
const char* password = "your_password";
const char* mqtt_server   = "io.adafruit.com";
const int   mqtt_port     = 1883;
const char* mqtt_user     = "your_username";
const char* mqtt_password = "your_api_key";

WiFiClient net;
PubSubClient client(net);

#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long previousMillis = 0;
const long interval = 60000;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing DHT11 sensor...");
  dht.begin();

  Serial.println("Initializing LCD...");
  lcd.begin(16, 2);

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  Serial.println("MQTT client initialized.");
}

void loop() {
  if (!client.connected()) {
    Serial.println("Connecting to Adafruit IO...");
    while (!client.connect("arduinoDHTLCD", mqtt_user, mqtt_password)) {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
    }
    Serial.println("Connected to Adafruit IO!");
  }

  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    Serial.println("Reading DHT11 sensor...");
    if (!isnan(temp) && !isnan(hum)) {
      Serial.print("Temperature: ");
      Serial.print(temp);
      Serial.println(" °C");

      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.println(" %");

      client.publish("SrijanSuresh/feeds/temp", String(temp, 1).c_str());
      client.publish("SrijanSuresh/feeds/humidity", String(hum, 1).c_str());

      Serial.println("Published to Adafruit IO.");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(temp);
      lcd.print("C");

      lcd.setCursor(0, 1);
      lcd.print("Hum:");
      lcd.print(hum);
      lcd.print("%");
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
  }
}