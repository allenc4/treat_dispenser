#ifndef _C_LOCAL_HTTP_SERVER_H
#define _C_LOCAL_HTTP_SERVER_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "../motor/MotorController.h"


using namespace std::placeholders;

class LocalHttpServer {
public:
    LocalHttpServer(uint16_t port, MotorController& motor);
    LocalHttpServer(uint16_t port, MotorController& motor, bool debug);
    void begin();
private:
    AsyncWebServer server;
    MotorController& motor;
    bool _debug;

    // General info routes
    void handleNotFound(AsyncWebServerRequest *request);
    void handleAbout(AsyncWebServerRequest *request);

    // Motor
    void handlePostMotorCommand(AsyncWebServerRequest *request, JsonVariant &json);

    // Settings info
    void handleGetSettings(AsyncWebServerRequest *request);
    void handlePostSettings(AsyncWebServerRequest *request, JsonVariant &json);
    void handleSaveSettings(AsyncWebServerRequest *request);
    void handleResetSettings(AsyncWebServerRequest *request);

    // Audio
    void handleDeleteSound(AsyncWebServerRequest *request);
    void handleShowSound(AsyncWebServerRequest *request);
    void handlePostAudioCommand(AsyncWebServerRequest *request);

    // General helpers
    void handleListDirectory(const char* dir, AsyncWebServerRequest *request);
    void handleCreateFile(const char* filePrefix, AsyncWebServerRequest *request);
};

#endif