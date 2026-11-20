# Sensors

The `sensor` and `binary_sensor` platforms report values. Six classes cover the different state characteristics; expiration applies to both platforms. See the [Home Assistant sensor documentation](https://www.home-assistant.io/integrations/sensor.mqtt/) and the [Home Assistant binary sensor documentation](https://www.home-assistant.io/integrations/binary_sensor.mqtt/).

## Value (text sensor)

A `sensor` without a state class — for text and non-numeric data. See the [Home Assistant sensor documentation](https://www.home-assistant.io/integrations/sensor.mqtt/).

```cpp
haDiscovery.publish<Mycila::HA::Value>(
    "hostname", "Hostname", "~/config/hostname",
    nullptr, "mdi:lan", Mycila::HA::Category::DIAGNOSTIC);
```

## State (binary sensor)

A `binary_sensor` reporting a state like `true`/`false` with configurable payloads. See the [Home Assistant binary sensor documentation](https://www.home-assistant.io/integrations/binary_sensor.mqtt/).

```cpp
haDiscovery.publish<Mycila::HA::State>(
    "grid", "Grid Electricity", "~/grid/online", "true", "false", "connectivity");
```

## Gauge (measurement)

A `sensor` with `state_class: measurement`, typically for temperature, power...
Values change both ways. See the [Home Assistant sensor documentation](https://www.home-assistant.io/integrations/sensor.mqtt/#measurement).

```cpp
haDiscovery.publish<Mycila::HA::Gauge>(
    "memory_used", "Memory Used", "~/system/heap_used",
    "data_size", "mdi:memory", "B");
```

## Counter (total increasing)

A `sensor` with `state_class: total_increasing`, for monotonic counters like energy.
See the [Home Assistant sensor documentation](https://www.home-assistant.io/integrations/sensor.mqtt/#total_increasing).

```cpp
haDiscovery.publish<Mycila::HA::Counter>(
    "uptime", "Uptime", "~/system/uptime",
    "duration", nullptr, "s");
```

## Total

A `sensor` with `state_class: total`, for totals that can decrease.
See the [Home Assistant sensor documentation](https://www.home-assistant.io/integrations/sensor.mqtt/#total).

```cpp
haDiscovery.publish<Mycila::HA::Total>(
    "energy", "Total Energy", "~/energy/total",
    "energy", nullptr, "kWh");
```

## Enum

A `sensor` with a closed set of possible values. Uses the `enum` device class.
See the [Home Assistant sensor documentation](https://www.home-assistant.io/integrations/sensor.mqtt/#enum).

```cpp
haDiscovery.publish<Mycila::HA::Enum>(
    "mode", "Mode", "~/mode/state",
    std::vector<const char*>{"auto", "manual", "eco"});
```

## Configuration Variables

| Option | Field | Type | Applies to | Required | Default |
| :-- | :-- | :-- | :--------- | :------- | :------ |
| state_topic | `stateTopic` | string | all | **Required** | |
| value_template | `valueTemplate` | template | all | Optional | |
| unit_of_measurement | `unit` | string | sensors | Optional | |
| device_class | constructor | string | all | Optional | |
| state_class | *(set by class)* | | sensors | | `measurement` / `total_increasing` / `total` |
| expire_after | `expireAfter` | `std::optional<uint32_t>` | all | Optional | unset = no expiration |
| state_class / options | `options` | `std::vector<const char*>` | Enum | **Required** | |
| payload_on / payload_off | `payloadOn` / `payloadOff` | string | State | Optional | `ON` / `OFF` |
| device_class | | string | State | Optional | e.g. `connectivity` |

## Expiration

`expireAfter` (seconds) is available on every sensor class and on `State`. When set, Home Assistant marks the entity unavailable if no update is received within the delay:

```cpp
auto temp = std::make_unique<Mycila::HA::Gauge>("temperature", "Temperature", "~/sensor/temp", "temperature");
temp->expireAfter = 300; // 5 minutes
```

Unset or `0` disables expiration.
