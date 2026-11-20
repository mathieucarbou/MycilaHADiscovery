# Humidifier

The `humidifier` platform exposes humidifiers and dehumidifiers with a target humidity and optional modes. See the [Home Assistant humidifier documentation](https://www.home-assistant.io/integrations/humidifier.mqtt/).

```cpp
auto humidifier = std::make_unique<Mycila::HA::Humidifier>(
    "bedroom", "Bedroom Humidifier", "~/hum/set", "~/hum/humidity/set", "humidifier");
humidifier->stateTopic = "~/hum/state";
humidifier->currentHumidityTopic = "~/hum/current_humidity";
humidifier->targetHumidityStateTopic = "~/hum/humidity";
humidifier->modeCommandTopic = "~/hum/mode/set";
humidifier->modeStateTopic = "~/hum/mode";
humidifier->modes = {"normal", "eco", "boost", "sleep"};
humidifier->actionTopic = "~/hum/action";
humidifier->minHumidity = 30;
humidifier->maxHumidity = 80;
haDiscovery.publish(std::move(humidifier));
```

Device classes: `humidifier` (default), `dehumidifier`.

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| target_humidity_command_topic | `targetHumidityCommandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | Optional | |
| state_value_template | `stateValueTemplate` | template | Optional | |
| command_template | `commandTemplate` | template | Optional | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| current_humidity_topic | `currentHumidityTopic` | string | Optional | |
| current_humidity_template | `currentHumidityTemplate` | template | Optional | |
| target_humidity_command_template | `targetHumidityCommandTemplate` | template | Optional | |
| target_humidity_state_topic | `targetHumidityStateTopic` | string | Optional | |
| target_humidity_state_template | `targetHumidityStateTemplate` | template | Optional | |
| payload_reset_humidity | `payloadResetHumidity` | string | Optional | `None` |
| min_humidity | `minHumidity` | `std::optional<float>` | Optional | `0` |
| max_humidity | `maxHumidity` | `std::optional<float>` | Optional | `100` |
| mode_command_topic | `modeCommandTopic` | string | Optional | |
| mode_command_template | `modeCommandTemplate` | template | Optional | |
| mode_state_topic | `modeStateTopic` | string | Optional | |
| mode_state_template | `modeStateTemplate` | template | Optional | |
| payload_reset_mode | `payloadResetMode` | string | Optional | `None` |
| modes | `modes` | `std::vector<const char*>` | Optional | |
| action_topic | `actionTopic` | string | Optional | |
| action_template | `actionTemplate` | template | Optional | |

Valid action payloads: `humidifying`, `drying`, `idle`, `off`.
