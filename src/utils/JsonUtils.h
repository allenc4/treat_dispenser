#include "../settings/Settings.h"
#include <ArduinoJson.h>


#ifndef _INTERNAL_JSON_UTILS_H
#define _INTERNAL_JSON_UTILS_H

class JsonUtils {

public:
  static int getOrDefault(const JsonObject& command, const char* key, int defaultVal);
  static double getOrDefault(const JsonObject& command, const char* key, double defaultVal);
};

#endif
