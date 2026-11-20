# Water Heater

The `water_heater` platform exposes a water heater / boiler with operation modes and a target temperature. See the [Home Assistant water heater documentation](https://www.home-assistant.io/integrations/water_heater.mqtt/).

```cpp
auto heater = std::make_unique<Mycila::HA::WaterHeater>("boiler", "Water Boiler");
heater->currentTemperatureTopic = "~/boiler/current_temp";
heater->temperatureCommandTopic = "~/boiler/temp/set";
heater->temperatureStateTopic = "~/boiler/temp";
heater->modeCommandTopic = "~/boiler/mode/set";
heater->modeStateTopic = "~/boiler/mode";
heater->modes = {"off", "eco", "heat_pump"};
heater->temperatureUnit = "C";
heater->powerCommandTopic = "~/boiler/power/set";
heater->minTemp = 40;
heater->maxTemp = 75;
heater->precision = 0.5f;
heater->initial = 60;
haDiscovery.publish(std::move(heater));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| current_temperature_topic | `currentTemperatureTopic` | string | Optional | |
| current_temperature_template | `currentTemperatureTemplate` | template | Optional | |
| command_topic | `commandTopic` | string | Optional | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| precision | `precision` | `std::optional<float>` | Optional | `0.1` (C) / `1.0` (F); allowed: `0.1`, `0.5`, `1.0` |
| power_command_topic | `powerCommandTopic` | string | Optional | |
| power_command_template | `powerCommandTemplate` | template | Optional | |
| temperature_command_topic | `temperatureCommandTopic` | string | Optional | |
| temperature_command_template | `temperatureCommandTemplate` | template | Optional | |
| temperature_state_topic | `temperatureStateTopic` | string | Optional | |
| temperature_state_template | `temperatureStateTemplate` | template | Optional | |
| temperature_unit | `temperatureUnit` | string | Optional | system unit; `C` or `F` |
| min_temp | `minTemp` | `std::optional<float>` | Optional | Home Assistant default |
| max_temp | `maxTemp` | `std::optional<float>` | Optional | Home Assistant default |
| mode_command_topic | `modeCommandTopic` | string | Optional | |
| mode_command_template | `modeCommandTemplate` | template | Optional | |
| mode_state_topic | `modeStateTopic` | string | Optional | |
| mode_state_template | `modeStateTemplate` | template | Optional | |
| modes | `modes` | `std::vector<const char*>` | Optional | subset of `off`, `eco`, `electric`, `gas`, `heat_pump`, `high_demand`, `performance` |
| initial | `initial` | `std::optional<float>` | Optional | target temperature on startup |

All topics are Optional on this platform; without `temperature_command_topic` the heater works in temperature-optimistic mode.
