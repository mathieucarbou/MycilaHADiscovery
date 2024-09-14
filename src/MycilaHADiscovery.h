// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */

/* https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery
 * https://www.home-assistant.io/integrations/sensor/#device-class
 * https://www.home-assistant.io/integrations/binary_sensor/#device-class
 * https://pictogrammers.com/library/mdi/
 */
#pragma once

#include <WString.h>
#include <functional>
#include <vector>

#define MYCILA_HA_VERSION          "2.2.2"
#define MYCILA_HA_VERSION_MAJOR    2
#define MYCILA_HA_VERSION_MINOR    2
#define MYCILA_HA_VERSION_REVISION 2

#ifndef MYCILA_HA_DISCOVERY_TOPIC
  #define MYCILA_HA_DISCOVERY_TOPIC "homeassistant/discovery"
#endif

#ifndef MYCILA_HA_SENSOR_EXPIRATION_TIME
  #define MYCILA_HA_SENSOR_EXPIRATION_TIME 0
#endif

namespace Mycila {
  enum class HACategory { NONE,
                          CONFIG,
                          DIAGNOSTIC };

  enum class HAStateClass { NONE,
                            GAUGE,
                            COUNTER,
                            TOTAL };

  enum class HANumberMode { AUTO,
                            BOX,
                            SLIDER };

  typedef struct {
      String id;
      String name;
      String version;
      String model;
      String manufacturer;
      String url;
  } HADevice;

  class HAComponent {
    public:
      HAComponent(const char* type, const char* id, const char* name, const char* deviceClass = nullptr, const char* icon = nullptr, const HACategory category = HACategory::NONE) : type(type), id(id), name(name), deviceClass(deviceClass), icon(icon), category(category) {}

    public:
      const char* type;
      const char* id;
      const char* name;
      const char* deviceClass;
      const char* icon;
      const HACategory category;
      HAStateClass stateClass = HAStateClass::NONE;
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
      HANumberMode mode = HANumberMode::AUTO;
      float min = 0;
      float max = 100;
      float step = 1;
  };

  class HAButton : public HAComponent {
    public:
      HAButton(const char* id, const char* name, const char* commandTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const HACategory category = HACategory::NONE) : HAComponent("button", id, name, deviceClass, icon, category) { this->commandTopic = commandTopic; }
  };

  class HASwitch : public HAComponent {
    public:
      HASwitch(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const HACategory category = HACategory::NONE) : HAComponent("switch", id, name, "switch", icon, category) {
        this->commandTopic = commandTopic;
        this->stateTopic = stateTopic;
        this->payloadOn = payloadOn;
        this->payloadOff = payloadOff;
      }
  };

  class HASelect : public HAComponent {
    public:
      HASelect(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* icon = nullptr, const HACategory category = HACategory::NONE, const std::vector<const char*>& options = {}) : HAComponent("select", id, name, nullptr, icon, category) {
        this->commandTopic = commandTopic;
        this->stateTopic = stateTopic;
        this->options = options;
      }
  };

  class HATextField : public HAComponent {
    public:
      HATextField(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* pattern = nullptr, const char* icon = nullptr, const HACategory category = HACategory::NONE) : HAComponent("text", id, name, nullptr, icon, category) {
        this->commandTopic = commandTopic;
        this->stateTopic = stateTopic;
        this->pattern = pattern;
      }
  };

  class HANumber : public HAComponent {
    public:
      HANumber(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const HANumberMode mode = HANumberMode::AUTO, const float min = 0, const float max = 100, const float step = 1, const char* icon = nullptr, const HACategory category = HACategory::NONE) : HAComponent("number", id, name, nullptr, icon, category) {
        this->commandTopic = commandTopic;
        this->stateTopic = stateTopic;
        this->mode = mode;
        this->min = min;
        this->max = max;
        this->step = step;
      }
  };

  class HAOutlet : public HAComponent {
    public:
      HAOutlet(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const HACategory category = HACategory::NONE) : HAComponent("switch", id, name, "outlet", icon, category) {
        this->commandTopic = commandTopic;
        this->stateTopic = stateTopic;
        this->payloadOn = payloadOn;
        this->payloadOff = payloadOff;
      }
  };

