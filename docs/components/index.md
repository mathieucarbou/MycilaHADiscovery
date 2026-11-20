# Components

Each component maps to one Home Assistant MQTT platform. Configuration options below use the full option names from the [Home Assistant MQTT documentation](https://www.home-assistant.io/integrations/mqtt/); the library serializes them as the abbreviated keys Home Assistant expects (`~` is replaced by the base topic at setup).

Common options available on every component (settable as fields):

| Option | Field | Description |
| :-- | :-- | :-- |
| name | `name` | Constructor argument; pass `""` to inherit the device name |
| device_class | `deviceClass` | Constructor argument on supporting platforms |
| icon | `icon` | Constructor argument or field |
| entity_category | `category` | `Category::CONFIG` or `Category::DIAGNOSTIC` |
| default_entity_id | *(auto)* | Auto-suggested as `<platform>.<device_id>_<id>` unless overridden |
| enabled_by_default | `enabledByDefault` | `std::optional<bool>` |
| unique_id | `uniqueId` | Auto-suggested as `<device_id>_<id>` unless overridden |
| availability_topic | `availabilityTopic` | See [Availability](../availability.md) |
| discovery topic | `configTopic` | Full discovery topic override, including `/config` |

The list of supported platforms and their YAML/discovery capabilities is in the [Home Assistant MQTT documentation](https://www.home-assistant.io/integrations/mqtt/).

Component pages:

- [Button](button.md)
- [Switch and Outlet](switch.md)
- [Select](select.md)
- [Text](text.md)
- [Number](number.md)
- [Climate](climate.md)
- [Cover](cover.md)
- [Fan](fan.md)
- [Date / Time / Datetime](datetime.md)
- [Notify](notify.md)
- [Siren](siren.md)
- [Valve](valve.md)
- [Humidifier](humidifier.md)
- [Lock](lock.md)
- [Water Heater](water-heater.md)
- [Device Tracker](device-tracker.md)
- [Light](light.md)
- [Sensors](sensors.md) (Value, State, Gauge, Counter, Total, Enum)
- [Update](update.md)
