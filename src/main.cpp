#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include "settings/AuthConstants.h"
#include "http/LocalHttpServer.h"
#include "motor/MotorController.h"
#include "settings/Settings.h"
#include "ArduinoNvs.h"


MotorSettings motorSettings(32, 26, 27);
MotorController motor(motorSettings);

LocalHttpServer server(80, motor, true);

void setup() {
  Serial.begin(115200);
  NVS.begin();
  // Load settings from NVS
  motorSettings.loadSettings();

  // Connect to wifi
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PWD);
  // Wait until wifi is connected
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  if(!MDNS.begin("petcamcontrol")) {
     Serial.println("Error starting mDNS - petcamcontrol");
     return;
  } else {
    Serial.println("Started mDNS - petcamcontrol.local");
  }
  server.begin();
}

void loop() {
  // Run tick methods to see if we need to handle any logic within classes
  motor.tick();
}