#include "JsonUtils.h"

int JsonUtils::getOrDefault(const JsonObject& json, const char* key, int defaultVal) {
    if (json.containsKey(key)) {
        return json[key];
    } else {
        return defaultVal;
    }
}

double JsonUtils::getOrDefault(const JsonObject& json, const char* key, double defaultVal) {
    if (json.containsKey(key)) {
        return json[key];
    } else {
        return defaultVal;
    }
}