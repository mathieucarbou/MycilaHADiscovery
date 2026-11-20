# Switch and Outlet

The `switch` platform turns a device on and off. `Outlet` emits the same platform with the `outlet` device class; `Switch` uses the `switch` device class. See the [Home Assistant switch documentation](https://www.home-assistant.io/integrations/switch.mqtt/).

## Switch

```cpp
haDiscovery.publish<Mycila::HA::Switch>(
    "output1_auto_bypass_enable",
    "Output 1 Auto Bypass",
    "~/config/switch/set",   // command
    "~/config/switch",       // state
    "true",
    "false",
    "mdi:water-boiler-auto",
    Mycila::HA::Category::CONFIG);
```

## Outlet

```cpp
auto relay = std::make_unique<Mycila::HA::Outlet>(
    "rel_commute",
    "Relay 1",
    "~/relays/rel1/state/set",
    "~/relays/rel1/state",
    "on",
    "off");
relay->availabilityTopic = "~/relays/rel1/enabled";
haDiscovery.publish(std::move(relay));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | **Required** | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| value_template | `valueTemplate` | template | Optional | |
| device_class | constructor | string | Optional | `switch` / `outlet` |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |
