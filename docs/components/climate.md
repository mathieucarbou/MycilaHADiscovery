# Climate

The `climate` platform exposes a thermostat/HVAC device. The class is configured with fields after construction since thermostats combine many optional topics. See the [Home Assistant climate documentation](https://www.home-assistant.io/integrations/climate.mqtt/).

```cpp
auto climate = std::make_unique<Mycila::HA::Climate>(
    "thermostat", "Thermostat", "mdi:radiator");
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
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| current_temperature_topic | `currentTemperatureTopic` | string | Optional | |
| current_temperature_template | `currentTemperatureTemplate` | template | Optional | |
| temperature_command_topic | `temperatureCommandTopic` | string | Optional | |
| temperature_command_template | `temperatureCommandTemplate` | template | Optional | |
| temperature_state_topic | `temperatureStateTopic` | string | Optional | |
| temperature_state_template | `temperatureStateTemplate` | template | Optional | |
| min_temp | `minTemp` | `std::optional<float>` | Optional | Home Assistant default |
| max_temp | `maxTemp` | `std::optional<float>` | Optional | Home Assistant default |
| temp_step | `tempStep` | `std::optional<float>` | Optional | `1` |
| temp_unit | `temperatureUnit` | string | Optional | system unit; `C` or `F` |
| mode_command_topic | `modeCommandTopic` | string | Optional | |
| mode_command_template | `modeCommandTemplate` | template | Optional | |
| mode_state_topic | `modeStateTopic` | string | Optional | |
| mode_state_template | `modeStateTemplate` | template | Optional | |
| modes | `modes` | `std::vector<const char*>` | Optional | subset of `auto`/`off`/`cool`/`heat`/`dry`/`fan_only` |
| action_topic | `actionTopic` | string | Optional | |
| action_template | `actionTemplate` | template | Optional | |
| preset_mode_command_topic | `presetModeCommandTopic` | string | Optional | |
| preset_mode_command_template | `presetModeCommandTemplate` | template | Optional | |
| preset_mode_state_topic | `presetModeStateTopic` | string | Optional | |
| preset_mode_value_template | `presetModeValueTemplate` | template | Optional | |
| preset_modes | `presetModes` | `std::vector<const char*>` | Optional | |
| power_command_topic | `powerCommandTopic` | string | Optional | |
| power_command_template | `powerCommandTemplate` | template | Optional | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| value_template | `valueTemplate` | template | Optional | |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |

`power_command_topic` enables the `climate.turn_on` and `climate.turn_off` actions; `payload_on`/`payload_off` are its payloads (default `ON`/`OFF`).

A `None` payload received on `action_topic` resets the current action. Valid action values: `off`, `heating`, `cooling`, `drying`, `idle`, `fan`.
