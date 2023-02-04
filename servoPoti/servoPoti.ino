#include <Servo.h>
// https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/
Servo myservo;
const int pinPoti = A0;
float value = 0;          //initialization of sensor variable, equivalent to EMA Y
float EMA_a = 0.2;      //initialization of EMA alpha 0.6
int EMA_S = 0;          //initialization of EMA S
int filteredValue;

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  myservo.attach(2); // motor
  Serial.begin(9600);
  EMA_S = analogRead(pinPoti);  //set EMA S for t=1
}

void loop() {
  value = analogRead(pinPoti);
  
  EMA_S = (EMA_a*value) + ((1-EMA_a)*EMA_S);    //run the EMA
  filteredValue = (1023 - EMA_S);            //invert
  filteredValue = floatMap(filteredValue, 0, 1023, 0, 360);     // map to degrees

  Serial.println(filteredValue);

  if (filteredValue >= 130) {
    myservo.write(filteredValue + 10);
  }
  else {
    myservo.write(filteredValue);
  }
    
  delay(1);
}
