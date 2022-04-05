//
// Created by victo on 01/10/2021.
//
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <DNSServer.h>
#include "Connections.h"
#include "Hardware.h"

extern String device_name;
extern Preferences flash;
extern PubSubClient mqttClient;
extern DNSServer dnsServer;
extern String ssid;
extern String pass;
extern String mqtt_server_ip;
extern int mqtt_server_port;
extern int WIFI_CONNECTION_STATUS;
extern int MQTT_CONNECTION_STATUS;
extern String mqtt_user;
extern String mqtt_password;
extern bool MQTT_LOGIN_REQUIRED;
static const char* TAG = "Connections";

struct network_connection_error: public std::exception
{
    const char * what () const noexcept override
    {
        return "Network Connection Error";
    }
};

struct mqtt_connection_error: public std::exception
{
    const char * what () const noexcept override
    {
        return "MQTT Connection Error";
    }
};


void wifi_connect(){
    try{
        flash.begin("config");
        WiFi.begin(ssid.c_str(), pass.c_str());
        ESP_LOGD(TAG, "Connecting to WiFi");
        const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

        //Waiting first connection attempt
        vTaskDelay(xDelay*5);

        //If first attempt was not successfully made, this loop will start a counter to a predefined number of attempts
        //WIFI_ATTEMPTS Macro is used in here
        for(int counter = 0; WiFiClass::status() != WL_CONNECTED && counter < WIFI_ATTEMPTS; counter++){
            vTaskDelay(xDelay*5);
            ESP_LOGE(TAG, "Connection Failed! %d Attempts remaining!", WIFI_ATTEMPTS - counter);
            ESP_LOGD(TAG, "Retrying...");
        }
        //If after all attempts the connection was not successfully made, an Exception is thrown
        if(WiFiClass::status() != WL_CONNECTED){
            ESP_LOGE(TAG, "Connection failed after %d attempts.", WIFI_ATTEMPTS);
            flash.end();
            throw network_connection_error();
        }else{
            //Connection made with success
            WIFI_CONNECTION_STATUS = CONNECTED;
            ESP_LOGD(TAG, "Connected");

            //Storing Wi-Fi Credentials in Flash Memory
            flash.putString("wifi_ssid", ssid);
            flash.putString("wifi_password", pass);
            ESP_LOGD(TAG, "WiFi Credentials has been written in memory");

            //Closing Flash Memory
            flash.end();
        }
    }catch(std::exception& e){
        //An error has occurred during connection
        WIFI_CONNECTION_STATUS = NOT_READY;
        ESP_LOGE(TAG, "Network Connection Error -> Throwing Exception.");
        flash.end();
        throw e;
    }
}

IPAddress activate_internal_wifi(){
    //Setting Hotspot name
    ESP_LOGD(TAG, "Initializing Internal Wireless Network");
    String name = "Persiana Inteligente ";
    name = name + "(" + device_name + ")";

    //Setting device in Access Point + Station Mode
    WiFiClass::mode(WIFI_AP_STA);
    WiFi.softAP(name.c_str(), nullptr);
    IPAddress IP = WiFi.softAPIP();

    //Starting DNS Server
    dnsServer.start(53, "*", IP);
    ESP_LOGD(TAG, "Starting DNS Server");

    //Setting Hostname
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(name.c_str());
    ESP_LOGI(TAG, "Configuration Access Point set on %s", IP.toString().c_str());
    return IP;
}

bool mqtt_connect(){
    try{
        flash.begin("config");
        if(WIFI_CONNECTION_STATUS == CONNECTED){
            //Setting the MQTT Server IP and Port
            mqttClient.setServer(mqtt_server_ip.c_str(), mqtt_server_port);

            //Setting device name in MQTT Server
            String name = "SmartBlind " + device_name;

            //Retrieving Macro information if available
            bool temp = IS_MQTT_LOGIN_REQUIRED;
            if(temp){
                MQTT_LOGIN_REQUIRED = temp;
            }

            //Connecting to Server
            if(MQTT_LOGIN_REQUIRED){
                mqttClient.connect(name.c_str(), mqtt_user.c_str(), mqtt_password.c_str());
            }else{
                mqttClient.connect(name.c_str());
            }

            //Setting callback function when a message is received from the server
            mqttClient.setCallback(mqtt_callback);

            //Error control
            if(!mqttClient.connected()){
                throw mqtt_connection_error();
            }

            //Subscribing to Topic Predefined
            String sub_topic = MQTT_TOPIC_to_SUB;
            mqttClient.subscribe(sub_topic.c_str());

            //Setting to Publish Topic
            String pub_topic = MQTT_TOPIC_to_PUB;
            String message = name + " initialized";
            mqttClient.publish(pub_topic.c_str(),message.c_str());

            //Changing the Flag state
            MQTT_CONNECTION_STATUS = CONNECTED;
            ESP_LOGD(TAG, "MQTT Connected");

            //Storing Credentials in Flash Memory
            flash.putString("mqtt_ip", mqtt_server_ip);
            flash.putInt("mqtt_port", mqtt_server_port);
            flash.putBool("mqtt_login", MQTT_LOGIN_REQUIRED);
            flash.putString("mqtt_user", mqtt_user);
            flash.putString("mqtt_pass", mqtt_password);

            //Closing Flash Memory
            ESP_LOGD(TAG, "MQTT Credentials has been written in memory");
            flash.end();
        }else{
            //Wi-Fi isn't connected, not able to connect MQTT
            MQTT_CONNECTION_STATUS = NOT_READY;
            ESP_LOGE(TAG, "Can't connect MQTT. Wi-Fi is not connected");
            flash.end();
            throw mqtt_connection_error();
        }
    }
    catch(std::exception& e){
        //Error caught during connection
        MQTT_CONNECTION_STATUS = NOT_READY;
        ESP_LOGE(TAG, "MQTT Connection Error -> Throwing Exception.");
        flash.end();
        throw e;
    }

    return true;
}

void mqtt_callback(char* topic, byte* message, unsigned int length){
    ESP_LOGD(TAG, "Data Received");

    String sub_topic = MQTT_TOPIC_to_SUB;
    if(!strcmp(topic, sub_topic.c_str())){
        char buffer[length + 1];
        for(int i = 0; i < length; i++) {
            buffer[i] = message[i];
        }
        buffer[length] = '\0';

        if (strcmp(buffer, "0001") == 0) {
            blinds_down(0);
            ESP_LOGD(TAG, "Opening Blinds");
        }else if (strcmp(buffer, "0002") == 0) {
            blinds_up(0);
            ESP_LOGD(TAG, "Closing Blinds");
        }else if (strcmp(buffer, "0003") == 0){
            blinds_stop(0);
            ESP_LOGD(TAG, "Blinds Stop");
        }else{
            ESP_LOGE(TAG, "An MQTT Message was received but cannot be decoded by the system");
        }
    }
}

String get_mac_address(){
    uint8_t baseMac[6];
    //Get MAC address for Wi-Fi station
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char baseMacChr[18] = {0};
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    return {baseMacChr};
}
