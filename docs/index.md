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
  haDiscovery.setWillTopic("~/status");

  // begin takes the Device, base topic and a callback to publish messages
  haDiscovery.begin({
                      .id = "my-app-1234",
                      .name = "My Application Name",
                      .version = "1.0.1",
                      .model = "OSS",
                      .manufacturer = "Mathieu Carbou",
                      .url = "http://" + WiFi.localIP().toString(),
                    },
                    "/my-app", // base topic
                    [](const char* topic, const char* payload) {
                      // Here, you would call your mqttClient.publish() code
                    });
```

Then query state:

```c++
  // one-call form: construct and publish in a single statement, when no optional property is needed
  haDiscovery.publish<Mycila::HA::State>("grid", "Grid Electricity", "~/grid/online", "true", "false", "connectivity");

  // when optional properties are needed, create with make_unique and configure them by name
  auto restart = std::make_unique<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart");
  restart->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(restart));

  auto uptime = std::make_unique<Mycila::HA::Counter>("uptime", "Uptime", "~/system/uptime", "duration");
  uptime->unit = "s";
  uptime->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(uptime));

  auto hostname = std::make_unique<Mycila::HA::Value>("hostname", "Hostname", "~/config/hostname");
  hostname->icon = "mdi:lan";
  hostname->category = Mycila::HA::Category::DIAGNOSTIC;
  haDiscovery.publish(std::move(hostname));

  auto powerThreshold = std::make_unique<Mycila::HA::Number>("relay1_power_threshold", "Relay 1 Power Threshold", "~/config/rel1_power/set", "~/config/rel1_power");
  powerThreshold->mode = Mycila::HA::NumberMode::SLIDER;
  powerThreshold->min = 0;
  powerThreshold->max = 3000;
  powerThreshold->step = 50;
  powerThreshold->icon = "mdi:flash";
  powerThreshold->category = Mycila::HA::Category::CONFIG;
  haDiscovery.publish(std::move(powerThreshold));

  // when you need to modify a component after construction, use std::make_unique + std::move
  std::unique_ptr<Mycila::HA::Outlet> relay1Commute = std::make_unique<Mycila::HA::Outlet>("rel_commute", "Relay 1", "~/relays/rel1/state/set", "~/relays/rel1/state", "on", "off");
  relay1Commute->availabilityTopic = "~/relays/rel1/enabled";
  relay1Commute->payloadAvailable = "true";
  relay1Commute->payloadNotAvailable = "false";
  haDiscovery.publish(std::move(relay1Commute));

  haDiscovery.end();
```

### API overview

`publish()` and `unpublish()` accept either:

1. **A `std::unique_ptr<Component>`** — for cases where you need to modify the component after construction (e.g. setting `availabilityTopic`):

   ```c++
   auto relay = std::make_unique<Mycila::HA::Outlet>("relay1", "Relay 1", "~/relays/rel1/set", "~/relays/rel1", "on", "off");
   relay->availabilityTopic = "~/relays/rel1/enabled";
   haDiscovery.publish(std::move(relay));
   ```

2. **Template parameters** — for the common case where you construct and publish in one call (component is allocated on the heap internally):

   ```c++
   haDiscovery.publish<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart");
   haDiscovery.publish<Mycila::HA::Gauge>("temperature", "Temperature", "~/sensor/temp", "temperature", nullptr, "°C");
   ```

When a component's optional `configTopic` override is set, `publish()` and `unpublish()` use it verbatim as the full discovery topic instead of `<discovery_topic>/<type>/<device_id>/<id>/config`. `unpublish()` must be given the same `configTopic` as the original `publish()` to clear the retained configuration — if the override differs or is left empty, the old retained message stays at the broker and Home Assistant keeps the entity:

   ```c++
   auto sensor = std::make_unique<Mycila::HA::Gauge>("temp1", "Temperature", "~/sensor/temp1", "temperature");
   sensor->configTopic = "homeassistant/sensor/my_device/temp1/config";
   haDiscovery.publish(std::move(sensor));

   // later: the override must match exactly to remove it
   auto old = std::make_unique<Mycila::HA::Gauge>("temp1", "Temperature", "");
   old->configTopic = "homeassistant/sensor/my_device/temp1/config";
   haDiscovery.unpublish(std::move(old));
   ```

The [dynamic entities example](https://github.com/mathieucarbou/MycilaHADiscovery/blob/main/examples/DynamicEntities/DynamicEntities.ino) creates components from generated IDs, names and topics, renames an entity while preserving its identity, and removes it. Its factory returns each component before publication: the component owns its strings, so the caller does not need to keep separate backing buffers alive. Constructors accept strings by value and move them into the component. Templates, payloads and constant metadata remain borrowed.

The example prints discovery topics and payloads to Serial. To use it with Home Assistant, replace the publisher callback with your MQTT transport and publish discovery messages with the retain flag set. Sensor readings are published separately on each sensor's state topic.

Component constructors take `std::string` arguments by value and move them into the component; string literals convert implicitly. `nullptr` is not accepted where a `std::string` is expected: use an empty string for omitted values, and use `clear()` instead of assigning `nullptr` to an owned field. For example, removal only needs the identity:

```c++
  haDiscovery.unpublish<Mycila::HA::Value>("firmware_filename", "");
```

Usage example in YaSolR: https://github.com/mathieucarbou/YaSolR/blob/main/src/yasolr_mqtt.cpp
