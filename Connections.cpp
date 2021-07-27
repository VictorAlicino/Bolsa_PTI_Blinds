#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <DNSServer.h>
#include "Connections.h"
#include "WebServers.h"

extern String device_name;
extern Preferences flash;
extern PubSubClient mqttClient;
extern DNSServer dnsServer;
static const char* TAG = "Connections";

void wifi_connect(String ssid, String password){
    try{
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        ESP_LOGD(TAG, "Connecting to WiFi");
        const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;

        //Aguardando a primeira tentativa de conexão
        vTaskDelay(xDelay);

        //Realizando mais tentativas 
        int attemps = 5;
        for(int counter = 0; WiFi.status() == WL_CONNECTED && counter < attemps; counter++){
            vTaskDelay(xDelay);
            ESP_LOGE(TAG, "Connection Failed! %d Attemps remaining!", attemps - counter);
            ESP_LOGD(TAG, "Retrying...");
        }
        if(WiFi.status() != WL_CONNECTED){
            ESP_LOGE(TAG, "Connection failed after 10 attemps.");
            throw network_connection_error();
        }else{
            ESP_LOGD(TAG, "Connected");
            //flash.putString("wifi_ssid", ssid);
            //flash.putString("wifi_password", password);
            ESP_LOGD(TAG, "WiFi Credentials has been written in memory");
        }
    }catch(std::exception& e){
        ESP_LOGE(TAG, "Network Connection Error -> Throwing Exception.");
        
        throw e;
    }
}

IPAddress activate_internal_wifi(){
    ESP_LOGD(TAG, "Initializing Internal Wireless Netowrk");
    String name = "Persiana Inteligente ";
    name = name + "(" + device_name + ")";
    WiFi.softAP(name.c_str(), NULL);
    IPAddress IP = WiFi.softAPIP();
    dnsServer.start(53, "*", IP);
    ESP_LOGD(TAG, "Starting DNS Server");
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(name.c_str());
    ESP_LOGI(TAG, "Configuration Access Point set on %s", IP.toString().c_str());
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
        ESP_LOGE(TAG, "MQTT Connection Error -> Throwing Exception.");
        throw e;
    }
}

void mqtt_callback(char* topic, byte* message, unsigned length){
    ESP_LOGD(TAG, "Data Received");
	
	if(!strcmp(topic, "0001")){
        char buffer[length + 1];
        for(int i = 0; i < length; i++) {
            buffer[i] = message[i];
        }
        buffer[length] = '\0';

        if (strcmp(buffer, "0001") == 0) {
            ESP_LOGD(TAG, "Abrindo Persiana");
            //TODO
        }else if (strcmp(buffer, "0002") == 0) {
            ESP_LOGD(TAG, "Fechando Persiana");
            //TODO
        }else if (strcmp(buffer, "0003") == 0){
            ESP_LOGD(TAG, "Parando Imediatamente");
            digitalWrite(2, LOW);
        }else{
            ESP_LOGE(TAG, "Não foi possível processar a mensagem");
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