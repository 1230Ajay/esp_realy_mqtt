#include <Arduino.h>
#include <MQTTService.h>
#include <constants.h>
#include <ESP8266WiFi.h>
#include <TaskManager.h>
#include <Ticker.h>

WiFiClient espClient;
PeriodicTaskManager taskManager;
MQTTService mqttService(espClient, taskManager);

// Watchdog variables
Ticker wdtTicker;
const unsigned long WDT_CHECK_INTERVAL = 10000*6; // 10 seconds
unsigned long lastWatchdogReset = 0;

void feedHardwareWatchdog() {
  ESP.wdtFeed(); // Feed hardware WDT
  lastWatchdogReset = millis();
}

void checkSoftwareWatchdog() {
  if (millis() - lastWatchdogReset > 15000) { // 15s without reset
    Serial.println("Software Watchdog timeout! Restarting...");
    ESP.restart();
  }
}

void restartDeviceWrapper() {
  ESP.restart();
}

void setup() {
  Serial.begin(115200);

  // Initialize hardware watchdog
  ESP.wdtDisable(); // First disable
  ESP.wdtEnable(WDTO_8S); // Enable with 8s timeout (max for ESP8266)

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  WiFi.begin(DEFAULT_SSID, DEFAULT_WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    ESP.wdtFeed(); // Feed WDT during connection attempt
    Serial.print(".");
  }

  // Setup software watchdog ticker
  wdtTicker.attach_ms(WDT_CHECK_INTERVAL, checkSoftwareWatchdog);
  
  mqttService.setupMQTT();
  feedHardwareWatchdog(); // Initial feed
}

void loop() {
  ESP.wdtFeed(); // Feed hardware WDT
  
  taskManager.run();
  mqttService.loopMQTT();

  // if (!mqttService.isConnected() && !taskManager.isTaskRegistered(INTERNET_DISCONNECTION_TASK)) {
  //   taskManager.registerTask(INTERNET_DISCONNECTION_TASK, restartDeviceWrapper, 5 * 60 * 1000);
  // }

  // Additional watchdog feed point
  if (millis() - lastWatchdogReset > 5000) {
    feedHardwareWatchdog();
  }
}