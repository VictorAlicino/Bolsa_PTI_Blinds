//
// Created by victo on 01/10/2021.
//

#ifndef SMART_BLINDS_USERCONFIG_H
#define SMART_BLINDS_USERCONFIG_H

// ------------------ Wi-Fi Configurations  ----------------- //
/**
 * Change the SELF_WIFI Macro to "true" if you want the user to input the Wi-Fi Credentials via Webserver
 * or change to false in order to set the Wi-Fi Credentials in compilation time, be advised that error caused by
 * connection errors will not be treated in the SELF_WIFI false Mode. Be careful.
 */
#define SELF_WIFI           true
#define WIFI_SSID           "LabIoT"
#define WIFI_PASSWORD       "labiot2020."

/**
 * The WIFI_ATTEMPTS Macro will define how many times the device should try to connect to Wi-Fi before communicate
 * a failure.
 * This Macro is an Integer. Example: WIFI_ATTEMPTS 5
 */
#define WIFI_ATTEMPTS       2
// -------------- end of WiFi Configurations  -------------- //


// ------------------ MQTT Configurations  ----------------- //
/**
 * Change the SELF_MQTT Macro to "true" if you want the user to input the MQTT Credentials via Webserver
 * or change to false in order to set the MQTT Credentials in compilation time, be advised that error caused
 * by connection errors will not be treated in the SELF_MQTT false Mode. Be careful.
 */
#define SELF_MQTT               true

//If the MQTT Network requires Log-in via User and Password, set this variable to "true"
#define IS_MQTT_LOGIN_REQUIRED  false;

#define MQTT_USER               ""
#define MQTT_PASSWORD           ""
#define MQTT_SERVER             ""
#define MQTT_SERVER_PORT        1883

#define MQTT_TOPIC_to_SUB       "0001"
#define MQTT_TOPIC_to_PUB       "SMARTOFFICE_COMM"

/**
 * The MQTT_ATTEMPTS Macro will define how many times the device should try to connect to the broker before communicate
 * a failure.
 * This Macro is an Integer. Example: MQTT_ATTEMPTS 5
 */
#define MQTT_ATTEMPTS       5
// -------------- end of MQTT Configurations  -------------- //


// ------------------ Blinds Configurations  ----------------- //
#define NUMBER_OF_BLINDS_SUPPORTED  1   /** Change in order to modify the number of Blinds supported by the device */
#define ENCODER_0_PIN_A             32  /** Encoder 0 First Pin */
#define ENCODER_0_PIN_B             33  /** Encoder 0 Second Pin */
#define MOTOR_0_PIN_A               18  /** Motor 0 First Pin */
#define MOTOR_0_PIN_B               19  /** Motor 0 Second Pin */
#define REED_SWITCH_0               4   /** Reed switch 0 Pin */


#if NUMBER_OF_BLINDS_SUPPORTED < 1
#error At least 1 blind is required
#endif
#if NUMBER_OF_BLINDS_SUPPORTED > 4
#error Blinds exceeds the limit of 4
#endif
#if NUMBER_OF_BLINDS_SUPPORTED >= 2
    #define ENCODER_1_PIN_A         (void *) 0  /** Encoder 1 First Pin */
    #define ENCODER_1_PIN_B         (void *) 0  /** Encoder 1 Second Pin */
    #define MOTOR_1_PIN_A           (void *) 0  /** Motor 1 First Pin */
    #define MOTOR_1_PIN_B           (void *) 0  /** Motor 1 Second Pin */
    #define REED_SWITCH_1           (void *) 0  /** Reedswitch 1 Pin */
#endif
#if NUMBER_OF_BLINDS_SUPPORTED >= 3
#define ENCODER_2_PIN_A         (void *) 0  /** Encoder 2 First Pin */
    #define ENCODER_2_PIN_B         (void *) 0  /** Encoder 2 Second Pin */
    #define MOTOR_2_PIN_A           (void *) 0  /** Motor 2 First Pin */
    #define MOTOR_2_PIN_B           (void *) 0  /** Motor 2 Second Pin */
    #define REED_SWITCH_2           (void *) 0  /** Reedswitch 2 Pin */
#endif
#if NUMBER_OF_BLINDS_SUPPORTED == 4
#define ENCODER_3_PIN_A         (void *) 0  /** Encoder 3 First Pin */
    #define ENCODER_3_PIN_B         (void *) 0  /** Encoder 3 Second Pin */
    #define MOTOR_3_PIN_A           (void *) 0  /** Motor 3 First Pin */
    #define MOTOR_3_PIN_B           (void *) 0  /** Motor 3 Second Pin */
    #define REED_SWITCH_3           (void *) 0  /** Reedswitch 3 Pin */
#endif
// -------------- end of Blinds Configurations  -------------- //

//TODO REMOVE BEFORE PRODUCTION
#if NUMBER_OF_BLINDS_SUPPORTED > 1
#error More than one blind are not currently supported
#endif

#endif //SMART_BLINDS_USERCONFIG_H
