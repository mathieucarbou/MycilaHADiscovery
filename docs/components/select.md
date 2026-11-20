# Select

The `select` platform lets users pick a value from a list of options. See the [Home Assistant select documentation](https://www.home-assistant.io/integrations/select.mqtt/).

```cpp
haDiscovery.publish<Mycila::HA::Select>(
    "day", "Day",
    "~/config/day/set",   // command
    "~/config/day",       // state
    nullptr,              // icon
    Mycila::HA::Category::CONFIG,
    std::vector<const char*>{"mon", "tue", "wed", "thu", "fri", "sat", "sun"});
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | **Required** | |
| options | `options` | `std::vector<const char*>` | **Required** | |
| value_template | `valueTemplate` | template | Optional | |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |
