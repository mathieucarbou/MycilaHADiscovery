# Valve

The `valve` platform exposes water or gas valves, optionally with position control. See the [Home Assistant valve documentation](https://www.home-assistant.io/integrations/valve.mqtt/).

```cpp
auto valve = std::make_unique<Mycila::HA::Valve>("zone1", "Zone 1 Valve", "water");
valve->commandTopic = "~/valves/zone1/set";
valve->stateTopic = "~/valves/zone1/state";
valve->reportsPosition = true; // stateTopic also accepts position 0-100
valve->payloadStop = "STOP";
haDiscovery.publish(std::move(valve));
```

Device classes: `water`, `gas`.

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | Optional | |
| command_template | `commandTemplate` | template | Optional | |
| state_topic | `stateTopic` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| reports_position | `reportsPosition` | `std::optional<bool>` | Optional | `false` |
| position_open | `positionOpen` | `std::optional<int>` | Optional | `100` |
| position_closed | `positionClosed` | `std::optional<int>` | Optional | `0` |
| payload_open | `payloadOpen` | string | Optional | `OPEN` |
| payload_close | `payloadClose` | string | Optional | `CLOSE` |
| payload_stop | `payloadStop` | string | Optional | unset = no `valve.stop` action |
| state_open | `stateOpen` | string | Optional | `open` |
| state_opening | `stateOpening` | string | Optional | `opening` |
| state_closed | `stateClosed` | string | Optional | `closed` |
| state_closing | `stateClosing` | string | Optional | `closing` |

!!! note "Position model"
    Unlike `cover`, a valve has no separate position topics: `state_topic` accepts state payloads (`open`, `opening`, `closed`, `closing`) or, when `reports_position` is `true`, numeric position values 0–100, and `command_topic` receives the position to set.
