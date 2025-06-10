
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WIFI CREDENTIALS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEFAULT_SSID "RKI_LAB"
#define DEFAULT_WIFI_PASS "inRKI@0510"

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MQTT SERVER DETAILS ~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define HOST "railkriti.co.in"
#define PORT 1883
#define USER "robo"
#define USER_PASS "rkirail"

/// --- UID
#define UID ( char*) "9e009754-a1ea-42aa-bf8b-71d7083f9ff1"

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SUBSCRIBING TOPICS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// --- WILL 
#define WILL_QOS 1
#define WILL_RETAIN true
#define WILL_TOPIC ( char*)"relay/status/brwlms/"

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PUBLISHING TOPICS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEVICE_STATUS_TOPIC ( char*)"device/status/brwlms/"
#define DEVICE_RESTART_TOPIC ( char*)"device/restart/brwlms/"

#define INTERNET_DISCONNECTION_TASK (char*)"internet_disconnected"
#define DEVICE_WENT_OFFLINE_TIMER (char*)"device_disconnected"

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PIN DEFINATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// --- RELAY PIN
#define RELAY_PIN 0