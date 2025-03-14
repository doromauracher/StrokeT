//include libaries
#include <Wire.h>                   //libary for I2C
#include <LiquidCrystal_I2C.h>      //libary for LCD-I2C
#include <Servo.h>                  //libary for Servo
#include <SoftwareSerial.h>         // sender to ESP
#include <ArduinoJson.h>            // JSON for sending to ESP
SoftwareSerial s(51,52);            // 51:RX 52:TX pins for ESP connection
StaticJsonDocument<48> doc;         // doc for sending the message

LiquidCrystal_I2C lcd(0x3F, 16, 2); //HEX-adress, columns and rows of LCD (0x3F)

//create objects to control servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//names and pinnumbers of fixed variables
// sending message to ESP
String message = "";

//button variables
int button1 = 22;
int button2 = 23;
int button3 = 24;
int button4 = 25;

int button_res = 26;
int button_select = 27;
int button_mode1 = 28;
int button_mode2 = 29;
//vibration motor variables of display buttons
int vibration1 = 30;
int vibration2 = 31;
int vibration3 = 32;
int vibration4 = 33;
//vibration motor variables of affected fingers
int vibration5 = 6;
int vibration6 = 7;
int vibration7 = 8;
int vibration8 = 9;
//poti reading variables
const int pot1 = 5;
const int pot2 = 6;
const int pot3 = 7;
const int pot4 = 8;
float value1 = 0;
float value2 = 0;
float value3 = 0;
float value4 = 0;
//constands for filter
float EMA_a = 0.2;      //initialization of EMA alpha 0.6
int EMA_S1 = 0;          //initialization of EMA S
int EMA_S2 = 0;
int EMA_S3 = 0;
int EMA_S4 = 0;
int filteredValue1;
int filteredValue2;
int filteredValue3;
int filteredValue4;
//function for poti reading
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//FSR sensor variable (four on one pin)
int FSR = A0;
int FSR4 = A4;
int FSR3 = A3;
int FSR2 = A2;
int FSR1 = A1;
//names and pinnumbers of changing variables
//variable for increasing only once if button 1 is pressed and not counting up as long as finger is on button 1
int prestate1 = 0;
int prestate2 = 0;
int prestate3 = 0;
int prestate4 = 0;
int prestate_mode1 = 0;
int prestate_mode2 = 0;
int prestate_select = 0;
int prestate_res = 0;
//variable for button-press counter (increases with every buttonpress)
int button_press1 = 0;
int button_press2 = 0;
int button_press3 = 0;
int button_press4 = 0;
//variable for going through menu (menu starts with one)
int menu = 1;
//variables for measurement with FSR
int voltage;                                //variable for the voltage
int voltage1;
int voltage2;
int voltage3;
int voltage4;
unsigned long resistance;                   //variable for the resistance --> can be very big (long), unsigned = no negative values
unsigned long resistance1;
unsigned long resistance2;
unsigned long resistance3;
unsigned long resistance4;
unsigned long conductance;                  //variable for the conductance --> can be very big (long), unsigned = no negative values
unsigned long conductance1;
unsigned long conductance2;
unsigned long conductance3;
unsigned long conductance4;
int weight;
int weight1;
int weight2;
int weight3;
int weight4;
int force;                                  //variable for the force
int force1;
int force2;
int force3;
int force4;
int maxForce;

