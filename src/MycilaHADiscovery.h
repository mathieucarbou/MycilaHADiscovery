// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2025 Mathieu Carbou
 */
#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#define MYCILA_HA_VERSION          "6.2.1"
#define MYCILA_HA_VERSION_MAJOR    6
#define MYCILA_HA_VERSION_MINOR    2
#define MYCILA_HA_VERSION_REVISION 1

#ifndef MYCILA_HA_DISCOVERY_TOPIC
  #define MYCILA_HA_DISCOVERY_TOPIC "homeassistant"
#endif

#ifndef MYCILA_HA_SENSOR_EXPIRATION_TIME
  #define MYCILA_HA_SENSOR_EXPIRATION_TIME 0
#endif

namespace Mycila {
  namespace HA {
    enum class Category { NONE,
                          CONFIG,
                          DIAGNOSTIC };

    enum class StateClass { NONE,
                            GAUGE,
                            COUNTER,
                            TOTAL };

    enum class NumberMode { AUTO,
                            BOX,
                            SLIDER };

    typedef struct {
        std::string id;
        std::string name;
        std::string version;
        std::string model;
        std::string manufacturer;
        std::string url;
    } Device;

    class Component {
      public:
        Component(const char* type, const char* id, const char* name, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : type(type), id(id), name(name), deviceClass(deviceClass), icon(icon), category(category) {}

      public:
        const char* type;
        const char* id;
        const char* name;
        const char* deviceClass;
        const char* icon;
        const Category category;
        StateClass stateClass = StateClass::NONE;
        const char* unit = nullptr;
        const char* stateTopic = nullptr;
        const char* valueTemplate = nullptr;
        const char* commandTopic = nullptr;
        const char* pattern = nullptr;
        const char* availabilityTopic = nullptr;
        const char* payloadAvailable = nullptr;
        const char* payloadNotAvailable = nullptr;
        const char* payloadOn = nullptr;
        const char* payloadOff = nullptr;
        std::vector<const char*> options;
        NumberMode mode = NumberMode::AUTO;
        float min = 0;
        float max = 100;
        float step = 1;
    };

    // https://www.home-assistant.io/integrations/button.mqtt/
    class Button : public Component {
      public:
        Button(const char* id, const char* name, const char* commandTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : Component("button", id, name, deviceClass, icon, category) { this->commandTopic = commandTopic; }
    };

    // https://www.home-assistant.io/integrations/switch.mqtt/ (switch)
    class Switch : public Component {
      public:
        Switch(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const Category category = Category::NONE) : Component("switch", id, name, "switch", icon, category) {
          this->commandTopic = commandTopic;
          this->stateTopic = stateTopic;
          this->payloadOn = payloadOn;
          this->payloadOff = payloadOff;
        }
    };

    // https://www.home-assistant.io/integrations/switch.mqtt/ (outlet)
    class Outlet : public Component {
      public:
        Outlet(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const Category category = Category::NONE) : Component("switch", id, name, "outlet", icon, category) {
          this->commandTopic = commandTopic;
          this->stateTopic = stateTopic;
          this->payloadOn = payloadOn;
          this->payloadOff = payloadOff;
        }
    };

    // https://www.home-assistant.io/integrations/select.mqtt/
    class Select : public Component {
      public:
        Select(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* icon = nullptr, const Category category = Category::NONE, std::vector<const char*> options = {}) : Component("select", id, name, nullptr, icon, category) {
          this->commandTopic = commandTopic;
          this->stateTopic = stateTopic;
          this->options = std::move(options);
        }
    };

    // https://www.home-assistant.io/integrations/text.mqtt/
    class Text : public Component {
      public:
        Text(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* pattern = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : Component("text", id, name, nullptr, icon, category) {
          this->commandTopic = commandTopic;
          this->stateTopic = stateTopic;
          this->pattern = pattern;
        }
    };

    // https://www.home-assistant.io/integrations/number.mqtt/
    class Number : public Component {
      public:
        Number(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const NumberMode mode = NumberMode::AUTO, const float min = 0, const float max = 100, const float step = 1, const char* icon = nullptr, const Category category = Category::NONE) : Component("number", id, name, nullptr, icon, category) {
          this->commandTopic = commandTopic;
          this->stateTopic = stateTopic;
          this->mode = mode;
          this->min = min;
          this->max = max;
          this->step = step;
        }
    };

    // https://www.home-assistant.io/integrations/binary_sensor.mqtt/
    class State : public Component {
      public:
        State(const char* id, const char* name, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE)
            : Component("binary_sensor", id, name, deviceClass, icon, category) {
          this->stateTopic = stateTopic;
          this->payloadOn = payloadOn;
          this->payloadOff = payloadOff;
        }
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/
    class Sensor : public Component {
      public:
        Sensor(const char* id, const char* name, const char* stateTopic, const StateClass stateClass = StateClass::NONE, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Component("sensor", id, name, deviceClass, icon, category) {
          this->unit = unit;
          this->stateTopic = stateTopic;
          this->stateClass = stateClass;
        }
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (measurement)
    class Gauge : public Sensor {
      public:
        Gauge(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(id, name, stateTopic, StateClass::GAUGE, deviceClass, icon, unit, category) {
          this->valueTemplate = valueTemplate;
        }
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (total_increasing)
    class Counter : public Sensor {
      public:
        Counter(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(id, name, stateTopic, StateClass::COUNTER, deviceClass, icon, unit, category) {
          this->valueTemplate = valueTemplate;
        }
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (total)
    class Total : public Sensor {
      public:
        Total(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Sensor(id, name, stateTopic, StateClass::TOTAL, deviceClass, icon, unit, category) {}
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (no state class)
    class Value : public Sensor {
      public:
        Value(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(id, name, stateTopic, StateClass::NONE, deviceClass, icon, nullptr, category) {
          this->valueTemplate = valueTemplate;
        }
    };

    typedef std::function<void(const char* topic, const std::string& payload)> PublisherCallback;

    // https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery
    class Discovery {
      public:
        // OPTIONAL: HA discovery topic, default to MYCILA_HA_DISCOVERY_TOPIC
        void setDiscoveryTopic(std::string discoveryTopic) { _discoveryTopic = std::move(discoveryTopic); };

        // OPTIONAL: set will topic (absolute), default to empty string, which is disabled
        void setWillTopic(std::string willTopic) { _willTopic = std::move(willTopic); };

        // OPTIONAL: sensor expiration time in seconds, 0 for no expiration, default to MYCILA_HA_SENSOR_EXPIRATION_TIME
        void setSensorExpirationTime(const uint32_t sensorExpirationTime) { _sensorExpirationTime = sensorExpirationTime; };

        // called each time before publishing components
        // will prepare the buffers
        void begin(Device device, std::string baseTopic, const PublisherCallback publisher) { begin(device, baseTopic, 1024, publisher); }
        void begin(Device device, std::string baseTopic, const size_t bufferSise, const PublisherCallback publisher);

        void publish(std::unique_ptr<Component> component);

        // called after all components are published
        // will clear the buffers
        void end();

      private:
        Device _device;
        PublisherCallback _publisher = nullptr;
        std::string _baseTopic;
        std::string _discoveryTopic = MYCILA_HA_DISCOVERY_TOPIC;
        std::string _willTopic;
        uint32_t _sensorExpirationTime = MYCILA_HA_SENSOR_EXPIRATION_TIME;
    };
  } // namespace HA
} // namespace Mycila
