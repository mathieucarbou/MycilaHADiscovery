// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2025 Mathieu Carbou
 */
#include <MycilaHADiscovery.h>

#include <ArduinoJson.h>

#include <functional>
#include <string>
#include <utility>

#ifdef MYCILA_LOGGER_SUPPORT
  #include <MycilaLogger.h>
extern Mycila::Logger logger;
  #define LOGD(tag, format, ...) logger.debug(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) logger.info(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) logger.warn(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) logger.error(tag, format, ##__VA_ARGS__)
#else
  #define LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#endif

#define TAG "HA"

void Mycila::HA::Discovery::begin(Device device, std::string baseTopic, const size_t bufferSise, const PublisherCallback publisher) {
  _device = std::move(device);
  _baseTopic = std::move(baseTopic);
  _publisher = publisher;
}

void Mycila::HA::Discovery::publish(std::unique_ptr<Component> component) {
  if (_discoveryTopic.empty())
    return;

  if (_baseTopic.empty())
    return;

  if (!_publisher)
    return;

  if (_device.id.empty())
    return;

#if ARDUINOJSON_VERSION_MAJOR == 6
  DynamicJsonDocument root(1024);
#else
  JsonDocument root;
#endif

  // device
  JsonObject device = root["dev"].to<JsonObject>();
  device["ids"] = _device.id.c_str();
  device["name"] = _device.name.c_str();
  device["mf"] = _device.manufacturer.c_str();
  device["mdl"] = _device.model.c_str();
  device["sw"] = _device.version.c_str();
  device["cu"] = _device.url.c_str();

  root["name"] = component->name;
  root["uniq_id"] = _device.id + "_" + component->id;
  root["obj_id"] = _device.id + "_" + component->id;

  if (!component->availabilityTopic) {
    if (!_willTopic.empty()) {
      root["avty_t"] = _willTopic.c_str();
    }
  } else {
    root["avty_mode"] = "all";
    JsonArray array = root["availability"].to<JsonArray>();
    if (!_willTopic.empty()) {
#if ARDUINOJSON_VERSION_MAJOR == 6
      JsonObject deviceAvail = array.createNestedObject();
#else
      JsonObject deviceAvail = array.add<JsonObject>();
#endif
      deviceAvail["topic"] = _willTopic.c_str();
      deviceAvail["pl_avail"] = "online";
      deviceAvail["pl_not_avail"] = "offline";
    }
#if ARDUINOJSON_VERSION_MAJOR == 6
    JsonObject compAvail = array.createNestedObject();
#else
    JsonObject compAvail = array.add<JsonObject>();
#endif
    compAvail["topic"] = _baseTopic + component->availabilityTopic;
    if (component->payloadAvailable)
      compAvail["pl_avail"] = component->payloadAvailable;
    if (component->payloadNotAvailable)
      compAvail["pl_not_avail"] = component->payloadNotAvailable;
  }

  if (component->deviceClass)
    root["dev_cla"] = component->deviceClass;

  if (component->icon)
    root["ic"] = component->icon;

  if (component->category == Category::CONFIG)
    root["ent_cat"] = "config";
  if (component->category == Category::DIAGNOSTIC)
    root["ent_cat"] = "diagnostic";

  if (component->stateClass == StateClass::COUNTER)
    root["stat_cla"] = "total_increasing";
  if (component->stateClass == StateClass::GAUGE)
    root["stat_cla"] = "measurement";
  if (component->stateClass == StateClass::TOTAL)
    root["stat_cla"] = "total";

  if (component->unit)
    root["unit_of_meas"] = component->unit;

  if (component->commandTopic)
    root["cmd_t"] = _baseTopic + component->commandTopic;

  if (component->stateTopic)
    root["stat_t"] = _baseTopic + component->stateTopic;

  if (component->valueTemplate)
    root["val_tpl"] = component->valueTemplate;

  if (component->payloadOn)
    root["pl_on"] = component->payloadOn;

  if (component->payloadOff)
    root["pl_off"] = component->payloadOff;

  if (component->pattern)
    root["ptrn"] = component->pattern;

  if (strcmp(component->type, "number") == 0) {
    root["mode"] = component->mode == NumberMode::BOX ? "box" : (component->mode == NumberMode::SLIDER ? "slider" : "auto");
    root["min"] = component->min;
    root["max"] = component->max;
    root["step"] = component->step;
  }

  if (_sensorExpirationTime > 0 && strcmp(component->type, "sensor") == 0)
    root["exp_aft"] = _sensorExpirationTime;

  if (component->options.size() > 0) {
    JsonArray opts = root["options"].to<JsonArray>();
    for (auto& opt : component->options)
      opts.add(opt);
  }

  std::string buffer;
  buffer.reserve(measureJson(root));

  std::string topic;
  topic.reserve(_discoveryTopic.length() + 1 + strlen(component->type) + 1 + _device.id.length() + 1 + strlen(component->id) + 7);
  topic.append(_discoveryTopic);
  topic.append("/");
  topic.append(component->type);
  topic.append("/");
  topic.append(_device.id);
  topic.append("/");
  topic.append(component->id);
  topic.append("/config");

  LOGD(TAG, "%s [%d b]", topic.c_str(), buffer.length());
  _publisher(topic.c_str(), buffer);
}

void Mycila::HA::Discovery::end() {
  _publisher = nullptr;
}
