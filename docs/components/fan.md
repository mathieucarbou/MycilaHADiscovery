# Fan

The `fan` platform exposes a fan with optional speed percentage, oscillation, direction and preset modes. See the [Home Assistant fan documentation](https://www.home-assistant.io/integrations/fan.mqtt/).

```cpp
auto fan = std::make_unique<Mycila::HA::Fan>("ceiling", "Ceiling Fan");
fan->stateTopic = "~/fan/state";
fan->commandTopic = "~/fan/set";
fan->percentageCommandTopic = "~/fan/speed/set";
fan->percentageStateTopic = "~/fan/speed";
fan->oscillationCommandTopic = "~/fan/osc/set";
fan->presetModeCommandTopic = "~/fan/preset/set";
fan->presetModeStateTopic = "~/fan/preset";
fan->presetModes = {"low", "medium", "high"};
haDiscovery.publish(std::move(fan));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| state_topic | `stateTopic` | string | Optional | |
| command_topic | `commandTopic` | string | Optional | |
| command_template | `commandTemplate` | template | Optional | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| state_value_template | `stateValueTemplate` | template | Optional | |
| percentage_command_topic | `percentageCommandTopic` | string | Optional | |
| percentage_command_template | `percentageCommandTemplate` | template | Optional | |
| percentage_state_topic | `percentageStateTopic` | string | Optional | |
| percentage_value_template | `percentageValueTemplate` | template | Optional | |
| payload_reset_percentage | `payloadResetPercentage` | string | Optional | `None` |
| speed_range_min | `speedRangeMin` | `std::optional<int>` | Optional | `1` |
| speed_range_max | `speedRangeMax` | `std::optional<int>` | Optional | `100` |
| oscillation_command_topic | `oscillationCommandTopic` | string | Optional | |
| oscillation_command_template | `oscillationCommandTemplate` | template | Optional | |
| oscillation_state_topic | `oscillationStateTopic` | string | Optional | |
| oscillation_value_template | `oscillationValueTemplate` | template | Optional | |
| payload_oscillation_on | `payloadOscillationOn` | string | Optional | `oscillate_on` |
| payload_oscillation_off | `payloadOscillationOff` | string | Optional | `oscillate_off` |
| direction_command_topic | `directionCommandTopic` | string | Optional | |
| direction_command_template | `directionCommandTemplate` | template | Optional | |
| direction_state_topic | `directionStateTopic` | string | Optional | |
| direction_value_template | `directionValueTemplate` | template | Optional | |
| preset_mode_command_topic | `presetModeCommandTopic` | string | Optional | |
| preset_mode_command_template | `presetModeCommandTemplate` | template | Optional | |
| preset_mode_state_topic | `presetModeStateTopic` | string | Optional | |
| preset_mode_value_template | `presetModeValueTemplate` | template | Optional | |
| payload_reset_preset_mode | `payloadResetPresetMode` | string | Optional | `None` |
| preset_modes | `presetModes` | `std::vector<const char*>` | Optional | |

!!! note "Preset modes"
    Home Assistant requires `preset_mode_command_topic` and `preset_modes` to be configured together, with `preset_mode_state_topic` optional for non-optimistic updates.

!!! note "Speed range"
    `speed_range_min` defaults to **1**: the percentage scale 0–100 maps onto the device speed range.
