#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

//Configurações
#define NUMBER_OF_BLINDS_SUPPORTED 3
#define ENCODER_1_PIN_A
#define ENCODER_1_PIN_B
#define MOTOR_1_PIN_A
#define MOTOR_1_PIN_B
#define REED_SWITCH_1 

#if NUMBER_OF_BLINDS_SUPPORTED < 1
    #error At least 1 blind is required
#endif
#if NUMBER_OF_BLINDS_SUPPORTED > 4
    #error Blinds exceeds the limit of 4
#endif
#if NUMBER_OF_BLINDS_SUPPORTED >= 2
    #define ENCODER_2_PIN_A
    #define ENCODER_2_PIN_B
    #define MOTOR_2_PIN_A
    #define MOTOR_2_PIN_B
    #define REED_SWITCH_2 
#elif NUMBER_OF_BLINDS_SUPPORTED >= 3
    #define ENCODER_3_PIN_A
    #define ENCODER_3_PIN_B
    #define MOTOR_3_PIN_A
    #define MOTOR_3_PIN_B
    #define REED_SWITCH_3
#elif NUMBER_OF_BLINDS_SUPPORTED >= 4
    #define ENCODER_4_PIN_A
    #define ENCODER_4_PIN_B
    #define MOTOR_4_PIN_A
    #define MOTOR_4_PIN_B
    #define REED_SWITCH_4    
#endif

#define ACTIVATED LOW
#define DEACTIVATED HIGH
 //Fim das configurações

enum CONNECTION_STATUS{
    NOT_READY,
    READY_TO_CONNECT,
    CONNECTED
};

enum MESSAGES_IDENTIFIERS{
    SERVER_RECEIVED         = 0x00,
    DEVICE_SEND             = 0X01,
    DEVICE_GENERIC_ERROR    = 0x02,
};

enum MODE{
    BLINDS_STOP         = 0X00,
    BLINDS_UP           = 0X01,
    BLINDS_DOWN         = 0X02,
    BLINDS_LINEAR_VALUE = 0x03,
};

typedef struct BLINDS_AVAILABLE_PIN{
    uint8_t encoder_pin_A;
    uint8_t encoder_pin_B;
    uint8_t motor_pin_A;
    uint8_t motor_pin_B;
    uint8_t rspin;
}Blinds_Pinout;

extern Blinds_Pinout blinds_pin[NUMBER_OF_BLINDS_SUPPORTED];

String get_device_name();

void blinds_down();

void blinds_up();

void blinds_stop();

#endif