//run once
void setup()
{
  // baud rate for connection to ESP
  s.begin(9600);
  //value for filter
  EMA_S1 = analogRead(pot1);
  EMA_S2 = analogRead(pot2);
  EMA_S3 = analogRead(pot3);
  EMA_S4 = analogRead(pot4);
  Serial.begin(9600);
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
  //  pinMode(button1, INPUT);
  //  pinMode(button2, INPUT);
  //  pinMode(button3, INPUT);
  //  pinMode(button4, INPUT);
  pinMode(button_res, INPUT);
  pinMode(button_mode1, INPUT);
  pinMode(button_mode2, INPUT);
  pinMode(button_select, INPUT);
  //attaches the servo pins to the servo objects
  servo1.attach(10);
  //servo1.write(180);
  servo2.attach(12);
  //servo2.write(180);
  servo3.attach(11);
  //servo3.write(180);
  servo4.attach(13);
  //servo4.write(180);
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

//runs repeatedly
void loop()
{ updatemenu();                                   //fills display with menu after turning on
  //sets button-presses to 0
  button_press1 = 0;
  button_press2 = 0;
  button_press3 = 0;
  button_press4 = 0;

  if (digitalRead(button_mode1) == HIGH && prestate_mode1 == 0)      //checks if mode button is pressed (yes -> high, no -> low)
  { menu++;                                                        //increases menu variable by one
    if (menu > 4)                                                  //only 4 modes in menu, if menu > 4 -> menu becomes 1
    { menu = 1;
    }
    updatemenu();                                                  //calls function updatemenu (for arrow update)
    prestate_mode1 = 1;                                             //that menu does only increase once per button press
  }
  else if (digitalRead(button_mode1) == LOW)                        //checks if mode button is pressed
  { prestate_mode1 = 0;                                             //sets prestate_mode to 0
  }

  if (digitalRead(button_mode2) == HIGH && prestate_mode2 == 0)      //checks if mode button is pressed (yes -> high, no -> low)
  { menu--;                                                        //increases menu variable by one
    if (menu < 1)                                                  //only 4 modes in menu, if menu > 4 -> menu becomes 1
    { menu = 4;
    }
    updatemenu();                                                  //calls function updatemenu (for arrow update)
    prestate_mode2 = 1;                                             //that menu does only increase once per button press
  }
  else if (digitalRead(button_mode2) == LOW)                        //checks if mode button is pressed
  { prestate_mode2 = 0;                                             //sets prestate_mode to 0
  }

  if (digitalRead(button_select) == HIGH && prestate_select == 0)  //checks if select button is pressed (yes -> high, no -> low)
  { //clears display (not with lcd.clear(); because this makes LCD to flicker
    lcd.setCursor(0, 0);                                           //sets cursor to column 1 and line 1
    lcd.print("                ");
    lcd.setCursor(0, 1);                                           //sets cursor to column 1 and line 2
    lcd.print("                ");
    executeaction();                                               //calls function executeaction() to actually do something
    prestate_select = 1;                                             //sets prestate_select to 1
  }
  else if (digitalRead(button_select) == LOW)                      //checks if select button is pressed
  { prestate_select = 0;                                           //sets prestate_select to 0
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
    case 4:                               //menu = 3
      lcd.setCursor(0, 0);
      lcd.print("   Mode 3       ");
      lcd.setCursor(0, 1);
      lcd.print(" > Mode 4       ");
      break;
    default:                              //in case of an unclear state
      lcd.print("false");
  }
}

void executeaction()                            //will be called when select button was pressed
{ switch (menu)
  {
    case 1:                                     //menu = 1
      while (digitalRead(button_mode1) == LOW)   //do action 1 until mode button is pressed
      { action1();                              //call function action1
      }
      break;
    case 2:                                     //menu = 2
      while (digitalRead(button_mode1) == LOW)   //do action 2 until mode button is pressed
      { action2();                              //call function action2
      }
      break;
    case 3:                                     //menu = 3
      while (digitalRead(button_mode1) == LOW)   //do action 3 until mode button is pressed
      { action3();                              //call function action3
      }
      break;
    case 4:                                     //menu = 3
      while (digitalRead(button_mode1) == LOW)   //do action 3 until mode button is pressed
      { action4();                              //call function action3
      }
      break;
    default:                                    //in case of an unclear state
      lcd.print("false");
  }
}

// ---------------------------------------------------------- MODE1 -----------------------------------------------------------------
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
      sendMessage(0,0);                                   //clear cache
      sendMessage(1, button_press1);                      //send value to ESP
      prestate1 = 1;                                      //sets prestate to 1
    }

    if (digitalRead(button2) == HIGH && prestate2 == 0)   //checks if button 2 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration2, HIGH);
      lcd.setCursor(4, 1);
      button_press2 ++;                                   //increases number of button-presses
      lcd.print(button_press2);                           //prints number of button-presses on LCD
      sendMessage(0,0);                                   //clear cache
      sendMessage(2, button_press2);                      //send value to ESP
      prestate2 = 1;                                      //sets prestate to 1
    }

    if (digitalRead(button3) == HIGH && prestate3 == 0)   //checks if button 3 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration3, HIGH);
      lcd.setCursor(8, 1);
      button_press3 ++;                                   //increases number of button-presses
      lcd.print(button_press3);                           //prints number of button-presses on LCD
      sendMessage(0,0);                                   //clear cache
      sendMessage(3, button_press3);                      //send value to ESP
      prestate3 = 1;                                      //sets prestate to 1
    }

    if (digitalRead(button4) == HIGH && prestate4 == 0)   //checks if button 4 is pressed (yes -> high, no -> low)
    { digitalWrite(vibration4, HIGH);
      lcd.setCursor(12, 1);
      button_press4 ++;                                   //increases number of button-presses
      lcd.print(button_press4);                           //prints number of button-presses on LCD
      sendMessage(0,0);                                   //clear cache
      sendMessage(4, button_press4);                      //send value to ESP
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
  digitalRead(button_mode1);                         //reads mode button status (high or low)
}

