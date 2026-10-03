#include <ArduinoJson.h>
#include "attributes.h"
#include "network.h"
#include "state.h"

// ---------------------------------------------------------------------
// Request current Shared Attributes from ThingsBoard
// ---------------------------------------------------------------------
void requestSharedAttributes() {

  StaticJsonDocument<128> doc;

  doc["sharedKeys"] =
      "maxStationLoadW,"
      "overloadCurrentA,"
      "peakTariffStartHr,"
      "peakTariffEndHr,"
      "predictionThreshold";

  char buffer[128];

  serializeJson(doc, buffer);

  bool sent =
      mqtt.publish(
          "v1/devices/me/attributes/request/1",
          buffer
      );

  Serial.print("Shared attributes request: ");

  if (sent) {
    Serial.println("SENT");
  } else {
    Serial.println("FAILED");
  }
}

// ---------------------------------------------------------------------
// Apply Shared Attributes received from ThingsBoard
// ---------------------------------------------------------------------
void applySharedAttributes(JsonObject attrs) {

  if (attrs.containsKey("maxStationLoadW")) {

    maxStationLoadW =
        attrs["maxStationLoadW"];
  }

  if (attrs.containsKey("overloadCurrentA")) {

    overloadCurrentA =
        attrs["overloadCurrentA"];
  }

  if (attrs.containsKey("peakTariffStartHr")) {

    peakTariffStartHr =
        attrs["peakTariffStartHr"];
  }

  if (attrs.containsKey("peakTariffEndHr")) {

    peakTariffEndHr =
        attrs["peakTariffEndHr"];
  }

  if (attrs.containsKey("predictionThreshold")) {

    predictionThreshold =
        attrs["predictionThreshold"];
  }

  // ---------------------------------------------------------------
  // Print received values
  // ---------------------------------------------------------------

  Serial.println("================================");

  Serial.println("[Attributes updated]");

  Serial.print("maxStationLoadW = ");
  Serial.println(maxStationLoadW);

  Serial.print("overloadCurrentA = ");
  Serial.println(overloadCurrentA);

  Serial.print("peakTariffStartHr = ");
  Serial.println(peakTariffStartHr);

  Serial.print("peakTariffEndHr = ");
  Serial.println(peakTariffEndHr);

  Serial.print("predictionThreshold = ");
  Serial.println(predictionThreshold);

  Serial.println("================================");
}