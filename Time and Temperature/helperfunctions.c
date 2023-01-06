#include <peripherals.h>
void printToDisplay(unsigned char* message,int xpos,int ypos){
    Graphics_drawStringCentered(&g_sContext, message , AUTO_STRING_LENGTH, xpos, ypos, TRANSPARENT_TEXT);
}
void initLeds(void)
{
    P6SEL &= ~(BIT4|BIT3|BIT2|BIT1);
    P6DIR |=  (BIT4|BIT3|BIT2|BIT1);
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
}
void displayTime(long unsigned int inTime){
    long unsigned int mm,day,day1,day2,hour,hour1,hour2,min,min1,min2,sec,sec1,sec2;
    unsigned char mm1,mm2,mm3;
    mm = inTime / (2592000);
    if (mm % 12 == 0){
        mm1 = 'J';
        mm2 = 'A';
        mm3 = 'N';
    }else if (mm % 12 == 1){
        mm1 = 'F';
        mm2 = 'E';
        mm3 = 'B';
    }else if (mm % 12 == 2){
        mm1 = 'M';
        mm2 = 'A';
        mm3 = 'R';
    }else if (mm % 12 == 3){
        mm1 = 'A';
        mm2 = 'P';
        mm3 = 'R';
    }else if (mm % 12 == 4){
        mm1 = 'M';
        mm2 = 'A';
        mm3 = 'Y';
    }else if (mm % 12 == 5){
        mm1 = 'J';
        mm2 = 'U';
        mm3 = 'N';
    }else if (mm % 12 == 6){
        mm1 = 'J';
        mm2 = 'U';
        mm3 = 'L';
    }else if (mm % 12 == 7){
        mm1 = 'A';
        mm2 = 'U';
        mm3 = 'G';
    }else if (mm % 12 == 8){
        mm1 = 'S';
        mm2 = 'E';
        mm3 = 'P';
    }else if (mm % 12 == 9){
        mm1 = 'O';
        mm2 = 'C';
        mm3 = 'T';
    }else if (mm % 12 == 10){
        mm1 = 'N';
        mm2 = 'O';
        mm3 = 'V';
    }else if (mm % 12 == 11){
        mm1 = 'D';
        mm2 = 'E';
        mm3 = 'C';
    }
    day = ((inTime - mm*(2592000)) / (86400));
    day1 = (day % 10);
    day2 = (day - day1)/10;

    hour = (inTime - mm*(2592000) - day *(86400))/ (3600);
    hour1 = (hour % 10);
    hour2 = (hour - hour1)/10;

    min = (inTime - mm*(2592000) - day *(86400) - hour * (3600))/ 60;
    min1 = (min % 10);
    min2 = (min - min1) / 10;

    sec = (inTime - mm*(2592000) - day *(86400) - hour * (3600) - min *60);
    sec1 = (sec % 10);
    sec2 = ((sec - sec1) /10);

   unsigned char dispHrMinSec[9] = {hour2 + 0x30, hour1 + 0x30, 0x3A, min2 + 0x30, min1 + 0x30, 0x3A, sec2 + 0x30 ,sec1 + 0x30};
   unsigned char dispDay[3] = {0x20, day2 + 0x30, day1 + 0x30};
   unsigned char dispMM[3] = {mm1,mm2,mm3};
   printToDisplay(dispMM, 30, 15);
   printToDisplay(dispDay, 48, 15);
   printToDisplay(dispHrMinSec,48, 35);
}
void displayTemp(float inAvgTempC){
    unsigned int tempC = inAvgTempC;
    float floatC = inAvgTempC;
    float floatF = (float)(1.8 * inAvgTempC) + 32;
    unsigned int tempF = floatF;


    unsigned int decimalC = (floatC - tempC)*10;

    unsigned char tempCOnes = (tempC % 10);
    unsigned char tempCTens = (tempC - tempCOnes)/ 10;
    unsigned char tempCTenths = (decimalC % 10);


    unsigned int decimalF = (floatF - tempF)*10;

    unsigned char tempFOnes= (tempF % 10);
    unsigned char tempFTens = (tempF - tempFOnes)/ 10;
    unsigned char tempFTenths = (decimalF % 10);

    unsigned char dispTemp[11] = {tempCTens + 0x30, tempCOnes + 0x30,'.', tempCTenths + 0x30, 'C',' ',tempFTens + 0x30, tempFOnes + 0x30,'.', tempFTenths + 0x30, 'F'};

    printToDisplay(dispTemp,45,75);
}
float averageTemp(float tempC[]){
    int i = 0;
    float sum;
    for(i = 0; i < 37; i++){
        sum = sum + tempC[i];
    }
    float avg = sum / 36;
    return avg;
}
void displayTimeAndTemp(long unsigned int inTime,float inAvgTempC){
    Graphics_clearDisplay(&g_sContext);
    displayTime(inTime);
    displayTemp(inAvgTempC);
    Graphics_flushBuffer(&g_sContext);
}

