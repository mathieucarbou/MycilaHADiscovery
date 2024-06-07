// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#include <MycilaHADiscovery.h>

#include <ArduinoJson.h>
#include <functional>

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

void Mycila::HADiscovery::setDevice(const HADevice& device) {
  _device = device;
}

void Mycila::HADiscovery::begin() {
  // cache device
  JsonDocument doc;
  JsonObject json = doc.to<JsonObject>();
  json["name"] = _device.name.c_str();
  json["ids"] = _device.id.c_str();
  json["mf"] = _device.manufacturer.c_str();
  json["mdl"] = _device.model.c_str();
  json["sw"] = _device.version.c_str();
  json["cu"] = _device.url.c_str();
  _deviceJsonCache.reserve(measureJson(json));
  serializeJson(json, _deviceJsonCache);

  // prepare buffer
  _buffer.reserve(_bufferSise);
}

void Mycila::HADiscovery::publish(const HAComponent& component) {
  if (_discoveryTopic.isEmpty())
    return;

  if (_baseTopic.isEmpty())
    return;

  if (!_publisher)
    return;

  if (_device.id.isEmpty())
    return;

#if ARDUINOJSON_VERSION_MAJOR == 6
  DynamicJsonDocument root(1024);
#else
  JsonDocument root;
#endif

  root["name"] = component.name;
  root["uniq_id"] = _device.id + "_" + component.id;
  root["obj_id"] = _device.id + "_" + component.id;

  if (!component.availabilityTopic) {
    if (!_willTopic.isEmpty()) {
      root["avty_t"] = _willTopic.c_str();
    }
  } else {
    root["avty_mode"] = "all";
    JsonArray array = root["availability"].to<JsonArray>();
    if (!_willTopic.isEmpty()) {
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
    compAvail["topic"] = _baseTopic + component.availabilityTopic;
    if (component.payloadAvailable)
      compAvail["pl_avail"] = component.payloadAvailable;
    if (component.payloadNotAvailable)
      compAvail["pl_not_avail"] = component.payloadNotAvailable;
  }

  if (component.deviceClass)
    root["dev_cla"] = component.deviceClass;

  if (component.icon)
    root["ic"] = component.icon;

  if (component.category == HACategory::CONFIG)
    root["ent_cat"] = "config";
  if (component.category == HACategory::DIAGNOSTIC)
    root["ent_cat"] = "diagnostic";

  if (component.stateClass == HAStateClass::COUNTER)
    root["stat_cla"] = "total_increasing";
  if (component.stateClass == HAStateClass::GAUGE)
    root["stat_cla"] = "measurement";
  if (component.stateClass == HAStateClass::TOTAL)
    root["stat_cla"] = "total";

  if (component.unit)
    root["unit_of_meas"] = component.unit;

  if (component.commandTopic)
    root["cmd_t"] = _baseTopic + component.commandTopic;

  if (component.stateTopic)
    root["stat_t"] = _baseTopic + component.stateTopic;

  if (component.valueTemplate)
    root["val_tpl"] = component.valueTemplate;

  if (component.payloadOn)
    root["pl_on"] = component.payloadOn;

  if (component.payloadOff)
    root["pl_off"] = component.payloadOff;

  if (component.pattern)
    root["ptrn"] = component.pattern;

  if (strcmp(component.type, "number") == 0) {
    root["mode"] = component.mode == HANumberMode::BOX ? "box" : (component.mode == HANumberMode::SLIDER ? "slider" : "auto");
    root["min"] = component.min;
    root["max"] = component.max;
    root["step"] = component.step;
  }

  if (_sensorExpirationTime > 0 && strcmp(component.type, "sensor") == 0)
    root["exp_aft"] = _sensorExpirationTime;

  if (component.options.size() > 0) {
    JsonArray opts = root["options"].to<JsonArray>();
    for (auto& opt : component.options)
      opts.add(opt);
  }

  _buffer.clear();
  serializeJson(root, _buffer);
  _buffer.setCharAt(_buffer.length() - 1, ',');
  _buffer += "\"dev\":";
  _buffer += _deviceJsonCache;
  _buffer += "}";

  String topic = _discoveryTopic + "/" + component.type + "/" + _device.id + "/" + component.id + "/config";
  LOGD(TAG, "%s [%d b]", topic.c_str(), _buffer.length());
  _publisher(topic, _buffer);
}

void Mycila::HADiscovery::end() {
  _deviceJsonCache = String();
  _buffer = String();
}
