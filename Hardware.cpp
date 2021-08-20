#include <Arduino.h>
#include "esp_system.h"

static const char* TAG = "Hardware";

String get_device_name(){
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X%02X", baseMac[0], baseMac[5]);
	return String(baseMacChr);
}

void blinds_down(){

}

void blinds_up(){

}

void blinds_stop(){
	
}