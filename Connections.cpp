#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include "Connections.h"
#include "WebServers.h"

extern String device_name;
extern Preferences flash;
extern PubSubClient mqttClient;

void wifi_connect(String ssid, String password){
    try{
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        Serial.println("Connecting to WIFI");

        //Aguardando a primeira tentativa de conexão
        unsigned long start_time = millis();
        unsigned long now_time = millis();
        while((now_time - start_time) != 5000){
                now_time = millis();
        }

        //Realizando mais tentativas 
        int attemps = 5;
        for(int counter = 0; WiFi.status() != WL_CONNECTED && counter < attemps; counter++){
            start_time = millis();
            now_time = millis();
            while((now_time - start_time) != 5000){
                now_time = millis();
            }
            Serial.print("└───");
            Serial.printf("Connection Failed! %d Attemps remaining!\n", attemps - counter);
            Serial.print("└───");
            Serial.println("Retrying...");
        }
        if(WiFi.status() != WL_CONNECTED){
            Serial.print("└───");
            Serial.println("Connection failed after 10 attemps.");
            throw network_connection_error();
        }else{
            Serial.print("└───");
            Serial.println("Connected.");
            //flash.putString("wifi_ssid", ssid);
            //flash.putString("wifi_password", password);
            Serial.print("    └───");
            Serial.println("WiFi Credentials has been written in memory");
        }
    }catch(std::exception& e){
        Serial.print("└───");
        Serial.println("Network Connection Error -> Throwing Exception.");
        throw e;
    }
}

IPAddress activate_internal_wifi(){
    Serial.println("Initializing Internal Wireless Netowrk");
    String name = "Persiana Inteligente ";
    name = name + "(" + device_name + ")";
    WiFi.softAP(name.c_str(), NULL);
    IPAddress IP = WiFi.softAPIP();
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(name.c_str());
    Serial.print("└───");
    Serial.print("Configuration Access Point set on ");
    Serial.println(IP);
    return IP;
}

bool mqtt_connect(String server, int port){
    try{
        mqttClient.setServer(server.c_str(), port);

        if(mqttClient.connected() != true){
            throw mqtt_connection_error();
        }
        
        WiFiClient client;
        mqttClient.connect("0001");
        mqttClient.setCallback(mqtt_callback);
        mqttClient.setClient(client);

    }
    catch(std::exception& e){
        Serial.println("MQTT Connection Error -> Throwing Exception.");
        throw e;
    }
}

void mqtt_callback(char* topic, byte* message, unsigned length){
	Serial.println("\nData Received");
	
	if(!strcmp(topic, "0001")){
        char buffer[length + 1];
        for(int i = 0; i < length; i++) {
            buffer[i] = message[i];
        }
        buffer[length] = '\0';

        if (strcmp(buffer, "0001") == 0) {
            Serial.println("Abrindo Persiana");
            //TODO
        }else if (strcmp(buffer, "0002") == 0) {
            Serial.println("Fechando Persiana");
            //TODO
        }else if (strcmp(buffer, "0003") == 0){
            Serial.println("Parando Imediatamente");
            digitalWrite(2, LOW);
        }else{
            Serial.println("Não foi possível processar a mensagem");
        }
	}
}

String get_mac_address(){
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
	return String(baseMacChr);
}