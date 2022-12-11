#include <Servo.h>

Servo myservo;
int pot = 2;
int value;

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  myservo.attach(2);
  Serial.begin(9600);
}

void loop() {
  value = analogRead(pot);
  value = floatMap (value, 0, 1023, 0, 180); //map
  myservo.write(value);
  delay(5);
}
