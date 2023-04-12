#include "mode4.h"
#include "definitions.h"

int voltage1;  
int voltage2;  
int voltage3;  
int voltage4; 

unsigned long resistance1;  
unsigned long resistance2;  
unsigned long resistance3;  
unsigned long resistance4;

unsigned long conductance1; 
unsigned long conductance2; 
unsigned long conductance3; 
unsigned long conductance4; 

int weight1;
int weight2;
int weight3;
int weight4;

int force1;       
int force2;  
int force3;       
int force4;

void action4()
{ //displays name of mode
  lcd.setCursor(0, 0);
  lcd.print("Passive Training");
  voltage1 = map((analogRead(FSR1)), 0, 1023, 0, 5000); //mapping analog value to voltage in mV
  voltage2 = map((analogRead(FSR2)), 0, 1023, 0, 5000); 
  voltage3 = map((analogRead(FSR3)), 0, 1023, 0, 5000); 
  voltage4 = map((analogRead(FSR4)), 0, 1023, 0, 5000); 
  calculation1();
  calculation2();
  calculation3();
  calculation4();

  if ((digitalRead(button1) == HIGH && prestate1 == 0) || force1 >= 1)   //checks if button 1 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration5, HIGH);
    //lets servo 1 move
    servo1.write(0);
    delay(600);
    servo1.write(180);
    delay(500);
    digitalWrite(vibration5, LOW);
    prestate1 = 1;                                      //sets prestate to 1
  }

  if ((digitalRead(button2) == HIGH && prestate2 == 0)|| force2 >= 1)   //checks if button 2 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration6, HIGH);
    //lets servo 2 move
    servo2.write(0);
    delay(600);
    servo2.write(180);
    delay(500);
    digitalWrite(vibration6, LOW);
    prestate2 = 1;                                      //sets prestate to 1
  }

  if ((digitalRead(button3) == HIGH && prestate3 == 0)|| force3 >= 1)   //checks if button 3 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration7, HIGH);
    //lets servo 3 move
    servo3.write(0);
    delay(600);
    servo3.write(180);
    delay(500);
    digitalWrite(vibration7, LOW);
    prestate3 = 1;                                      //sets prestate to 1
  }

  if ((digitalRead(button4) == HIGH && prestate4 == 0)|| force4 >= 1)  //checks if button 4 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration8, HIGH);
    //lets servo 4 move
    servo4.write(0);
    delay(600);
    servo4.write(180);
    delay(500);
    digitalWrite(vibration8, LOW);
    prestate4 = 1;                                      //sets prestate to 1
  }

  else if (((digitalRead(button1) || digitalRead(button2) || digitalRead(button3) || digitalRead(button4)) == LOW) || ((force1 || force2 || force3 || force4) < 1))
  { digitalWrite(vibration5, LOW);
    digitalWrite(vibration6, LOW);
    digitalWrite(vibration7, LOW);
    digitalWrite(vibration8, LOW);
    //sets prestates to 0
    prestate1 = 0;
    prestate2 = 0;
    prestate3 = 0;
    prestate4 = 0;
  }

  digitalRead(button_mode_up);       //reads mode button status (high or low)
}

void calculation1()
{
  resistance1 = 5000 - voltage1;
  resistance1 *= 10000;                              //10K ohm resistor
  resistance1 /= voltage1;
  conductance1 = 1000000 / resistance1;               //converts into micro mhos (1/microohm)=microSiemens
  weight1 = (conductance1 + 1.5914) / 0.4707;         //equation with graph and linear approximation
  force1 = weight1 / 100;
}


void calculation2()
{
  resistance2 = 5000 - voltage2;
  resistance2 *= 10000;                              //10K ohm resistor
  resistance2 /= voltage2;
  conductance2 = 1000000 / resistance2;               //converts into micro mhos (1/microohm)=microSiemens
  weight2 = (conductance2 + 1.5914) / 0.4707;         //equation with graph and linear approximation
  force2 = weight2 / 100;
}


void calculation3()
{
  resistance3 = 5000 - voltage3;
  resistance3 *= 10000;                              //10K ohm resistor
  resistance3 /= voltage3;
  conductance3 = 1000000 / resistance3;               //converts into micro mhos (1/microohm)=microSiemens
  weight3 = (conductance3 + 1.5914) / 0.4707;         //equation with graph and linear approximation
  force3 = weight3 / 100;
}


void calculation4()
{
  resistance4 = 5000 - voltage4;
  resistance4 *= 10000;                              //10K ohm resistor
  resistance4 /= voltage4;
  conductance4 = 1000000 / resistance4;               //converts into micro mhos (1/microohm)=microSiemens
  weight4 = (conductance4 + 1.5914) / 0.4707;         //equation with graph and linear approximation
  force4 = weight4 / 100;
}
