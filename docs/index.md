# MycilaHADiscovery

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Continuous Integration](https://github.com/mathieucarbou/MycilaHADiscovery/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaHADiscovery/actions/workflows/ci.yml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaHADiscovery.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaHADiscovery)

Simple and efficient **Home Assistant MQTT Discovery** library for Arduino / ESP32.

The library builds Home Assistant discovery configuration payloads for you: you describe entities with plain C++ objects, publish them through your MQTT transport, and Home Assistant creates the corresponding devices and entities. Strings are owned by the components, so IDs, names and topics can be generated at runtime.

## Supported Components

Editable components:

- [Button](components/button.md)
- [Switch](components/switch.md) / [Outlet](components/switch.md)
- [Select](components/select.md)
- [Number](components/number.md) (slider, box, auto with min, max, step)
- [Text](components/text.md) (input)
- [Climate](components/climate.md) (temperature, mode, action and preset topics)
- [Cover](components/cover.md) (positions and tilt)
- [Fan](components/fan.md) (speed, oscillation, direction, presets)
- [Date](components/datetime.md), [Time](components/datetime.md) and [Datetime](components/datetime.md)
- [Notify](components/notify.md)
- [Siren](components/siren.md) (tones, duration and volume)
- [Valve](components/valve.md)
- [Humidifier](components/humidifier.md)
- [Lock](components/lock.md)
- [Water Heater](components/water-heater.md)
- [Device Tracker](components/device-tracker.md)
- [Light](components/light.md) (brightness, color temperature, RGB / RGBW / RGBWW, HS, XY, effects)

The components map to these [Home Assistant MQTT platforms](https://www.home-assistant.io/integrations/mqtt/): button, switch, select, number, text, climate, cover, fan, date, time, datetime, notify, siren, valve, humidifier, lock, water_heater, device_tracker, light, sensor, binary_sensor and update.

Sensors:

- [Value](components/sensors.md) (text), [State](components/sensors.md) (binary), [Gauge](components/sensors.md) (measurement), [Counter](components/sensors.md) (total increasing), [Total](components/sensors.md) and [Enum](components/sensors.md)
- [Update](components/update.md) (firmware)

## Features

- Device grouping: entities are attached to a single device
- [Availability](availability.md): global (LWT) and per component topics with payloads and templates
- Optional entity IDs (`default_entity_id`, slugified like Home Assistant), enabled-by-default settings and expiration (`expireAfter`) for sensors
- Custom discovery topics per component (`configTopic` override)
- String ownership: components take IDs, names and topics by value — safe for dynamically generated entities

Every component reference page lists the options using the same names as the [Home Assistant MQTT documentation](https://www.home-assistant.io/integrations/mqtt/).

## Installation

=== "PlatformIO"

    ```ini
    lib_deps =
      mathieucarbou/MycilaHADiscovery
    ```

=== "Arduino CLI"

    ```bash
    arduino-cli lib install MycilaHADiscovery
    ```

## Usage example in YaSolR

[YaSolR MQTT integration](https://github.com/mathieucarbou/YaSolR/blob/main/src/yasolr_mqtt.cpp)
