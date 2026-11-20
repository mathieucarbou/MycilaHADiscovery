# Notify

The `notify` platform lets Home Assistant push a text message to the device through a service call. See the [Home Assistant notify documentation](https://www.home-assistant.io/integrations/notify.mqtt/).

```cpp
haDiscovery.publish<Mycila::HA::Notify>("display", "Display Notifications", "~/display/notify");
```

Home Assistant publishes the notification text to the command topic; the rendered text is available in the command template as `{{ value }}`.

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | Optional | |
| command_template | `commandTemplate` | template | Optional | |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |

!!! note
    This is the complete schema: MQTT notify deliberately has no state topic, no optimistic mode and no value template. Without `command_topic` the entity cannot send messages.
