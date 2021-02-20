#include "MotorController.h"

MotorController::MotorController(MotorSettings& settings)
  : settings(settings)
{ 
    init();
}

void MotorController::continuousTurn(float numTurns, MicrostepResolution speed, RotationDirection direction) {
    // Enable the motor if we are sleeping
    enable();
    digitalWrite(settings.getDirectionPin(), static_cast<uint8_t>(direction));

    std::vector<uint8_t> speedPinValues;

    switch (speed) {
        case MicrostepResolution::FULL:
            speedPinValues = {LOW, LOW, LOW};
            break;
        case MicrostepResolution::HALF:
            speedPinValues = {HIGH, LOW, LOW};
            break;
        case MicrostepResolution::QUARTER:
            speedPinValues = {LOW, HIGH, LOW};
            break;
        case MicrostepResolution::EIGHTH:
            speedPinValues = {HIGH, HIGH, LOW};
            break;
        case MicrostepResolution::SIXTEENTH:
            speedPinValues = {HIGH, HIGH, HIGH};
            break;
    }

    if (!settings.microstepPinsEnabled()) {
        digitalWrite(settings.getMicrostepPin1(), speedPinValues[0]);
        digitalWrite(settings.getMicrostepPin2(), speedPinValues[1]);
        digitalWrite(settings.getMicrostepPin3(), speedPinValues[2]);
    } else {
        speed = MicrostepResolution::FULL;
    }
    
    size_t numSteps = ceil(numTurns * settings.numMicrosteps * static_cast<uint8_t>(speed));

    if (settings.isDebug()) {
        Serial.printf("Stepping %d times...\n", numSteps);
    }

    for (size_t i = 0; i < numSteps; ++i) {
        digitalWrite(settings.getStepPin(), LOW);
        delayMicroseconds(settings.microsecondsBetweenSteps);
        digitalWrite(settings.getStepPin(), HIGH);
        delayMicroseconds(settings.microsecondsBetweenSteps);
    }
    lastMotorTurnTime = millis();

}
  
void MotorController::continuousTurn(float numTurns, RotationDirection direction) {
    continuousTurn(numTurns, MicrostepResolution::FULL, direction);
}

void MotorController::dispenseTurn() {
    // Jitter back and forth a few times to unstick
    for (size_t i = 0; i < settings.dispenseJitterCount; ++i) {
        continuousTurn(settings.dispenseJitterNumTurns, RotationDirection::CLOCKWISE);
        continuousTurn(settings.dispenseJitterNumTurns, RotationDirection::COUNTERCLOCKWISE);
    }

    continuousTurn(settings.numTurns, RotationDirection::CLOCKWISE);
}

bool MotorController::jsonCommand(const JsonObject& command) {
    if (!command.containsKey("type")) {
        return false;
    }

    const String& type = command["type"];
    if (type.equalsIgnoreCase("simple")) {
        const JsonObject& args = command["args"];
        const RotationDirection dir = rotationDirectionFromStr(args["direction"]);
        const MicrostepResolution res = microstepResolutionFromStr(args["resolution"]);
        const float numTurns = args["num_turns"];

        if (settings.isDebug()) {
            Serial.printf("Executing command: %d, %d, %f\n", static_cast<uint8_t>(dir), static_cast<uint8_t>(res), numTurns);
        }
        continuousTurn(numTurns, res, dir);
        return true;
    } else if (type.equalsIgnoreCase("dispense")) {
        dispenseTurn();
        return true;
    } else if (type.equalsIgnoreCase("enable")) {
        enable();
        return true;
    } else if (type.equalsIgnoreCase("disable")) {
        disable();
        return true;
    }

    return false;
}

