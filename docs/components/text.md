# Text

The `text` platform lets users input text through Home Assistant. An optional `pattern` regex validates the input. See the [Home Assistant text documentation](https://www.home-assistant.io/integrations/text.mqtt/).

```cpp
haDiscovery.publish<Mycila::HA::Text>(
    "mqtt_publish_interval",
    "MQTT Publish Interval",
    "~/config/mqtt_interval/set", // command
    "~/config/mqtt_interval",     // state
    "^\\d+$",                     // pattern: digits only
    "mdi:timer-sand",
    Mycila::HA::Category::CONFIG);
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | **Required** | |
| pattern | `pattern` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |

Both topics are required: users submit the text on `command_topic` and the device echoes it on `state_topic`.
