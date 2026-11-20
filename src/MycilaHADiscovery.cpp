// SPDX-License-Identifier: MIT
/*
 * Copyright (C) Mathieu Carbou
 */
#include <MycilaHADiscovery.h>

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

void Mycila::HA::Discovery::begin(Device device, std::string baseTopic, const size_t bufferSise, PublisherCallback publisher) {
  _device = std::move(device);
  _baseTopic = std::move(baseTopic);
  _publisher = std::move(publisher);
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

  JsonDocument doc;
  JsonObject root = doc.to<JsonObject>();

  // base topic
  root["~"] = _baseTopic.c_str();

  // unique id
  if (component->uniqueId.empty())
    component->uniqueId = _device.id + "_" + component->id;
  root["uniq_id"] = component->uniqueId;

  if (component->defaultEntityId.empty())
    component->defaultEntityId = std::string(component->type) + "." + component->uniqueId;

  // device
  JsonObject device = root["dev"].to<JsonObject>();
  device["ids"] = _device.id.c_str();
  device["name"] = _device.name.c_str();
  device["mf"] = _device.manufacturer.c_str();
  device["mdl"] = _device.model.c_str();
  device["sw"] = _device.version.c_str();
  device["cu"] = _device.url.c_str();

  // availability
  if (component->availabilityTopic.empty()) {
    if (component->useDeviceAvailability && !_willTopic.empty()) {
      root["avty_t"] = _willTopic.c_str();
    }
  } else {
    root["avty_mode"] = "all";

    JsonArray array = root["availability"].to<JsonArray>();
    if (component->useDeviceAvailability && !_willTopic.empty()) {
      JsonObject deviceAvail = array.add<JsonObject>();
      deviceAvail["topic"] = _willTopic.c_str();
      deviceAvail["pl_avail"] = "online";
      deviceAvail["pl_not_avail"] = "offline";
    }

    JsonObject compAvail = array.add<JsonObject>();
    compAvail["topic"] = component->availabilityTopic;
    if (component->availabilityTemplate)
      compAvail["val_tpl"] = component->availabilityTemplate;
    if (component->payloadAvailable)
      compAvail["pl_avail"] = component->payloadAvailable;
    if (component->payloadNotAvailable)
      compAvail["pl_not_avail"] = component->payloadNotAvailable;
  }

  // component
  component->toJson(root);

  if (doc.overflowed()) {
    LOGE(TAG, "Not enough memory for discovery config");
    return;
  }

  std::string buffer;
  buffer.reserve(measureJson(root));
  serializeJson(root, buffer);

  const std::string topic = componentConfigTopic(component.get(), _discoveryTopic, _device.id);

  LOGD(TAG, "%s [%d b]", topic.c_str(), buffer.length());
  _publisher(topic.c_str(), buffer);
}

void Mycila::HA::Discovery::unpublish(std::unique_ptr<Component> component) {
  if (_discoveryTopic.empty())
    return;

  if (_baseTopic.empty())
    return;

  if (!_publisher)
    return;

  if (_device.id.empty())
    return;

  const std::string topic = componentConfigTopic(component.get(), _discoveryTopic, _device.id);

  LOGD(TAG, "unpublish: %s", topic.c_str());
  _publisher(topic.c_str(), "");
}

void Mycila::HA::Discovery::end() {
  _publisher = nullptr;
}
