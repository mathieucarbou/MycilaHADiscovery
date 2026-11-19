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
        Component(const char* type, std::string id, std::string name, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : type(type), id(std::move(id)), name(std::move(name)), deviceClass(deviceClass), icon(icon), category(category) {}
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Component(const char* type, const char* id, const char* name, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE)
            : Component(type, stringOrEmpty(id), stringOrEmpty(name), deviceClass, icon, category) {}
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
        std::optional<uint32_t> expireAfter; // 0 disables expiration.
        bool useDeviceAvailability = true;
        std::string availabilityTopic;
        const char* availabilityTemplate = nullptr;
        const char* payloadAvailable = nullptr;
        const char* payloadNotAvailable = nullptr;

      protected:
        // Legacy constructors accepted nullptr for omitted string values.
        static std::string stringOrEmpty(const char* value) { return value ? value : ""; }
    };

    // https://www.home-assistant.io/integrations/button.mqtt/
    class Button : public Component {
      public:
        Button(std::string id, std::string name) : Button(std::move(id), std::move(name), {}) {}
        Button(std::string id, std::string name, std::string commandTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE) : Component("button", std::move(id), std::move(name), deviceClass, icon, category) { this->commandTopic = std::move(commandTopic); }
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Button(const char* id, const char* name, const char* commandTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE)
            : Button(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(commandTopic), deviceClass, icon, category) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Switch(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const Category category = Category::NONE)
            : Switch(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(commandTopic), stringOrEmpty(stateTopic), payloadOn, payloadOff, icon, category) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Outlet(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* icon = nullptr, const Category category = Category::NONE)
            : Outlet(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(commandTopic), stringOrEmpty(stateTopic), payloadOn, payloadOff, icon, category) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Select(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* icon = nullptr, const Category category = Category::NONE, std::vector<const char*> options = {})
            : Select(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(commandTopic), stringOrEmpty(stateTopic), icon, category, std::move(options)) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Text(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const char* pattern = nullptr, const char* icon = nullptr, const Category category = Category::NONE)
            : Text(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(commandTopic), stringOrEmpty(stateTopic), pattern, icon, category) {}
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
        Number(std::string id, std::string name, std::string commandTopic, std::string stateTopic, const NumberMode mode = NumberMode::AUTO, const float min = 0, const float max = 100, const float step = 1, const char* icon = nullptr, const Category category = Category::NONE) : Component("number", std::move(id), std::move(name), nullptr, icon, category) {
          this->commandTopic = std::move(commandTopic);
          this->stateTopic = std::move(stateTopic);
          this->mode = mode;
          this->min = min;
          this->max = max;
          this->step = step;
        }
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Number(const char* id, const char* name, const char* commandTopic, const char* stateTopic, const NumberMode mode = NumberMode::AUTO, const float min = 0, const float max = 100, const float step = 1, const char* icon = nullptr, const Category category = Category::NONE)
            : Number(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(commandTopic), stringOrEmpty(stateTopic), mode, min, max, step, icon, category) {}
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
        NumberMode mode = NumberMode::AUTO;
        std::optional<float> min = 0;
        std::optional<float> max = 100;
        float step = 1;
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
        const char* payloadOn = nullptr;
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        State(const char* id, const char* name, const char* stateTopic, const char* payloadOn, const char* payloadOff, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE)
            : State(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), payloadOn, payloadOff, deviceClass, icon, category) {}
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
        }

        std::string stateTopic;
        const char* valueTemplate = nullptr;
        const char* payloadOn = nullptr;
        const char* payloadOff = nullptr;
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Sensor(const char* id, const char* name, const char* stateTopic, const StateClass stateClass = StateClass::NONE, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Sensor(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), stateClass, deviceClass, icon, unit, category) {}
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
        }

        const char* unit = nullptr;
        std::string stateTopic;
        StateClass stateClass = StateClass::NONE;
        const char* valueTemplate = nullptr;
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (measurement)
    class Gauge : public Sensor {
      public:
        Gauge(std::string id, std::string name) : Gauge(std::move(id), std::move(name), {}) {}
        Gauge(std::string id, std::string name, std::string stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::GAUGE, deviceClass, icon, unit, category) {
          this->valueTemplate = valueTemplate;
        }
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Gauge(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Gauge(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), deviceClass, icon, unit, category, valueTemplate) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Counter(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Counter(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), deviceClass, icon, unit, category, valueTemplate) {}
        virtual ~Counter() = default;
    };

    // https://www.home-assistant.io/integrations/sensor.mqtt/ (total)
    class Total : public Sensor {
      public:
        Total(std::string id, std::string name, std::string stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Sensor(std::move(id), std::move(name), std::move(stateTopic), StateClass::TOTAL, deviceClass, icon, unit, category) {}
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Total(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const char* unit = nullptr, const Category category = Category::NONE)
            : Total(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), deviceClass, icon, unit, category) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Value(const char* id, const char* name, const char* stateTopic, const char* deviceClass = nullptr, const char* icon = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Value(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), deviceClass, icon, category, valueTemplate) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Enum(const char* id, const char* name, const char* stateTopic, std::vector<const char*> options, const char* icon = nullptr, const Category category = Category::NONE, const char* valueTemplate = nullptr)
            : Enum(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(stateTopic), std::move(options), icon, category, valueTemplate) {}
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
        [[deprecated("Use std::string arguments and empty strings instead of nullptr")]]
        Update(const char* id, const char* name, const char* url = nullptr, const char* stateTopic = nullptr, const char* latestVersionTopic = nullptr, const char* deviceClass = "firmware", const char* icon = nullptr, const Category category = Category::NONE)
            : Update(stringOrEmpty(id), stringOrEmpty(name), stringOrEmpty(url), stringOrEmpty(stateTopic), stringOrEmpty(latestVersionTopic), deviceClass, icon, category) {}
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

        // OPTIONAL: sensor expiration time in seconds, 0 for no expiration, default to MYCILA_HA_SENSOR_EXPIRATION_TIME
        void setSensorExpirationTime(const uint32_t sensorExpirationTime) { _sensorExpirationTime = sensorExpirationTime; };

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
        Device _device;
        PublisherCallback _publisher = nullptr;
        std::string _baseTopic;
        std::string _discoveryTopic = MYCILA_HA_DISCOVERY_TOPIC;
        std::string _willTopic;
        uint32_t _sensorExpirationTime = MYCILA_HA_SENSOR_EXPIRATION_TIME;
    };
  } // namespace HA
} // namespace Mycila
