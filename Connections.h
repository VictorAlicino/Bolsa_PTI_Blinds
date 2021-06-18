#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <Arduino.h>
#include <WiFi.h>

struct networkConnectionError: public std::exception
{
	const char * what () const throw ()
    {
    	return "Network Connection Error";
    }
};

void wifi_connect(String ssid, String password);

IPAddress activate_internal_wifi();

#endif