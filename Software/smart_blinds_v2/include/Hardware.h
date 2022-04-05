#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <RotaryEncoder.h>
#include <UserConfig.h>


//Configurações
#define ACTIVATED HIGH 
#define DEACTIVATED LOW
//Fim das configurações


enum CONNECTION_STATUS{
    NOT_READY,
    READY_TO_CONNECT,
    CONNECTED
};

enum DEVICE_STATUS{
    IN_PROGRESS,
    READY,
    DONE,
    FAILURE,
    CRITICAL_FAILURE
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

/**
 * @brief Data type representation of a Blind and its components
 */
typedef struct BLINDS_DATA{
    RotaryEncoder *encoder;
    uint8_t motor_A;
    uint8_t motor_B;
    uint8_t reed_switch;
}Blind;

/**
 * @brief Get the device name
 * 
 * @return String The first two and the last two characters of the MAC Address
 */
String get_device_name();

/**
 * @brief Checks if the actual position matches the requested position. If they differ, the function calls the Up or Down functions
 * to adjust the blind to the desired position.
 * 
 */
void blind_check_position();

/**
 * @brief Moves the Blinds Down
 * 
 * @param blind_id ID of the desired blind in the device
 */
void blinds_down(uint8_t blind_id);

/**
 * @brief Moves the Blinds Up
 * 
 * @param blind_id ID of the desired blind in the device
 */
void blinds_up(uint8_t blind_id);

/**
 * @brief Stop the Blinds from moving
 * 
 * @param blind_id ID of the desired blind in the device
 */
void blinds_stop(uint8_t blind_id);

/**
 * @brief Set all the hardware connected for use
 * @details All the devices attached to pins in the device will be somehow prepared for use and the Blind will reset its
 * own position in order to guarantee a precise control
 * 
 */
void activate_hardware();

#endif