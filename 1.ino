#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoOTA.h>
#include <RotaryEncoder.h>
#include <EEPROM.h>
#include <Preferences.h>

//Pinos dos Dispotivos
#define BUTTON_01	  2 
#define BUTTON_02	  3
#define BUTTON_03  	4
#define pHa 		    18
#define pHb 		    19
//Fim da definição de pinos

//Configurações
#define DEFAULT_BROKER_IP ""
#define DEFAULT_BROKER_PASS ""
#define DEFAULT_BROKER_USER ""
#define DEAFULT_BROKER_PORT ""

#define ACTIVATED LOW
#define DEACTIVATED HIGH
//Fim das configurações

//Configurações:
#define SSID_NAME "" //Alterar para nome da Rede Sem Fio
#define WIFI_PASSWORD "" //Alterar com a senha da Rede Sem Fio
//Fim das configurações

//Protótipos de Funções:
void callbackMqtt(char* topic, byte* message, unsigned int lenght);

//Variáveis Globais:
PubSubClient mqttClient(DEFAULT_BROKER_IP, DEFAULT_BROKER_PORT, callbackMqtt, wifiClient);
RotaryEncoder encoder(32, 33);
WiFiClient client;
Preferences flash;

int encoderPosition = 0;
//Fim das variáveis globais

void setup(){
	//Inicializações
	Serial.begin(115200); 		//Serial
	flash.begin("net-cred"); 	//EEPROM
	
	String ssid = getString("ssid", "");
	if(ssid == ""){
		//Iniciar WebServer\
	}
	String pass = getString("ssid", "");
	flash.end();
	
    //Iniciando WiFi
	WiFi.hostname("Smart-PTI Blinds Controller");
	WiFiMulti.addAP(ssid.c_str(), pass.c_str()); //Adiciona um ponto de conexão para o ESP
	Serial.println();
	Serial.print("WiFi - Connected to: ");
	Serial.println(ssid);  

	//Aguarda por conexão WiFi
	while(WiFiMulti.run() != WL_CONNECTED) {
		Serial.print(".");
	}

	//Se conectar, imprime a rede conectada no monitor e faz um sinal sonoro
	if(WiFiMulti.run() == WL_CONNECTED) {
		Serial.println("");
		Serial.print("WiFi Connected. ");
		Serial.print("IP Addres: ");
		Serial.println(WiFi.localIP());
	}

	//Inicilização do Over The Air Updater
	ArduinoOTA
    .onStart([]() {
    	String type;
    	if (ArduinoOTA.getCommand() == U_FLASH)
    		type = "sketch";
    	else // U_SPIFFS
    		type = "filesystem";

    	// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    	Serial.println("Start updating " + type);
    })
    .onEnd([]() {
    	Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
    	Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
    	Serial.printf("Error[%u]: ", error);
    	if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    	else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    	else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    	else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    	else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  	ArduinoOTA.begin();

	//Inicialização dos pinos
    pinMode(BUTTON_01, INPUT); //Botão 1
    pinMode(BUTTON_02, INPUT); //Botão 2
    pinMode(BUTTON_03, INPUT); //Botão 3

  	pinMode(pHa, OUTPUT);
 	pinMode(pHb, OUTPUT);

	//Definição de Posição do Motor
	X = EEPROM.read(0);
	if(X != 0) encoderPosition = X;	
}

void loop(){
	while(digitalRead(BUTTON_01) == ACTIVATED){
		wDown();
		encoder.tick();
		encoderPosition = encoder.getPosition();
	}

	while(digitalRead(BUTTON_02) == ACTIVATED){
		wUp();
		encoder.tick();
		encoderPosition = encoder.getPosition();
	}

	if(digitalRead(BUTTON_03) == ACTIVATED){
		wStop();
		encoder.tick();
		encoderPosition = encoder.getPosition();
	}

	if (!mqttCliennt.connect()){
		mqtt_reconnect();
	}
	mqttClient.loop();
}

void mqtt_reconnect() {
    Serial.println("Connecting to Broker...");
    while (!mqttClient.connected()) {
        Serial.println("Reconnecting\n");

        if (mqttClient.connect("Somente_teste", DEFAULT_BROKER_USER, DEFAULT_BROKER_PASS)) {
            Serial.println("Connected.");
            // subscribe to topic
            mqttClient.subscribe("persiana");
        }
    }
}

void callbackMqtt(char* topic, byte* message, unsigned int length) {
    Serial.println("\n\nData Received");
    if (!strcmp(topic, "persiana")) {
        char buffer[length + 1];
        for(int i = 0; i < length; i++) {
            buffer[i] = message[i];
        }
        buffer[length] = '\0';

        if (!strcmp(buffer, "0001")) {
            Serial.println("Window Down\n");
			wDown();
        } else if (!strcmp(buffer, "0002")) {
            Serial.println("Windows Up\n");
			wUp();
        } else else if (!strcmp(buffer, "0003")){
			Serial.println("Windows Down\n");
			wStop();
        }
    }
}

// Função leitura encoder ---------------------------------------------------
void encodUpdate () {
  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  
  //Se a posicao foi alterada, mostra o valor no Serial Monitor
  if (pos < newPos) {
    pos = newPos;
    X -= 1;
    inicialTime = millis();
    verify = 1;
    Serial.println(X);
  } else if (pos > newPos) {
    pos = newPos;
    X += 1;
    inicialTime = millis();
    verify = 1;
    Serial.println(X);
  }
  
  // Teste para salvar no EEPROM
  currentTime = millis();
  if (((inicialTime + waitSeconds * 1000) <= currentTime) && verify == 1) {
    Serial.println("Salvo");
    if (X != EEPROM.read(0)) {
      Serial.print(X);
      Serial.println(" Atualizado");
      EEPROM.write(0, X);
      EEPROM.commit();
    }
    verify = 0;
  }
}

// Funções individuais - Desce - Sobe - Para --------------------------------
void wDown () {
  digitalWrite(pHa, HIGH);
  digitalWrite(pHb, LOW);
}

void wUp () {
  digitalWrite(pHa, LOW);
  digitalWrite(pHb, HIGH);
}

void wStop () {
  digitalWrite(pHa, LOW);
  digitalWrite(pHb, LOW);
}