bool MotorController::updateSettings(const JsonObject& command) {

    settings.setStepPin(JsonUtils::getOrDefault(command, "stepPin", settings.getStepPin()));
    settings.setSleepPin(JsonUtils::getOrDefault(command, "sleepPin", settings.getSleepPin()));
    settings.setDirectionPin(JsonUtils::getOrDefault(command, "dirPin", settings.getDirectionPin()));
    settings.setMicrostepPin1(JsonUtils::getOrDefault(command, "ms1Pin", settings.getMicrostepPin1()));
    settings.setMicrostepPin2(JsonUtils::getOrDefault(command, "ms2Pin", settings.getMicrostepPin2()));
    settings.setMicrostepPin3(JsonUtils::getOrDefault(command, "ms3Pin", settings.getMicrostepPin3()));

    settings.numMicrosteps=JsonUtils::getOrDefault(command, "numMicrosteps", settings.numMicrosteps);
    settings.microsecondsBetweenSteps=JsonUtils::getOrDefault(command, "microsecondsBetweenSteps", settings.microsecondsBetweenSteps);
    settings.dispenseJitterCount=JsonUtils::getOrDefault(command, "dispenseJitterCount", settings.dispenseJitterCount);

    settings.numTurns=JsonUtils::getOrDefault(command, "numTurns", settings.numTurns);
    settings.dispenseJitterNumTurns=JsonUtils::getOrDefault(command, "dispenseJitterNumTurns", settings.dispenseJitterNumTurns);
    _settingsDirty = true;
    if (settings.isDebug()) {
        Serial.println(F("Updated settings for motor commands"));
    }
    return true;
}

 void MotorController::getSettings(JsonObject& root) {
    JsonObject motorJson = root.createNestedObject("motor");
    motorJson["stepPin"] = settings.getStepPin();
    motorJson["dirPin"] = settings.getDirectionPin();
    motorJson["sleepPin"] = settings.getSleepPin();
    motorJson["ms1Pin"] = settings.getMicrostepPin1();
    motorJson["ms2Pin"] = settings.getMicrostepPin2();
    motorJson["ms3Pin"] = settings.getMicrostepPin3();
    motorJson["numMicrosteps"] = settings.numMicrosteps;
    motorJson["microsecondsBetweenSteps"] = settings.microsecondsBetweenSteps;
    motorJson["dispenseJitterCount"] = settings.dispenseJitterCount;
    motorJson["numTurns"] = settings.numTurns;
    motorJson["dispenseJitterNumTurns"] = settings.dispenseJitterNumTurns;
}

bool MotorController::saveSettings() {
    if (_settingsDirty) {
        if (settings.isDebug()) {
            Serial.println(F("There are overridden settings. Saving them"));
        }
        // There are overridden settings. Save them now
        _settingsDirty = false;
        return settings.saveSettings();
    }
    return false;
}

bool MotorController::resetSettings() {
    return settings.resetSettings();
}

void MotorController::disable() {
    if (!_enabled) {
        return;
    }
    if (settings.isDebug()) {
        Serial.println(F("Disabling the motor"));
    }
    digitalWrite(settings.getSleepPin(), LOW);
    delay(100);
    _enabled = false;
}

void MotorController::enable() {
    if (_enabled) {
        return;
    }
    if (settings.isDebug()) {
        Serial.println(F("Enabling the motor"));
    }
    digitalWrite(settings.getSleepPin(), HIGH);
    delay(100);
    _enabled = true;
}

void MotorController::init() {
    // Set step, direction, and sleep pins as outputs
    pinMode(settings.getStepPin(), OUTPUT);
    pinMode(settings.getDirectionPin(),  OUTPUT);
    pinMode(settings.getSleepPin(), OUTPUT);
    // Currently no stepper motor movement
    digitalWrite(settings.getStepPin(), LOW);
    digitalWrite(settings.getDirectionPin(), LOW);
    // Disable the motor by default
    disable();
    _settingsDirty = false;
}

void MotorController::tick() {
    if (_enabled) {
        // If motor is currently enabled, check current time vs last turn time 
        // to see if we need to disable the motor
        if (millis() - lastMotorTurnTime >= settings.getDisableTimeout()) {
            // More than allowed time has past since we used the motor, 
            // so disable it
            disable();
        }
    }
}

RotationDirection MotorController::rotationDirectionFromStr(const String& str) {
    if (str.equalsIgnoreCase("CLOCKWISE")) 
        return RotationDirection::CLOCKWISE;
    return RotationDirection::COUNTERCLOCKWISE;
}

MicrostepResolution MotorController::microstepResolutionFromStr(const String& str) {
    if (str.equalsIgnoreCase("HALF")) 
        return MicrostepResolution::HALF;
    else if (str.equalsIgnoreCase("QUARTER")) 
        return MicrostepResolution::QUARTER;
    else if (str.equalsIgnoreCase("EIGHTH"))
        return MicrostepResolution::EIGHTH;
    else if (str.equalsIgnoreCase("SIXTEENTH"))
        return MicrostepResolution::SIXTEENTH;
    return MicrostepResolution::FULL;
}