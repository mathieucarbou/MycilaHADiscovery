#include <MycilaHADiscovery.h>

#include <HardwareSerial.h>

#include <memory>
#include <string>
#include <utility>

namespace HA = Mycila::HA;

HA::Discovery haDiscovery(
  {
    .id = "dynamic-demo",
    .name = "Dynamic Discovery Demo",
    .version = "1.0.0",
    .model = "ESP32",
    .manufacturer = "Example",
    .url = "",
  },
  "dynamic-demo", // base topic
  [](const char* topic, const std::string& payload) {
    // Replace this output with an MQTT publish using retain=true.
    // An empty retained payload removes the discovery config.
    Serial.println(topic);
    Serial.println(payload.empty() ? "(remove retained config)" : payload.c_str());
  });

// The component owns these generated strings after this function returns.
static std::unique_ptr<HA::Gauge> makeTemperatureSensor(unsigned channel, std::string name) {
  std::string id = "temperature_" + std::to_string(channel);
  std::string stateTopic = "~/sensors/" + id + "/temperature";

  return std::make_unique<HA::Gauge>(
    std::move(id), std::move(name), std::move(stateTopic),
    "temperature", nullptr, "°C");
}

void setup() {
  Serial.begin(115200);

  Serial.println("Create sensor 1");
  // Publication happens after the factory's local strings have gone out of scope.
  haDiscovery.publish(makeTemperatureSensor(1, "Room 1 temperature"));
}

void loop() {
  static uint32_t lastChange = millis();
  static unsigned step = 0;
  const uint32_t now = millis();

  if (now - lastChange >= 5000) {
    lastChange = now;

    switch (step) {
      case 0:
        Serial.println("Create sensor 2");
        haDiscovery.publish(makeTemperatureSensor(2, "Room " + std::to_string(2) + " temperature"));
        break;

      case 1:
        Serial.println("Rename sensor 1, preserving its discovery topic and unique ID");
        haDiscovery.publish(makeTemperatureSensor(1, "Living room temperature"));
        break;

      case 2:
        Serial.println("Remove sensor 2 using only its identity");
        haDiscovery.unpublish<HA::Gauge>("temperature_" + std::to_string(2));
        break;

      case 3:
        Serial.println("Restore sensor 1's name before repeating the demo");
        haDiscovery.publish(makeTemperatureSensor(1, "Room 1 temperature"));
        break;
    }

    step = (step + 1) % 4;
  }

  // Yield between simulated configuration changes.
  delay(10);
}
