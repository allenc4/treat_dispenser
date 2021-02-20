#include "LocalHttpServer.h"

LocalHttpServer::LocalHttpServer(uint16_t port, MotorController& motor):
    server(port),
    motor(motor),
    _debug(false)
{}
LocalHttpServer::LocalHttpServer(uint16_t port, MotorController& motor, bool debug):
    server(port),
    motor(motor),
    _debug(debug)
{}

void LocalHttpServer::begin() {
    // Use the builtin server (ESP8266WebServer for ESP8266, WebServer for ESP32).
    // Listen on defined port.

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "ESP server working");
    });

    // Bind the about method
    server.on("/about", HTTP_GET, std::bind(&LocalHttpServer::handleAbout, this, _1));

    // Bind the settings methods (get and post)
    server.on("/settings/save", HTTP_GET, std::bind(&LocalHttpServer::handleSaveSettings, this, _1));
    server.on("/settings/reset", HTTP_GET, std::bind(&LocalHttpServer::handleResetSettings, this, _1));
    server.on("/settings", HTTP_GET, std::bind(&LocalHttpServer::handleGetSettings, this, _1));
    server.addHandler(new AsyncCallbackJsonWebHandler("/settings",
        std::bind(&LocalHttpServer::handlePostSettings, this, _1, _2)));

    // Motor command
    server.addHandler(new AsyncCallbackJsonWebHandler("/motor/commands",
        std::bind(&LocalHttpServer::handlePostMotorCommand, this, _1, _2)));
    
    // On anything not found...
    server.onNotFound(std::bind(&LocalHttpServer::handleNotFound, this, _1));

    if (_debug) {
        Serial.println("Server started...");
    }

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    // Begin the server
    server.begin();

}

// General info routes
void LocalHttpServer::handleNotFound(AsyncWebServerRequest *request) {
    if (_debug) {
        Serial.println("Endpoint not found...");
    }
    request->send(404, "text/plain", "Not found");
}

void LocalHttpServer::handleAbout(AsyncWebServerRequest *request) {
    if (_debug) {
        Serial.println("/about endpoint");
    }
    AsyncJsonResponse *response = new AsyncJsonResponse();
    response->addHeader("Server","ESP Async Web Server");
    JsonObject root = response->getRoot();
    root["heap"] = ESP.getFreeHeap();
    root["ssid"] = WiFi.SSID();
    root["ip_address"] = WiFi.localIP().toString();
    response->setLength();
    request->send(response);
}

void LocalHttpServer::handlePostMotorCommand(AsyncWebServerRequest *request, JsonVariant &json) {

    JsonObject body = json.as<JsonObject>();
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject root = response->getRoot();
    response->addHeader("Server","ESP Async Web Server");

    if (motor.jsonCommand(body)) {
        root["success"] = true;
    } else {
        root["error"] = "Invalid Command";
        response->setCode(400);
    }
    response->setLength();
    request->send(response);
    if (_debug) {
        Serial.println("sending postmotor response");
    }
}

void LocalHttpServer::handlePostSettings(AsyncWebServerRequest *request, JsonVariant &json) {

    JsonObject body = json.as<JsonObject>();
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject root = response->getRoot();
    response->addHeader("Server","ESP Async Web Server");

    // Check if we have any motor settings to update
    boolean status = true;
    if (body.containsKey("motor")) {
        status = motor.updateSettings(body["motor"]);
    }

    if (status) {
        root["success"] = true;
    } else {
        root["error"] = "Invalid Command";
        response->setCode(400);
    }
    response->setLength();
    request->send(response);
}

void LocalHttpServer::handleGetSettings(AsyncWebServerRequest *request) {
    if (_debug) {
        Serial.println(F("/settings endpoint"));
    }
    AsyncJsonResponse *response = new AsyncJsonResponse();
    response->addHeader("Server","ESP Async Web Server");
    JsonObject root = response->getRoot();
    motor.getSettings(root);
    response->setLength();
    request->send(response);
}

void LocalHttpServer::handleSaveSettings(AsyncWebServerRequest *request) {
    if (_debug) {
        Serial.println(F("/settings/save endpoint"));
    }
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject root = response->getRoot();
    response->addHeader("Server","ESP Async Web Server");
    boolean status = true;
    status = motor.saveSettings();

    if (status) {
        root["success"] = true;
    } else {
        root["error"] = "Invalid Command";
        response->setCode(400);
    }
    response->setLength();
    request->send(response);
}

void LocalHttpServer::handleResetSettings(AsyncWebServerRequest *request) {
    if (_debug) {
        Serial.println(F("/settings/reset endpoint"));
    }
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject root = response->getRoot();
    response->addHeader("Server","ESP Async Web Server");
    boolean status =true;
    status = motor.resetSettings();

    if (status) {
        root["success"] = true;
    } else {
        root["error"] = "Invalid Command";
        response->setCode(400);
    }
    response->setLength();
    request->send(response);
}

void LocalHttpServer::handleDeleteSound(AsyncWebServerRequest *request) {

}

void LocalHttpServer::handleShowSound(AsyncWebServerRequest *request) {

}

void LocalHttpServer::handlePostAudioCommand(AsyncWebServerRequest *request) {

}

void LocalHttpServer::handleListDirectory(const char* dir, AsyncWebServerRequest *request) {

}

void LocalHttpServer::handleCreateFile(const char* filePrefix, AsyncWebServerRequest *request) {

}