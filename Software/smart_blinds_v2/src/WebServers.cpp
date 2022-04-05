//
// Created by victo on 06/10/2021.
//
#include <ESPAsyncWebserver.h>
#include <SPIFFS.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WebServers.h>
#include <Connections.h>
#include <Hardware.h>

static const char* TAG = "WebServers";
extern String device_name;
extern String ssid;
extern String pass;
extern String mqtt_server_ip;
extern AsyncWebServer server;
extern int mqtt_server_port;
extern int WIFI_CONNECTION_STATUS;
extern int MQTT_CONNECTION_STATUS;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request){
        //request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/Teste1.html", String(), false, index_config_processor);
    }
};


String index_config_processor(const String& var){
    if(var == "DEVICE_NAME"){
        String name = device_name + " Persiana Inteligente";
        return name;
    }
    return {};
}

String index_running_processor(const String& var){
    if(var == "DEVICE_NAME"){
        String name = device_name + " Persiana Inteligente";
        return name;
    }
    return {};
}

void startup_server(){
    ESP_LOGD(TAG, "Entering Web Server Configuration Mode");

    if(!SPIFFS.begin(true)){
        ESP_LOGE(TAG, "An Error has occurred while mounting SPIFFS");
    }

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/Teste1.html", String(), false, index_config_processor);
        ESP_LOGI(TAG, "A Client has been Connected");
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        if (request->hasParam("wifi_ssid") && request->hasParam("wifi_password") &&
            request->hasParam("mqtt_ip") && request->hasParam("mqtt_port")){
            ssid = request->getParam("wifi_ssid")->value();
            pass = request->getParam("wifi_password")->value();
            mqtt_server_ip = request->getParam("mqtt_ip")->value();
            mqtt_server_port = (request->getParam("mqtt_port")->value()).toInt();
            request->send(200, "text/plain", "Connecting");
            WIFI_CONNECTION_STATUS = READY_TO_CONNECT;
            MQTT_CONNECTION_STATUS = READY_TO_CONNECT;
        }
        else {
            request->send(SPIFFS, "/Teste1.html", String(), false, index_config_processor);
        }
    });

    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

    server.on("/Aguas-da-integracao", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/Aguas-da-integracao.png", "image/png");
    });


    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });


    server.begin();
    ESP_LOGD(TAG, "Web Server successfully activated");
}

void running_server(){
    ESP_LOGD(TAG, "Entering Web Server Running Mode");

    if(!SPIFFS.begin(true)){
        ESP_LOGE(TAG, "An Error has occurred while mounting SPIFFS");
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/Teste2.html", String(), false, index_running_processor);
        ESP_LOGI(TAG, "A Client has been Connected");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){
        blinds_up(0);
        request->send(SPIFFS, "/Teste2.html", String(), false, index_running_processor);
    });

    server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request){
        blinds_down(0);
        request->send(SPIFFS, "/Teste2.html", String(), false, index_running_processor);
    });

    server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
        blinds_stop(0);
        request->send(SPIFFS, "/Teste2.html", String(), false, index_running_processor);
    });

    server.on("/Aguas-da-integracao", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/Aguas-da-integracao.png", "image/png");
    });


    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Not found");
    });


    server.begin();
    ESP_LOGD(TAG, "Web Server successfully activated");
}