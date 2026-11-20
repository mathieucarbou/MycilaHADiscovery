# Dynamic Entities

Components own their strings, which makes dynamically created entities safe. IDs, names and topics can be generated at runtime and moved into a component without keeping any backing storage alive:

```cpp
// The component owns these generated strings after this function returns.
std::unique_ptr<Mycila::HA::Gauge> makeTemperatureSensor(unsigned channel, std::string name) {
  std::string id = "temperature_" + std::to_string(channel);
  std::string stateTopic = "~/sensors/" + id + "/temperature";

  return std::make_unique<Mycila::HA::Gauge>(
      std::move(id), std::move(name), std::move(stateTopic),
      "temperature", nullptr, "°C");
}
```

The caller can publish the returned component after the helper finishes: the component owns its strings, and there is no separate buffer to keep alive. Constructors accept strings by value and move them into members.

Ownership increases per-component storage and may introduce allocations. Templates, payloads and constant metadata remain borrowed, so they can live in flash when assigned from string literals.

## Renaming while preserving identity

Republishing a component with a new name but the same `id` updates the entity (the discovery topic and unique ID stay identical):

```cpp
haDiscovery.publish(makeTemperatureSensor(1, "Living room temperature"));
```

## Removing

Removal only needs the identity — the component type and its `id`:

```cpp
haDiscovery.unpublish<Mycila::HA::Gauge>("temperature_1", "");
```

The [DynamicEntities example](https://github.com/mathieucarbou/MycilaHADiscovery/blob/main/examples/DynamicEntities/DynamicEntities.ino) demonstrates creation from generated strings, renaming with preserved identity and removal in a loop.
