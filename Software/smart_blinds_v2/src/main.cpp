#include <Arduino.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Hardware.h>
#include <Connections.h>
#include <UserConfig.h>
#include <WebServers.h>

//Global Variables
PubSubClient mqttClient(*(new WiFiClient()));
Blind Blinds[NUMBER_OF_BLINDS_SUPPORTED];
Preferences flash;
String device_name;
DNSServer dnsServer;
AsyncWebServer server(80);
String ssid;
String pass;
String mqtt_server_ip;
int mqtt_server_port;
String mqtt_user;
String mqtt_password;
bool MQTT_LOGIN_REQUIRED = false;
//end of Global Variables

//Global FLAGS
int WIFI_CONNECTION_STATUS;
int MQTT_CONNECTION_STATUS;
int DEVICE_STATUS;
//end of Global FLAGS

void setup(){
    Serial.begin(115200); Serial.println();

    //Loading configs
    flash.begin("config");

    //First Boot control
    bool first_boot = flash.getBool("first_boot", true);
    if(first_boot){
        int CONFIGURATION_STATUS = IN_PROGRESS;
        ESP_LOGD(TAG, "FIRST BOOT DETECTED");
        ESP_LOGD(TAG, "Initializing configuration mode");

        //Setting up name
        device_name = get_device_name();
        flash.putString("device_name", device_name);
        ESP_LOGD(TAG, "Device Name: %s", device_name.c_str());
        //end of Setting up name

    #if SELF_WIFI == false
        ssid = WIFI_SSID;
        pass = WIFI_PASSWORD;
        WIFI_CONNECTION_STATUS = READY_TO_CONNECT;

        mqtt_server_ip = MQTT_SERVER;
        mqtt_server_port = MQTT_SERVER_PORT;
        MQTT_CONNECTION_STATUS = READY_TO_CONNECT;

        ESP_LOGD(TAG, "The Wi-Fi Credentials are code locked!");
        ESP_LOGD(TAG, "SSID: %s", ssid.c_str());
        ESP_LOGD(TAG, "Password: %s", pass.c_str());

        ESP_LOGD(TAG, "The MQTT Credentials are code locked!");
        ESP_LOGD(TAG, "Server IP: %s", mqtt_server_ip.c_str());
        ESP_LOGD(TAG, "Server Port: %d", mqtt_server_port);

        if(MQTT_LOGIN_REQUIRED){
            mqtt_user = MQTT_USER;
            mqtt_password = MQTT_PASSWORD;
        }

        /**
         * The system will try to connect to the Wi-Fi and MQTT the number of times defined in WIFI_ATTEMPTS Macro
         * If the connections failed in all attempts, the device will ask in the Serial Port for a Reboot
         */
        int vote = 0; //If Wi-Fi and MQTT vote positive, the system will finish the configuration
        int counter = 0;
        while(CONFIGURATION_STATUS == IN_PROGRESS && counter < WIFI_ATTEMPTS){
            if(WIFI_CONNECTION_STATUS == READY_TO_CONNECT){
                try{
                    wifi_connect();
                    vote++;
                }catch(...){
                    ESP_LOGE("Wi-Fi Connection Error");
                }
            }
            if(MQTT_CONNECTION_STATUS == READY_TO_CONNECT){
                try{
                    mqtt_connect();
                    vote++;
                }catch(...){
                    ESP_LOGE("MQTT Connection Error");
                }
            }
            if(vote == 2){
                CONFIGURATION_STATUS = DONE;
            }
            counter++;
            ESP_LOGD(TAG, "%d connection attempts left before system failure", WIFI_ATTEMPTS - counter);
        }

        //Capturing possible erros
        if(counter == WIFI_ATTEMPTS){
            ESP_LOGE(TAG, "The system tried to connect to Wi-Fi %d times and %d times to MQTT and failed",
                     WIFI_ATTEMPTS, MQTT_ATTEMPTS);
            ESP_LOGD(TAG, "The following connection(s) failed:");
            if(WIFI_CONNECTION_STATUS != CONNECTED){
                ESP_LOGD(TAG, "Wi-Fi Connection failed");
            }
            if(MQTT_CONNECTION_STATUS != CONNECTED){
                ESP_LOGD(TAG, "MQTT Connection failed");
            }

            ESP_LOGE(TAG, "Send [Y] when you wish to reboot the device");
            while(Serial.available() == 0){ /* Empty Loop */ }
            String command = Serial.readString();
            if(command == "Y"){
                ESP_LOGD(TAG, "Restarting Now...");
                ESP.restart();
            }
        }
        //end of Capturing possible erros

        //Configuration ended
        flash.putBool("first_boot", false);
        mqttClient.disconnect();
        flash.end();
        server.end();
        ESP_LOGD("All credentials has been verified and no erros was found");
        ESP_LOGD("Exiting configuration mode");
        ESP_LOGD("Rebooting now.");
        ESP.restart();
    #else
        WIFI_CONNECTION_STATUS = NOT_READY;

        //Setting up name
        device_name = get_device_name();
        flash.putString("device_name", device_name);
        ESP_LOGD(TAG, "Device Name: %s", device_name.c_str());
        //end of Setting up name

        flash.end();

        //Activation of Web Server
        IPAddress IP = activate_internal_wifi();
        startup_server();
        //end of Activation of Web Server

        //Configuration Loop
        int vote = 0; //If Wi-Fi and MQTT vote positive, the system will finish the configuration
        while(CONFIGURATION_STATUS != DONE){
            if(WIFI_CONNECTION_STATUS == READY_TO_CONNECT){
                try{
                    wifi_connect();
                    vote++;
                }catch(...){
                    ESP_LOGD("Wi-Fi Connection Error");
                }
            }
            if(MQTT_CONNECTION_STATUS == READY_TO_CONNECT){
                try{
                    mqtt_connect();
                    vote++;
                }catch(...){
                    ESP_LOGD("MQTT Connection Error");
                }
            }
            if(vote == 2){
                CONFIGURATION_STATUS = DONE;
            }
            dnsServer.processNextRequest();
            mqttClient.loop();
        }
        //end of Configuration Loop

        //Finishing First Boot Configurations
        flash.begin("config");
        flash.putBool("first_boot", false);
        mqttClient.disconnect();
        flash.end();
        server.end();
        ESP_LOGD("Configuration ended");
        ESP_LOGD("Rebooting now.");
        ESP.restart();
        //end of Finishing First Boot Configurations

    #endif

    }
    //end of First Boot control

    //Setting up name
    device_name = flash.getString("device_name", get_device_name());
    String name = "Persiana Inteligente ";
    name = name + "(" + device_name + ")";
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(name.c_str());
    ESP_LOGD(TAG, "Device Name: %s", device_name.c_str());
    //end of Setting up name

    //Welcome message
    ESP_LOGD(TAG, "Initializing Smart Blind");
    ESP_LOGD(TAG, "Device name: %s", name.c_str());
    ESP_LOGD(TAG, "Device MAC Address: %s", get_mac_address().c_str());
    //end of Welcome message

    //Pins wake up
    flash.end();
    activate_hardware();

    //Retrieving Connection Credentials
    //Wi-Fi
    flash.begin("config");
    ssid = flash.getString("wifi_ssid", "");
    pass = flash.getString("wifi_password", "");

    //MQTT
    mqtt_server_ip = flash.getString("mqtt_ip", "");
    mqtt_server_port = flash.getInt("mqtt_port", 1883);
    MQTT_LOGIN_REQUIRED = flash.getBool("mqtt_login", false);
    //if MQTT requires an authentication
    if(MQTT_LOGIN_REQUIRED){
        mqtt_user = flash.getString("mqtt_user", "");
        mqtt_password = flash.getString("mqtt_pass", "");
    }
    //end of Retrieving Connection Credentials

    flash.end();
    //end of Loading configs

    //Connecting to Wi-Fi
    while(WIFI_CONNECTION_STATUS != CONNECTED) {
        try {
            wifi_connect();
        } catch (...) {
            ESP_LOGD("WiFi Connection Error");
#if SELF_WIFI == true
            flash.begin("config");
            flash.putBool("first_boot", true);
            flash.end();
            ESP.restart();
#endif
        }
    }

    //Connecting to MQTT Server
    while(MQTT_CONNECTION_STATUS != CONNECTED) {
        try {
            mqtt_connect();
        } catch (...) {
            ESP_LOGD("MQTT Connection Error");
#if SELF_WIFI == true
            flash.begin("config");
            flash.putBool("first_boot", true);
            flash.end();
            ESP.restart();
#endif
        }
    }

    //Starting Running Server
    running_server();

    ESP_LOGD(TAG, "%s successfully initialized", name.c_str());
}

void loop(){
    mqttClient.loop();
}