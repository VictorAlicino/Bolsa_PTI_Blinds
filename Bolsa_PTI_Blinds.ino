#include <RotaryEncoder.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "WebServers.h"
#include "Connections.h"
#include "Hardware.h"

//Pinos dos Dispotivos
#define BUTTON_01 2 
#define BUTTON_02 3
#define BUTTON_03 4
#define pHa 18
#define pHb 19
//Fim da definição de pinos

//Configurações
#define ACTIVATED LOW
#define DEACTIVATED HIGH
 //Fim das configurações

//Variáveis Globais
PubSubClient mqttClient(*(new WiFiClient()));
RotaryEncoder encoder(32, 33);
Preferences flash;
String device_name;
DNSServer dnsServer;
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
        AsyncWebServer server = startup_server();
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
                }catch(...){
                    ESP_LOGD("MQTT Connection Error");
                }
            }
            dnsServer.processNextRequest();
            mqttClient.loop();
        }
        
        //Ativando Bluetooth
    }
}

void loop(){
    //TODO
}
