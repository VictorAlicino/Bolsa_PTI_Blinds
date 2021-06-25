#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "WebServers.h"
#include "Connections.h"

// Replaces placeholder with LED state value
String processor(const String& var){
}

AsyncWebServer startup_server(){
    AsyncWebServer server(80);

    if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    }

    String wifi_ssid;
    String wifi_pass;
    String broker_ip;
    String broker_password;

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/Teste1.html", String(), false, processor);
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [&wifi_ssid, &wifi_pass, &broker_ip, &broker_password] (AsyncWebServerRequest *request) {
      // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
      if (request->hasParam("input1")) {
        wifi_ssid = request->getParam("input1")->value();
      }
      else if (request->hasParam("input2")) {
        wifi_pass = request->getParam("input2")->value();
      }
      else if (request->hasParam("input3")) {
        broker_ip = request->getParam("input3")->value();
      }
      else if (request->hasParam("input4")) {
        broker_password = request->getParam("input4")->value();
      }
      else {
        request->send(SPIFFS, "/Teste1.html", String(), false, processor);
      }

      try{
        wifi_connect(wifi_ssid, wifi_pass);
      }catch(std::exception& e){
        request->send(SPIFFS, "/Teste1.html", String(), false, processor);
      }
    });
}