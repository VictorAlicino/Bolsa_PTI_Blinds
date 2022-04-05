#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

String _index_config_processor(const String& var);

String _index_running_processor(const String& var);

void startup_server();

void running_server();

#endif