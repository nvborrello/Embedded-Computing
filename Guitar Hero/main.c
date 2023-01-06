#include <msp430.h>
#include <peripherals.h>
#include <MSP430HeroFunctions.h>
unsigned long int timer = 0;
#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void){
    timer++;
}
void main(void){

    WDTCTL = WDTPW + WDTHOLD; //disable watch dog
    _BIS_SR(GIE);// enable interrupts

    //Initialize Peripherals
    initButtons();
    initLeds();
    configKeypad();
    configDisplay();
    initclk();

    //Set Arrays
    int ledNotes[28] = {led1,led2,led3,led4,led1,led2,led3,led4,led1,led4,led3,led2,led1,led4,led3,led2,led1,led2,led3,led4,led1,led2,led3,led4,led1,led1,led2,led1};
    double noteLength[28] = {1.5,1.5,1.5,0.3,0.3,0.3,0.75,0.3,1.5,1.5,0.75,0.3,0.75,0.3,1.5,0.3,1.5,0.75,0.3,0.75,0.75,0.3,1.5,1.5,0.3,0.75,0.75,0.75};
    int songNotes[28] = {A,B,C_sharp,D, E, F_sharp, G, G_sharp, A_higher, G_sharp, G, F_sharp, E, D, C_sharp, B, A, B, C_sharp, D, E, F_sharp, G, G_sharp, A_higher, A, B, A};
    int correctButton[28] = {button4, button3, button2, button1, button4, button3, button2, button1, button4, button1, button2, button3, button4, button1, button2, button3, button4, button3, button2, button1, button4, button3, button2, button1, button4, button4, button3, button4};
    char* countDown[4] = {"3","2","1"};
    int ledCountDown[4] = {led2,led3,led4};

    //Set Global Variables
    int gameState = 0;
    int pressedKey = 0;
    int i = 0;
    int j = 0;
    unsigned char pressedButton;
    unsigned int current_time;
    int score = 0;
while(1){
    startTimer();

        switch(gameState){
        case (0):
        gameState = 0;
        BuzzerOff();
        setLeds(0x00);
        titleScreen();
        while(pressedKey == 0){
            pressedKey = getKey();
            if (pressedKey == '*'){
                Graphics_clearDisplay(&g_sContext);
                for (j=0; j < 3; j++){
                    current_time = timer / 200;
                while(timer/200 < current_time + 1)
                    printToDisplay(countDown[j]);
                    setLeds(ledCountDown[j]);
                }
                Graphics_clearDisplay(&g_sContext);
                gameState = 1;
            }
        }
        case (1):
                for(i=0; i < 28; i++){
                    current_time = timer / 200;
                    if (pressedButton == correctButton[i]){
                        score++;
                }
                    while(timer/200 < current_time + (noteLength[i] * 1)){
                        pressedButton = getButton();
                        BuzzerPitched(songNotes[i]);
                        setLeds(ledNotes[i]);
                    }
                }
        gameState = 2;
        case(2):
          stopTimer();
          BuzzerOff();
          setLeds(0x00);
          startTimer();
          current_time = timer / 200;
          printToDisplay("Your Score Was:");
          displayScore(score);
          pressedKey = 0;
              while(pressedKey == 0){
                  pressedKey = getKey();
                      if (pressedKey == '#'){
                          titleScreen();
                          score = 0;
                          pressedKey = 0;
                              while(pressedKey == 0){
                                  pressedKey = getKey();
                                      if (pressedKey == '*'){
                                          startTimer();
                                          Graphics_clearDisplay(&g_sContext);
                                              for (j=0; j < 3; j++){
                                                  current_time = timer / 200;
                                                      while(timer/200 < current_time + 1)
                                                          printToDisplay(countDown[j]);
                                                          setLeds(ledCountDown[j]);
                                                          }
                                                          Graphics_clearDisplay(&g_sContext);
                                                          gameState = 1;
      }
    }
  }
}
    }
  }
}



