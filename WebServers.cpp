#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <PubSubClient.h>
#include "WebServers.h"
#include "Connections.h"

// Replaces placeholder with LED state value
String processor(const String& var){
}

AsyncWebServer startup_server(){
    AsyncWebServer server(80);
	
	try{
    	SPIFFS.begin(true);
	}catch(...){
		Serial.println("!!! Exception during SPIFFS Mounting !!!");
	}

    String wifi_ssid;
    String wifi_pass;
    String server_ip;
    String server_password;
	String server_port;
	
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/Teste1.html", String(), false, processor);
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [&wifi_ssid, &wifi_pass, &server_ip, &server_password, &server_port] (AsyncWebServerRequest *request) {
      if (request->hasParam("wifi_ssid") && request->hasParam("wifi_password")){
		wifi_ssid = request->getParam("wifi_ssid")->value();
		wifi_pass = request->getParam("wifi_password")->value();

		try{
			wifi_connect(wifi_ssid, wifi_pass);
			
		}catch(...){
			request->send(SPIFFS, "/Teste1.html", String(), false, processor);
		}
      }
	  else if(request->hasParam("mqtt_ip") && request->hasParam("mqtt_port")){  
		server_ip = request->getParam("mqtt_ip")->value();
		server_port = request->getParam("mqtt_port")->value();
		int port = server_port.toInt();

		mqtt_connect(server_ip, port);
	  }
	  else {
        request->send(SPIFFS, "/Teste1.html", String(), false, processor);
      }
    });
	return server;
}