#include <MycilaHADiscovery.h>

#include <HardwareSerial.h>
#include <WiFi.h>

Mycila::HA::Discovery haDiscovery;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  haDiscovery.setWillTopic("~/status");

  haDiscovery.begin({
                      .id = "my-app-1234",
                      .name = "My Application Name",
                      .version = "1.0.1",
                      .model = "OSS",
                      .manufacturer = "Mathieu Carbou",
                      .url = std::string("http://") + WiFi.localIP().toString().c_str(),
                    },
                    "/my-app", // base topic
                    [](const char* topic, const std::string& payload) {
                      // Here, you would call your mqttClient.publish() code
                      Serial.println(topic);
                      Serial.println(payload.c_str());
                    });

  // some diagnostic info
  haDiscovery.publish(std::make_unique<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart", nullptr, Mycila::HA::Category::DIAGNOSTIC));
  haDiscovery.publish(std::make_unique<Mycila::HA::Counter>("uptime", "Uptime", "~/system/uptime", "duration", nullptr, "s", Mycila::HA::Category::DIAGNOSTIC));
  haDiscovery.publish(std::make_unique<Mycila::HA::Gauge>("memory_used", "Memory Used", "~/system/heap_used", "data_size", "mdi:memory", "B", Mycila::HA::Category::DIAGNOSTIC));
  haDiscovery.publish(std::make_unique<Mycila::HA::State>("ntp", "NTP", "~/ntp/synced", "true", "false", "connectivity", nullptr, Mycila::HA::Category::DIAGNOSTIC));
  haDiscovery.publish(std::make_unique<Mycila::HA::Value>("hostname", "Hostname", "~/config/hostname", nullptr, "mdi:lan", Mycila::HA::Category::DIAGNOSTIC));

  // some configuration
  haDiscovery.publish(std::make_unique<Mycila::HA::Text>("mqtt_publish_interval", "MQTT Publish Interval", "~/config/mqtt_interval/set", "~/config/mqtt_interval", "^\\d+$", "mdi:timer-sand", Mycila::HA::Category::CONFIG));
  haDiscovery.publish(std::make_unique<Mycila::HA::Number>("relay1_power_threshold", "Relay 1 Power Threshold", "~/config/rel1_power/set", "~/config/rel1_power", Mycila::HA::NumberMode::SLIDER, 0, 3000, 50, "mdi:flash", Mycila::HA::Category::CONFIG));
  haDiscovery.publish(std::make_unique<Mycila::HA::Switch>("output1_auto_bypass_enable", "Output 1 Auto Bypass", "~/config/switch/set", "~/config/switch", "true", "false", "mdi:water-boiler-auto", Mycila::HA::Category::CONFIG));
  haDiscovery.publish(std::make_unique<Mycila::HA::Select>("day", "Day", "~/config/day/set", "~/config/day", nullptr, Mycila::HA::Category::CONFIG, std::vector<const char*>{"mon", "tue", "wed", "thu", "fri", "sat", "sun"}));

  // some sensors
  haDiscovery.publish(std::make_unique<Mycila::HA::State>("grid", "Grid Electricity", "~/grid/online", "true", "false", "connectivity"));
  std::unique_ptr<Mycila::HA::Outlet> relay1Commute = std::make_unique<Mycila::HA::Outlet>("rel_commute", "Relay 1", "~/relays/rel1/state/set", "~/relays/rel1/state", "on", "off");
  relay1Commute->availabilityTopic = "~/relays/rel1/enabled";
  relay1Commute->payloadAvailable = "true";
  relay1Commute->payloadNotAvailable = "false";
  haDiscovery.publish(std::move(relay1Commute));
}

void loop() {
  delay(1000);
}
