#include "mode2.h"
#include "definitions.h"

void action2()
{ //variables for the timer
  unsigned long starttimer, stoptimer, stopwatch, buttonTimer, buttonStartTimer, buttonEndTimer, s, nachkommastelle;

  //displays start condition
  lcd.setCursor(0, 0);
  lcd.print("Press OK to     ");
  lcd.setCursor(0, 1);
  lcd.print("start the game  ");

  if (digitalRead(button_res) == LOW)                     //checks if reset button is pressed (yes -> high, no -> low)
  {
start:
    int zufall1 = 0;
    int zufall2 = 0;
    int vibration = 0;
    int state = 0;

    boolean correctButton;
    boolean prestate1 = false;                          //variables to count button pushing and save the state of a button
    boolean prestate2 = false;
    boolean prestate3 = false;
    boolean prestate4 = false;

    String buttons = "s";                               //to print buttonS on lcd display if number of wrong buttons is not 1

    int cntWrong = 0;                                   //counter for wrong pushed buttons
    
    buttonTimer = 0;                                    //initialize timer variables for pushing the button
    buttonStartTimer = 0;
    buttonEndTimer = 0;  

    delay(200); //that vibration does not start early

    if (digitalRead(button_select) == HIGH)             //checks if select button is pressed
    {
      for (int i = 0; i < 10; i++)                      //game ends after 10 buttons are whacked
      { //starts timer
        if (i == 1)
        {
          starttimer = millis();
        }

        zufall2 = zufall1;
        vibration = zufall2 - 29;
        zufall1 = random(30, 34);                       //generates random number

        if (zufall1 == zufall2)                         //if second random number is same as first than find a new second random number
        {
          while (zufall1 == zufall2)                    //that first random number is not the same as second random number
            zufall1 = random(30, 34);
          digitalWrite(zufall1, HIGH);
        }
        else                                            //if it is not the same, start vibration motor
        {
          digitalWrite(zufall1, HIGH);
        }

        state = 0;                                      //variables to left while loop
        correctButton = false;
        
        prestate1 = false;                      
        prestate2 = false;
        prestate3 = false;
        prestate4 = false;     
        
        while (state == 0)
        { //condition to left while-loop when mode or select button is pressed
          if ((digitalRead(button_mode_up) || digitalRead(button_res)) == HIGH)
          {
            digitalWrite(zufall1, LOW);
            goto start;
            state = 1;
          }
          //functions that turn off the randomly turned on vibration motor
          if (digitalRead(vibration1) == HIGH)
          { //display
            displayTimerStarted();
            if (digitalRead(button1) == HIGH) {
              buttonStartTimer = millis();  //start timer for pushing state of button
              digitalWrite(zufall1, LOW);   //turns off vibration motor
              correctButton = true;         //sets variable to remeber the right button had been pushed
            }
            // checks if another button is pushed and increments cntWrong 
            else if (digitalRead(button2) == HIGH && !prestate2){
              cntWrong ++;
              prestate2 = true;
            }
            else if (digitalRead(button3) == HIGH && !prestate3){
              cntWrong ++;
              prestate3 = true;
            }
            else if (digitalRead(button4) == HIGH && !prestate4){
              cntWrong ++;
              prestate4 = true;
            }
          }

          if (digitalRead(vibration2) == HIGH)
          { //display
            displayTimerStarted();
            if (digitalRead(button2) == HIGH) {
              buttonStartTimer = millis();  //start timer for pushing state of button
              digitalWrite(zufall1, LOW);   //turns off vibration motor
              correctButton = true;         //sets variable to remeber the right button had been pushed
            }
            else if (digitalRead(button1) == HIGH && !prestate1){
              cntWrong ++;
              prestate1 = true;
            }
            else if (digitalRead(button3) == HIGH && !prestate3){
              cntWrong ++;
              prestate3 = true;
            }
            else if (digitalRead(button4) == HIGH && !prestate4){
              cntWrong++;
              prestate4 = true;
            }
          }
          
          if (digitalRead(vibration3) == HIGH && !prestate3)
          { //display
            displayTimerStarted();
            if (digitalRead(button3) == HIGH) { // && !prestate3
              buttonStartTimer = millis();  //start timer for pushing state of button
              digitalWrite(zufall1, LOW);   //turns off vibration motor
              correctButton = true;         //sets variable to remeber the right button had been pushed
            }
            else if (digitalRead(button1) == HIGH && !prestate1){
              cntWrong ++;
              prestate1 = true;
            }
            else if (digitalRead(button2) == HIGH && !prestate2){
              cntWrong ++;
              prestate2 = true;
            }
            else if (digitalRead(button4) == HIGH && !prestate4){
              cntWrong++;
              prestate4 = true;
            }
          }
          
          if (digitalRead(vibration4) == HIGH && !prestate4)
          { //display
            displayTimerStarted();
            if (digitalRead(button4) == HIGH) {
              buttonStartTimer = millis();  //start timer for pushing state of button
              digitalWrite(zufall1, LOW);   //turns off vibration motor
              correctButton = true;         //sets variable to remeber the right button had been pushed
            }
            else if (digitalRead(button1) == HIGH && !prestate1){
              cntWrong ++;
              prestate1 = true;
            }
            else if (digitalRead(button2) == HIGH && !prestate2){
              cntWrong ++;
              prestate2 = true;
            }
            else if (digitalRead(button3) == HIGH && !prestate3){
              cntWrong++;
              prestate3 = true;
            }
          }

          // only leave while loop if correct button has been pushed and is released again to avoid wrong counting in next iteration
          if ((digitalRead(button1) == LOW && digitalRead(button2) == LOW && digitalRead(button3) == LOW && digitalRead(button4) == LOW) && correctButton){
            // end button timer and sum it up
            buttonEndTimer = millis();
            buttonTimer += (buttonEndTimer - buttonStartTimer);            
            state = 1;
          }
          
          if ((digitalRead(button1) || digitalRead(button2) || digitalRead(button3) || digitalRead(button4)) == LOW){
            prestate1 = false;
            prestate2 = false;
            prestate3 = false;
            prestate4 = false;
          }
        }        


        //stop timer
        if (i == 9 && state == 1)
        { lcd.setCursor(0, 1);
          lcd.print("                ");                      //clear second line of display
          stoptimer = millis();
          stopwatch = stoptimer - starttimer - buttonTimer;
          s = stopwatch / 1000;                               //calculation to get the needed time in seconds
          nachkommastelle = (stopwatch - (s * 1000)) / 100;   //calculation to get the decimale place of the number
          //conditon to display needed time until reset-, mode- or select-button is pressed
          while ((digitalRead(button_res) || digitalRead(button_mode_up) || digitalRead(button_select)) == LOW)
          { //display
            lcd.setCursor(0, 0);
            lcd.print(cntWrong);
            lcd.setCursor(1, 0);
            lcd.print("  wrong button" + ((cntWrong != 1) ? buttons : ""));
            lcd.setCursor(0, 1);
            lcd.print(s);
            lcd.setCursor(4, 1);
            lcd.print("  seconds");

            //condition to display the decimale place of the number
            if (s >= 10 )
            {
              lcd.setCursor(2, 1);
              lcd.print(",");
              lcd.setCursor(3, 1);
              lcd.print(nachkommastelle);
            }
            else
            {
              lcd.setCursor(1, 1);
              lcd.print(",");
              lcd.setCursor(2, 1);
              lcd.print(nachkommastelle);
            }
            
            //checks if those buttons are pressed
            digitalRead(button_res);
            digitalRead(button_mode_up);
            digitalRead(button_select);
          }
        }
      }
    }
  }
  else if (digitalRead(button_res) == HIGH) //go to "start" if reset button is pressed
  {
    goto start;
  }
  digitalRead(button_mode_up);
}

void displayTimerStarted() {
  lcd.setCursor(0, 0);
  lcd.print("timer has been  ");
  lcd.setCursor(0, 1);
  lcd.print("started         ");
}
