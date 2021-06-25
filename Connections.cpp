#include <Arduino.h>
#include <WiFi.h>
#include "Connections.h"
#include "WebServers.h"

void wifi_connect(String ssid, String password){
    try{
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());

        Serial.println("Connecting to WIFI...");
        for(int counter = 0; WiFi.status() != WL_CONNECTED || counter < 10; counter++){
            unsigned long start_time = millis();
            unsigned long now_time = millis();
            while((start_time - now_time) != 1000){
                Serial.print(".");
                now_time = millis();
            }
            Serial.println("Connection Failed!");
            Serial.println("Retrying...");
        }
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("Connection failed after 10 retries.");
            throw networkConnectionError();
        }else{
            Serial.println("Connected.");
        }
    }catch(std::exception& e){
        throw;
    }
}

IPAddress activate_internal_wifi(){
    WiFi.softAP("Persiana Inteligente (0001)", NULL, NULL, 0, 1);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("Configuration Access Point set on ");
    Serial.println(IP);
    return IP;
}