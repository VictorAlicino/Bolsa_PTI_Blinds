#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Replaces placeholder with LED state value
String processor(const String& var){
}

AsyncWebServer startup_server(String ssid, String password){
    AsyncWebServer server(80);

    if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    }

    String wifi_ssid;
    String wifi_pass;
    String broker_ip;
    String broker_password;

    const char* PARAM_INPUT_1 = "input1";
    const char* PARAM_INPUT_2 = "input2";
    const char* PARAM_INPUT_3 = "input3";
    const char* PARAM_INPUT_4 = "input4";

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/Teste1.html", String(), false, processor);
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
      if (request->hasParam(PARAM_INPUT_1)) {
        &wifi_ssid = request->getParam(PARAM_INPUT_1)->value();
      }
      else if (request->hasParam(PARAM_INPUT_2)) {
        &wifi_pass = request->getParam(PARAM_INPUT_2)->value();
      }
      else if (request->hasParam(PARAM_INPUT_3)) {
        &broker_ip = request->getParam(PARAM_INPUT_3)->value();
      }
      else if (request->hasParam(PARAM_INPUT_4)) {
        &broker_password = request->getParam(PARAM_INPUT_4)->value();
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