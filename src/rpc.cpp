#include <ArduinoJson.h>
#include "rpc.h"
#include "network.h"
#include "state.h"
#include "config.h"

// ---------------------------------------------------------------------
// RPC command handling
// ---------------------------------------------------------------------
void handleRpc(String requestId, char* payload) {

  StaticJsonDocument<200> doc;

  if (deserializeJson(doc, payload)) {

    Serial.println("RPC JSON: FAILED");

    return;
  }

  String method =
      doc["method"] | "";

  JsonObject params =
      doc["params"];

  // ---------------------------------------------------------------
  // RPC received
  // ---------------------------------------------------------------

  Serial.println("================================");
  Serial.println("RPC received: SUCCESS");

  Serial.print("RPC Request ID: ");
  Serial.println(requestId);

  Serial.print("RPC Method: ");
  Serial.println(method);

  // ---------------------------------------------------------------
  // Response document
  // ---------------------------------------------------------------

  StaticJsonDocument<200> response;

  // ---------------------------------------------------------------
  // SET RELAY STATE
  // ---------------------------------------------------------------

  if (method == "setRelayState") {

    bool state =
        params["state"] | false;

    // Control physical relay

    if (state) {

      digitalWrite(RELAY_PIN, HIGH);

    } else {

      digitalWrite(RELAY_PIN, LOW);
    }

    // Update software state

    manualOverrideActive = true;

    throttleLevel =
        state ? 100 : 0;

    loadDecision =
        state
        ? "ALLOW"
        : "MANUAL_OFF";

    // RPC response

    response["success"] = true;

    response["throttleLevel"] =
        throttleLevel;

    Serial.print("Relay State: ");

    if (state) {
      Serial.println("ON");
    } else {
      Serial.println("OFF");
    }

    Serial.print("Throttle Level: ");
    Serial.println(throttleLevel);

    Serial.print("Load Decision: ");
    Serial.println(loadDecision);

  }

  // ---------------------------------------------------------------
  // SET THROTTLE
  // ---------------------------------------------------------------

  else if (method == "setThrottle") {

    int level =
        params["level"] | 100;

    level =
        constrain(level, 0, 100);

    manualOverrideActive =
        true;

    throttleLevel =
        level;

    loadDecision =
        "MANUAL_THROTTLE";

    response["success"] =
        true;

    response["throttleLevel"] =
        throttleLevel;

    Serial.print("Throttle Level: ");
    Serial.println(level);
  }

  // ---------------------------------------------------------------
  // UNKNOWN RPC
  // ---------------------------------------------------------------

  else {

    response["success"] =
        false;

    response["error"] =
        "unknown method";

    Serial.println("RPC Method: UNKNOWN");
  }

  // ---------------------------------------------------------------
  // Send RPC response to ThingsBoard
  // ---------------------------------------------------------------

  char buffer[220];

  serializeJson(
      response,
      buffer
  );

  String responseTopic =
      "v1/devices/me/rpc/response/"
      + requestId;

  bool responseSent =
      mqtt.publish(
          responseTopic.c_str(),
          buffer
      );

  Serial.print("RPC response: ");

  if (responseSent) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAILED");
  }

  Serial.println("================================");
}