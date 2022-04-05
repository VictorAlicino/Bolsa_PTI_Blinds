#include <Arduino.h>
#include <PubSubClient.h>
#include <RotaryEncoder.h>
#include <Preferences.h>
#include "Hardware.h"
#include "esp_system.h"

int blind_0_actual_position;                /// Blind 0 actual position         ///
int blind_0_requested_position;             /// Blind 0 requested position      ///
int blind_0_absolute_zero;                  /// Blind 0 absolute zero position  ///
bool blind_0_moving;                        /// "Is Blind 0 moving?" flag       ///

#if NUMBER_OF_BLINDS_SUPPORTED >= 2
    int blind_1_actual_position;            /// Blind 1 actual position         ///
    int blind_1_requested_position;         /// Blind 1 requested position      ///
    int blind_1_absolute_zero;              /// Blind 1 absolute zero position  ///
    bool blind_1_moving;                    /// "Is Blind 1 moving?" flag       ///
#endif
#if NUMBER_OF_BLINDS_SUPPORTED >= 3
    int blind_2_actual_position;            /// Blind 2 actual position         ///
    int blind_2_requested_position;         /// Blind 2 requested position      ///
    int blind_2_absolute_zero;              /// Blind 2 absolute zero position  ///
    bool blind_2_moving;                    /// "Is Blind 2 moving?" flag       ///
#endif
#if NUMBER_OF_BLINDS_SUPPORTED == 4
    int blind_3_actual_position;            /// Blind 3 actual position         ///
    int blind_3_requested_position;         /// Blind 3 requested position      ///
    int blind_3_absolute_zero;              /// Blind 3 absolute zero position  ///
    bool blind_3_moving;                    /// "Is Blind 3 moving?" flag       ///
#endif

extern PubSubClient mqttClient;
extern String device_name;
extern Blind Blinds[NUMBER_OF_BLINDS_SUPPORTED];
extern Preferences flash;


String get_device_name(){
	uint8_t baseMac[6];
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X%02X", baseMac[0], baseMac[5]);
	return {baseMacChr};
}

void blind_check_position(){
	while(blind_0_actual_position != blind_0_requested_position){
		if(blind_0_actual_position < blind_0_requested_position) blinds_up(0);
		if(blind_0_actual_position > blind_0_requested_position) blinds_down(0);
	}
	if(blind_0_moving){
		blinds_stop(0);
	}
}

void blinds_down(uint8_t blind_id=0){
	ESP_LOGD("Hardware", "Blind %d Down", blind_id);

	blind_0_moving = true;

	//Activating Motors and changing direction
	digitalWrite(MOTOR_0_PIN_A, ACTIVATED);
	digitalWrite(MOTOR_0_PIN_B, DEACTIVATED);
	//End of Activating Motors and changing direction

	//MQTT Confirmation Publish
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

	String fourth = "0" + String(blind_id);

	String fifth = "00";

	if(blind_id == 0){
		fifth = String(blind_0_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#if NUMBER_OF_BLINDS_SUPPORTED >= 2
	if(blind_id == 1){
		String fifth = String(blind_1_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED >= 3
	if(blind_id == 2){
		String fifth = String(blind_2_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED == 4
	if(blind_id == 3){
		String fifth = String(blind_3_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif

	String message = first + second + third + fourth + fifth;
	mqttClient.publish("0002", message.c_str());
	//End of MQTT Confirmation Publish
}

void blinds_up(uint8_t blind_id=0){
	ESP_LOGD("Hardware", "Blind %d Up", blind_id);

	blind_0_moving = true;

	//Activating Motors and changing direction
	digitalWrite(MOTOR_0_PIN_A, DEACTIVATED);
	digitalWrite(MOTOR_0_PIN_B, ACTIVATED);
	//End of Activating Motors and changing direction

	//MQTT Confirmation Publish
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

	String fourth = "0" + String(blind_id);

	String fifth = "00";

	if(blind_id == 0){
		fifth = String(blind_0_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#if NUMBER_OF_BLINDS_SUPPORTED >= 2
	if(blind_id == 1){
		String fifth = String(blind_1_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED >= 3
	if(blind_id == 2){
		String fifth = String(blind_2_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED == 4
	if(blind_id == 3){
		String fifth = String(blind_3_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif

	String message = first + second + third + fourth + fifth;
	mqttClient.publish("0002", message.c_str());
	//End of MQTT Confirmation Publish

}

void blinds_stop(uint8_t blind_id=0){
	ESP_LOGD("Hardware", "Blind %d Stopped", blind_id);

	blind_0_moving = false;

	//Activating Motors and changing direction
	digitalWrite(MOTOR_0_PIN_A, DEACTIVATED);
	digitalWrite(MOTOR_0_PIN_B, DEACTIVATED);
	//End of Activating Motors and changing direction

	//MQTT Confirmation Publish
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

	String fourth = "0" + String(blind_id);

	String fifth = "00";

	if(blind_id == 0){
		fifth = String(blind_0_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#if NUMBER_OF_BLINDS_SUPPORTED >= 2
	if(blind_id == 1){
		String fifth = String(blind_1_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED >= 3
	if(blind_id == 2){
		String fifth = String(blind_2_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED == 4
	if(blind_id == 3){
		String fifth = String(blind_3_actual_position);
		if(fifth.length() < 2){
			fifth += first.charAt(0);
			fifth[0] = '0';
		}
	}
	#endif

	String message = first + second + third + fourth + fifth;
	mqttClient.publish("0002", message.c_str());
	//End of MQTT Confirmation Publish
	
}

void activate_hardware(){
	ESP_LOGD("Hardware", "Initializing the Hardware");

	pinMode(MOTOR_0_PIN_A, OUTPUT);
	pinMode(MOTOR_0_PIN_B, OUTPUT);
	ESP_LOGD("Hardware", "Motor 1 Initialized");

	flash.begin("blinds");
	blind_0_requested_position = flash.getInt("0_last_position", 0);
	flash.end();

    //TODO Transformar esse trecho de código em Função Paralela (Thread)
	const TickType_t xDelay = 1 / portTICK_PERIOD_MS;

	//Resetting Blinds 0 Position to 0
	blinds_up(0);

	while(digitalRead(REED_SWITCH_0 != DEACTIVATED)){
		vTaskDelay(xDelay);
	}
	blinds_stop(0);
	blind_0_actual_position = 0;
	ESP_LOGD("Hardware", "Motor 1 0 Position Set by Reed switch");
	//End of Resetting Blinds 0 Position to 0

	#if NUMBER_OF_BLINDS_SUPPORTED >= 2
		
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED >= 3
		
	#endif
	#if NUMBER_OF_BLINDS_SUPPORTED == 4
		
	#endif

}
