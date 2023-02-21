#include <WiFi.h>
#include <PubSubClient.h> // mqtt client for the arduino use, see documentation there : https://pubsubclient.knolleary.net/api
#include <ArduinoJson.h>

// Pins for connection to Arduino Mega
#define RXD2 16
#define TXD2 17

// WIFI LOGIN DATA
const char* ssid = "Doro"; //name of wifi network
const char* password =  "Doertiboy"; //password of wifi network
WiFiClient espClient;  // name of the client to be called when the program runs
PubSubClient client(espClient); // declaring the mqtt client 

//MQTT Settings
const char* mqtt_Server =  "broker.hivemq.com"; //  name and port of the broker (can also be  own PC  AIO_SERVER; for own Mosquitto broker)
const int mqttPort = 1883;

// MQTT Topic Names
//Publish Topics
const char* publishTopicSatus = "StrokeT/Arduino/Satus"; 
const char* publishTopicMode = "StrokeT/Arduino/Mode"; 
const char* publishTopicMode1 = "StrokeT/Arduino/Mode1"; 
const char* publishTopicMode2 = "StrokeT/Arduino/Mode2"; 
const char* publishTopicMode3 = "StrokeT/Arduino/Mode3"; 
const char* publishTopicMode4 = "StrokeT/Arduino/Mode4"; 

//Subscribe Topics
const char* subscribeTopicModeMenu = "StrokeT/Mode"; 
const char* subscribeTopicMode1 = "StrokeT/Mode1"; 
const char* subscribeTopicMode2 = "StrokeT/Mode2"; 
const char* subscribeTopicMode3 = "StrokeT/Mode4"; 
const char* subscribeTopicMode4 = "StrokeT/Mode4"; 
const char* subscribeTopicMode2Game = "StrokeT/Mode2/Game"; 

const char* clientName = "StrokeTArduino";  // client id that will be used for the connection with the broker 

String message = "";
String msg = "";

void setup() {
  Serial.begin(9600);

  // Connection to WIFI and MQTT
  setup_wifi();
  client.setServer(mqtt_Server, 1883); 
  client.setCallback(callback);

  // Connection to Arduino
  // Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() { 
  if (!client.connected()) {
  reconnect();
  }

  client.loop(); // this line of code should be used quite often, to assure that the subscription gets messages on time 
                 //see the documentation for more info : https://pubsubclient.knolleary.net/api#loop
  
  while (Serial2.available()) {
    msg = String(char(Serial2.read()));
    
    if(msg == "{"){
      while (msg != "}") {
        message.concat(msg); // += msg;
        msg = String(char(Serial2.read()));
      }
      message.concat("}");

      const char *text = message.c_str();       // convert string message in char array to be able to send with the mqtt client 
      client.publish(publishTopicMode, text);  // publish the payload to the topic
      //Serial.println(message);
      message = "";
    }
  }
}

// ------------------------------------------ FUNCTIONS ----------------------------------------------------

// WIFI
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // print your IP address
}

 // Incoming values are saved in payload array  
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Incoming value -> String -> Integer  
  String inString="";
  for (int i = 0; i < length; i++) {
    inString += (char)payload[i];
  } 
  if (strcmp(topic,subscribeTopicModeMenu) == 0)
  {
    Serial.print("Message: ");
    Serial.println(inString);
  }
  else if (strcmp(topic,subscribeTopicMode1) == 0)
  {
    Serial.print("Message: ");
    Serial.println(inString);
  }
  else if (strcmp(topic,subscribeTopicMode2) == 0)
  {
    Serial.print("Message: ");
    Serial.println(inString);
  }
  else if (strcmp(topic,subscribeTopicMode3) == 0)
  {
    Serial.print("Message: ");
    Serial.println(inString);
  }
  else if (strcmp(topic,subscribeTopicMode2Game) == 0)
  {
    Serial.print("Message: ");
    Serial.println(inString);
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      client.publish(publishTopicSatus, "Reconnected");  
      // resubscribe
      client.subscribe(subscribeTopicModeMenu);
      client.subscribe(subscribeTopicMode1);
      client.subscribe(subscribeTopicMode2);
      client.subscribe(subscribeTopicMode2Game);
      client.subscribe(subscribeTopicMode3);
      client.subscribe(subscribeTopicMode4);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" auto trying again in 5 seconds");
      delay(5000);
    }
  }
}
