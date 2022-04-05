//
// Created by victo on 01/10/2021.
//

#ifndef SMART_BLINDS_CONNECTIONS_H
#define SMART_BLINDS_CONNECTIONS_H


#include <Arduino.h>
#include <WiFi.h>

/**
 * Attempt to connect to the Wi-Fi Network using the global variables credentials
 */
void wifi_connect();

/**
 * Attempt to connect to the MQTT Server using the global variables credentials
 * @return true if the connect is established with the server
 */
bool mqtt_connect();

/**
 * Activates the Access Point on the device
 * @return The IP Address of the Access Point
 */
IPAddress activate_internal_wifi();

/**
 * MQTT Callback Function, called everytime the device receives a message via MQTT
 * @param topic     Don't worry about it
 * @param message   Don't worry about it
 * @param length    Don't worry about it
 */
void mqtt_callback(char* topic, byte* message, unsigned int length);

/**
 * Get the device Mac Address
 * @return Device's Mac Address
 */
String get_mac_address();


#endif //SMART_BLINDS_CONNECTIONS_H
