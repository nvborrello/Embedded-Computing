//swkarche Samuel Karkache
//Helper functions for the guitar hero lab, some are reused from the space invader lab and some are from the demo project
#include <peripherals.h>
#include <msp430.h>

void printToDisplay(unsigned char* message) //From Lab 1, takes a string input and displays it to the LCD
{
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, message , AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void initButtons(void) //Function created in Pre-Lab, initializes the 4 buttons on main board
{
    P7SEL &= ~BIT4;
    P7DIR &= ~BIT4;
    P7REN |= BIT4;
    P7OUT |= BIT4;

    P7SEL &= ~BIT0;
    P7DIR &= ~BIT0;
    P7REN |= BIT0;
    P7OUT |= BIT0;

    P3SEL &= ~BIT6;
    P3DIR &= ~BIT6;
    P3REN |= BIT6;
    P3OUT |= BIT6;

    P2SEL &= ~BIT2;
    P2DIR &= ~BIT2;
    P2REN |= BIT2;
    P2OUT |= BIT2;
}
void initLeds(void) //From DEMO Project Lab 0, initializes the 4 LEDs on the main board
{
    P6SEL &= ~(BIT4|BIT3|BIT2|BIT1);
    P6DIR |=  (BIT4|BIT3|BIT2|BIT1);
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
}
void initclk(void) //We will be using default values for the clock
{
    P5SEL = P5SEL | (BIT5|BIT4|BIT3|BIT2);
    UCSCTL0 = 0x14B8;
    UCSCTL1 = 0x0020;
    UCSCTL2 = 0x101F;
    UCSCTL3 = 0x0000;
    UCSCTL4 = 0x0044;
    UCSCTL5 = 0x0000;
    UCSCTL6 = 0xC1CD;
    UCSCTL7 = 0x0403;
    UCSCTL8 = 0x0707;
}
void BuzzerPitched(int pitch) //Alternate version of BuzzerOn from the Demo Project, takes an int pitch
{
    P3SEL |= BIT5;
    P3DIR |= BIT5;
    TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);
    TB0CTL  &= ~TBIE;
    TB0CCR0   = 32768/pitch;  //This value is altered to change pitch, lower number create a higher pitch and higher numbers create a lower pitch
    TB0CCTL0 &= ~CCIE;
    TB0CCTL5  = OUTMOD_7;
    TB0CCTL5 &= ~CCIE;
    TB0CCR5   = TB0CCR0/2;
}
unsigned char getButton(void) //Returns a 4 bit int with each bit corresponding to which button is pressed
{
    unsigned char return_value = 0x0;

    char button_1 = ~P7IN & BIT0;
    char button_2 = ~P3IN & BIT6;
    char button_3 = ~P2IN & BIT2;
    char button_4 = ~P7IN & BIT4;
    if (button_1)
        return_value = 0x1;
    if (button_2)
        return_value = 0x2;
    if (button_3)
        return_value = 0x4;
    if (button_4)
        return_value = 0x8;
    return return_value;
}
void startTimer(void) //Starts and sets the MAX_COUNT for our timer
{
    TA2CTL = (TASSEL_1)|(ID_0)|(MC_1);
    TA2CCTL0 = CCIE;
    TA2CCR0 = 163; //5 ms resolution
}
void stopTimer(void) //Stops timer
{
    TA2CTL = (MC_0);
    TA2CCTL0 &= ~CCIE;
}

void titleScreen(void) //Alternate version of printToDisplay that prints the title screen
{
    setLeds(0x00);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "MSP430", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "HERO", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "PRESS * TO BEGIN", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}
void dontClearPrintToDisplay(unsigned char* message){
    Graphics_drawStringCentered(&g_sContext, message , AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}
void dontClearPrintToDisplayLower(unsigned char* message){
    Graphics_drawStringCentered(&g_sContext, message , AUTO_STRING_LENGTH, 48, 50, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}
void displayScore(int score){
    switch(score){

    case (0):
                dontClearPrintToDisplay("0 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (1):
                dontClearPrintToDisplay("1 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (2):
                dontClearPrintToDisplay("2 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (3):
                dontClearPrintToDisplay("3 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (4):
                dontClearPrintToDisplay("4 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (5):
                dontClearPrintToDisplay("5 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (6):
                dontClearPrintToDisplay("6 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (7):
                dontClearPrintToDisplay("7 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (8):
                dontClearPrintToDisplay("8 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (9):
                dontClearPrintToDisplay("9 out of 28...");
                dontClearPrintToDisplayLower("You Lose!");
                break;
    case (10):
                dontClearPrintToDisplay("10 out of 28.");
                dontClearPrintToDisplayLower("Not so great");
                break;
    case (11):
                dontClearPrintToDisplay("11 out of 28.");
                dontClearPrintToDisplayLower("Not so great");
                break;
    case (12):
                dontClearPrintToDisplay("12 out of 28.");
                dontClearPrintToDisplayLower("Not so great");
                break;
    case (13):
                dontClearPrintToDisplay("13 out of 28.");
                dontClearPrintToDisplayLower("Not so great");
                break;
    case (14):
                dontClearPrintToDisplay("14 out of 28.");
                dontClearPrintToDisplayLower("Not so great");
                break;
    case (15):
                dontClearPrintToDisplay("15 out of 28!");
                dontClearPrintToDisplayLower("Keep Trying!");
                break;
    case (16):
                dontClearPrintToDisplay("16 out of 28!");
                dontClearPrintToDisplayLower("Keep Trying!");
                break;
    case (17):
                dontClearPrintToDisplay("17 out of 28!");
                dontClearPrintToDisplayLower("Keep Trying!");
                break;
    case (18):
                dontClearPrintToDisplay("18 out of 28!");
                dontClearPrintToDisplayLower("Keep Trying!");
                break;
    case (19):
                dontClearPrintToDisplay("19 out of 28!");
                dontClearPrintToDisplayLower("Keep Trying!");
                break;
    case (20):
                dontClearPrintToDisplay("20 out of 28!");
                dontClearPrintToDisplayLower("Great!");
                break;
    case (21):
                dontClearPrintToDisplay("21 out of 28!");
                dontClearPrintToDisplayLower("Great!");
                break;
    case (22):
                dontClearPrintToDisplay("22 out of 28!");
                dontClearPrintToDisplayLower("Great!");
                break;
    case (23):
                dontClearPrintToDisplay("23 out of 28!");
                dontClearPrintToDisplayLower("Great!");
                break;
    case (24):
                dontClearPrintToDisplay("24 out of 28!");
                dontClearPrintToDisplayLower("Great!");
                break;
    case (25):
                dontClearPrintToDisplay("25 out of 28!");
                dontClearPrintToDisplayLower("Almost There!");
                break;
    case (26):
                dontClearPrintToDisplay("26 out of 28!");
                dontClearPrintToDisplayLower("Almost There!");
                break;
    case (27):
                dontClearPrintToDisplay("27 out of 28!");
                dontClearPrintToDisplayLower("Almost There!");
                break;
    case (28):
                dontClearPrintToDisplay("28 out of 28!!");
                dontClearPrintToDisplayLower("Perfect!!");
                break;
    }
}

