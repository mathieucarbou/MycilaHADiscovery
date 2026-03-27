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
  root["uniq_id"] = _device.id + "_" + component->id;

  // device
  JsonObject device = root["dev"].to<JsonObject>();
  device["ids"] = _device.id.c_str();
  device["name"] = _device.name.c_str();
  device["mf"] = _device.manufacturer.c_str();
  device["mdl"] = _device.model.c_str();
  device["sw"] = _device.version.c_str();
  device["cu"] = _device.url.c_str();

  // availability
  if (!component->availabilityTopic) {
    if (!_willTopic.empty()) {
      root["avty_t"] = _willTopic.c_str();
    }
  } else {
    root["avty_mode"] = "all";

    JsonArray array = root["availability"].to<JsonArray>();
    if (!_willTopic.empty()) {
      JsonObject deviceAvail = array.add<JsonObject>();
      deviceAvail["topic"] = _willTopic.c_str();
      deviceAvail["pl_avail"] = "online";
      deviceAvail["pl_not_avail"] = "offline";
    }

    JsonObject compAvail = array.add<JsonObject>();
    compAvail["topic"] = component->availabilityTopic;
    if (component->payloadAvailable)
      compAvail["pl_avail"] = component->payloadAvailable;
    if (component->payloadNotAvailable)
      compAvail["pl_not_avail"] = component->payloadNotAvailable;
  }

  // component
  component->toJson(root);

  if (_sensorExpirationTime > 0 && strcmp(component->type, "sensor") == 0)
    root["exp_aft"] = _sensorExpirationTime;

  std::string buffer;
  buffer.reserve(measureJson(root));
  serializeJson(root, buffer);

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

void Mycila::HA::Discovery::unpublish(std::unique_ptr<Component> component) {
  if (_discoveryTopic.empty())
    return;

  if (_baseTopic.empty())
    return;

  if (!_publisher)
    return;

  if (_device.id.empty())
    return;

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

  LOGD(TAG, "unpublish: %s", topic.c_str());
  _publisher(topic.c_str(), "");
}

void Mycila::HA::Discovery::end() {
  _publisher = nullptr;
}
