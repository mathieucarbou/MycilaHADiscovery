# Number

The `number` platform lets users set a numeric value through a slider, a box, or the `auto` mode selected by Home Assistant. See the [Home Assistant number documentation](https://www.home-assistant.io/integrations/number.mqtt/).

```cpp
auto powerThreshold = std::make_unique<Mycila::HA::Number>(
    "relay1_power_threshold",
    "Relay 1 Power Threshold",
    "~/relays/rel1/power/set",
    "~/relays/rel1/power");
powerThreshold->mode = Mycila::HA::NumberMode::SLIDER;
powerThreshold->min = 0;
powerThreshold->max = 3000;
powerThreshold->step = 50;
powerThreshold->icon = "mdi:flash";
powerThreshold->category = Mycila::HA::Category::CONFIG;
haDiscovery.publish(std::move(powerThreshold));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | **Required** | |
| min | `min` | `std::optional<float>` | Optional | `0` (Home Assistant default) |
| max | `max` | `std::optional<float>` | Optional | `100` (Home Assistant default) |
| step | `step` | float | Optional | `1` |
| mode | `mode` | `NumberMode::AUTO` / `BOX` / `SLIDER` | Optional | `auto` |
| unit_of_measurement | `unit` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |

`min` and `max` are `std::optional<float>`: when unset, they are not emitted and Home Assistant applies its own defaults. Assign `std::nullopt` or `reset()` to unset them.
