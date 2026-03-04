#include <Wire.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

MPU6050 mpu;
WebSocketsServer webSocket(81);

const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5); // SDA, SCL
  mpu.initialize();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
  webSocket.begin();
}

void loop() {
  webSocket.loop();
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert to g and deg/s
  float Ax = ax / 16384.0, Ay = ay / 16384.0, Az = az / 16384.0;
  float Gx = gx / 131.0,   Gy = gy / 131.0,   Gz = gz / 131.0;

  StaticJsonDocument<200> doc;
  doc["ax"] = Ax; doc["ay"] = Ay; doc["az"] = Az;
  doc["gx"] = Gx; doc["gy"] = Gy; doc["gz"] = Gz;

  String json;
  serializeJson(doc, json);
  webSocket.broadcastTXT(json);
  delay(50); // ~20Hz
}
