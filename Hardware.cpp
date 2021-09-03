#include <Arduino.h>
#include <PubSubClient.h>
#include <RotaryEncoder.h>
#include "Hardware.h"
#include "esp_system.h"

extern PubSubClient mqttClient;
extern String device_name;
extern RotaryEncoder encoder[NUMBER_OF_BLINDS_SUPPORTED];;
static const char* TAG = "Hardware";

Blinds_Pinout blinds_pin[NUMBER_OF_BLINDS_SUPPORTED];

void Blinds_Init(){
	blinds_pin[0].encoder_pin_A = 
}

String get_device_name(){
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X%02X", baseMac[0], baseMac[5]);
	return String(baseMacChr);
}

void blinds_down(){
	ESP_LOGD("Blinds Down");

	String first = String(DEVICE_SEND);
	if(first.length() < 2){
		first += first.charAt(0);
		first[0] = '0';
	}

	String second = device_name;

	String third = String(BLINDS_DOWN);
	if(third.length() < 2){
		third += third.charAt(0);
		third[0] = '0';
	}

	String fourth = "00";

	String message = first + second + third + fourth;
	mqttClient.publish("0001", message.c_str());
}

void blinds_up(){
	ESP_LOGD("Blinds Up");

	String first = String(DEVICE_SEND);
	if(first.length() < 2){
		first += first.charAt(0);
		first[0] = '0';
	}

	String second = device_name;

	String third = String(BLINDS_UP);
	if(third.length() < 2){
		third += third.charAt(0);
		third[0] = '0';
	}

	String fourth = "00";

	String message = first + second + third + fourth;
	mqttClient.publish("0001", message.c_str());

}

void blinds_stop(){
	ESP_LOGD("Blinds Stop");

	String first = String(DEVICE_SEND);
	if(first.length() < 2){
		first += first.charAt(0);
		first[0] = '0';
	}

	String second = device_name;

	String third = String(BLINDS_STOP);
	if(third.length() < 2){
		third += third.charAt(0);
		third[0] = '0';
	}

	String fourth = "00";

	String message = first + second + third + fourth;
	mqttClient.publish("0001", message.c_str());
	
}

