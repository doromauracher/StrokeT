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
  
  sendMessage(1, 1);    
  sendMessage(5, 2); 

  delay(1000);
}

// send a message to the ESP in the format {"Finger": 1, "count": 1}
void sendMessage(int finger, int count) {
  doc["Finger"] = finger;              // finger id from 1 to 8
  doc["count"] = count;                 // measured value of the finger
  serializeJson(doc, message);          // convert to Json format
  s.println(message);                   // send to ESP
  message ="";                          // reset message to avoid concatenating
}
