// SPDX-License-Identifier: MIT
/*
 * Copyright (C) Mathieu Carbou
 */
#include <MycilaHADiscovery.h>

#include <functional>
#include <string>
#include <utility>

#define TAG "HA"

namespace Mycila {
  namespace HA {
    namespace {
      // ASCII subset of Home Assistant's slugify (homeassistant.util.slugify, separator '_'):
      // lowercase alphanumerics kept, everything else replaced by '_' (HA also folds unicode
      // and strips leading/trailing separators; entity ids only accept [a-z0-9_]).
      std::string slug(const std::string& text) {
        std::string out;
        out.reserve(text.length());
        for (char c : text) {
          if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
            out.push_back(c);
          else if (c >= 'A' && c <= 'Z')
            out.push_back(c + ('a' - 'A'));
          else
            out.push_back('_');
        }
        return out;
      }
    } // namespace
  } // namespace HA
} // namespace Mycila

Mycila::HA::err_t Mycila::HA::Discovery::publish(std::unique_ptr<Component> component) {
  if (_discoveryTopic.empty())
    return Error::NO_DISCOVERY_TOPIC;

  if (_baseTopic.empty())
    return Error::NO_BASE_TOPIC;

  if (!_publisher)
    return Error::NO_PUBLISHER;

  if (_device.id.empty())
    return Error::NO_DEVICE_ID;

  JsonDocument doc;
  JsonObject root = doc.to<JsonObject>();

  // base topic
  root["~"] = _baseTopic.c_str();

  // unique id
  if (component->uniqueId.empty())
    component->uniqueId = _device.id + "_" + component->id;
  root["uniq_id"] = component->uniqueId;

  // default_entity_id must stay "<domain>.<object_id>": Home Assistant partitions on the first
  // dot and slugifies only the object_id part (entity.py), so each side is slugged separately.
  if (component->defaultEntityId.empty())
    component->defaultEntityId = slug(component->type) + "." + slug(component->uniqueId);

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
    ESP_LOGE(TAG, "Not enough memory for discovery config");
    return Error::OUT_OF_MEMORY;
  }

  std::string buffer;
  buffer.reserve(measureJson(root));
  serializeJson(root, buffer);

  const std::string topic = componentConfigTopic(component.get(), _discoveryTopic, _device.id);

  ESP_LOGD(TAG, "%s [%d b]", topic.c_str(), buffer.length());
  return _publisher(topic.c_str(), buffer);
}

Mycila::HA::err_t Mycila::HA::Discovery::unpublish(std::unique_ptr<Component> component) {
  if (_discoveryTopic.empty())
    return Error::NO_DISCOVERY_TOPIC;

  if (_baseTopic.empty())
    return Error::NO_BASE_TOPIC;

  if (!_publisher)
    return Error::NO_PUBLISHER;

  if (_device.id.empty())
    return Error::NO_DEVICE_ID;

  const std::string topic = componentConfigTopic(component.get(), _discoveryTopic, _device.id);

  ESP_LOGD(TAG, "unpublish: %s", topic.c_str());
  return _publisher(topic.c_str(), "");
}
