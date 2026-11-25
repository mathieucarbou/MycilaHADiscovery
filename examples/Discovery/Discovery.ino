#include <MycilaHADiscovery.h>

#include <HardwareSerial.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  Mycila::HA::Discovery haDiscovery(
    {
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

  haDiscovery.setWillTopic("~/status");

  // some diagnostic info
  auto restart = std::make_unique<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart");
  restart->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(restart));

  auto uptime = std::make_unique<Mycila::HA::Counter>("uptime", "Uptime", "~/system/uptime", "duration");
  uptime->unit = "s";
  uptime->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(uptime));

  auto memory = std::make_unique<Mycila::HA::Gauge>("memory_used", "Memory Used", "~/system/heap_used", "data_size");
  memory->icon = "mdi:memory";
  memory->unit = "B";
  memory->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(memory));

  auto ntp = std::make_unique<Mycila::HA::State>("ntp", "NTP", "~/ntp/synced", "true", "false", "connectivity");
  ntp->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(ntp));

  auto hostname = std::make_unique<Mycila::HA::Value>("hostname", "Hostname", "~/config/hostname");
  hostname->icon = "mdi:lan";
  hostname->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(hostname));

  // some configuration
  auto mqttInterval = std::make_unique<Mycila::HA::Text>("mqtt_publish_interval", "MQTT Publish Interval", "~/config/mqtt_interval/set", "~/config/mqtt_interval");
  mqttInterval->pattern = "^\\d+$";
  mqttInterval->icon = "mdi:timer-sand";
  mqttInterval->category = Mycila::HA::Category::CONFIG;
  haDiscovery.publish(std::move(mqttInterval));

  auto powerThreshold = std::make_unique<Mycila::HA::Number>("relay1_power_threshold", "Relay 1 Power Threshold", "~/config/rel1_power/set", "~/config/rel1_power");
  powerThreshold->mode = Mycila::HA::NumberMode::SLIDER;
  powerThreshold->min = 0;
  powerThreshold->max = 3000;
  powerThreshold->step = 50;
  powerThreshold->icon = "mdi:flash";
  powerThreshold->category = Mycila::HA::Category::CONFIG;
  haDiscovery.publish(std::move(powerThreshold));

  auto bypass = std::make_unique<Mycila::HA::Switch>("output1_auto_bypass_enable", "Output 1 Auto Bypass", "~/config/switch/set", "~/config/switch", "true", "false");
  bypass->icon = "mdi:water-boiler-auto";
  bypass->category = Mycila::HA::Category::CONFIG;
  haDiscovery.publish(std::move(bypass));

  auto day = std::make_unique<Mycila::HA::Select>("day", "Day", "~/config/day/set", "~/config/day");
  day->options = {"mon", "tue", "wed", "thu", "fri", "sat", "sun"};
  day->category = Mycila::HA::Category::CONFIG;
  haDiscovery.publish(std::move(day));

  // one-call form when no optional property is needed
  haDiscovery.publish<Mycila::HA::State>("grid", "Grid Electricity", "~/grid/online", "true", "false", "connectivity");

  std::unique_ptr<Mycila::HA::Outlet> relay1Commute = std::make_unique<Mycila::HA::Outlet>("rel_commute", "Relay 1", "~/relays/rel1/state/set", "~/relays/rel1/state", "on", "off");
  relay1Commute->availabilityTopic = "~/relays/rel1/enabled";
  relay1Commute->payloadAvailable = "true";
  relay1Commute->payloadNotAvailable = "false";
  haDiscovery.publish(std::move(relay1Commute));

  // climate with temperature, operating mode, action and a boost preset
  auto climate = std::make_unique<Mycila::HA::Climate>("thermostat", "Thermostat", "mdi:radiator");
  climate->currentTemperatureTopic = "~/thermostat/current_temperature";
  climate->temperatureCommandTopic = "~/thermostat/target_temperature/set";
  climate->temperatureStateTopic = "~/thermostat/target_temperature";
  climate->modeCommandTopic = "~/thermostat/mode/set";
  climate->modeStateTopic = "~/thermostat/mode";
  climate->modes = {"off", "heat"};
  climate->actionTopic = "~/thermostat/action";
  climate->presetModeCommandTopic = "~/thermostat/preset/set";
  climate->presetModeStateTopic = "~/thermostat/preset";
  climate->presetModes = {"boost"};
  climate->temperatureUnit = "C";
  climate->minTemp = 5;
  climate->maxTemp = 30;
  climate->tempStep = 0.1f;
  haDiscovery.publish(std::move(climate));
}

void loop() {
  delay(1000);
}
