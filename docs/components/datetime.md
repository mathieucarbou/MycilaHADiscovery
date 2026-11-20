# Date, Time and Datetime

The `date`, `time` and `datetime` platforms let users set an ISO 8601 value through the entity. The state and command payloads are:

- **Date**: `YYYY-MM-DD`
- **Time**: `HH:MM:SS`
- **Datetime**: `YYYY-MM-DDTHH:MM:SS` (optionally with timezone)

See the [Home Assistant date](https://www.home-assistant.io/integrations/date.mqtt/), [time](https://www.home-assistant.io/integrations/time.mqtt/) and [datetime](https://www.home-assistant.io/integrations/datetime.mqtt/) documentation.

```cpp
haDiscovery.publish<Mycila::HA::Date>("alarms_date", "Alarm Date", "~/alarm/date/set", "~/alarm/date");
haDiscovery.publish<Mycila::HA::Time>("alarms_time", "Alarm Time", "~/alarm/time/set", "~/alarm/time");

auto alarm = std::make_unique<Mycila::HA::Datetime>("alarm", "Alarm", "~/alarm/set", "~/alarm");
alarm->timezone = "Europe/Paris";
haDiscovery.publish(std::move(alarm));
```

## Configuration Variables

All three platforms share this set; the `timezone` option exists only on `datetime`:

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | **Required** | |
| command_template | `commandTemplate` | template | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| timezone | `timezone` (Datetime only) | string | Optional | |

!!! note
    `command_topic` and `state_topic` are both **Required** for these platforms, which is why both are constructor arguments.
