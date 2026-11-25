# Basic Usage

## Setup

```cpp
#include <MycilaHADiscovery.h>

Mycila::HA::Discovery haDiscovery(
  {
    .id = "my-app-1234",
    .name = "My Application Name",
    .version = "1.0.1",
    .model = "OSS",
    .manufacturer = "Mathieu Carbou",
    .url = "http://192.168.1.12",
  },
  "/my-app", // base topic
  [](const char* topic, const std::string& payload) -> Mycila::HA::err_t {
    // here, call your mqttClient.publish() code with retain=true,
    // and return an error code: Mycila::HA::Error::SUCCESS when the transport
    // accepted the message, or any code >= Mycila::HA::Error::ERR_MQTT_PUBLISHER on failure
    return mqttClient.publish(topic, payload.c_str(), true)
             ? Mycila::HA::Error::SUCCESS
             : Mycila::HA::err_t(32); // custom transport error
  });

void setup() {
  haDiscovery.setWillTopic("~/status");
}
```

The `~` in component topics is replaced by Home Assistant with the base topic.

`setDiscoveryTopic()` (default `homeassistant`) and `setWillTopic()` configure where configurations and the last will are published.

## Publishing Components

`publish()` and `unpublish()` accept two forms:

1. **Template parameters** — construct and publish in one call, when no optional property is needed:

   ```cpp
   haDiscovery.publish<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart", Mycila::HA::Category::DIAGNOSTIC);
   ```

2. **A `std::unique_ptr<Component>`** — when optional properties (`category`, `unit`, `icon`, bounds, availability...) need to be configured:

   ```cpp
   auto relay = std::make_unique<Mycila::HA::Outlet>("relay1", "Relay 1", "~/relays/rel1/set", "~/relays/rel1", "on", "off");
   relay->availabilityTopic = "~/relays/rel1/enabled";
   relay->payloadAvailable = "true";
   relay->payloadNotAvailable = "false";
   haDiscovery.publish(std::move(relay));
   ```

The component is allocated on the heap, serialized, then automatically freed during `publish()`. Publication is synchronous: strings owned by the component can be freed once `publish()` returns.

`publish()` and `unpublish()` return a `Mycila::HA::err_t` code: `Error::SUCCESS` on success, a library code when the config cannot be prepared, or the value returned by your publisher callback. See the [API reference](api.md) for the error list. Codes from `Error::ERR_MQTT_PUBLISHER` (32) on are free for applications: return them from your callback to propagate transport-specific errors.

```cpp
// remove an update from the pending queue only on success;
// on failure, decide how to retry based on the code
Mycila::HA::err_t err = haDiscovery.publish<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart");
if (err != Mycila::HA::Error::SUCCESS) {
  // err == Mycila::HA::Error::NO_PUBLISHER, OUT_OF_MEMORY, or a custom transport code...
}
```

The component is consumed and freed even when publication fails. To retry, construct a new component. The library does not retry automatically.

## Removing Components

`unpublish()` publishes an empty payload to the component discovery topic, which removes the entity from Home Assistant. Only the identity is needed:

```cpp
haDiscovery.unpublish<Mycila::HA::Value>("firmware_filename");
```

!!! warning "configTopic overrides must match on unpublish"
    When `component->configTopic` is set, `publish()` and `unpublish()` use it verbatim as the full discovery topic instead of `<discovery_topic>/<type>/<device_id>/<id>/config`. The override must be **exactly** the same for `publish()` and `unpublish()`, otherwise the retained configuration stays in the broker and Home Assistant keeps the entity.

## String Ownership

Component constructors take `std::string` arguments by value and move them into the component; string literals convert implicitly. `nullptr` is not accepted where a `std::string` is expected: use an empty string for omitted values, and use `clear()` instead of assigning `nullptr` to an owned field.

Templates, payloads and constant metadata (`icon`, `deviceClass`, `payloadOn`, ...) remain borrowed `const char*` and live in flash when assigned from string literals.

## Example

The [DynamicEntities example](https://github.com/mathieucarbou/MycilaHADiscovery/blob/main/examples/DynamicEntities/DynamicEntities.ino) creates components from generated IDs, names and topics, renames an entity while preserving its identity, and removes it.