void initclk(void) //deafult values
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
void startTimer(void)
{
    TA2CTL = (TASSEL_1)|(ID_0)|(MC_1);
    TA2CCTL0 = CCIE;
    TA2CCR0 = 32767; //1 second resolution
}
void initButtons(void){
    //Left Button
    P2SEL &= ~BIT1;
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    //Right Button

    P1SEL &= ~BIT1;
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

}
unsigned char getButton(void){
    unsigned char ret_val = 0x00;

    char leftbutton = ~P2IN & BIT1;
    char rightbutton = ~P1IN & BIT1;

    if(leftbutton)
        ret_val = 0x10;
    if(rightbutton)
        ret_val = 0x01;

    return ret_val;
}
void initWheel(void){
    //Wheel Init
    P8SEL &= ~BIT0;
    P8DIR |= BIT0;
    P8OUT |= BIT0;
}
void stopTimer(void){
    TA2CTL = (MC_0);
    TA2CCTL0 &= ~CCIE;
}
long unsigned int editMonth(long unsigned int capture_time){
    char pressedButton = 0x00;
    long unsigned int capture_time2;
    long unsigned int next1Time, next2Time, next3Time, next4Time, next5Time, next6Time, next7Time, next8Time, next9Time, next10Time ,next11Time;
    while(pressedButton == 0x00){
        pressedButton = getButton();
        ADC12CTL0 &= ~ADC12SC; //clear starting bit
        ADC12CTL0 |= ADC12SC; //start conversion
        while (ADC12CTL1 & ADC12BUSY) //do not convert if busy
        __no_operation();

        int wheel_code = ADC12MEM0 & 0x0FFF;

        if (wheel_code < 340){
            Graphics_clearDisplay(&g_sContext);
            displayTime(capture_time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = capture_time;
                return capture_time2;
            }
        } else if (wheel_code > 340 && wheel_code < 680){
            next1Time = capture_time + 2592000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next1Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next1Time;
                return capture_time2;
            }
        }else if (wheel_code > 680 && wheel_code < 1020){
            next2Time = capture_time + 5184000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next2Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next2Time;
                return capture_time2;
            }
        }else if (wheel_code > 1020 && wheel_code < 1360){
            next3Time = capture_time + 7776000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next3Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next3Time;
                return capture_time2;
            }
        }else if (wheel_code > 1360 && wheel_code < 1700){
            next4Time = capture_time + 10368000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next4Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next4Time;
                return capture_time2;
            }
        }else if (wheel_code > 1700 && wheel_code < 2040){
            next5Time = capture_time + 12960000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next5Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next5Time;
                return capture_time2;
            }
        }else if (wheel_code > 2040 && wheel_code < 2380){
            next6Time = capture_time + 15552000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next6Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next6Time;
                return capture_time2;
            }
        }else if (wheel_code > 2380 && wheel_code < 2720){
            next7Time = capture_time + 18144000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next7Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next7Time;
                return capture_time2;
            }
        }else if (wheel_code > 2720 && wheel_code < 3060){
            next8Time = capture_time + 20736000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next8Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next8Time;
                return capture_time2;
            }
        }else if (wheel_code > 3060 && wheel_code < 3400){
            next9Time = capture_time + 23328000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next9Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next9Time;
                return capture_time2;
            }
        }else if (wheel_code > 3400 && wheel_code < 3740){
            next10Time = capture_time + 25920000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next10Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next10Time;
                return capture_time2;
            }
        }else if (wheel_code > 3740){
            next11Time = capture_time + 28512000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next11Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time2 = next11Time;
                return capture_time2;
            }
        }
    }
    return 0;
}
long unsigned int editDay(long unsigned int capture_time2){
    char pressedButton = 0x00;
    long unsigned int capture_time3;
    long unsigned int next1Time, next2Time, next3Time, next4Time, next5Time, next6Time, next7Time, next8Time, next9Time, next10Time ,next11Time, next12Time;
    long unsigned int next13Time, next14Time, next15Time, next16Time, next17Time, next18Time, next19Time, next20Time, next21Time, next22Time ,next23Time,next24Time;
    long unsigned int next25Time, next26Time, next27Time ,next28Time,next29Time;

    while(pressedButton == 0x00){
        pressedButton = getButton();
        ADC12CTL0 &= ~ADC12SC; //clear starting bit
        ADC12CTL0 |= ADC12SC; //start conversion
        while (ADC12CTL1 & ADC12BUSY) //do not convert if busy
        __no_operation();

        int wheel_code = ADC12MEM0 & 0x0FFF;

        if (wheel_code < 130){
            Graphics_clearDisplay(&g_sContext);
            displayTime(capture_time2);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = capture_time2;
                return capture_time3;
            }
        } else if (wheel_code > 130 && wheel_code < 260){
            next1Time = capture_time2 + 86400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next1Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next1Time;
                return capture_time3;
            }
        }else if (wheel_code > 260 && wheel_code < 390){
            next2Time = capture_time2 + 172800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next2Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next2Time;
                return capture_time3;
            }
        }else if (wheel_code > 390 && wheel_code < 520){
            next3Time = capture_time2 + 259200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next3Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next3Time;
                return capture_time3;
            }
        }else if (wheel_code > 520 && wheel_code < 650){
            next4Time = capture_time2 + 345600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next4Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next4Time;
                return capture_time3;
            }
        }else if (wheel_code > 650 && wheel_code < 780){
            next5Time = capture_time2 + 432000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next5Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next5Time;
                return capture_time3;
            }
        }else if (wheel_code > 780 && wheel_code < 910){
            next6Time = capture_time2 + 518400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next6Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next6Time;
                return capture_time3;
            }
        }else if (wheel_code > 910 && wheel_code < 1040){
            next7Time = capture_time2 + 604800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next7Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next7Time;
                return capture_time3;
            }
        }else if (wheel_code > 1040 && wheel_code < 1170){
            next8Time = capture_time2 + 691200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next8Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next8Time;
                return capture_time3;
            }
        }else if (wheel_code > 1170 && wheel_code < 1300){
            next9Time = capture_time2 + 777600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next9Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next9Time;
                return capture_time3;
            }
        }else if (wheel_code > 1300 && wheel_code < 1430){
            next10Time = capture_time2 + 864000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next10Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next10Time;
                return capture_time3;
            }
        }else if (wheel_code > 1430 && wheel_code < 1560){
            next11Time = capture_time2 + 950400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next11Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next11Time;
                return capture_time3;
            }
        }else if (wheel_code > 1560 && wheel_code < 1690){
            next12Time = capture_time2 + 1036800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next12Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next12Time;
                return capture_time3;
            }
        }else if (wheel_code > 1690 && wheel_code < 1820){
            next13Time = capture_time2 + 1123200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next13Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next13Time;
                return capture_time3;
            }
        }else if (wheel_code > 1820 && wheel_code < 1950){
            next14Time = capture_time2 + 1209600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next14Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next14Time;
                return capture_time3;
            }
        }else if (wheel_code > 1950 && wheel_code < 2080){
            next15Time = capture_time2 + 1382400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next15Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next15Time;
                return capture_time3;
            }
        }else if (wheel_code > 2080 && wheel_code < 2210){
            next16Time = capture_time2 + 1468800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next16Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next16Time;
                return capture_time3;
            }
        }else if (wheel_code > 2210 && wheel_code < 2340){
            next17Time = capture_time2 + 1555200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next17Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next17Time;
                return capture_time3;
            }
        }else if (wheel_code > 2340 && wheel_code < 2470){
            next18Time = capture_time2 + 1641600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next18Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next18Time;
                return capture_time3;
            }
        }else if (wheel_code > 2470 && wheel_code < 2600){
            next19Time = capture_time2 + 1728000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next19Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next19Time;
                return capture_time3;
            }
        }else if (wheel_code > 2600 && wheel_code < 2730){
            next20Time = capture_time2 + 1814400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next20Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next20Time;
                return capture_time3;
            }
        }else if (wheel_code > 2730 && wheel_code < 2860){
            next21Time = capture_time2 + 1900800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next21Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next21Time;
                return capture_time3;
            }
        }else if (wheel_code > 2860 && wheel_code < 2990){
            next22Time = capture_time2 + 1987200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next22Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next22Time;
                return capture_time3;
            }
        }else if (wheel_code > 2990 && wheel_code < 3120){
            next23Time = capture_time2 + 2073600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next23Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next23Time;
                return capture_time3;
            }
        }else if (wheel_code > 3120 && wheel_code < 3250){
            next24Time = capture_time2 + 2160000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next24Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next24Time;
                return capture_time3;
            }
        }else if (wheel_code > 3250 && wheel_code < 3380){
            next25Time = capture_time2 + 2246400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next25Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next25Time;
                return capture_time3;
            }
        }else if (wheel_code > 3380 && wheel_code < 3510){
            next26Time = capture_time2 + 2332800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next26Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next26Time;
                return capture_time3;
            }
        }else if (wheel_code > 3510 && wheel_code < 3640){
            next27Time = capture_time2 + 2419200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next27Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next27Time;
                return capture_time3;
            }
        }else if (wheel_code > 3640 && wheel_code < 3770){
            next28Time = capture_time2 + 2505600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next28Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next28Time;
                return capture_time3;
            }
        }else if (wheel_code > 3770){
            next29Time = capture_time2 + 2592000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next29Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time3 = next29Time;
                return capture_time3;
            }
        }
    }
    return 0;
}
long unsigned int editHour(long unsigned int capture_time3){
    char pressedButton = 0x00;
    long unsigned int capture_time4;
    long unsigned int next1Time, next2Time, next3Time, next4Time, next5Time, next6Time, next7Time, next8Time, next9Time, next10Time ,next11Time, next12Time;
    long unsigned int next13Time, next14Time, next15Time, next16Time, next17Time, next18Time, next19Time, next20Time, next21Time, next22Time ,next23Time;

    while(pressedButton == 0x00){
        pressedButton = getButton();
        ADC12CTL0 &= ~ADC12SC; //clear starting bit
        ADC12CTL0 |= ADC12SC; //start conversion
        while (ADC12CTL1 & ADC12BUSY) //do not convert if busy
        __no_operation();

        int wheel_code = ADC12MEM0 & 0x0FFF;

        if (wheel_code < 170){
            Graphics_clearDisplay(&g_sContext);
            displayTime(capture_time3);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = capture_time3;
                return capture_time4;
            }
        } else if (wheel_code > 170 && wheel_code < 340){
            next1Time = capture_time3 + 3600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next1Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next1Time;
                return capture_time4;
            }
        }else if (wheel_code > 340 && wheel_code < 510){
            next2Time = capture_time3 + 7200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next2Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next2Time;
                return capture_time4;
            }
        }else if (wheel_code > 510 && wheel_code < 680){
            next3Time = capture_time3 + 10800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next3Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next3Time;
                return capture_time4;
            }
        }else if (wheel_code > 680 && wheel_code < 850){
            next4Time = capture_time3 + 14400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next4Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next4Time;
                return capture_time4;
            }
        }else if (wheel_code > 850 && wheel_code < 1020){
            next5Time = capture_time3 + 18000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next5Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next5Time;
                return capture_time4;
            }
        }else if (wheel_code > 1020 && wheel_code < 1190){
            next6Time = capture_time3 + 21600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next6Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next6Time;
                return capture_time4;
            }
        }else if (wheel_code > 1190 && wheel_code < 1360){
            next7Time = capture_time3 + 25200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next7Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next7Time;
                return capture_time4;
            }
        }else if (wheel_code > 1360 && wheel_code < 1530){
            next8Time = capture_time3 + 28800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next8Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next8Time;
                return capture_time4;
            }
        }else if (wheel_code > 1530 && wheel_code < 1700){
            next9Time = capture_time3 + 32400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next9Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next9Time;
                return capture_time4;
            }
        }else if (wheel_code > 1700 && wheel_code < 1870){
            next10Time = capture_time3 + 36000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next10Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next10Time;
                return capture_time4;
            }
        }else if (wheel_code > 1870 && wheel_code < 2040){
            next11Time = capture_time3 + 39600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next11Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next11Time;
                return capture_time4;
            }
        }else if (wheel_code > 2040 && wheel_code < 2210){
            next12Time = capture_time3 + 43200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next12Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next12Time;
                return capture_time4;
            }
        }else if (wheel_code > 2210 && wheel_code < 2380){
            next13Time = capture_time3 + 46800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next13Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next13Time;
                return capture_time4;
            }
        }else if (wheel_code > 2380 && wheel_code < 2550){
            next14Time = capture_time3 + 50400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next14Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next14Time;
                return capture_time4;
            }
        }else if (wheel_code > 2550 && wheel_code < 2720){
            next15Time = capture_time3 + 54000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next15Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next15Time;
                return capture_time4;
            }
        }else if (wheel_code > 2720 && wheel_code < 2890){
            next16Time = capture_time3 + 57600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next16Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next16Time;
                return capture_time4;
            }
        }else if (wheel_code > 2890 && wheel_code < 3060){
            next17Time = capture_time3 + 61200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next17Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next17Time;
                return capture_time4;
            }
        }else if (wheel_code > 3060 && wheel_code < 3230){
            next18Time = capture_time3 + 64800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next18Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next18Time;
                return capture_time4;
            }
        }else if (wheel_code > 3230 && wheel_code < 3400){
            next19Time = capture_time3 + 38400;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next19Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next19Time;
                return capture_time4;
            }
        }else if (wheel_code > 3400 && wheel_code < 3570){
            next20Time = capture_time3 + 72000;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next20Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next20Time;
                return capture_time4;
            }
        }else if (wheel_code > 3570 && wheel_code < 3740){
            next21Time = capture_time3 + 75600;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next21Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next21Time;
                return capture_time4;
            }
        }else if (wheel_code > 3740 && wheel_code < 3910){
            next22Time = capture_time3 + 79200;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next22Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next22Time;
                return capture_time4;
            }
        }else if (wheel_code > 3910 && wheel_code < 4080){
            next23Time = capture_time3 + 82800;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next23Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time4 = next23Time;
                return capture_time4;
            }
        }
    }
    return 0;
}
long unsigned int editMin(long unsigned int capture_time4){
    char pressedButton = 0x00;
    long unsigned int capture_time5;
    long unsigned int next1Time, next2Time, next3Time, next4Time, next5Time, next6Time, next7Time, next8Time, next9Time, next10Time ,next11Time, next12Time;
    long unsigned int next13Time, next14Time, next15Time, next16Time, next17Time, next18Time, next19Time, next20Time, next21Time, next22Time ,next23Time,next24Time;
    long unsigned int next25Time, next26Time, next27Time ,next28Time,next29Time;

    long unsigned int next30Time, next31Time, next32Time, next33Time, next34Time, next35Time, next36Time, next37Time, next38Time, next39Time ,next40Time, next41Time;
    long unsigned int next42Time, next43Time, next44Time, next45Time, next46Time, next47Time, next48Time, next49Time, next50Time ,next51Time,next52Time;
    long unsigned int next53Time, next54Time, next55Time ,next56Time,next57Time,next58Time ,next59Time;

    while(pressedButton == 0x00){
        pressedButton = getButton();
        ADC12CTL0 &= ~ADC12SC; //clear starting bit
        ADC12CTL0 |= ADC12SC; //start conversion
        while (ADC12CTL1 & ADC12BUSY) //do not convert if busy
        __no_operation();

        int wheel_code = ADC12MEM0 & 0x0FFF;

        if (wheel_code < 70){
            Graphics_clearDisplay(&g_sContext);
            displayTime(capture_time4);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = capture_time4;
                return capture_time5;
            }
        } else if (wheel_code > 60 && wheel_code < 120){
            next1Time = capture_time4 + 60;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next1Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next1Time;
                return capture_time5;
            }
        }else if (wheel_code > 120 && wheel_code < 180){
            next2Time = capture_time4 + 120;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next2Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next2Time;
                return capture_time5;
            }
        }else if (wheel_code > 180 && wheel_code < 240){
            next3Time = capture_time4 + 180;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next3Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next3Time;
                return capture_time5;
            }
        }else if (wheel_code > 240 && wheel_code < 300){
            next4Time = capture_time4 + 240;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next4Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next4Time;
                return capture_time5;
            }
        }else if (wheel_code > 300 && wheel_code < 360){
            next5Time = capture_time4 + 300;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next5Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next5Time;
                return capture_time5;
            }
        }else if (wheel_code > 360 && wheel_code < 420){
            next6Time = capture_time4 + 360;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next6Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next6Time;
                return capture_time5;
            }
        }else if (wheel_code > 420 && wheel_code < 490){
            next7Time = capture_time4 + 420;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next7Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next7Time;
                return capture_time5;
            }
        }else if (wheel_code > 490 && wheel_code < 550){
            next8Time = capture_time4 + 490;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next8Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next8Time;
                return capture_time5;
            }
        }else if (wheel_code > 550 && wheel_code < 610){
            next9Time = capture_time4 + 550;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next9Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next9Time;
                return capture_time5;
            }
        }else if (wheel_code > 610 && wheel_code < 690){
            next10Time = capture_time4 + 610;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next10Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next10Time;
                return capture_time5;
            }
        }else if (wheel_code > 690 && wheel_code < 750){
            next11Time = capture_time4 + 670;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next11Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next11Time;
                return capture_time5;
            }
        }else if (wheel_code > 750 && wheel_code < 810){
            next12Time = capture_time4 + 730;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next12Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next12Time;
                return capture_time5;
            }
        }else if (wheel_code > 810 && wheel_code < 870){
            next13Time = capture_time4 + 790;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next13Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next13Time;
                return capture_time5;
            }
        }else if (wheel_code > 870 && wheel_code < 930){
            next14Time = capture_time4 + 850;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next14Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next14Time;
                return capture_time5;
            }
        }else if (wheel_code > 930 && wheel_code < 990){
            next15Time = capture_time4 + 910;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next15Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next15Time;
                return capture_time5;
            }
        }else if (wheel_code > 990 && wheel_code < 1050){
            next16Time = capture_time4 + 970;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next16Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next16Time;
                return capture_time5;
            }
        }else if (wheel_code > 1050 && wheel_code < 1110){
            next17Time = capture_time4 + 1030;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next17Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next17Time;
                return capture_time5;
            }
        }else if (wheel_code > 1110 && wheel_code < 1170){
            next18Time = capture_time4 + 1090;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next18Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next18Time;
                return capture_time5;
            }
        }else if (wheel_code > 1170 && wheel_code < 1230){
            next19Time = capture_time4 + 1150;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next19Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next19Time;
                return capture_time5;
            }
        }else if (wheel_code > 1230 && wheel_code < 1300){
            next20Time = capture_time4 + 1210;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next20Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next20Time;
                return capture_time5;
            }
        }else if (wheel_code > 1300 && wheel_code < 1360){
            next21Time = capture_time4 + 1270;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next21Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next21Time;
                return capture_time5;
            }
        }else if (wheel_code > 1360 && wheel_code < 1420){
            next22Time = capture_time4 + 1330;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next22Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next22Time;
                return capture_time5;
            }
        }else if (wheel_code > 1420 && wheel_code < 1480){
            next23Time = capture_time4 + 1390;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next23Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next23Time;
                return capture_time5;
            }
        }else if (wheel_code > 1480 && wheel_code < 1540){
            next24Time = capture_time4 + 1450;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next24Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next24Time;
                return capture_time5;
            }
        }else if (wheel_code > 1540 && wheel_code < 1600){
            next25Time = capture_time4 + 1510;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next25Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next25Time;
                return capture_time5;
            }
        }else if (wheel_code > 1600 && wheel_code < 1660){
            next26Time = capture_time4 + 1570;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next26Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next26Time;
                return capture_time5;
            }
        }else if (wheel_code > 1660 && wheel_code < 1720){
            next27Time = capture_time4 + 1630;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next27Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next27Time;
                return capture_time5;
            }
        }else if (wheel_code > 1720 && wheel_code < 1800){
            next28Time = capture_time4 + 1690;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next28Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next28Time;
                return capture_time5;
            }
        }else if (wheel_code > 1800 && wheel_code < 1860){
            next29Time = capture_time4 + 1750;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next29Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next29Time;
                return capture_time5;
            }
        }else if (wheel_code > 1860 && wheel_code < 1920){
            next30Time = capture_time4 + 1810;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next30Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next30Time;
                return capture_time5;
            }
        }else if (wheel_code > 1920 && wheel_code < 2000){
            next31Time = capture_time4 + 1870;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next31Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next31Time;
                return capture_time5;
            }
        }else if (wheel_code > 2000 && wheel_code < 2060){
            next32Time = capture_time4 + 1930;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next32Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next32Time;
                return capture_time5;
            }
        }else if (wheel_code > 2060 && wheel_code < 2130){
            next33Time = capture_time4 + 1990;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next33Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next4Time;
                return capture_time4;
            }
        }else if (wheel_code > 2130 && wheel_code < 2200){
            next34Time = capture_time4 + 2050;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next34Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next34Time;
                return capture_time5;
            }
        }else if (wheel_code > 2200 && wheel_code < 2260){
            next35Time = capture_time4 + 2110;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next35Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next35Time;
                return capture_time5;
            }
        }else if (wheel_code > 2260 && wheel_code < 2330){
            next36Time = capture_time4 + 2170;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next36Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next36Time;
                return capture_time5;
            }
        }else if (wheel_code > 2330 && wheel_code < 2400){
            next37Time = capture_time4 + 2230;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next37Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next37Time;
                return capture_time5;
            }
        }else if (wheel_code > 2400 && wheel_code < 2460){
            next38Time = capture_time4 + 2290;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next38Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next38Time;
                return capture_time5;
            }
        }else if (wheel_code > 2460 && wheel_code < 2520){
            next39Time = capture_time4 + 2350;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next39Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next39Time;
                return capture_time5;
            }
        }else if (wheel_code > 2520 && wheel_code < 2600){
            next40Time = capture_time4 + 2410;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next40Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next40Time;
                return capture_time5;
            }
        }else if (wheel_code > 2600 && wheel_code < 2660){
            next41Time = capture_time4 + 2470;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next41Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next41Time;
                return capture_time5;
            }
        }else if (wheel_code > 2660 && wheel_code < 2750){
            next42Time = capture_time4 + 2530;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next42Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next42Time;
                return capture_time5;
            }
        }else if (wheel_code > 2750 && wheel_code < 2800){
            next43Time = capture_time4 + 2590;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next43Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next43Time;
                return capture_time5;
            }
        }else if (wheel_code > 2800 && wheel_code < 2860){
            next44Time = capture_time4 + 2650;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next44Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next44Time;
                return capture_time5;
            }
        }else if (wheel_code > 2860 && wheel_code < 2920){
            next45Time = capture_time4 + 2710;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next45Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next45Time;
                return capture_time5;
            }
        }else if (wheel_code > 2920 && wheel_code < 2980){
            next46Time = capture_time4 + 2770;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next46Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next46Time;
                return capture_time5;
            }
        }else if (wheel_code > 2980 && wheel_code < 3050){
            next47Time = capture_time4 + 2830;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next47Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next47Time;
                return capture_time5;
            }
        }else if (wheel_code > 3050 && wheel_code < 3120){
            next48Time = capture_time4 + 2890;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next48Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next48Time;
                return capture_time5;
            }
        }else if (wheel_code > 3120 && wheel_code < 3200){
            next49Time = capture_time4 + 2950;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next49Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next49Time;
                return capture_time5;
            }
        }else if (wheel_code > 3200 && wheel_code < 3280){
            next50Time = capture_time4 + 3010;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next50Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next50Time;
                return capture_time5;
            }
        }else if (wheel_code > 3280 && wheel_code < 3350){
            next51Time = capture_time4 + 3070;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next51Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next51Time;
                return capture_time5;
            }
        }else if (wheel_code > 3350 && wheel_code < 3410){
            next52Time = capture_time4 + 3130;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next52Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next52Time;
                return capture_time5;
            }
        }else if (wheel_code > 3410 && wheel_code < 3490){
            next53Time = capture_time4 + 3190;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next53Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next53Time;
                return capture_time5;
            }
        }else if (wheel_code > 3490 && wheel_code < 3600){
            next54Time = capture_time4 + 3250;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next54Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next54Time;
                return capture_time5;
            }
        }else if (wheel_code > 3600 && wheel_code < 3660){
            next55Time = capture_time4 + 3310;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next55Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next55Time;
                return capture_time5;
            }
        }else if (wheel_code > 3660 && wheel_code < 3730){
            next56Time = capture_time4 + 3370;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next56Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next56Time;
                return capture_time5;
            }
        }else if (wheel_code > 3730 && wheel_code < 3800){
            next57Time = capture_time4 + 3430;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next57Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next57Time;
                return capture_time5;
            }
        }else if (wheel_code > 3880 && wheel_code < 3960){
            next58Time = capture_time4 + 3490;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next58Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next58Time;
                return capture_time5;
            }
        }else if (wheel_code > 3960 && wheel_code < 4080){
            next59Time = capture_time4 + 3550;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next59Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time5 = next59Time;
                return capture_time5;
            }
        }
    }
    return 0;
}
long unsigned int editSec(long unsigned int capture_time5){
    char pressedButton = 0x00;
    long unsigned int capture_time6;
    long unsigned int next1Time, next2Time, next3Time, next4Time, next5Time, next6Time, next7Time, next8Time, next9Time, next10Time ,next11Time, next12Time;
    long unsigned int next13Time, next14Time, next15Time, next16Time, next17Time, next18Time, next19Time, next20Time, next21Time, next22Time ,next23Time,next24Time;
    long unsigned int next25Time, next26Time, next27Time ,next28Time,next29Time;

    long unsigned int next30Time, next31Time, next32Time, next33Time, next34Time, next35Time, next36Time, next37Time, next38Time, next39Time ,next40Time, next41Time;
    long unsigned int next42Time, next43Time, next44Time, next45Time, next46Time, next47Time, next48Time, next49Time, next50Time ,next51Time,next52Time;
    long unsigned int next53Time, next54Time, next55Time ,next56Time,next57Time,next58Time ,next59Time;

    while(pressedButton == 0x00){
        pressedButton = getButton();
        ADC12CTL0 &= ~ADC12SC; //clear starting bit
        ADC12CTL0 |= ADC12SC; //start conversion
        while (ADC12CTL1 & ADC12BUSY) //do not convert if busy
        __no_operation();

        int wheel_code = ADC12MEM0 & 0x0FFF;

        if (wheel_code < 70){
            Graphics_clearDisplay(&g_sContext);
            displayTime(capture_time5);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = capture_time5;
                return capture_time6;
            }
        } else if (wheel_code > 60 && wheel_code < 120){
            next1Time = capture_time5 + 1;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next1Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next1Time;
                return capture_time6;
            }
        }else if (wheel_code > 120 && wheel_code < 180){
            next2Time = capture_time5 + 2;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next2Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next2Time;
                return capture_time6;
            }
        }else if (wheel_code > 180 && wheel_code < 240){
            next3Time = capture_time5 + 3;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next3Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next3Time;
                return capture_time6;
            }
        }else if (wheel_code > 240 && wheel_code < 300){
            next4Time = capture_time5 + 4;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next4Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next4Time;
                return capture_time6;
            }
        }else if (wheel_code > 300 && wheel_code < 360){
            next5Time = capture_time5 + 5;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next5Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next5Time;
                return capture_time6;
            }
        }else if (wheel_code > 360 && wheel_code < 420){
            next6Time = capture_time5 + 6;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next6Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next6Time;
                return capture_time6;
            }
        }else if (wheel_code > 420 && wheel_code < 490){
            next7Time = capture_time5 + 7;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next7Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next7Time;
                return capture_time6;
            }
        }else if (wheel_code > 490 && wheel_code < 550){
            next8Time = capture_time5 + 8;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next8Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next8Time;
                return capture_time6;
            }
        }else if (wheel_code > 550 && wheel_code < 610){
            next9Time = capture_time5 + 9;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next9Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next9Time;
                return capture_time6;
            }
        }else if (wheel_code > 610 && wheel_code < 690){
            next10Time = capture_time5 + 10;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next10Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next10Time;
                return capture_time6;
            }
        }else if (wheel_code > 690 && wheel_code < 750){
            next11Time = capture_time5 + 11;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next11Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next11Time;
                return capture_time6;
            }
        }else if (wheel_code > 750 && wheel_code < 810){
            next12Time = capture_time5 + 12;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next12Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next12Time;
                return capture_time6;
            }
        }else if (wheel_code > 810 && wheel_code < 870){
            next13Time = capture_time5 + 13;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next13Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next13Time;
                return capture_time6;
            }
        }else if (wheel_code > 870 && wheel_code < 930){
            next14Time = capture_time5 + 14;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next14Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next14Time;
                return capture_time6;
            }
        }else if (wheel_code > 930 && wheel_code < 990){
            next15Time = capture_time5 + 15;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next15Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next15Time;
                return capture_time6;
            }
        }else if (wheel_code > 990 && wheel_code < 1050){
            next16Time = capture_time5 + 16;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next16Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next16Time;
                return capture_time6;
            }
        }else if (wheel_code > 1050 && wheel_code < 1110){
            next17Time = capture_time5 + 17;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next17Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next17Time;
                return capture_time6;
            }
        }else if (wheel_code > 1110 && wheel_code < 1170){
            next18Time = capture_time5 + 18;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next18Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next18Time;
                return capture_time6;
            }
        }else if (wheel_code > 1170 && wheel_code < 1230){
            next19Time = capture_time5 + 19;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next19Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next19Time;
                return capture_time6;
            }
        }else if (wheel_code > 1230 && wheel_code < 1300){
            next20Time = capture_time5 + 20;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next20Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next20Time;
                return capture_time6;
            }
        }else if (wheel_code > 1300 && wheel_code < 1360){
            next21Time = capture_time5 + 21;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next21Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next21Time;
                return capture_time6;
            }
        }else if (wheel_code > 1360 && wheel_code < 1420){
            next22Time = capture_time5 + 22;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next22Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next22Time;
                return capture_time6;
            }
        }else if (wheel_code > 1420 && wheel_code < 1480){
            next23Time = capture_time5 + 23;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next23Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next23Time;
                return capture_time6;
            }
        }else if (wheel_code > 1480 && wheel_code < 1540){
            next24Time = capture_time5 + 24;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next24Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next24Time;
                return capture_time6;
            }
        }else if (wheel_code > 1540 && wheel_code < 1600){
            next25Time = capture_time5 + 25;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next25Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next25Time;
                return capture_time6;
            }
        }else if (wheel_code > 1600 && wheel_code < 1660){
            next26Time = capture_time5 + 26;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next26Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next26Time;
                return capture_time6;
            }
        }else if (wheel_code > 1660 && wheel_code < 1720){
            next27Time = capture_time5 + 27;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next27Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next27Time;
                return capture_time6;
            }
        }else if (wheel_code > 1720 && wheel_code < 1800){
            next28Time = capture_time5 + 28;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next28Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next28Time;
                return capture_time6;
            }
        }else if (wheel_code > 1800 && wheel_code < 1860){
            next29Time = capture_time5 + 29;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next29Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next29Time;
                return capture_time6;
            }
        }else if (wheel_code > 1860 && wheel_code < 1920){
            next30Time = capture_time5 + 30;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next30Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next30Time;
                return capture_time6;
            }
        }else if (wheel_code > 1920 && wheel_code < 2000){
            next31Time = capture_time5 + 31;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next31Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next31Time;
                return capture_time6;
            }
        }else if (wheel_code > 2000 && wheel_code < 2060){
            next32Time = capture_time5 + 32;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next32Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next32Time;
                return capture_time6;
            }
        }else if (wheel_code > 2060 && wheel_code < 2130){
            next33Time = capture_time5 + 33;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next33Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next4Time;
                return capture_time5;
            }
        }else if (wheel_code > 2130 && wheel_code < 2200){
            next34Time = capture_time5 + 34;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next34Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next34Time;
                return capture_time6;
            }
        }else if (wheel_code > 2200 && wheel_code < 2260){
            next35Time = capture_time5 + 35;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next35Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next35Time;
                return capture_time6;
            }
        }else if (wheel_code > 2260 && wheel_code < 2330){
            next36Time = capture_time5 + 36;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next36Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next36Time;
                return capture_time6;
            }
        }else if (wheel_code > 2330 && wheel_code < 2400){
            next37Time = capture_time5 + 37;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next37Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next37Time;
                return capture_time6;
            }
        }else if (wheel_code > 2400 && wheel_code < 2460){
            next38Time = capture_time5 + 38;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next38Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next38Time;
                return capture_time6;
            }
        }else if (wheel_code > 2460 && wheel_code < 2520){
            next39Time = capture_time5 + 39;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next39Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next39Time;
                return capture_time6;
            }
        }else if (wheel_code > 2520 && wheel_code < 2600){
            next40Time = capture_time5 + 40;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next40Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next40Time;
                return capture_time6;
            }
        }else if (wheel_code > 2600 && wheel_code < 2660){
            next41Time = capture_time5 + 41;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next41Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next41Time;
                return capture_time6;
            }
        }else if (wheel_code > 2660 && wheel_code < 2750){
            next42Time = capture_time5 + 42;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next42Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next42Time;
                return capture_time6;
            }
        }else if (wheel_code > 2750 && wheel_code < 2800){
            next43Time = capture_time5 + 43;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next43Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next43Time;
                return capture_time6;
            }
        }else if (wheel_code > 2800 && wheel_code < 2860){
            next44Time = capture_time5 + 44;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next44Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next44Time;
                return capture_time6;
            }
        }else if (wheel_code > 2860 && wheel_code < 2920){
            next45Time = capture_time5 + 45;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next45Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next45Time;
                return capture_time6;
            }
        }else if (wheel_code > 2920 && wheel_code < 2980){
            next46Time = capture_time5 + 46;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next46Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next46Time;
                return capture_time6;
            }
        }else if (wheel_code > 2980 && wheel_code < 3050){
            next47Time = capture_time5 + 47;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next47Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next47Time;
                return capture_time6;
            }
        }else if (wheel_code > 3050 && wheel_code < 3120){
            next48Time = capture_time5 + 48;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next48Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next48Time;
                return capture_time6;
            }
        }else if (wheel_code > 3120 && wheel_code < 3200){
            next49Time = capture_time5 + 49;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next49Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next49Time;
                return capture_time6;
            }
        }else if (wheel_code > 3200 && wheel_code < 3280){
            next50Time = capture_time5 + 50;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next50Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next50Time;
                return capture_time6;
            }
        }else if (wheel_code > 3280 && wheel_code < 3350){
            next51Time = capture_time5 + 51;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next51Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next51Time;
                return capture_time6;
            }
        }else if (wheel_code > 3350 && wheel_code < 3410){
            next52Time = capture_time5 + 52;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next52Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next52Time;
                return capture_time6;
            }
        }else if (wheel_code > 3410 && wheel_code < 3490){
            next53Time = capture_time5 + 53;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next53Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next53Time;
                return capture_time6;
            }
        }else if (wheel_code > 3490 && wheel_code < 3600){
            next54Time = capture_time5 + 54;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next54Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next54Time;
                return capture_time6;
            }
        }else if (wheel_code > 3600 && wheel_code < 3660){
            next55Time = capture_time5 + 55;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next55Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next55Time;
                return capture_time6;
            }
        }else if (wheel_code > 3660 && wheel_code < 3730){
            next56Time = capture_time5 + 56;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next56Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next56Time;
                return capture_time6;
            }
        }else if (wheel_code > 3730 && wheel_code < 3800){
            next57Time = capture_time5 + 57;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next57Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next57Time;
                return capture_time6;
            }
        }else if (wheel_code > 3880 && wheel_code < 3960){
            next58Time = capture_time5 + 58;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next58Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next58Time;
                return capture_time6;
            }
        }else if (wheel_code > 3960 && wheel_code < 4080){
            next59Time = capture_time5 + 59;
            Graphics_clearDisplay(&g_sContext);
            displayTime(next59Time);
            Graphics_flushBuffer(&g_sContext);
            if (pressedButton == 0x10){
                capture_time6 = next59Time;
                return capture_time6;
            }
        }
    }
    return 0;
}



