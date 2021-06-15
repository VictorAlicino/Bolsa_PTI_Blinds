#include <Arduino.h>
#include <WiFi.h>

void wifi_connect(String ssid = nullptr, String password = nullptr){
    try:
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());

        Serial.println("Connecting to WIFI...");
        for(counter = 0; WiFi.status() != WL_CONNECTED || counter < 10; counter++){
            unsigned long start_time = millis();
            unsigned long now_time = millis();
            while((start_time - now_time) != 1000){
                Serial.print(".");
            }
            Serial.println("Connection Failed!");
            Serial.println("Retrying...");
        }
        if(Wifi.status() != WL_CONNECTED){
            Serial.println("Connection failed after 10 retries.");
            throw networkConnectionError;
        }else{
            Serial.println("Connected.");
        }
    catch(networkConnectionError){
        throw;
    }
}

IPAddress activate_internal_wifi(){
    WiFi.softAP(ssid = "Persiana Inteligente (0001)", max_connection = 1);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("Configuration Access Point set on ");
    Serial.println(IP);
    return IP;
}