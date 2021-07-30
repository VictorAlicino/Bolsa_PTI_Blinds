#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <Arduino.h>
#include <WiFi.h>

struct network_connection_error: public std::exception
{
	const char * what () const throw ()
    {
    	return "Network Connection Error";
    }
};

struct mqtt_connection_error: public std::exception
{
	const char * what () const throw ()
    {
    	return "MQTT Connection Error";
    }
};

void wifi_connect();

bool mqtt_connect(String server, int port);

IPAddress activate_internal_wifi();

void mqtt_callback(char* topic, byte* message, unsigned length);

String get_mac_address();

#endif