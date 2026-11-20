# Update

The `update` platform exposes a firmware update entity: Home Assistant shows the installed and latest versions and can trigger the installation. See the [Home Assistant update documentation](https://www.home-assistant.io/integrations/update.mqtt/).

```cpp
haDiscovery.publish<Mycila::HA::Update>(
    "firmware",
    "Device Firmware",
    "https://example.com/firmware/release.json", // release URL
    "~/system/update/state",                     // state topic (required by Home Assistant)
    "~/system/update/latest");                   // latest version topic
```

The device reports its state as JSON on `state_topic`:

```json
{
  "installed_version": "1.0.1",
  "latest_version": "1.0.2",
  "title": "Device Firmware"
}
```

Installation: set `commandTopic` and `payloadInstall`. When the user triggers the install action, Home Assistant publishes `payload_install` to `command_topic` (the `update` platform advertises the install feature when `command_topic` is configured).

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| state_topic | `stateTopic` | string | **Required** | |
| url | `url` | string | Optional | |
| latest_version_topic | `latestVersionTopic` | string | Optional | |
| command_topic | `commandTopic` | string | Optional | |
| payload_install | `payloadInstall` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| device_class | constructor | string | Optional | `firmware` |
| icon | constructor | string | Optional | |
| entity_category | constructor | `Category` | Optional | |

!!! note "state_topic is required"
    Home Assistant validates the update platform with a state-topic-required schema: a discovery payload without `state_topic` fails to create the entity. The library therefore requires it in the constructor; pass `""` to `url` and `latestVersionTopic` when they are not used.
