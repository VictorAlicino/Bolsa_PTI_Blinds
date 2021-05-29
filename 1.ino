#define BUTTON_01   2 
#define BUTTON_02   3
#define BUTTON_03   4

void callbackMqtt(char* topic, byte* message, unsigned int lenght);
PubSubClient mqttClient(RASP_BROKER_IP, 1883, callbackMqtt, wifiClient);


void setup(){
    pinMode(BUTTON_01, INPUT);
    pinMode(BUTTON_02, INPUT);
    pinMode(BUTTON_03, INPUT);
    
    
}

void loop(){
  if (!mqttCliennt.connect()) reconnect();
  if (mqttClient.publish("teste"), "hello world"){
    Serial.println("Sent");
  }else{
    Serial.println("Not sent");
  }
  Serial.println("Waiting response...");
  mqttClient.loop();
  delay(500);
}

void callbackMqtt(char* topic, byte* message, unsigned int lenght){
  Serial.println("Data Received");
  switch(topic){
    
  }
}