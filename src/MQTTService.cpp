#include "MQTTService.h"
#include <constants.h>

MQTTService::MQTTService(WiFiClient &espClient, PeriodicTaskManager &taskManager)
    : espClient(espClient), client(espClient), lastReconnectAttempt(0), taskManager(taskManager)
{
}

void MQTTService::setupMQTT()
{
    client.setServer(HOST, PORT);
    client.setKeepAlive(30);
    client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { callback(topic, payload, length); });
}

char *MQTTService::concatenateChar(char *char1, char *char2)
{
    size_t len = strlen(char1) + strlen(char2) + 1; // +1 for null terminator
    char *concatenated = (char *)malloc(len);
    if (!concatenated)
        return nullptr;

    snprintf(concatenated, len, "%s%s", char1, char2);
    return concatenated;
}

void MQTTService::callback(char *topic, byte *payload, unsigned int length)
{

    if (millis() - connectedAt < 15000)
    {
        Serial.println("⚠ Ignoring messages for 15s after connection");
        return;
    }

    String topicStr = String(topic);
    String payloadStr;

    for (unsigned int i = 0; i < length; i++)
    {
        payloadStr += (char)payload[i];
    }

    if (topicStr == String(concatenateChar(DEVICE_RESTART_TOPIC, UID)))
    {
        Serial.println("Restart button clicked");
        restartDevice();
    }

    if (topicStr == String(concatenateChar(DEVICE_STATUS_TOPIC, UID)))
    {
        // Split the payload by '~' to extract status only
        String status = payloadStr;
        int separatorIndex = payloadStr.indexOf('~');
        if (separatorIndex != -1)
        {
            status = payloadStr.substring(0, separatorIndex);
        }

        if (status == "offline")
        {
            Serial.println("Device went offline");
            taskManager.registerTask(DEVICE_WENT_OFFLINE_TIMER, MQTTService::restartDevice, 5 * 60 * 1000);
        }
        else if (status == "online")
        {
            Serial.println("Device is online now");
            taskManager.unregisterTask(DEVICE_WENT_OFFLINE_TIMER);
        }
    }
}

void MQTTService::restartDevice()
{
    Serial.println("Device has been restrted");
    digitalWrite(RELAY_PIN,LOW);
    delay(1000 * 40);
    digitalWrite(RELAY_PIN, HIGH);
}

void MQTTService::subscribeWithUID(const char *baseTopic)
{
    char topic[128];
    snprintf(topic, sizeof(topic), "%s%s", baseTopic, UID);
    client.subscribe(topic);
}

void MQTTService::reconnect()
{
    if (!client.connected())
    {
        Serial.println("Attempting MQTT connection...");

        char will_topic[128];
        snprintf(will_topic, sizeof(will_topic), "%s%s", WILL_TOPIC, UID);

        char client_id[64];
        snprintf(client_id, sizeof(client_id), "%s%s", UID, "-relay");

        if (client.connect(client_id, USER, USER_PASS, will_topic, WILL_QOS, WILL_RETAIN, "offline"))
        {
            connectedAt = millis();
            Serial.println("Connected to MQTT broker");

            client.publish(will_topic, "online", WILL_RETAIN);
            subscribeWithUID(DEVICE_RESTART_TOPIC);
            // subscribeWithUID(DEVICE_STATUS_TOPIC);
        }
        else
        {
            Serial.print("MQTT connection failed, rc=");
            Serial.println(client.state());
        }
    }
}

void MQTTService::loopMQTT()
{
    if (!client.connected())
    {
        long now = millis();
        if (now - lastReconnectAttempt > 5000)
        {
            lastReconnectAttempt = now;
            reconnect();
        }
    }
    else
    {
        client.loop();
    }
}

bool MQTTService::isConnected()
{
    return client.connected();
}
