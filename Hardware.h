#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

enum CONNECTION_STATUS{
    NOT_READY,
    READY_TO_CONNECT,
    CONNECTED
};

String get_device_name();

#endif