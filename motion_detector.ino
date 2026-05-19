#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
const char* ssid = "Airtel_Krishna";
const char* password = "Poornima_13";
#define BOT_TOKEN "8146182188:AAFedvpuXdda4rKzlYSeV8JQf5A29TwRgzk"
#define CHAT_ID "6307470883"
#define PIR_PIN D2
#define LED_PIN D1
unsigned long lastAlert = 0;
const unsigned long cooldown = 30000;
int motionCount = 0;
bool motionActive = false;
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
void sendTelegramAlert() {
  String message = "🚨 MOTION DETECTED! 🚨\n";
  message += "Location: ESP8266 Security System\n";
  message += "Motion Count: " + String(motionCount) + "\n";
  message += "System is monitoring...";
  bot.sendMessage(CHAT_ID, message, "");
  Serial.println("Telegram alert sent!");
}
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  client.setInsecure();
  delay(2000);
  Serial.println("System Armed! Monitoring for motion...");
  bot.sendMessage(CHAT_ID, "✅ Motion Detector System is now ONLINE!", "");
}
void loop() {
  int motionState = digitalRead(PIR_PIN);
  if (motionState == HIGH && !motionActive) {
    motionActive = true;
    motionCount++;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("MOTION DETECTED!");
    Serial.print("Motion Count: ");
    Serial.println(motionCount);
    if (millis() - lastAlert > cooldown) {
      Serial.println("Sending Telegram alert...");
      sendTelegramAlert();
      lastAlert = millis();
    } else {
      Serial.println("Cooldown active - alert not sent");
    }
  } else if (motionState == LOW && motionActive) {
    motionActive = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("Motion stopped - LED OFF");
  }
  delay(200);
}
