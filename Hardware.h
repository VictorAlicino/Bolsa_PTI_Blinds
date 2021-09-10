#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <RotaryEncoder.h>


//Configurações
#define NUMBER_OF_BLINDS_SUPPORTED  1
#define ENCODER_0_PIN_A             32
#define ENCODER_0_PIN_B             33
#define MOTOR_0_PIN_A               18
#define MOTOR_0_PIN_B               19
#define REED_SWITCH_0               4


#if NUMBER_OF_BLINDS_SUPPORTED < 1
    #error At least 1 blind is required
#endif
#if NUMBER_OF_BLINDS_SUPPORTED > 4
    #error Blinds exceeds the limit of 4
#endif
#if NUMBER_OF_BLINDS_SUPPORTED >= 2
    #define ENCODER_1_PIN_A         (void *) 0
    #define ENCODER_1_PIN_B         (void *) 0
    #define MOTOR_1_PIN_A           (void *) 0
    #define MOTOR_1_PIN_B           (void *) 0
    #define REED_SWITCH_1           (void *) 0
#endif
#if NUMBER_OF_BLINDS_SUPPORTED >= 3
    #define ENCODER_2_PIN_A         (void *) 0
    #define ENCODER_2_PIN_B         (void *) 0
    #define MOTOR_2_PIN_A           (void *) 0
    #define MOTOR_2_PIN_B           (void *) 0
    #define REED_SWITCH_2           (void *) 0
#endif
#if NUMBER_OF_BLINDS_SUPPORTED == 4
    #define ENCODER_3_PIN_A         (void *) 0
    #define ENCODER_3_PIN_B         (void *) 0
    #define MOTOR_3_PIN_A           (void *) 0
    #define MOTOR_3_PIN_B           (void *) 0
    #define REED_SWITCH_3           (void *) 0
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

typedef struct BLINDS_DATA{
    RotaryEncoder *encoder;
    uint8_t motor_A;
    uint8_t motor_B;
    uint8_t reed_switch;
}Blind;

String get_device_name();

void blinds_down(uint8_t blind_id=0);

void blinds_up(uint8_t blind_id=0);

void blinds_stop(uint8_t blind_id=0);

void activate_hardware();

#endif