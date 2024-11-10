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
  haDiscovery.setWillTopic("/my-app/status");

  // begin takes the Device, base topic and a callback to publish messages
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
                    });
```

Then query state:

```c++
  haDiscovery.begin();

  // some diagnostic info
  haDiscovery.publish(Button("restart", "Restart", "/system/restart", "restart", nullptr, Category::DIAGNOSTIC));
  haDiscovery.publish(Counter("uptime", "Uptime", "/system/uptime", "duration", nullptr, "s", Category::DIAGNOSTIC));
  haDiscovery.publish(Gauge("memory_used", "Memory Used", "/system/heap_used", "data_size", "mdi:memory", "B", Category::DIAGNOSTIC));
  haDiscovery.publish(State("ntp", "NTP", "/ntp/synced", "true", "false", "connectivity", nullptr, Category::DIAGNOSTIC));
  haDiscovery.publish(Value("hostname", "Hostname", "/config/hostname", nullptr, "mdi:lan", Category::DIAGNOSTIC));

  // some configuration
  haDiscovery.publish(Text("mqtt_publish_interval", "MQTT Publish Interval", "/config/mqtt_interval/set", "/config/mqtt_interval", "^\\d+$", "mdi:timer-sand", Category::CONFIG));
  haDiscovery.publish(Number("relay1_power_threshold", "Relay 1 Power Threshold", "/config/rel1_power/set", "/config/rel1_power", NumberMode::SLIDER, 0, 3000, 50, "mdi:flash", Category::CONFIG));
  haDiscovery.publish(Switch("output1_auto_bypass_enable", "Output 1 Auto Bypass", "/config/switch/set", "/config/switch", "true", "false", "mdi:water-boiler-auto", Category::CONFIG));
  haDiscovery.publish(Select("day", "Day", "/config/day/set", "/config/day", nullptr, Category::CONFIG, {"mon", "tue", "wed", "thu", "fri", "sat", "sun"}));

  // some sensors
  haDiscovery.publish(State("grid", "Grid Electricity", "/grid/online", "true", "false", "connectivity"));
  Outlet relay1Commute("rel_commute", "Relay 1", "/relays/rel1/state/set", "/relays/rel1/state", "on", "off");
  relay1Commute.availabilityTopic = "/relays/rel1/enabled";
  relay1Commute.payloadAvailable = "true";
  relay1Commute.payloadNotAvailable = "false";
  haDiscovery.publish(relay1Commute);

  haDiscovery.end();
```
