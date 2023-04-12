#ifndef mode5_header
#define mode5_header

void action5();

void generate_sequence();
void show_sequence();
void get_sequence();
void wrong_sequence();
void right_sequence();
void buttonsLow();
void vibrateButton(int button);
void vibrateButtons (int buttonA, int buttonB);
void vibrateButtons (int buttonA, int buttonB, int buttonC);
void vibrateButtons (int buttonA, int buttonB, int buttonC, int buttonD);
void resetGame();
void gameOver();
void playSequence();
void readSequence();
void setLED(int id);
void setLEDs(bool red, bool green, bool blue, bool yellow );
void clearLEDs();
int readButtons(void);
#endif 
