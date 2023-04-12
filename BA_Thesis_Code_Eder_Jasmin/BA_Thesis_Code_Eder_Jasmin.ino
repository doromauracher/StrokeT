#include "definitions.h"
#include "mode1.h"
#include "mode2.h"
#include "mode3.h"
#include "mode4.h"
#include "mode5.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2); //HEX-adress, columns and rows of LCD (0x3F) 

//create objects to control servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int prestate_mode_up = 0;
int prestate_mode_down = 0;
int prestate_select = 0;
int prestate_res = 0;

//variable for going through menu (menu starts with one)
int menu = 1;

void setup()
{ 
  Serial.begin(19200);
  //initialization of variables as outputs
  pinMode(vibration1, OUTPUT);
  pinMode(vibration2, OUTPUT);
  pinMode(vibration3, OUTPUT);
  pinMode(vibration4, OUTPUT);
  pinMode(vibration5, OUTPUT);
  pinMode(vibration6, OUTPUT);
  pinMode(vibration7, OUTPUT);
  pinMode(vibration8, OUTPUT);
  //initialization of variables as inputs
  pinMode(FSR, INPUT);
  pinMode(FSR1, INPUT);
  pinMode(FSR2, INPUT);
  pinMode(FSR3, INPUT);
  pinMode(FSR4, INPUT);
  pinMode(button_res, INPUT);
  pinMode(button_mode_up, INPUT);
  pinMode(button_mode_down, INPUT);
  pinMode(button_select, INPUT);
  //attaches the servo pins to the servo objects
  servo1.attach(13);
  servo1.write(180);
  servo2.attach(10);
  servo2.write(180);
  servo3.attach(11);
  servo3.write(180);
  servo4.attach(12);
  servo4.write(180);
  //LCD initialization
  lcd.init();                                     //starts initialisation of LCD
  lcd.backlight();                                //turns backlight of LCD on
  //printing something everytime arduino starts
  lcd.setCursor(0, 0);                            //go to first row and first column of LCD --> not necessary because it always starts there first
  lcd.print("Stroke-T");
  lcd.setCursor(0, 1);                            //go to second row and first column of LCD
  lcd.print("by Jasmin Eder");
  delay(4000);                                    //waits 4 seconds
}

void loop()
{ updatemenu();                                   //fills display with menu after turning on
  //sets button-presses to 0
  button_press1 = 0;
  button_press2 = 0;
  button_press3 = 0;
  button_press4 = 0;

  if (digitalRead(button_mode_up) == HIGH && prestate_mode_up == 0)    //checks if mode button is pressed (yes -> high, no -> low)
  { menu++;                                                             //increases menu variable by one
    if (menu > 5)                                                       //only 5 modes in menu, if menu > 5 -> menu becomes 1
    { menu = 1;
    }
    updatemenu();                                                       //calls function updatemenu (for arrow update)
    prestate_mode_up = 1;                                              //that menu does only increase once per button press
  }
  else if (digitalRead(button_mode_up) == LOW)                          //checks if mode button is pressed
  { prestate_mode_up = 0;                                              //sets prestate_mode to 0
  }

  if (digitalRead(button_mode_down) == HIGH && prestate_mode_down == 0)//checks if mode button is pressed (yes -> high, no -> low)
  { menu--;                                                             //increases menu variable by one
    if (menu < 1)                                                       //only 5 modes in menu, if menu > 5 -> menu becomes 1
    { menu = 5;
    }
    updatemenu();                                                       //calls function updatemenu (for arrow update)
    prestate_mode_down = 1;                                            //that menu does only increase once per button press
  }
  else if (digitalRead(button_mode_down) == LOW)                        //checks if mode button is pressed
  { prestate_mode_down = 0;                                            //sets prestate_mode to 0
  }

  if (digitalRead(button_select) == HIGH && prestate_select == 0)       //checks if select button is pressed (yes -> high, no -> low)
  { //clears display (not with lcd.clear(); because this makes LCD to flicker)
    lcd.setCursor(0, 0);                                                //sets cursor to column 1 and line 1
    lcd.print("                ");
    lcd.setCursor(0, 1);                                                //sets cursor to column 1 and line 2
    lcd.print("                ");
    executeaction();                                                    //calls function executeaction() to actually do something
    prestate_select = 1;                                                //sets prestate_select to 1
  }
  else if (digitalRead(button_select) == LOW)                           //checks if select button is pressed
  { prestate_select = 0;                                                //sets prestate_select to 0
  }
}

void updatemenu()                         //function for the menu
{ switch (menu)
  {
    case 1:                               //menu = 1
      lcd.setCursor(0, 0);
      lcd.print(" > Mode 1       ");
      lcd.setCursor(0, 1);
      lcd.print("   Mode 2       ");
      break;
    case 2:                               //menu = 2
      lcd.setCursor(0, 0);
      lcd.print("   Mode 1       ");
      lcd.setCursor(0, 1);
      lcd.print(" > Mode 2       ");
      break;
    case 3:                               //menu = 3
      lcd.setCursor(0, 0);
      lcd.print(" > Mode 3       ");
      lcd.setCursor(0, 1);
      lcd.print("   Mode 4       ");
      break;
    case 4:                               //menu = 4
      lcd.setCursor(0, 0);
      lcd.print("   Mode 3       ");
      lcd.setCursor(0, 1);
      lcd.print(" > Mode 4       ");
      break;
    case 5:                               //menu = 5
      lcd.setCursor(0, 0);
      lcd.print(" > Mode 5       ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    default:                              //in case of an unclear state
      lcd.print("ERROR 404");
  }
}

void executeaction()                            //will be called when select button was pressed
{ switch (menu)
  {
    case 1:                                     //menu = 1
      while (digitalRead(button_mode_up) == LOW)//do action 1 until mode button is pressed
      { action1();                              //call function action1
      }
      break;
    case 2:                                     //menu = 2
      while (digitalRead(button_mode_up) == LOW)//do action 2 until mode button is pressed
      { action2();                              //call function action2
      }
      break;
    case 3:                                     //menu = 3
      while (digitalRead(button_mode_up) == LOW)//do action 3 until mode button is pressed
      { action3();                              //call function action3
      }
      break;
    case 4:                                     //menu = 4
      while (digitalRead(button_mode_up) == LOW)//do action 4 until mode button is pressed
      { action4();                              //call function action4
      }
      break;
    case 5:                                     //menu = 5
      while (digitalRead(button_mode_up) == LOW)//do action 5 until mode button is pressed
      { action5();                              //call function action5
      }
      break;
    default:                                    //in case of an unclear state
      lcd.print("ERROR 404");
  }
}
