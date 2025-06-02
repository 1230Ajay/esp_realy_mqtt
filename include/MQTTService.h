#ifndef MQTT_SERVICE_H
#define MQTT_SERVICE_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <TaskManager.h>

class MQTTService {
public:
    MQTTService(WiFiClient &espClient,PeriodicTaskManager& taskManager);
    void setupMQTT();
    void loopMQTT();
    bool isConnected();
    char* concatenateChar(char* char1,char* char2);
    static void restartDevice();
private:
    PeriodicTaskManager& taskManager;
    WiFiClient &espClient;
    PubSubClient client;
    long lastReconnectAttempt;
    void callback(char *topic, byte *payload, unsigned int length);
    void reconnect();
    void subscribeWithUID(const char* baseTopic);
    unsigned long connectedAt = 0;
};

#endif
