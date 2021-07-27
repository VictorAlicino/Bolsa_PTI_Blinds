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
PubSubClient mqttClient;
RotaryEncoder encoder(32, 33);
Preferences flash;
String device_name;
DNSServer dnsServer;
static const char* TAG = "Main";
//Fim das Variáveis Globais

void setup(){
    //Inicializações
	Serial.begin(115200); Serial.println();

    //Carregar linhas de configurações
    flash.begin("config");

    //Em caso de ser o primeiro boot, a placa entrará em modo de configuração inicial
    bool first_boot = flash.getBool("first_boot", true);
    if(first_boot == true){
        ESP_LOGD(TAG, "This device is not configured yet");
        
        //Definindo o nome
        device_name = get_device_name();
        flash.putString("device_name", device_name);
        ESP_LOGD(TAG, "Device Name: %s", device_name);
        

        //Ativando Web Server
        IPAddress IP = activate_internal_wifi();
        AsyncWebServer server = startup_server();

        while(true){
            dnsServer.processNextRequest();
        }
        
        //Ativando Bluetooth
    }
}

void loop(){
    //TODO
}