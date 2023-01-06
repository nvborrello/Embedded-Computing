void printToDisplay(char* message);
void titleScreen(void);
void countDown(void);
void initButtons(void);
void initLEDs(void);
char getButton(void);
void initclk(void);
void startTimer(void);
void stopTimer(void);
void playNote(int);
void BuzzerPitched(int);
void dontClearPrintToDisplay(unsigned char* message);
void displayScore(int score);
void dontClearPrintToDisplayLower(unsigned char* message);
#define A 440
#define A_sharp 466
#define B_flat 466
#define B 494
#define C_ 523
#define C_sharp 554
#define D 587
#define E_flat 622
#define E 659
#define F 698
#define F_sharp 740
#define G 784
#define G_sharp 831
#define A_flat 831
#define A_higher 880

#define led1 0x01
#define led2 0x02
#define led3 0x04
#define led4 0x08

#define button1 0x01
#define button2 0x02
#define button3 0x04
#define button4 0x08

