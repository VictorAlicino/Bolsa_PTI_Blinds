#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "WebServers.h"
#include "Connections.h"

// Replaces placeholder with LED state value
String processor(const String& var){
}

AsyncWebServer startup_server(){
	Serial.println("Entering Web Server Configuration Mode");
    AsyncWebServer server(80);
	
	if(!SPIFFS.begin(true)){
		Serial.print("└───");
    	Serial.println("An Error has occurred while mounting SPIFFS");
  	}

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
		String wifi_ssid;
    	String wifi_pass;
    	String server_ip;
		int server_port;

		if (request->hasParam("wifi_ssid") && request->hasParam("wifi_password") &&
			request->hasParam("mqtt_ip") && request->hasParam("mqtt_port")){
			wifi_ssid = request->getParam("wifi_ssid")->value();
			wifi_pass = request->getParam("wifi_password")->value();
			server_ip = request->getParam("mqtt_ip")->value();
			server_port = (request->getParam("mqtt_port")->value()).toInt();

			try{
        		const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
				vTaskDelay(xDelay);
				wifi_connect(wifi_ssid, wifi_pass);

				mqtt_connect(server_ip, server_port);
				request->send(202, "text/plain", "Connected");

			}catch(...){
				request->send(SPIFFS, "/Teste1.html", String(), false, processor);
			}
    	}
		else {
    		request->send(SPIFFS, "/Teste1.html", String(), false, processor);
    	}
    });

	server.onNotFound([](AsyncWebServerRequest *request){
    	request->send(404, "text/plain", "Not found");
    });
	

	server.begin();
	Serial.print("└───");
	Serial.println("Web Server successfully activated");

	return server;
}