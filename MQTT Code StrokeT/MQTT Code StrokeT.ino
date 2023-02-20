//led slide control with RGB LED

#include <WiFi.h>
#include <PubSubClient.h> // mqtt client for the arduino use, see documentation there : https://pubsubclient.knolleary.net/api
#include <ArduinoJson.h>

// WIFI LOGIN DATA
const char* ssid = "#######"; //name of wifi network
const char* password =  "#######"; //password of wifi network

//MQTT Settings
const char* mqtt_Server =  "broker.hivemq.com"; //  name and port of the broker (can also be  own PC  AIO_SERVER; for own Mosquitto broker)
const int mqttPort = 1883;

// MQTT Topic Names

//Publish Topics
const char* publishTopicSatus = "StrokeT/Arduino/Satus"; //all the topics that are published by the arduino are named after it 
const char* publishTopicModeMenu = "StrokeT/Arduino/Mode"; //
const char* publishTopicMode1 = "StrokeT/Arduino/Mode1"; //
const char* publishTopicMode2 = "StrokeT/Arduino/Mode2"; //
const char* publishTopicMode3 = "StrokeT/Arduino/Mode3"; //
const char* publishTopicMode4 = "StrokeT/Arduino/Mode4"; //

//Subscribe Topics
const char* subscribeTopicModeMenu = "StrokeT/Mode"; // All the topics that publish data from the app 
const char* subscribeTopicMode1 = "StrokeT/Mode1"; //
const char* subscribeTopicMode2 = "StrokeT/Mode2"; //
const char* subscribeTopicMode3 = "StrokeT/Mode4"; //
const char* subscribeTopicMode4 = "StrokeT/Mode4"; //
const char* subscribeTopicMode2Game = "StrokeT/Mode2/Game"; //

const char* clientName = "StrokeTArduino";  // client id that will be used for the connection with the broker 


// int AnalogValue; 
// String poti_str; //see last code block below use these to convert the float that you get back from DHT to a string =str
// char poti[4] = {0};
// int ledRed=18; // D25 Pin pin of rgb led is connected to 25 gpio pin
// int ledblue=21; // D26 Pin pin of rgb led is connected to 26 gpio pin 
// int ledgreen=19; // D27 Pin pin of rgb led is connected to 27 gpio pin

long lastMsg = 0;
char msg[50];

WiFiClient espClient;  // name of the client to be called when the program runs
PubSubClient client(espClient); // declaring the mqtt client 


int i = 1;
StaticJsonDocument<48> doc;
String Message = "";

void setup() {
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_Server, 1883); // set the server 
  client.setCallback(callback);

  


}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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

 // Incoming values are save in playload array  
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


 // constant mqtt connection 
 void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(publishTopicSatus, "Reconnected");  
      // ... and resubscribe
      client.subscribe(subscribeTopicModeMenu);
      client.subscribe(subscribeTopicMode1);
      client.subscribe(subscribeTopicMode2);
      client.subscribe(subscribeTopicMode2Game);
      client.subscribe(subscribeTopicMode3);
      client.subscribe(subscribeTopicMode4);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" auto trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

    if (!client.connected()) {
    reconnect();
  }
  client.loop(); // this line of code should be used quite often, to assure that the subscription gets messages on time 
                 //see the documentation for more info : https://pubsubclient.knolleary.net/api#loop


  delay(2000);
  


}


/* Below is a code to serialize a message to be sent to the app or stored on the SD Card with explanations 

int i = 1;                        // for the example to get different values over time and check if its working

StaticJsonDocument<48> doc;      // declaration of the object with its length. https://arduinojson.org/v6/assistant/#/step1
String Message = "";             // declaration of the String to store the json data 
                                    declarations can be made with all the normal variables

doc["Finger"] = i;                // insertion in the object of the different attributes 
doc["count"] = i;                 // the attributes are used in the deserialization of the message, in the subscribe() function of each modes, to get the specific data and display it.
                                  // for mode 1 it is Finger (between 1 and 4) and count (an integer)
                                  // for mode 2 it is Action (Reaction or Wrong) and Time (in the case of Recation, in milliseconds),  
                                  // for mode 3 it is Finger (between 1 and 4) and Force (a floating number). The max force is actually calculated in the app and updated on the fly
                                  // mode 4 isn't implemented yet, can be if needed. 

                                  // an additional value may be added, for example the timestamp, to store data over time. simply add: 
                                  // doc["insert_name"] = insert_value; value can be anything: string, int, .... 
serializeJson(doc, Message);      // the object (here doc for example) is serialized and stored in a String (here Message for example) : {"Finger": 1, "count": 1}

const char *text = Message.c_str(); //convert string message in char to be able to send with the mqtt client 
client.publish(publishTopicMode1, text); // publish the payload (here text) to the topic (here publishTopicMode1)

Serial.println(text); // line to check what it looks like 
i++;                  // incrementing the value, see first comment of this paragraph
Message ="";          // reset the String (optional) !!!! IF THE STRING IS NOT RESETTED, THE DATA WILL BE ADDED OVER AND OVER TO THE STRING !!!! 
                                                                                                      (it isn't implemented this way in the app)
// Furthermore, the json library can be used to store data in the sd app directly. 


*/



