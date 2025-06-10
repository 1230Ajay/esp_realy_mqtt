#include <Arduino.h>
#include <MQTTService.h>
#include <constants.h>
#include <ESP8266WiFi.h>
#include <TaskManager.h>

WiFiClient espClient;
PeriodicTaskManager taskManager;
MQTTService mqttService(espClient, taskManager);

void restartDeviceWrapper() {
  ESP.restart();
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  WiFi.begin(DEFAULT_SSID, DEFAULT_WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  mqttService.setupMQTT();
}

void loop() {
  taskManager.run();
  mqttService.loopMQTT();

  if (!mqttService.isConnected() && !taskManager.isTaskRegistered(INTERNET_DISCONNECTION_TASK)) {
    taskManager.registerTask(INTERNET_DISCONNECTION_TASK, restartDeviceWrapper, 12 * 60 * 60 * 1000);
  }
}
