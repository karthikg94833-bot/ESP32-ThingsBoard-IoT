#ifndef CONFIG_H

#define CONFIG_H

// Pin configuration — SRS Section 8.1

// ---------------------------------------------------------------------

#define VOLTAGE_PIN   34

#define CURRENT_PIN   35

#define DHT_PIN       15

#define RELAY_PIN     26

#define BTN_PLUGIN    32

#define BTN_PLUGOUT   33

#define LED_GREEN     18

#define LED_YELLOW    19

#define LED_RED       21

#define DHT_TYPE DHT22

// ---------------- WiFi Details ----------------

#define WIFI_SSID "Wokwi-GUEST"

#define WIFI_PASSWORD ""

#define MQTT_SERVER "mqtt.thingsboard.cloud"

#define MQTT_PORT 1883

#define TB_TOKEN "0o6CsFmX6kckw6cTEQbE"

#define BAY_ID "BAY1"

#endif