# MycilaHADiscovery

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Continuous Integration](https://github.com/mathieucarbou/MycilaHADiscovery/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaHADiscovery/actions/workflows/ci.yml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaHADiscovery.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaHADiscovery)

Simple and efficient Home Assistant Discovery library for Arduino / ESP32

## Features

- Supports diagnostic sensors, config sensors and normal sensors
- Editable components:
  - button
  - switch / outlet
  - select
  - slider / box with min, max, step
  - input (number and text)
- Sensors:
  - text
  - binary (state)
  - gauge
  - counter
- Supports regex validation
- Supports availability (global and per component)
- Supports device class, icon, state class, unit of measurement

## Basic Usage

Setup:

```c++
  haDiscovery.setBaseTopic("/my-app");
  haDiscovery.setWillTopic("/my-app/status");

  haDiscovery.setPublisher([](const char* topic, const char* payload) {
    // Here, you would call your mqttClient.publish() code
    Serial.println(topic);
    Serial.println(payload);
  });

  haDiscovery.setDevice({
    .id = "my-app-1234",
    .name = "My Application Name",
    .version = "1.0.1",
    .model = "OSS",
    .manufacturer = "Mathieu Carbou",
    .url = "http://" + WiFi.localIP().toString(),
  });

```

Then query state:

```c++
  haDiscovery.begin();

  // some diagnostic info
  haDiscovery.publish(HAButton("restart", "Restart", "/system/restart", "restart", nullptr, HACategory::DIAGNOSTIC));
  haDiscovery.publish(HACounter("uptime", "Uptime", "/system/uptime", "duration", nullptr, "s", HACategory::DIAGNOSTIC));
  haDiscovery.publish(HAGauge("memory_used", "Memory Used", "/system/heap_used", "data_size", "mdi:memory", "B", HACategory::DIAGNOSTIC));
  haDiscovery.publish(HAState("ntp", "NTP", "/ntp/synced", "true", "false", "connectivity", nullptr, HACategory::DIAGNOSTIC));
  haDiscovery.publish(HAText("hostname", "Hostname", "/config/hostname", nullptr, "mdi:lan", HACategory::DIAGNOSTIC));

  // some configuration
  haDiscovery.publish(HATextField("mqtt_publish_interval", "MQTT Publish Interval", "/config/mqtt_interval/set", "/config/mqtt_interval", "^\\d+$", "mdi:timer-sand", HACategory::CONFIG));
  haDiscovery.publish(HANumber("relay1_power_threshold", "Relay 1 Power Threshold", "/config/rel1_power/set", "/config/rel1_power", HANumberMode::SLIDER, 0, 3000, 50, "mdi:flash", HACategory::CONFIG));
  haDiscovery.publish(HASwitch("output1_auto_bypass_enable", "Output 1 Auto Bypass", "/config/switch/set", "/config/switch", "true", "false", "mdi:water-boiler-auto", HACategory::CONFIG));
  haDiscovery.publish(HASelect("day", "Day", "/config/day/set", "/config/day", nullptr, HACategory::CONFIG, {"mon", "tue", "wed", "thu", "fri", "sat", "sun"}));

  // some sensors
  haDiscovery.publish(HAState("grid", "Grid Electricity", "/grid/online", "true", "false", "connectivity"));
  HAOutlet relay1Commute("rel_commute", "Relay 1", "/relays/rel1/state/set", "/relays/rel1/state", "on", "off");
  relay1Commute.availabilityTopic = "/relays/rel1/enabled";
  relay1Commute.payloadAvailable = "true";
  relay1Commute.payloadNotAvailable = "false";
  haDiscovery.publish(relay1Commute);

  haDiscovery.end();
```
