#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

String processor(const String& var);

AsyncWebServer startup_server();

#endif