// ---------------------------------------------------------- MODE2 ------------------------------------------------------------------

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
          if ((digitalRead(button_mode1) || digitalRead(button_res)) == HIGH)
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
          
          // send values to ESP (reactiontime and wrong pushed buttons)

          sendMessage(0,0);                                   //clear cache
          sendMessage(2, cntWrong);
          sendMessage(1, stopwatch);
          
          //conditon to display needed time until reset-, mode- or select-button is pressed
          while ((digitalRead(button_res) || digitalRead(button_mode1) || digitalRead(button_select)) == LOW)
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
            digitalRead(button_mode1);
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
  digitalRead(button_mode1);
}

// ------------------------------------------------------- MODE3 --------------------------------------------------------

void action3()
{ voltage = map((analogRead(FSR)), 0, 1023, 0, 5000); //mapping analog value to voltage in mV
  calculation();
  if (force >= 1)
  {
    if (digitalRead(button1) == HIGH)                 //checks if button 1 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce) {
        maxForce = force;
      }
     
      sendMessage(1, force);    // send force of finger to ESP
      sendMessage(5, maxForce); // send max force of finger to ESP
      
      lcd.setCursor(0, 0);
      lcd.print("Finger 1 max:    ");
      lcd.setCursor(14, 0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }

    if (digitalRead(button2) == HIGH)                 //checks if button 2 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce) {
        maxForce = force;
      }
     
      sendMessage(2, force);    // send force of finger to ESP
      sendMessage(6, maxForce); // send max force of finger to ESP
      
      lcd.setCursor(0, 0);
      lcd.print("Finger 2 max:    ");
      lcd.setCursor(14, 0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }

    if (digitalRead(button3) == HIGH)                 //checks if button 3 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce) {
        maxForce = force;
      }
      
      sendMessage(3, force);    // send force of finger to ESP
      sendMessage(7, maxForce); // send max force of finger to ESP
      
      lcd.setCursor(0, 0);
      lcd.print("Finger 3 max:    ");
      lcd.setCursor(14, 0);
      lcd.print(maxForce);
      lcd.setCursor(0, 1);
      lcd.print("Force (N):       ");
      lcd.setCursor(14, 1);
      lcd.print(force);
      delay(200);
    }

    if (digitalRead(button4) == HIGH)                 //checks if button 4 is pressed (yes -> high, no -> low)
    {
      if (force > maxForce) {
        maxForce = force;
      }
      
      sendMessage(4, force);    // send force of finger to ESP
      sendMessage(8, maxForce); // send max force of finger to ESP
      
      lcd.setCursor(0, 0);
      lcd.print("Finger 4 max:    ");
      lcd.setCursor(14, 0);
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
  digitalRead(button_mode1);
}


