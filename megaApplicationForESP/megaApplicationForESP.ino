#include <SoftwareSerial.h> // sender
#include <ArduinoJson.h>    // JSON for sending
SoftwareSerial s(51,52);    // 51:RX 52:TX

StaticJsonDocument<48> doc;

String message = "";

void setup(){
  Serial.begin(9600);
  s.begin(9600);
  Serial.println("UART");
}

void loop(){
  
 doc["Finger"] = 4 ;               // finger id from 1 to 8
 doc["count"] = 5;                 // measured value of the finger
  
  serializeJson(doc, message);     // serialize and store the object in a String e.g. {"Finger": 1, "count": 1}

  Serial.println(message);
  s.println(message);
  delay(1000);
  message ="";

 
}
