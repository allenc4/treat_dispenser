#include "../settings/Settings.h"
#include "../utils/JsonUtils.h"
#include <ArduinoJson.h>


#ifndef _MOTOR_CONTROLLER_H
#define _MOTOR_CONTROLLER_H

enum class MicrostepResolution {
  FULL = 1, HALF = 2, QUARTER = 4, EIGHTH = 8, SIXTEENTH = 16
};

enum class RotationDirection {
  CLOCKWISE, COUNTERCLOCKWISE
};

class MotorController {
private:
  MotorSettings& settings;
  bool _enabled, _settingsDirty;
  unsigned long lastMotorTurnTime;

public:
  MotorController(MotorSettings& settings);

  void continuousTurn(float numTurns, MicrostepResolution speed, RotationDirection direction);
  void continuousTurn(float numTurns, RotationDirection direction);
  void dispenseTurn();

  bool jsonCommand(const JsonObject& command);

  bool updateSettings(const JsonObject& command);
  void getSettings(JsonObject& root);
  bool saveSettings();
  bool resetSettings();

  void disable();
  void enable();

  void init();

  void tick();
  
  RotationDirection rotationDirectionFromStr(const String& str);
  MicrostepResolution microstepResolutionFromStr(const String& str);

};

#endif