void action4()
{ //displays name of mode
  lcd.setCursor(0, 0);
  lcd.print("Passive Training");
  //Poti reading

  value1 = analogRead(pot1);
  value2 = analogRead(pot2);
  value3 = analogRead(pot3);
  value4 = analogRead(pot4);
  calculation_poti1();
  calculation_poti2();
  calculation_poti3();
  calculation_poti4();

  //write poti position to servo motor
  if ((filteredValue1 || filteredValue2 || filteredValue3 || filteredValue4) >= 130) {
    servo1.write(filteredValue2 + 10);
    servo2.write(filteredValue1 + 10);
    servo3.write(filteredValue3 + 10);
    servo4.write(filteredValue4 + 10);
  }
  else {
    servo1.write(filteredValue2);
    servo2.write(filteredValue1);
    servo3.write(filteredValue3);
    servo4.write(filteredValue4);
  }
  delay(1);

  //FSR reading
  voltage1 = map((analogRead(FSR1)), 0, 1023, 0, 5000); //mapping analog value to voltage in mV
  voltage2 = map((analogRead(FSR2)), 0, 1023, 0, 5000);
  voltage3 = map((analogRead(FSR3)), 0, 1023, 0, 5000);
  voltage4 = map((analogRead(FSR4)), 0, 1023, 0, 5000);
  calculation1();
  calculation2();
  calculation3();
  calculation4();

  if (force1 >= 1)   //checks if button 1 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration8, HIGH);
    delay (1000);
    digitalWrite(vibration8, LOW);
  }

  if (force2 >= 1)   //checks if button 2 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration7, HIGH);
    delay(1000);
    digitalWrite(vibration7, LOW);
  }

  if (force3 >= 1)   //checks if button 3 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration6, HIGH);
    delay(1000);
    digitalWrite(vibration6, LOW);
  }

  if (force4 >= 1)  //checks if button 4 is pressed (yes -> high, no -> low)
  {
    digitalWrite(vibration5, HIGH);
    delay(1000);
    digitalWrite(vibration5, LOW);
  }

  else if ((force1 || force2 || force3 || force4) < 1)
  { digitalWrite(vibration5, LOW);
    digitalWrite(vibration6, LOW);
    digitalWrite(vibration7, LOW);
    digitalWrite(vibration8, LOW);
  }

  digitalRead(button_mode1);       //reads mode button status (high or low)
}

// -------------------------------------------------- FUNCTIONS -------------------------------------------------------

// send a message to the ESP in the format {"Finger": 1, "count": 1}
void sendMessage(int finger, int count) {
  doc["Finger"] = finger;               // finger id from 1 to 8
  doc["count"] = count;                 // measured value of the finger
  serializeJson(doc, message);          // convert to Json format
  s.println(message);                   // send to ESP
  message ="";                          // reset message to avoid concatenating
}

// displays "timer has been started" on the lcd display - used in mode 2
void displayTimerStarted() {
  lcd.setCursor(0, 0);
  lcd.print("timer has been  ");
  lcd.setCursor(0, 1);
  lcd.print("started         ");
}

// calculate the force - used in mode 3
void calculation()
{
  resistance = 5000 - voltage;
  resistance *= 10000;                              //10K ohm resistor
  resistance /= voltage;
  conductance = 1000000 / resistance;               //converts into micro mhos (1/microohm)=microSiemens
  weight = (conductance + 1.5914) / 0.4707;         //equation with graph and linear approximation
  force = weight / 100;
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

// map and filter the poti value from 0 to 180 degree
void calculation_poti1()
{
  EMA_S1 = (EMA_a * value1) + ((1 - EMA_a) * EMA_S1); //run the EMA
  filteredValue1 = (1023 - EMA_S1);            //invert
  filteredValue1 = floatMap(filteredValue1, 0, 1023, 0, 180);     // map to degrees
}
void calculation_poti2()
{
  EMA_S2 = (EMA_a * value2) + ((1 - EMA_a) * EMA_S2); //run the EMA
  filteredValue2 = (1023 - EMA_S2);            //invert
  filteredValue2 = floatMap(filteredValue2, 0, 1023, 0, 180);     // map to degrees
}

void calculation_poti3()
{
  EMA_S3 = (EMA_a * value3) + ((1 - EMA_a) * EMA_S3); //run the EMA
  filteredValue3 = (1023 - EMA_S3);            //invert
  filteredValue3 = floatMap(filteredValue3, 0, 1023, 0, 180);     // map to degrees
}

void calculation_poti4()
{
  EMA_S4 = (EMA_a * value4) + ((1 - EMA_a) * EMA_S4); //run the EMA
  filteredValue4 = (1023 - EMA_S4);            //invert
  filteredValue4 = floatMap(filteredValue4, 0, 1023, 0, 180);     // map to degrees
}
