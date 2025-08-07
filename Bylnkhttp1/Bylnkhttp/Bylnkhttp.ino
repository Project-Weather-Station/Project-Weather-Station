#include <LiquidCrystal_I2C.h>


#include <ESP8266HTTPClient.h>

#include <SD.h>

#define BLYNK_TEMPLATE_ID "TMPL3xH38-i1O"
#define BLYNK_TEMPLATE_NAME "weather project"
#define BLYNK_AUTH_TOKEN "GLqE3fP7BPmABNqEYejTnMipOgTwGeCN"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#include <SoftwareSerial.h>
SoftwareSerial stmSerial(D7, D8);  // D7=GPIO13 (RX), D8=GPIO15 (TX)


// WiFi credentials
const char *ssid = "Oppo";
const char *password = "987654321";

// Sensor configuration
#define DHTPIN D4
#define DHTTYPE DHT11
#define RAINSENSOR_PIN A0
#define SOIL_PIN D3

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// Notification flags
bool soilNotified = false;
bool rainNotified = false;

void sendData() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int rainValue = analogRead(RAINSENSOR_PIN);
  int soilValue = digitalRead(SOIL_PIN);  // Change to analogRead(SOIL_PIN) if analog sensor


  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(rainValue);
  lcd.print(" S:");
  lcd.print(soilValue);

  // Send to Blynk virtual pins
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, rainValue);
  Blynk.virtualWrite(V3, soilValue);

  // Send to HTTP Server
  WiFiClient wifi;
  HTTPClient http;

  if (!isnan(temp)) {
    http.begin(wifi, "http://10.116.169.171:4000/temperature");
    http.addHeader("Content-Type", "application/json");
    http.POST("{\"value\":" + String(temp) + "}");
    http.end();
  }

  if (!isnan(humidity)) {
    http.begin(wifi, "http://10.116.169.171:4000/humidity");
    http.addHeader("Content-Type", "application/json");
    http.POST("{\"value\":" + String(humidity) + "}");
    http.end();
  }

  http.begin(wifi, "http://10.116.169.171:4000/rain");
  http.addHeader("Content-Type", "application/json");
  http.POST("{\"value\":" + String(rainValue) + "}");
  http.end();

  http.begin(wifi, "http://10.116.169.171:4000/soil");
  http.addHeader("Content-Type", "application/json");
  http.POST("{\"value\":" + String(soilValue) + "}");
  http.end();

  // Notifications (one-time until cleared)
  if (soilValue == 1 && !soilNotified) {
    Blynk.logEvent("soil_dry", "🌱 Soil is Dry!");
    soilNotified = true;
  } else if (soilValue == 0) {
    soilNotified = false;
  }

  if (rainValue < 500 && !rainNotified) {
    Blynk.logEvent("rain_detected", "🌧️ Rain Detected!");
    rainNotified = true;
  } else if (rainValue >= 500) {
    rainNotified = false;
  }
}

void setup() {
stmSerial.begin(9600);
  dht.begin();
  pinMode(SOIL_PIN, INPUT);
  lcd.begin(16, 2);
  lcd.backlight();

  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.clear();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(2000L, sendData);  // Call sendData() every 2 seconds
}

void loop() {
  lcd.init();
  lcd.backlight();

  Blynk.run();
  timer.run();
}
