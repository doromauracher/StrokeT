#include "mode1.h"
#include "definitions.h"

//variable for increasing only once if button 1 is pressed and not counting up as long as finger is on button 1
int prestate1 = 0;
int prestate2 = 0;
int prestate3 = 0;
int prestate4 = 0;

//variable for button-press counter (increases with every buttonpress)
int button_press1 = 0;
int button_press2 = 0;
int button_press3 = 0;
int button_press4 = 0;

void action1()
{ //prints number of button-presses on LCD (all zero at the beginning)
  lcd.setCursor(0, 0);
  lcd.print("Active Training");
  lcd.setCursor(0, 1);
  lcd.print(button_press1);
  lcd.setCursor(4, 1);
  lcd.print(button_press2);
  lcd.setCursor(8, 1);
  lcd.print(button_press3);
  lcd.setCursor(12, 1);
  lcd.print(button_press4);

  if (digitalRead(button_res) == LOW)                     //checks if reset button is pressed (yes -> high, no -> low)
  {
    prestate_res = 0;

    if (digitalRead(button1) == HIGH && prestate1 == 0)   //checks if button1 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration1, HIGH);
      lcd.setCursor(0, 1);
      button_press1 ++;                                   //increases number of button-presses
      lcd.print(button_press1);                           //prints number of button-presses on LCD
      prestate1 = 1;                                      //sets prestate to 1
    }

    if (digitalRead(button2) == HIGH && prestate2 == 0)   //checks if button 2 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration2, HIGH);
      lcd.setCursor(4, 1);
      button_press2 ++;                                   //increases number of button-presses
      lcd.print(button_press2);                           //prints number of button-presses on LCD
      prestate2 = 1;                                      //sets prestate to 1
    }

    if (digitalRead(button3) == HIGH && prestate3 == 0)   //checks if button 3 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration3, HIGH);
      lcd.setCursor(8, 1);
      button_press3 ++;                                   //increases number of button-presses
      lcd.print(button_press3);                           //prints number of button-presses on LCD
      prestate3 = 1;                                      //sets prestate to 1
    }

    if (digitalRead(button4) == HIGH && prestate4 == 0)   //checks if button 4 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration4, HIGH);
      lcd.setCursor(12, 1);
      button_press4 ++;                                   //increases number of button-presses
      lcd.print(button_press4);                           //prints number of button-presses on LCD
      prestate4 = 1;                                      //sets prestate to 1
    }

    else if ((digitalRead(button1) || digitalRead(button2) || digitalRead(button3) || digitalRead(button4)) == LOW)
    { digitalWrite(vibration1, LOW);                      //turns off vibration motor 1
      digitalWrite(vibration2, LOW);                      //turns off vibration motor 2
      digitalWrite(vibration3, LOW);                      //turns off vibration motor 3
      digitalWrite(vibration4, LOW);                      //turns off vibration motor 4
      prestate1 = 0;                                      //sets prestate1 to 0
      prestate2 = 0;                                      //sets prestate2 to 0
      prestate3 = 0;                                      //sets prestate3 to 0
      prestate4 = 0;                                      //sets prestate4 to 0
    }
  }
  else if (digitalRead(button_res) == HIGH && prestate_res == 0)         //checks if reset button is pressed (yes -> high, no -> low)
  { digitalWrite(vibration1, LOW);
    digitalWrite(vibration2, LOW);
    digitalWrite(vibration3, LOW);
    digitalWrite(vibration4, LOW);
    //resets number of button-presses
    //lcd.setCursor(0, 0);
    //lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    button_press1 = 0;
    button_press2 = 0;
    button_press3 = 0;
    button_press4 = 0;
    prestate_res = 1;
  }
  digitalRead(button_mode_up);                         //reads mode button status (high or low)
}