  class HAState : public HAComponent {
    public:
      HAState(const char* id, const char* name, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* deviceClass = nullptr, const char* icon = nullptr, const HACategory category = HACategory::NONE)
          : HAComponent("binary_sensor", id, name, deviceClass, icon, category) {
        this->stateTopic = stateTopic;
        this->payloadOn = payloadOn;
        this->payloadOff = payloadOff;
      }
  };

  class HASensor : public HAComponent {
    public:
      HASensor(const char* id, const char* name, const char* stateTopic, const HAStateClass stateClass = HAStateClass::NONE, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const HACategory category = HACategory::NONE)
          : HAComponent("sensor", id, name, deviceClass, icon, category) {
        this->unit = unit;
        this->stateTopic = stateTopic;
        this->stateClass = stateClass;
      }
  };

  class HAGauge : public HASensor {
    public:
      HAGauge(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const HACategory category = HACategory::NONE, const char* valueTemplate = nullptr)
          : HASensor(id, name, stateTopic, HAStateClass::GAUGE, deviceClass, icon, unit, category) {
        this->valueTemplate = valueTemplate;
      }
  };

  class HACounter : public HASensor {
    public:
      HACounter(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const HACategory category = HACategory::NONE, const char* valueTemplate = nullptr)
          : HASensor(id, name, stateTopic, HAStateClass::COUNTER, deviceClass, icon, unit, category) {
        this->valueTemplate = valueTemplate;
      }
  };

  class HATotal : public HASensor {
    public:
      HATotal(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const HACategory category = HACategory::NONE)
          : HASensor(id, name, stateTopic, HAStateClass::TOTAL, deviceClass, icon, unit, category) {}
  };

  class HAValue : public HASensor {
    public:
      HAValue(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const HACategory category = HACategory::NONE)
          : HASensor(id, name, stateTopic, HAStateClass::NONE, deviceClass, icon, nullptr, category) {}
  };

  class HAText : public HASensor {
    public:
      HAText(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const HACategory category = HACategory::NONE, const char* valueTemplate = nullptr)
          : HASensor(id, name, stateTopic, HAStateClass::NONE, deviceClass, icon, nullptr, category) {
        this->valueTemplate = valueTemplate;
      }
  };

  typedef std::function<void(const char* topic, const char* payload)> PublisherCallback;

  class HADiscovery {
    public:
      // REQUIRED: set device information used to publish
      void setDevice(const HADevice& device);

      // REQUIRED: set base topic used to prepend all published component paths. This helps shorten the written code. usually this value is the base topic of your application in MQTT.
      void setBaseTopic(const char* baseTopic) { _baseTopic = baseTopic; };

      // REQUIRED: set MQTT publisher
      void setPublisher(PublisherCallback publisher) { _publisher = publisher; };

      // OPTIONAL: HA discovery topic, default to MYCILA_HA_DISCOVERY_TOPIC
      void setDiscoveryTopic(const char* discoveryTopic) { _discoveryTopic = discoveryTopic; };

      // OPTIONAL: set will topic (absolute), default to empty string, which is disabled
      void setWillTopic(const char* willTopic) { _willTopic = willTopic; };

      // OPTIONAL: sensor expiration time in seconds, 0 for no expiration, default to MYCILA_HA_SENSOR_EXPIRATION_TIME
      void setSensorExpirationTime(const uint32_t sensorExpirationTime) { _sensorExpirationTime = sensorExpirationTime; };

      // Set the re-used buffer size for JSON serialization, defaults to 1024
      void setBufferSise(const size_t bufferSise) { _bufferSise = bufferSise; };

      // called each time before publishing components
      // will prepare the buffers
      void begin();
      void publish(const HAComponent& component);
      // called after all components are published
      // will clear the buffers
      void end();

    private:
      HADevice _device;
      String _baseTopic;
      PublisherCallback _publisher = nullptr;
      String _willTopic;
      String _discoveryTopic = MYCILA_HA_DISCOVERY_TOPIC;
      uint32_t _sensorExpirationTime = MYCILA_HA_SENSOR_EXPIRATION_TIME;
      String _deviceJsonCache;
      size_t _bufferSise = 1024;
      String _buffer;
  };
} // namespace Mycila
