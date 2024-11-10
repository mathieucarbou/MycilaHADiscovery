#include <MycilaHADiscovery.h>

#include <HardwareSerial.h>
#include <WiFi.h>

Mycila::HADiscovery haDiscovery;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  haDiscovery.setWillTopic("/my-app/status");

  haDiscovery.begin({
                      .id = "my-app-1234",
                      .name = "My Application Name",
                      .version = "1.0.1",
                      .model = "OSS",
                      .manufacturer = "Mathieu Carbou",
                      .url = "http://" + WiFi.localIP().toString(),
                    },
                    "/my-app",
                    [](const char* topic, const char* payload) {
                      // Here, you would call your mqttClient.publish() code
                      Serial.println(topic);
                      Serial.println(payload);
                    });

  // some diagnostic info
  haDiscovery.publish(Mycila::HAButton("restart", "Restart", "/system/restart", "restart", nullptr, Mycila::HACategory::DIAGNOSTIC));
  haDiscovery.publish(Mycila::HACounter("uptime", "Uptime", "/system/uptime", "duration", nullptr, "s", Mycila::HACategory::DIAGNOSTIC));
  haDiscovery.publish(Mycila::HAGauge("memory_used", "Memory Used", "/system/heap_used", "data_size", "mdi:memory", "B", Mycila::HACategory::DIAGNOSTIC));
  haDiscovery.publish(Mycila::HAState("ntp", "NTP", "/ntp/synced", "true", "false", "connectivity", nullptr, Mycila::HACategory::DIAGNOSTIC));
  haDiscovery.publish(Mycila::HAText("hostname", "Hostname", "/config/hostname", nullptr, "mdi:lan", Mycila::HACategory::DIAGNOSTIC));

  // some configuration
  haDiscovery.publish(Mycila::HATextField("mqtt_publish_interval", "MQTT Publish Interval", "/config/mqtt_interval/set", "/config/mqtt_interval", "^\\d+$", "mdi:timer-sand", Mycila::HACategory::CONFIG));
  haDiscovery.publish(Mycila::HANumber("relay1_power_threshold", "Relay 1 Power Threshold", "/config/rel1_power/set", "/config/rel1_power", Mycila::HANumberMode::SLIDER, 0, 3000, 50, "mdi:flash", Mycila::HACategory::CONFIG));
  haDiscovery.publish(Mycila::HASwitch("output1_auto_bypass_enable", "Output 1 Auto Bypass", "/config/switch/set", "/config/switch", "true", "false", "mdi:water-boiler-auto", Mycila::HACategory::CONFIG));
  haDiscovery.publish(Mycila::HASelect("day", "Day", "/config/day/set", "/config/day", nullptr, Mycila::HACategory::CONFIG, {"mon", "tue", "wed", "thu", "fri", "sat", "sun"}));

  // some sensors
  haDiscovery.publish(Mycila::HAState("grid", "Grid Electricity", "/grid/online", "true", "false", "connectivity"));
  Mycila::HAOutlet relay1Commute("rel_commute", "Relay 1", "/relays/rel1/state/set", "/relays/rel1/state", "on", "off");
  relay1Commute.availabilityTopic = "/relays/rel1/enabled";
  relay1Commute.payloadAvailable = "true";
  relay1Commute.payloadNotAvailable = "false";
  haDiscovery.publish(relay1Commute);
}

void loop() {
  delay(1000);
}
