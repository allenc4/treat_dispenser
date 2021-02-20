#include <string>
#include "ArduinoNvs.h"
#include "WString.h"

#ifndef TREAT_DISPENSER_VERSION
#define TREAT_DISPENSER_VERSION unknown
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#ifndef _SETTINGS_H
#define _SETTINGS_H

class MotorSettings {
private:
    int _stepPin, _dirPin, _sleepPin;
    int _ms1Pin, _ms2Pin, _ms3Pin;
    unsigned long _disableTimeout;
    void setDefaults();
public:
    int numMicrosteps, microsecondsBetweenSteps, dispenseJitterCount;
    float numTurns, dispenseJitterNumTurns;
    MotorSettings();
    MotorSettings(int stepPin, int dirPin, int sleepPin);
    bool isDebug();
    int getStepPin();
    void setStepPin(int stepPin);
    int getDirectionPin();
    void setDirectionPin(int dirPin);
    int getSleepPin();
    void setSleepPin(int sleepPin);
    void setNumMicrosteps(int numMicrosteps);
    void setMicrosecondsBetweenSteps(int msBetweenSteps);
    void setDispenseJitterCount(int dispJitterCount);
    void setMicrostepPins(int ms1Pin, int ms2Pin, int ms3Pin);
    bool microstepPinsEnabled();
    int getMicrostepPin1();
    void setMicrostepPin1(int msPin1);
    int getMicrostepPin2();
    void setMicrostepPin2(int msPin2);
    int getMicrostepPin3();
    void setMicrostepPin3(int msPin3);
    unsigned long getDisableTimeout();
    bool saveSettings();
    void loadSettings();
    bool resetSettings();
};

class HttpSettings {
private:
    int _port;
    std::string _username, _password;
public:
    HttpSettings();
    HttpSettings(std::string username, std::string password);
    void setPort(int port);
    const bool isAuthenticationEnabled();
};

// class AudioSettings : public Configuration {
// public:
//   persistentIntVar(enable_pin, 16);
// };

// class Settings : public RootConfiguration {
// public:
//   subconfig(MotorSettings, motor);
//   subconfig(ArduCamSettings, arducam);
//   subconfig(HttpSettings, http);
//   subconfig(AudioSettings, audio);
// };

#endif