#include "mode3.h"
#include "definitions.h"

int voltage;                                //variable for the voltage
unsigned long resistance; 
unsigned long conductance; 
int weight;
int force; 
int maxForce; 

void action3()
{ voltage = map((analogRead(FSR)), 0, 1023, 0, 5000); //mapping analog value to voltage in mV
  calculation();
  if (force >= 1)
  { 
    if (digitalRead(button1) == HIGH)                 //checks if button 1 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce){
        maxForce = force;
      }
      lcd.setCursor(0, 0);
      lcd.print("Finger 1 max:    ");
      lcd.setCursor(14,0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }

    if (digitalRead(button2) == HIGH)                 //checks if button 2 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce){
        maxForce = force;
      }
      lcd.setCursor(0, 0);
      lcd.print("Finger 2 max:    ");
      lcd.setCursor(14,0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }

    if (digitalRead(button3) == HIGH)                 //checks if button 3 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce){
        maxForce = force;
      }
      lcd.setCursor(0, 0);
      lcd.print("Finger 3 max:    ");
      lcd.setCursor(14,0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }

    if (digitalRead(button4) == HIGH)                 //checks if button 4 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce){
        maxForce = force;
      }
      lcd.setCursor(0, 0);
      lcd.print("Finger 4 max:    ");
      lcd.setCursor(14,0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("No force        ");
    lcd.setCursor(0, 1);
    lcd.print("recognized      ");
    maxForce = 0;
  }
  digitalRead(button_mode_up);
}

void calculation()
{
  resistance = 5000 - voltage;
  resistance *= 10000;                              //10K ohm resistor
  resistance /= voltage;
  conductance = 1000000 / resistance;               //converts into micro mhos (1/microohm)=microSiemens
  weight = (conductance + 1.5914) / 0.4707;         //equation with graph and linear approximation
  force = weight / 100;
}
