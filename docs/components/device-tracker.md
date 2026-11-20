# Device Tracker

The `device_tracker` platform tracks the location or presence of a device. See the [Home Assistant device tracker documentation](https://www.home-assistant.io/integrations/device_tracker.mqtt/).

```cpp
// presence through a state topic (router-style)
auto presence = std::make_unique<Mycila::HA::DeviceTracker>("presence", "Phone Presence");
presence->stateTopic = "~/phone/presence";
presence->sourceType = "router";
haDiscovery.publish(std::move(presence));

// GPS through JSON attributes
auto tracker = std::make_unique<Mycila::HA::DeviceTracker>("car", "Car Tracker");
tracker->jsonAttributesTopic = "~/tracker/attrs";
tracker->sourceType = "gps";
haDiscovery.publish(std::move(tracker));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| state_topic | `stateTopic` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| json_attributes_topic | `jsonAttributesTopic` | string | Optional | |
| json_attributes_template | `jsonAttributesTemplate` | template | Optional | |
| payload_home | `payloadHome` | string | Optional | `home` |
| payload_not_home | `payloadNotHome` | string | Optional | `not_home` |
| payload_reset | `payloadReset` | string | Optional | `None` |
| source_type | `sourceType` | string | Optional | `gps`, `router`, `bluetooth` or `bluetooth_le` |

!!! note "State or JSON attributes"
    `state_topic` payloads are `home`, `not_home` or any custom location/zone name. GPS-based trackers publish location data through `json_attributes_topic` instead — `state_topic` can only be omitted when `json_attributes_topic` is used. Receiving `payload_reset` on `state_topic` falls back to zone-derived location.
