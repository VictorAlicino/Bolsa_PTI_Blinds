#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

enum CONNECTION_STATUS{
    NOT_READY,
    READY_TO_CONNECT,
    CONNECTED
};

enum MESSAGES_IDENTIFIERS{
    SERVER_RECEIVED = 0x00,
    DEVICE_SEND     = 0X01,
};

enum MODE{
    BLINDS_STOP         = 0X00,
    BLINDS_UP           = 0X01,
    BLINDS_DOWN         = 0X02,
    BLINDS_LINEAR_VALUE = 0x03,
};

String get_device_name();

void blinds_down();

void blinds_up();

void blinds_stop();

#endif