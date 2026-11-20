# Cover

The `cover` platform exposes shutters, blinds, garage doors... with optional position and tilt control. See the [Home Assistant cover documentation](https://www.home-assistant.io/integrations/cover.mqtt/).

```cpp
auto cover = std::make_unique<Mycila::HA::Cover>("garage", "Garage Door", "garage");
cover->commandTopic = "~/cover/set";
cover->stateTopic = "~/cover/state";
cover->payloadOpen = "OPEN";
cover->payloadClose = "CLOSE";
cover->payloadStop = "STOP";
haDiscovery.publish(std::move(cover));
```

Device classes: `awning`, `blind`, `curtain`, `damper`, `door`, `garage`, `gate`, `shade`, `shutter`.

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | Optional | |
| state_topic | `stateTopic` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| set_position_topic | `setPositionTopic` | string | Optional | |
| set_position_template | `setPositionTemplate` | template | Optional | |
| position_topic | `positionTopic` | string | Optional | |
| position_template | `positionTemplate` | template | Optional | |
| position_open | `positionOpen` | `std::optional<int>` | Optional | `100` |
| position_closed | `positionClosed` | `std::optional<int>` | Optional | `0` |
| payload_open | `payloadOpen` | string | Optional | `OPEN` |
| payload_close | `payloadClose` | string | Optional | `CLOSE` |
| payload_stop | `payloadStop` | string | Optional | `STOP` |
| state_open | `stateOpen` | string | Optional | `open` |
| state_opening | `stateOpening` | string | Optional | `opening` |
| state_closed | `stateClosed` | string | Optional | `closed` |
| state_closing | `stateClosing` | string | Optional | `closing` |
| state_stopped | `stateStopped` | string | Optional | `stopped` |
| tilt_command_topic | `tiltCommandTopic` | string | Optional | |
| tilt_command_template | `tiltCommandTemplate` | template | Optional | |
| tilt_status_topic | `tiltStatusTopic` | string | Optional | |
| tilt_status_template | `tiltStatusTemplate` | template | Optional | |
| payload_stop_tilt | `payloadStopTilt` | string | Optional | `STOP` |
| tilt_opened_value | `tiltOpenedValue` | `std::optional<int>` | Optional | `100` |
| tilt_closed_value | `tiltClosedValue` | `std::optional<int>` | Optional | `0` |
| tilt_min | `tiltMin` | `std::optional<int>` | Optional | `0` |
| tilt_max | `tiltMax` | `std::optional<int>` | Optional | `100` |

!!! note "Optimistic mode"
    Without `state_topic`, Home Assistant runs the cover in optimistic mode: the state changes immediately after each command without device confirmation.
