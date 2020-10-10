
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid ="BALAJI 3RD FLOOR 3B";
const char* password = "11223344";
String command;
 
#define ORG "rd09a0"
#define DEVICE_TYPE "Raspberry-Pi"
#define DEVICE_ID "123"
#define TOKEN "87654321"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

int led0 = 16;
int led1 = 5;
int led2 = 4;
int sensorValue;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
 Serial.begin(115200);
 Serial.println();
 //dht.begin();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
 Serial.begin(9600);
  pinMode(led0,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
}
 
void loop() {

  sensorValue = analogRead(A0);   // read the input on analog pin 0
  sensorValue = map(sensorValue,0,1023,0,255);
  Serial.println(sensorValue);

 if(sensorValue<20){
        digitalWrite(led0,LOW);
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
       
  }
                              
     if(sensorValue>20) {
        digitalWrite(led0,HIGH);
        digitalWrite(led1,HIGH);
        digitalWrite(led2,HIGH);
       
    }
PublishData(sensorValue);
 if (!client.loop()) {
    mqttConnect();
  }
delay(10);
// int sensorValue = analogRead(A0);
  // print out the value you read:
  //Serial.println(sensorValue);
  //delay(1);    
}
void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
//if(command == "LIGHTON"){
  //digitalWrite(D1,LOW);
 // Serial.println("Light is Switched ON");
//}
//if(command == "LIGHTOFF"){
  //digitalWrite(D1,HIGH);
  //Serial.println("Light is Switched OFF");
//}
command ="";
}
void PublishData(int sen){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(50);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"sensor\":";
  payload += sen;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
