// SPDX-License-Identifier: MIT
/*
 * Copyright (C) Mathieu Carbou
 */
#pragma once

#include <ArduinoJson.h>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define MYCILA_HA_VERSION          "7.2.0"
#define MYCILA_HA_VERSION_MAJOR    7
#define MYCILA_HA_VERSION_MINOR    2
#define MYCILA_HA_VERSION_REVISION 0

#ifndef MYCILA_HA_DISCOVERY_TOPIC
  #define MYCILA_HA_DISCOVERY_TOPIC "homeassistant"
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
        Component(const char* type, std::string id, std::string name, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : type(type), id(std::move(id)), name(std::move(name)), deviceClass(deviceClass), icon(icon), category(category) {}
        virtual ~Component() = default;

        virtual void toJson(const JsonObject& root) {
          root["name"] = name.empty() ? nullptr : name.c_str();
          if (!defaultEntityId.empty())
            root["def_ent_id"] = defaultEntityId;
          if (enabledByDefault.has_value())
            root["en"] = enabledByDefault.value();
          if (deviceClass)
            root["dev_cla"] = deviceClass;
          if (icon)
            root["ic"] = icon;
          if (category == Category::CONFIG)
            root["ent_cat"] = "config";
          if (category == Category::DIAGNOSTIC)
            root["ent_cat"] = "diagnostic";
        }

      public:
        const char* type;
        std::string id;
        std::string name;
        const char* deviceClass;
        const char* icon;
        Category category;
        std::string configTopic; // Full discovery topic override, including /config.
        std::string uniqueId;
        std::string defaultEntityId;
        // Unset values preserve the discovery and Home Assistant defaults.
        std::optional<bool> enabledByDefault;
        bool useDeviceAvailability = true;
        std::string availabilityTopic;
        const char* availabilityTemplate = nullptr;
        const char* payloadAvailable = nullptr;
        const char* payloadNotAvailable = nullptr;
    };

    // https://www.home-assistant.io/integrations/button.mqtt/
    class Button : public Component {
      public:
        Button(std::string id, std::string name) : Button(std::move(id), std::move(name), {}) {}
        Button(std::string id, std::string name, std::string commandTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : Component("button", std::move(id), std::move(name), deviceClass, icon, category) { this->commandTopic = std::move(commandTopic); }
        virtual ~Button() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (payloadPress)
            root["pl_prs"] = payloadPress;
        }

        std::string commandTopic;
        const char* payloadPress = nullptr;
    };

    // https://www.home-assistant.io/integrations/switch.mqtt/ (switch)
    class Switch : public Component {
      public:
        Switch(std::string id, std::string name) : Switch(std::move(id), std::move(name), {}, {}, nullptr, nullptr) {}
        Switch(std::string id, std::string name, std::string commandTopic, std::string stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const Category category = Category::NONE) : Component("switch", std::move(id), std::move(name), "switch", icon, category) {
          this->commandTopic = std::move(commandTopic);
          this->stateTopic = std::move(stateTopic);
          this->payloadOn = payloadOn;
          this->payloadOff = payloadOff;
        }
        virtual ~Switch() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (payloadOn)
            root["pl_on"] = payloadOn;
          if (payloadOff)
            root["pl_off"] = payloadOff;
        }

        std::string commandTopic;
        std::string stateTopic;
        const char* valueTemplate = nullptr;
        const char* payloadOn = nullptr;
        const char* payloadOff = nullptr;
    };

    // https://www.home-assistant.io/integrations/switch.mqtt/ (outlet)
    class Outlet : public Component {
      public:
        Outlet(std::string id, std::string name, std::string commandTopic, std::string stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const Category category = Category::NONE) : Component("switch", std::move(id), std::move(name), "outlet", icon, category) {
          this->commandTopic = std::move(commandTopic);
          this->stateTopic = std::move(stateTopic);
          this->payloadOn = payloadOn;
          this->payloadOff = payloadOff;
        }
        virtual ~Outlet() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (payloadOn)
            root["pl_on"] = payloadOn;
          if (payloadOff)
            root["pl_off"] = payloadOff;
        }

        std::string commandTopic;
        std::string stateTopic;
        const char* valueTemplate = nullptr;
        const char* payloadOn = nullptr;
        const char* payloadOff = nullptr;
    };

    // https://www.home-assistant.io/integrations/select.mqtt/
    class Select : public Component {
      public:
        Select(std::string id, std::string name, std::string commandTopic, std::string stateTopic, const char* icon = nullptr, const Category category = Category::NONE, std::vector<const char*> options = {}) : Component("select", std::move(id), std::move(name), nullptr, icon, category) {
          this->commandTopic = std::move(commandTopic);
          this->stateTopic = std::move(stateTopic);
          this->options = std::move(options);
        }
        virtual ~Select() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          JsonArray opts = root["options"].to<JsonArray>();
          for (auto& opt : options)
            opts.add(opt);
        }

        std::string commandTopic;
        std::string stateTopic;
        const char* valueTemplate = nullptr;
        std::vector<const char*> options;
    };

    // https://www.home-assistant.io/integrations/text.mqtt/
    class Text : public Component {
      public:
        Text(std::string id, std::string name, std::string commandTopic, std::string stateTopic, const char* pattern = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : Component("text", std::move(id), std::move(name), nullptr, icon, category) {
          this->commandTopic = std::move(commandTopic);
          this->stateTopic = std::move(stateTopic);
          this->pattern = pattern;
        }
        virtual ~Text() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (pattern)
            root["ptrn"] = pattern;
        }

        std::string commandTopic;
        std::string stateTopic;
        const char* valueTemplate = nullptr;
        const char* pattern = nullptr;
    };

    // https://www.home-assistant.io/integrations/number.mqtt/
    class Number : public Component {
      public:
        Number(std::string id, std::string name) : Number(std::move(id), std::move(name), {}, {}) {}
        Number(std::string id, std::string name, std::string commandTopic, std::string stateTopic, const NumberMode mode = NumberMode::AUTO, const std::optional<float> min = std::nullopt, const std::optional<float> max = std::nullopt, const float step = 1, const char* icon = nullptr, const Category category = Category::NONE) : Component("number", std::move(id), std::move(name), nullptr, icon, category) {
          this->commandTopic = std::move(commandTopic);
          this->stateTopic = std::move(stateTopic);
          this->mode = mode;
          this->min = min;
          this->max = max;
          this->step = step;
        }
        virtual ~Number() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (unit)
            root["unit_of_meas"] = unit;
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          root["mode"] = mode == NumberMode::BOX ? "box" : (mode == NumberMode::SLIDER ? "slider" : "auto");
          if (min.has_value())
            root["min"] = min.value();
          if (max.has_value())
            root["max"] = max.value();
          root["step"] = step;
        }

        std::string commandTopic;
        std::string stateTopic;
        const char* unit = nullptr;
        const char* valueTemplate = nullptr;
        NumberMode mode = NumberMode::AUTO; // always emitted; NumberMode::AUTO is the Home Assistant default
        std::optional<float> min; // unset preserves the Home Assistant default (0)
        std::optional<float> max; // unset preserves the Home Assistant default (100)
        float step = 1; // always emitted; 1 is the Home Assistant default
    };

    // https://www.home-assistant.io/integrations/climate.mqtt/
    class Climate : public Component {
      public:
        Climate(std::string id, std::string name, const char* icon = nullptr, const Category category = Category::NONE) : Component("climate", std::move(id), std::move(name), nullptr, icon, category) {}
        virtual ~Climate() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (!powerCommandTopic.empty())
            root["pow_cmd_t"] = powerCommandTopic;
          if (powerCommandTemplate)
            root["pow_cmd_tpl"] = powerCommandTemplate;
          if (payloadOn)
            root["pl_on"] = payloadOn;
          if (payloadOff)
            root["pl_off"] = payloadOff;
          if (!currentTemperatureTopic.empty())
            root["curr_temp_t"] = currentTemperatureTopic;
          if (currentTemperatureTemplate)
            root["curr_temp_tpl"] = currentTemperatureTemplate;
          if (!temperatureCommandTopic.empty())
            root["temp_cmd_t"] = temperatureCommandTopic;
          if (temperatureCommandTemplate)
            root["temp_cmd_tpl"] = temperatureCommandTemplate;
          if (!temperatureStateTopic.empty())
            root["temp_stat_t"] = temperatureStateTopic;
          if (temperatureStateTemplate)
            root["temp_stat_tpl"] = temperatureStateTemplate;
          if (!modeCommandTopic.empty())
            root["mode_cmd_t"] = modeCommandTopic;
          if (modeCommandTemplate)
            root["mode_cmd_tpl"] = modeCommandTemplate;
          if (!modeStateTopic.empty())
            root["mode_stat_t"] = modeStateTopic;
          if (modeStateTemplate)
            root["mode_stat_tpl"] = modeStateTemplate;
          if (!modes.empty()) {
            JsonArray opts = root["modes"].to<JsonArray>();
            for (auto& opt : modes)
              opts.add(opt);
          }
          if (!actionTopic.empty())
            root["act_t"] = actionTopic;
          if (actionTemplate)
            root["act_tpl"] = actionTemplate;
          if (!presetModeCommandTopic.empty())
            root["pr_mode_cmd_t"] = presetModeCommandTopic;
          if (presetModeCommandTemplate)
            root["pr_mode_cmd_tpl"] = presetModeCommandTemplate;
          if (!presetModeStateTopic.empty())
            root["pr_mode_stat_t"] = presetModeStateTopic;
          if (presetModeValueTemplate)
            root["pr_mode_val_tpl"] = presetModeValueTemplate;
          if (!presetModes.empty()) {
            JsonArray opts = root["pr_modes"].to<JsonArray>();
            for (auto& opt : presetModes)
              opts.add(opt);
          }
          if (temperatureUnit)
            root["temp_unit"] = temperatureUnit;
          if (minTemp.has_value())
            root["min_temp"] = minTemp.value();
          if (maxTemp.has_value())
            root["max_temp"] = maxTemp.value();
          if (tempStep.has_value())
            root["temp_step"] = tempStep.value();
        }

        const char* valueTemplate = nullptr;
        std::string powerCommandTopic; // enables climate.turn_on / climate.turn_off
        const char* powerCommandTemplate = nullptr;
        const char* payloadOn = nullptr; // payloads for powerCommandTopic, used instead of the ON/OFF defaults
        const char* payloadOff = nullptr;
        std::string currentTemperatureTopic;
        const char* currentTemperatureTemplate = nullptr;
        std::string temperatureCommandTopic;
        const char* temperatureCommandTemplate = nullptr;
        std::string temperatureStateTopic;
        const char* temperatureStateTemplate = nullptr;
        std::string modeCommandTopic;
        const char* modeCommandTemplate = nullptr;
        std::string modeStateTopic;
        const char* modeStateTemplate = nullptr;
        std::vector<const char*> modes;
        std::string actionTopic;
        const char* actionTemplate = nullptr;
        std::string presetModeCommandTopic;
        const char* presetModeCommandTemplate = nullptr;
        std::string presetModeStateTopic;
        const char* presetModeValueTemplate = nullptr;
        std::vector<const char*> presetModes;
        const char* temperatureUnit = nullptr;
        std::optional<float> minTemp;
        std::optional<float> maxTemp;
        std::optional<float> tempStep;
    };

    // https://www.home-assistant.io/integrations/binary_sensor.mqtt/
    class State : public Component {
      public:
        State(std::string id, std::string name) : State(std::move(id), std::move(name), {}, nullptr, nullptr) {}
        State(std::string id, std::string name, std::string stateTopic, const char* payloadOn, const char* payloadOff, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE)
            : Component("binary_sensor", std::move(id), std::move(name), deviceClass, icon, category) {
          this->stateTopic = std::move(stateTopic);
          this->payloadOn = payloadOn;
          this->payloadOff = payloadOff;
        }
        virtual ~State() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (payloadOn)
            root["pl_on"] = payloadOn;
          if (payloadOff)
            root["pl_off"] = payloadOff;
          if (expireAfter.value_or(0) > 0)
            root["exp_aft"] = expireAfter.value();
        }

        std::string stateTopic;
        const char* valueTemplate = nullptr;
        const char* payloadOn = nullptr;
        const char* payloadOff = nullptr;
        std::optional<uint32_t> expireAfter; // expire_after in seconds; unset or 0 disables expiration
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/
    class Sensor : public Component {
      public:
        Sensor(std::string id, std::string name, std::string stateTopic, const StateClass stateClass = StateClass::NONE, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Component("sensor", std::move(id), std::move(name), deviceClass, icon, category) {
          this->unit = unit;
          this->stateTopic = std::move(stateTopic);
          this->stateClass = stateClass;
        }
        virtual ~Sensor() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (stateClass == StateClass::COUNTER)
            root["stat_cla"] = "total_increasing";
          if (stateClass == StateClass::GAUGE)
            root["stat_cla"] = "measurement";
          if (stateClass == StateClass::TOTAL)
            root["stat_cla"] = "total";
          if (unit)
            root["unit_of_meas"] = unit;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (expireAfter.value_or(0) > 0)
            root["exp_aft"] = expireAfter.value();
        }

        const char* unit = nullptr;
        std::string stateTopic;
        StateClass stateClass = StateClass::NONE;
        const char* valueTemplate = nullptr;
        std::optional<uint32_t> expireAfter; // expire_after in seconds; unset or 0 disables expiration
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (measurement)
    class Gauge : public Sensor {
      public:
        Gauge(std::string id, std::string name) : Gauge(std::move(id), std::move(name), {}) {}
        Gauge(std::string id, std::string name, std::string stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::GAUGE, deviceClass, icon, unit, category) {
          this->valueTemplate = valueTemplate;
        }
        virtual ~Gauge() = default;
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (total_increasing)
    class Counter : public Sensor {
      public:
        Counter(std::string id, std::string name) : Counter(std::move(id), std::move(name), {}) {}
        Counter(std::string id, std::string name, std::string stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::COUNTER, deviceClass, icon, unit, category) {
          this->valueTemplate = valueTemplate;
        }
        virtual ~Counter() = default;
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (total)
    class Total : public Sensor {
      public:
        Total(std::string id, std::string name, std::string stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::TOTAL, deviceClass, icon, unit, category) {}
        virtual ~Total() = default;
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (no state class)
    class Value : public Sensor {
      public:
        Value(std::string id, std::string name) : Value(std::move(id), std::move(name), {}) {}
        Value(std::string id, std::string name, std::string stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::NONE, deviceClass, icon, nullptr, category) {
          this->valueTemplate = valueTemplate;
        }
        virtual ~Value() = default;
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (enum: closed set of possible values)
    class Enum : public Sensor {
      public:
        Enum(std::string id, std::string name, std::string stateTopic, std::vector<const char*> options, const char* icon = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::NONE, "enum", icon, nullptr, category) {
          this->valueTemplate = valueTemplate;
          this->options = std::move(options);
        }
        virtual ~Enum() = default;

        void toJson(const JsonObject& root) override {
          Sensor::toJson(root);
          JsonArray opts = root["options"].to<JsonArray>();
          for (auto& opt : options)
            opts.add(opt);
        }

        std::vector<const char*> options;
    };

    // https://www.home-assistant.io/integrations/update.mqtt/ (no state class)
    class Update : public Component {
      public:
        Update(std::string id, std::string name, std::string url = {}, std::string stateTopic = {}, std::string latestVersionTopic = {}, const char* deviceClass = "firmware", const char* icon = nullptr, const Category category = Category::NONE)
            : Component("update", std::move(id), std::move(name), deviceClass, icon, category) {
          this->stateTopic = std::move(stateTopic);
          this->url = std::move(url);
          this->latestVersionTopic = std::move(latestVersionTopic);
        }
        virtual ~Update() = default;

        void toJson(const JsonObject& root) override {
          Component::toJson(root);
          if (!commandTopic.empty())
            root["cmd_t"] = commandTopic;
          if (!stateTopic.empty())
            root["stat_t"] = stateTopic;
          if (valueTemplate)
            root["val_tpl"] = valueTemplate;
          if (!url.empty())
            root["rel_u"] = url;
          if (!latestVersionTopic.empty())
            root["l_ver_t"] = latestVersionTopic;
        }

        std::string commandTopic;
        std::string stateTopic;
        const char* valueTemplate = nullptr;
        std::string url;
        std::string latestVersionTopic;
    };

    typedef std::function<void(const char* topic, const std::string& payload)> PublisherCallback;

    // https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery
    class Discovery {
      public:
        // OPTIONAL: HA discovery topic, default to MYCILA_HA_DISCOVERY_TOPIC
        void setDiscoveryTopic(std::string discoveryTopic) { _discoveryTopic = std::move(discoveryTopic); };

        // OPTIONAL: set will topic (absolute), default to empty string, which is disabled
        void setWillTopic(std::string willTopic) { _willTopic = std::move(willTopic); };

        // called each time before publishing components
        // will prepare the buffers
        void begin(Device device, std::string baseTopic, PublisherCallback publisher) { begin(device, baseTopic, 1024, std::move(publisher)); }
        void begin(Device device, std::string baseTopic, const size_t bufferSise, PublisherCallback publisher);

        // Publish a component, transferring ownership.
        // The component is allocated on the heap, serialized, then automatically freed.
        void publish(std::unique_ptr<Component> component);

        // Unpublish (remove) a component, transferring ownership.
        // The component is allocated on the heap, its discovery topic is cleared, then automatically freed.
        void unpublish(std::unique_ptr<Component> component);

        // Convenience overload: construct the component in-place on the heap.
        // Equivalent to publish(std::make_unique<T>(args...)).
        //
        // Example:
        //   haDiscovery.publish<Mycila::HA::Button>("restart", "Restart", "~/system/restart", "restart", nullptr, Mycila::HA::Category::DIAGNOSTIC);
        template<typename T, typename... Args>
        void publish(Args&&... args) {
          publish(std::make_unique<T>(std::forward<Args>(args)...));
        }

        // Convenience overload: construct the component in-place on the heap.
        // Equivalent to unpublish(std::make_unique<T>(args...)).
        template<typename T, typename... Args>
        void unpublish(Args&&... args) {
          unpublish(std::make_unique<T>(std::forward<Args>(args)...));
        }

        // called after all components are published
        // will clear the buffers
        void end();

      private:
        // Discovery topic for a component: its configTopic override, or <discovery>/<type>/<device>/<id>/config
        static std::string componentConfigTopic(const Component* component, const std::string& discoveryTopic, const std::string& deviceId) {
          if (!component->configTopic.empty())
            return component->configTopic;

          std::string topic;
          topic.reserve(discoveryTopic.length() + 1 + strlen(component->type) + 1 + deviceId.length() + 1 + component->id.length() + 7);
          topic.append(discoveryTopic);
          topic.append("/");
          topic.append(component->type);
          topic.append("/");
          topic.append(deviceId);
          topic.append("/");
          topic.append(component->id);
          topic.append("/config");
          return topic;
        }

        Device _device;
        PublisherCallback _publisher = nullptr;
        std::string _baseTopic;
        std::string _discoveryTopic = MYCILA_HA_DISCOVERY_TOPIC;
        std::string _willTopic;
    };
  } // namespace HA
} // namespace Mycila
