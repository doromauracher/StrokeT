#include <Servo.h>

Servo myservo;
int pot = 2;
int value;

void setup() {
  myservo.attach(2);
  Serial.begin(9600);
}

void loop() {
  value = analogRead(pot);
  value = map (value, 0, 1023, 0, 180);
  myservo.write(value);
  delay(5);
}
