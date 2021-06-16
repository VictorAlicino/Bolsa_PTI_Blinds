#include <WiFi.h>
#include <ArduinoOTA.h>
#include <RotaryEncoder.h>
#include <EEPROM.h>
#include <Preferences.h>
#include <PubSubClient.h>

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
//Fim das Variáveis Globais

void setup(){
    //Inicializações
	Serial.begin(115200); 	//Serial

    flash.begin("config");
    bool first_boot = getBool("first_boot", true);
    if(first_boot){
        
    }
}

void loop(){

}