#include "settings.h"

////////////////////////////////////////////////
//
//  Settings to configure motor mappings and stepping
////////////////////////////////////////////////
MotorSettings::MotorSettings() {
    _stepPin = -1;
    _dirPin = -1;
    _sleepPin = -1;
    _ms1Pin = -1;
    _ms2Pin = -1;
    _ms3Pin = -1;
    setDefaults();
}

MotorSettings::MotorSettings(int stepPin, int dirPin, int sleepPin) {
    _stepPin = stepPin;
    _dirPin = dirPin;
    _sleepPin = sleepPin;
    _ms1Pin = -1;
    _ms2Pin = -1;
    _ms3Pin = -1;
    setDefaults();
}

bool MotorSettings::isDebug() {
    return DEBUG;
}

void MotorSettings::setDefaults() {
    // Defaults for microsteps, numturns, etc
    numMicrosteps = 200;
    microsecondsBetweenSteps = 1200;
    numTurns = 1.1;

    dispenseJitterCount = 5;
    dispenseJitterNumTurns = 0.1;

    // Default motor timeout to 1 minute
    _disableTimeout = 10000;
    //_disableTimeout = 60000;
}

int MotorSettings::getStepPin() {
    return _stepPin;
}
void MotorSettings::setStepPin(int stepPin) {
    this->_stepPin = stepPin;
}

int MotorSettings::getDirectionPin() {
    return _dirPin;
}
void MotorSettings::setDirectionPin(int dirPin) {
    this->_dirPin = dirPin;
}

int MotorSettings::getSleepPin() {
    return _sleepPin;
}
void MotorSettings::setSleepPin(int sleepPin) {
    this->_sleepPin = sleepPin;
}

void MotorSettings::setNumMicrosteps(int microsteps) {
    numMicrosteps = microsteps;
}

void MotorSettings::setMicrosecondsBetweenSteps(int msBetweenSteps) {
    microsecondsBetweenSteps = msBetweenSteps;
}

void MotorSettings::setDispenseJitterCount(int dispenseJitterCount) {
    this->dispenseJitterNumTurns = dispenseJitterCount;
}

void MotorSettings::setMicrostepPins(int ms1Pin, int ms2Pin, int ms3Pin) {
    _ms1Pin = ms1Pin;
    _ms2Pin = ms2Pin;
    _ms3Pin = ms3Pin;
}

bool MotorSettings::microstepPinsEnabled() {
    if (_ms1Pin <= 0 || _ms2Pin <= 0 || _ms3Pin <= 0) {
        return false;
    }
    return true;
}

int MotorSettings::getMicrostepPin1() {
    return _ms1Pin;
}
void MotorSettings::setMicrostepPin1(int msPin1) {
    this->_ms1Pin = msPin1;
}

int MotorSettings::getMicrostepPin2() {
    return _ms2Pin;
}
void MotorSettings::setMicrostepPin2(int msPin2) {
    this->_ms2Pin = msPin2;
}

int MotorSettings::getMicrostepPin3() {
    return _ms3Pin;
}
void MotorSettings::setMicrostepPin3(int msPin3) {
    this->_ms3Pin = msPin3;
}

unsigned long MotorSettings::getDisableTimeout() {
    return _disableTimeout;
}

bool MotorSettings::saveSettings() {
    // Save all variables to NVS
    NVS.setInt("m.", 1);
    return NVS.setInt("m.stepPin", _stepPin) && NVS.setInt("m.dirPin", _dirPin) && NVS.setInt("m.sleepPin", _sleepPin) && 
        NVS.setInt("m.ms1Pin", _ms1Pin) && NVS.setInt("m.ms2Pin", _ms2Pin) && NVS.setInt("m.ms3Pin", _ms3Pin) && 
        NVS.setInt("m.numMs", numMicrosteps) &&  NVS.setInt("m.msBtwSteps", microsecondsBetweenSteps) && 
        NVS.setInt("m.dispJitCount", dispenseJitterCount) && NVS.setFloat("m.numTurns", numTurns) && 
        NVS.setFloat("m.jitNumTurns", dispenseJitterNumTurns);
}

void MotorSettings::loadSettings() {
    // Load variables from NVS
    if (NVS.getInt("m.") != 1) {
        // No variables to load, so return
        return;
    }
    Serial.println("Loading settings from NVS");
    _stepPin = NVS.getInt("m.stepPin");
    _dirPin = NVS.getInt("m.dirPin");
    _sleepPin = NVS.getInt("m.sleepPin");
    _ms1Pin = NVS.getInt("m.ms1Pin");
    _ms2Pin = NVS.getInt("m.ms2Pin");
    _ms3Pin = NVS.getInt("m.ms3Pin");
    numMicrosteps = NVS.getInt("m.numMs");
    microsecondsBetweenSteps = NVS.getInt("m.msBtwSteps");
    numTurns = NVS.getFloat("m.numTurns");
    dispenseJitterCount = NVS.getInt("m.dispJitCount");
    dispenseJitterNumTurns = NVS.getFloat("m.jitNumTurns");
}

bool MotorSettings::resetSettings() {
    setDefaults();
    return saveSettings();
}

////////////////////////////////////////////////
//
//  Settings to configure HTTP flags, authentication, etc
////////////////////////////////////////////////
HttpSettings::HttpSettings() {
    _username = "";
    _password = "";
    _port = 80;
}

HttpSettings::HttpSettings(std::string username, std::string password) {
    _username = username;
    _password = password;
    _port = 80;
}

void HttpSettings::setPort(int port) {
    _port = port;
}



const bool HttpSettings::isAuthenticationEnabled() {
    return !(_username.length() == 0 || _password.length() == 0);
}