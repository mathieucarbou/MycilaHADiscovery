# Siren

The `siren` platform exposes alarm sirens with optional tones, duration and volume. See the [Home Assistant siren documentation](https://www.home-assistant.io/integrations/siren.mqtt/).

```cpp
auto siren = std::make_unique<Mycila::HA::Siren>("alarm", "Alarm");
siren->commandTopic = "~/alarm/set";
siren->stateTopic = "~/alarm/state";
siren->availableTones = {"siren", "chime", "burglar"};
siren->supportDuration = false;  // no timed feature on this device
siren->supportVolumeSet = false; // fixed volume
haDiscovery.publish(std::move(siren));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | Optional | |
| state_topic | `stateTopic` | string | Optional | |
| command_template | `commandTemplate` | template | Optional | |
| command_off_template | `commandOffTemplate` | template | Optional | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| state_on | `stateOn` | string | Optional | `on` |
| state_off | `stateOff` | string | Optional | `off` |
| state_value_template | `stateValueTemplate` | template | Optional | |
| available_tones | `availableTones` | `std::vector<const char*>` | Optional | enables the tone capability |
| support_duration | `supportDuration` | `std::optional<bool>` | Optional | `true` |
| support_volume_set | `supportVolumeSet` | `std::optional<bool>` | Optional | `true` |

!!! note "Command payloads"
    With no command template, Home Assistant publishes the siren settings as JSON to the command topic, e.g. `{"state":"ON","tone":"bell","duration":10,"volume_level":0.5}`. With a command template, `state`, `tone`, `duration` and `volume_level` are available as variables.

!!! note "Capability toggles"
    `support_duration` and `support_volume_set` default to `true` in Home Assistant: set them to `false` for devices without those features.
