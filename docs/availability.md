# Availability

Home Assistant marks entities as available/unavailable based on availability topics. The library supports the global device-level availability and per component availability.

## Global availability (will topic)

```cpp
haDiscovery.setWillTopic("~/status");
```

When set, every published component without its own `availabilityTopic` uses the will/LWT topic as its `availability_topic`. You publish `online`/`offline` to it yourself, and a lost connection to the broker triggers the LWT `offline` message.

## Per component availability

```cpp
auto relay = std::make_unique<Mycila::HA::Outlet>("relay1", "Relay 1", "~/relays/rel1/set", "~/relays/rel1", "on", "off");
relay->availabilityTopic = "~/relays/rel1/enabled";
relay->payloadAvailable = "true";
relay->payloadNotAvailable = "false";
haDiscovery.publish(std::move(relay));
```

Options in the payload:

| Option | Field | Default |
| :-- | :-- | :-- |
| availability_topic | `availabilityTopic` | *(empty = use will topic only)* |
| payload_available | `payloadAvailable` | `online` |
| payload_not_available | `payloadNotAvailable` | `offline` |
| value_template | `availabilityTemplate` | *(none)* |

## Combining both

When a component has its own `availabilityTopic` and the device has a will topic, Home Assistant receives both topics in an `availability` list with `availability_mode: all`, so the entity is available only when **both** the device is online and the component condition is met.

Set `useDeviceAvailability = false` to exclude the global will topic from the component's availability list and use only its own topic.
