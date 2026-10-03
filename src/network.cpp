#include <WiFi.h>
#include <ArduinoJson.h>
#include "network.h"
#include "config.h"
#include "attributes.h"
#include "rpc.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ---------------------------------------------------------------------
// WiFi connection
// ---------------------------------------------------------------------
void connectWiFi() {

  Serial.print("Connecting to WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 15000) {

    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" connected.");
  } else {
    Serial.println(" FAILED (will retry).");
  }
}

// ---------------------------------------------------------------------
// ThingsBoard MQTT connection
// ---------------------------------------------------------------------
void connectMQTT() {

  if (WiFi.status() != WL_CONNECTED)
    return;

  Serial.print("Connecting to ThingsBoard MQTT...");

  if (mqtt.connect(BAY_ID, TB_TOKEN, NULL)) {

    Serial.println(" connected.");

    // ---------------------------------------------------------------
    // Subscribe to Shared Attributes
    // ---------------------------------------------------------------

    bool attrSub =
        mqtt.subscribe("v1/devices/me/attributes");

    bool attrResponseSub =
        mqtt.subscribe("v1/devices/me/attributes/response/+");

    // ---------------------------------------------------------------
    // Subscribe to RPC
    // ---------------------------------------------------------------

    bool rpcSub =
        mqtt.subscribe("v1/devices/me/rpc/request/+");

    Serial.print("Attributes subscription: ");
    Serial.println(attrSub ? "SUCCESS" : "FAILED");

    Serial.print("Attributes response subscription: ");
    Serial.println(attrResponseSub ? "SUCCESS" : "FAILED");

    Serial.print("RPC subscription: ");
    Serial.println(rpcSub ? "SUCCESS" : "FAILED");

    // Request current Shared Attributes
    requestSharedAttributes();

  } else {

    Serial.print(" FAILED, rc=");
    Serial.println(mqtt.state());

    delay(1000);
  }
}

// ---------------------------------------------------------------------
// MQTT callback
// ---------------------------------------------------------------------
void mqttCallback(char* topic,
                  byte* payload,
                  unsigned int length) {

  String topicStr = String(topic);

  // ---------------------------------------------------------------
  // Copy MQTT payload safely
  // ---------------------------------------------------------------

  char buf[400];

  unsigned int n =
      length < sizeof(buf) - 1
      ? length
      : sizeof(buf) - 1;

  memcpy(buf, payload, n);

  buf[n] = '\0';

  Serial.print("[MQTT <<] ");
  Serial.print(topicStr);
  Serial.print(" ");
  Serial.println(buf);

  // ---------------------------------------------------------------
  // RPC REQUEST
  // ---------------------------------------------------------------

  if (topicStr.startsWith("v1/devices/me/rpc/request/")) {

    String requestId =
        topicStr.substring(
            topicStr.lastIndexOf('/') + 1
        );

    handleRpc(requestId, buf);

    return;
  }

  // ---------------------------------------------------------------
  // SHARED ATTRIBUTES
  // ---------------------------------------------------------------

  StaticJsonDocument<256> doc;

  DeserializationError err =
      deserializeJson(doc, buf);

  if (err) {

    Serial.println("Attribute JSON: FAILED");

    return;
  }

  JsonObject attrs;

  if (doc.containsKey("shared")) {

    attrs =
        doc["shared"].as<JsonObject>();

  } else {

    attrs =
        doc.as<JsonObject>();
  }

  // Check whether attributes were actually received

  if (attrs.size() > 0) {

    applySharedAttributes(attrs);

    Serial.println("Shared attributes: SUCCESS");

  } else {

    Serial.println("Shared attributes: NO DATA");
  }
}