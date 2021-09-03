#include <RotaryEncoder.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "WebServers.h"
#include "Connections.h"
#include "Hardware.h"

//Pinos dos Dispotivos

//Fim da definição de pinos

//Variáveis Globais
PubSubClient mqttClient(*(new WiFiClient()));
RotaryEncoder encoder[NUMBER_OF_BLINDS_SUPPORTED];
Preferences flash;
String device_name;
DNSServer dnsServer;
AsyncWebServer server(80);
String ssid;
String pass;
String mqtt_server_ip;
int mqtt_server_port;
String mqtt_user;
String mqtt_password;
static const char* TAG = "Main";
//Fim das Variáveis Globais

//FLAGS Globais
int WIFI_CONNECTION_STATUS;
int MQTT_CONNECTION_STATUS;
//Fim das FLAGS Globais


void setup(){
    //Inicializações
	Serial.begin(115200); Serial.println();

    //Carregar linhas de configurações
    flash.begin("config");

    //Em caso de ser o primeiro boot, a placa entrará em modo de configuração inicial
    bool first_boot = flash.getBool("first_boot", true);
    if(first_boot == true){
        ESP_LOGD(TAG, "This device is not configured yet");
        WIFI_CONNECTION_STATUS = NOT_READY;

        //Definindo o nome
        device_name = get_device_name();
        flash.putString("device_name", device_name);
        ESP_LOGD(TAG, "Device Name: %s", device_name);

        //Ativando Web Server
        IPAddress IP = activate_internal_wifi();
        startup_server();

        //Ativando Bluetooth
        //TODO Bluetooth

        //Loop de Configuração
        while(true){
            if(WIFI_CONNECTION_STATUS == READY_TO_CONNECT){
                try{
                    wifi_connect();
                }catch(...){
                    ESP_LOGD("WiFi Connection Error");
                }
            }
            if((WIFI_CONNECTION_STATUS == CONNECTED) && (MQTT_CONNECTION_STATUS == READY_TO_CONNECT)){
                try{
                    mqtt_connect();
                    flash.putBool("first_boot", false);
                    mqttClient.disconnect();
                    flash.end();
                    server.end();
                    ESP.restart();
                }catch(...){
                    ESP_LOGD("MQTT Connection Error");
                }
            }
            dnsServer.processNextRequest();
            mqttClient.loop();
        }
        
    }else{
        ESP_LOGD(TAG, "Device already configured");

        //Definindo o nome
        device_name = flash.getString("device_name", get_device_name());
        String name = "Persiana Inteligente ";
        name = name + "(" + device_name + ")";
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        WiFi.setHostname(name.c_str());
        ESP_LOGD(TAG, "Device Name: %s", device_name);
        
        //Definindo credenciais de conexão
        ssid = flash.getString("wifi_ssid", "");
        pass = flash.getString("wifi_password", "");
        mqtt_server_ip = flash.getString("mqtt_ip", "");
        mqtt_server_port = flash.getInt("mqtt_port", 0);
        
        //Conectando WIFI
        try{
            wifi_connect();
        }catch(...){
            ESP_LOGD("WiFi Connection Error");
            flash.putBool("first_boot", true);
            ESP.restart();
        }
        
        //Conectando MQTT
        try{
            mqtt_connect();
        }catch(...){
            ESP_LOGD("MQTT Connection Error");
            flash.putBool("first_boot", true);
            ESP.restart();
        }
        running_server();
    }
}

void loop(){
    mqttClient.loop();
}
