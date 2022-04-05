//
// Created by victo on 06/10/2021.
//

#ifndef BLINDS_WEBSERVERS_H
#define BLINDS_WEBSERVERS_H

#include <ESPAsyncWebServer.h>

/**
 * Text processor for the HTML page
 * @param var   Don't worry about it
 * @return      Don't worry about it
 */
String index_config_processor(const String& var);

/**
 * Text processor for the HTML page
 * @param var   Don't worry about it
 * @return      Don't worry about it
 */
String index_running_processor(const String& var);

/**
 * Initialize the WebServer which contains all the routes for configure the device for the first time
 */
void startup_server();

/**
 * Initialize the WebServer which contains all the routes to control the device
 */
void running_server();

#endif //BLINDS_WEBSERVERS_H
