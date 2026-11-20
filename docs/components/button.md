# Button

The `button` platform lets users trigger an action on the device. See the [Home Assistant button documentation](https://www.home-assistant.io/integrations/button.mqtt/).
```cpp
haDiscovery.publish<Mycila::HA::Button>(
    "restart",
    "Restart",
    "~/system/restart",
    "restart",
    Mycila::HA::Category::DIAGNOSTIC);
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| payload_press | `payloadPress` | string | Optional | `press` |
| device_class | constructor | string | Optional | |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |
