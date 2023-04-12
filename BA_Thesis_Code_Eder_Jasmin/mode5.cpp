#include "mode5.h"
#include "definitions.h"

const int MAX_LEVEL  = 100;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level  = 1;

String buttons = "s"; //to print buttonS on lcd display if number of correct buttons is not 1

void action5()
{ //displays start condition
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Press OK to     ");
  lcd.setCursor(0, 1);
  lcd.print("start the game  ");

  if (digitalRead(button_res) == LOW)                     //checks if reset button is pressed (yes -> high, no -> low)
  {
    
start:
    boolean prestate1 = false;                            //variables to count button pushing and save the state of a button
    boolean prestate2 = false;
    boolean prestate3 = false;
    boolean prestate4 = false;
    
    if (digitalRead(button_select) == HIGH || level != 1)             //checks if select button is pressed
    {
      if  (level == 1){
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        generate_sequence();
        delay(1000); //that vibration does not start early
      } 
      Serial.println("if button select || level != 1");
      show_sequence();    //show the sequence
      get_sequence();     //wait for your sequence
    }
  }
  digitalRead(button_mode_up);
}

void show_sequence()
{
  Serial.println("show sequence");
  lcd.setCursor(0, 0);
  lcd.print("memorize the    ");
  lcd.setCursor(0, 1);
  lcd.print("sequence        ");
  
  buttonsLow();

  for (int i = 0; i < level; i++)
  {
    Serial.println(sequence[i]);
    digitalWrite(sequence[i],  HIGH);
    delay(500);
    digitalWrite(sequence[i], LOW);
    delay(150);
  }
}

// TO DO
/*
 liest manchmal den Wert direkt hintereinander ein -> falsche sequenz!
 */

void get_sequence()
{
  lcd.setCursor(0, 0);
  lcd.print("enter the       ");
  lcd.setCursor(0, 1);
  lcd.print("sequence        ");
  
  int state = 0; //this state indicates if the sequence is correct
  
  for  (int i = 0; i < level; i++)
  {
    state = 0;
    while(state == 0)
    {
      if (digitalRead(button1) == HIGH && !prestate1)
      {
        Serial.println("get sequence 1");
        digitalWrite(vibration1, HIGH); 
        //your_sequence[i] = 30; 
        prestate1 = true;
        state = 1;
        delay(500);
        if  (30 != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(vibration1,  LOW);
      }
      
      if (digitalRead(button2) == HIGH && !prestate2) 
      {
        prestate2 = true;
        Serial.println("get sequence 2");
        digitalWrite(vibration2, HIGH); 
        //your_sequence[i]  = 31;
        state = 1;
        delay(500);
        if (31 != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(vibration2,  LOW); 
      }
      
      if (digitalRead(button3) == HIGH && !prestate3) 
      {
        Serial.println("get sequence 3");
        digitalWrite(vibration3, HIGH); 
        //your_sequence[i]  = 32; 
        prestate3 = true;
        state = 1;
        delay(500);
        if (32 != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(vibration3,  LOW); 
      }
      
      if (digitalRead(button4) == HIGH && !prestate4) 
      {
        Serial.println("get sequence 4");
        digitalWrite(vibration4, HIGH); 
        //your_sequence[i]  = 33; 
        prestate4 = true;
        state = 1;
        delay(500);
        if (33 != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(vibration4,  LOW);
      }
      
      if ((digitalRead(button1) || digitalRead(button2) || digitalRead(button3) || digitalRead(button4)) == LOW){
        prestate1 = false;
        prestate2 = false;
        prestate3 = false;
        prestate4 = false;
      }

      if (digitalRead(button_res) == HIGH || digitalRead(button_mode_down)) //go to "start" if reset button is pressed
      {
        Serial.println("button res");
        level = 1;
        return;
      }
    }
  }
  right_sequence();
}

void generate_sequence()
{
  Serial.println("generate sequence");
  randomSeed(millis());  
  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i]  = random(30,34); //2,6
//    if(i%2 == 0){
//      sequence[i] = 30;
//    }
//    else {
//      sequence[i] = 31;
//    }
  }
  
}

void wrong_sequence()
{
  Serial.println("wrong sequence");
  buttonsLow();
  lcd.setCursor(0, 0);
  lcd.print("   GAME OVER!   ");
  lcd.setCursor(0, 1);
  lcd.print(level - 1);
  lcd.setCursor(1, 1);
  lcd.print("  right cycle" + (((level - 1) != 1) ? buttons : ""));
  delay(3000);
  
  level  = 1;

  
}

void right_sequence()
{
  Serial.println("right sequence");
  lcd.setCursor(0, 0);
  lcd.print("    CORRECT!    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(1000); 
  if  (level < MAX_LEVEL);
    level++;
} 

void buttonsLow() {
  digitalWrite(vibration1, LOW); 
  digitalWrite(vibration2, LOW); 
  digitalWrite(vibration3, LOW); 
  digitalWrite(vibration4, LOW); 
}
