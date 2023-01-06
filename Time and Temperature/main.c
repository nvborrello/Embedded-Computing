#include <msp430.h>
#include <peripherals.h>
#include <helperfunctions.h>
long unsigned int timer = 0;
#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void){
    timer++;
}
//Global Constants for TLV table
#define CALADC12_15V_30C *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C *((unsigned int *)0x1A1C)

void main(void){
    WDTCTL = WDTPW + WDTHOLD; //stop watchdog timer
    _BIS_SR(GIE); //enable iterrupts


    configDisplay(); //config display
    initclk(); //initalize clock
    initButtons(); //Initializes Launchpad Buttons
    initWheel(); //Initializes Scroll Wheel
    initLeds(); //Initializes LEDs

    //Global Variables
    unsigned char pressedButton = 0x00;
    int state = 0;
    long unsigned int capture_time, capture_time2,capture_time3,capture_time4,capture_time5,capture_time6;
    volatile int temp_code, wheel_code;
    volatile float in_temp, slopeC;
    int i = 0;
    float tempC[36];
    for(i = 0; i < 37; i++){
        tempC[i] = 25;
    }

    //Find Slope for TLV table
    slopeC = ((float)85-30)/((float)(CALADC12_15V_85C-CALADC12_15V_30C));


    REFCTL0 &= ~REFMSTR; //gives reference voltage to adc

    ADC12CTL0 = ADC12SHT0_1|ADC12SHT1_1|ADC12REFON|ADC12MSC|ADC12ON; //1.5 Volt refrence volatage
    ADC12CTL1 = ADC12SHP|ADC12SSEL_1|ADC12CONSEQ_1; //Enable sample timer, single channel single conversion,select ACLK


    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_0; //use Vcc 3.3V channel A0 wheel
    ADC12MCTL1 = ADC12SREF_1 | ADC12INCH_10 |ADC12EOS;//use 1.5V ref volatge on channel A10 and write to mem reg 0

    ADC12CTL0 |= ADC12ENC; //Enable encoding

    __delay_cycles(100);

    startTimer(); //start timer with 1 seconds resolution

    while(1){
        switch(state){
        case(0):
               while(pressedButton == 0x00) {
                long int current_time = timer;
                  while (timer < current_time + 3) //Wait 3 seconds
                  {
                   pressedButton = getButton();
                   if(pressedButton == 0x10){
                       state = 1;
                       break;
                   }
                   ADC12CTL0 &= ~ADC12SC; //clear starting bit
                   ADC12CTL0 |= ADC12SC; //start conversion

                   while (ADC12CTL1 & ADC12BUSY) //do not convert if busy
                   __no_operation();

                   temp_code = ADC12MEM1 & 0x0FFF;

                   in_temp =(float)(((long)temp_code-CALADC12_15V_30C))*slopeC + 30.0; //convert to a temperature value
                   tempC[timer % 37] = in_temp;
                   }
                   long int capture_time0 = timer;
                   float avg_temp = averageTemp(tempC);
                   displayTimeAndTemp(capture_time0,avg_temp); //display temp and time every three seconds
                   }

        case(1):
                capture_time = timer;
                pressedButton = 0;
                int edit_state = 0;
                switch(edit_state){
                case(0):
                       setLeds(0x11);
                       capture_time2 = editMonth(capture_time);
                       edit_state = 1;
                       setLeds(0x10);
                case(1):
                       capture_time3 = editDay(capture_time2);
                       edit_state = 2;
                       setLeds(0x11);
                case(2):
                        capture_time4 = editHour(capture_time3);
                        edit_state = 3;
                        setLeds(0x10);
                case(3):
                        capture_time5 = editMin(capture_time4);
                        edit_state = 4;
                        setLeds(0x11);
                case(4):
                        capture_time6 = editSec(capture_time5);
                        timer = capture_time6;
                        state = 0;
                        break;

                        }

                }
        }
    }


