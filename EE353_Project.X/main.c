/* 
 * File:   main.c
 * Author: User
 *
 * Created on November 23, 2017, 12:43 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "lcd.h"
#include "timer_1ms.h"
#include <time.h>

#define ADC_CHANNEL_GAS_SENSOR PORTBbits.RB15
//#define       _LATA0
#define ADC_CHANNEL_GAS_SENSOR      _TRISB15
#define ADC_CHANNEL_GAS_SENSOR ANSBbits.ANSB15

//LEDs
#define LED_GREEN_PIN29_PORT _LATA10
//#define LED_GREEN_LAT      _LATA0
#define LED_GREEN_TRIS     _TRISA10
#define LED_GREEN_ANSEL ANSAbits.ANSA10


#define LED_YELLOW_PIN92_PORT _LATA7
//#define LED_YELLOW_LAT      _LATA1
#define LED_YELLOW_TRIS     _TRISA7
#define LED_YELLOW_ANSEL ANSAbits.ANSA7

#define LED_RED_PIN28_PORT _LATA9
//#define LED_RED_LAT      _LATA2
#define LED_RED_TRIS     _TRISA9
#define LED_RED_ANSEL ANSAbits.ANSA9

#define BUZEER_PIN38_PORT _LATA6
//#define LED_RED_LAT      _LATA2
#define BUZZER_TRIS     _TRISA6
#define BUZZER_ANSEL ANSAbits.ANSA6


//ADC
//#define ADC_CHANNEL_POTENTIOMETER 5
//#define ANS5 ANSBbits.ANSB5
    
#define ADC_CHANNEL_GAS_SENSOR 15
#define ANS15 ANSBbits.ANSB15



//PIN definition
#define PIN_INPUT           1
#define PIN_OUTPUT          0


#define ANALOG      1
#define DIGITAL     0


#define LED_ON  1
#define LED_OFF 0



void ADC_setConfiguration(){//Setting up the ADC deafualt configrations 
    //ADC_SetConfiguration ( ADC_CONFIGURATION_DEFAULT );
    AD1CON2bits.VCFG = 0x0 ;
    AD1CON3bits.ADCS = 0xFF ;
    AD1CON1bits.SSRC = 0x0;
    AD1CON3bits.SAMC = 0b10000;    
    AD1CON1bits.FORM = 0b00;
    AD1CON2bits.SMPI = 0x0;
    AD1CON1bits.ADON = 1;
}

extern void SYS_Initialize ( void ) ;


void waitie(double seconds){
    clock_t end = clock() + (seconds * CLOCKS_PER_SEC);
 
    while (clock() < end)
        ;
}



    
 //*****************************************ADC*****************************************
 // ADC_ChannelEnable ( ADC_CHANNEL_TEMPERATURE_SENSOR );
 /*ANS5 = ANALOG;
 ANS4 = ANALOG;*/
 //ADC_SetConfiguration ( ADC_CONFIGURATION_DEFAULT );
    

uint16_t read_10bit(uint8_t channel){//Reading ADC values and returning it into 10bit uint8_t 
    
    uint16_t i;//counter used in for loops
        
    AD1CHS = channel ;

    // Get an ADC sample
    AD1CON1bits.SAMP = 1;           //Start sampling
    for(i=0;i<1000;i++)
    {
        Nop(); //Sample delay, conversion start automatically
    } 

    AD1CON1bits.SAMP = 0;           //Start sampling
    for(i=0;i<1000;i++)
    {
        Nop(); //Sample delay, conversion start automatically
    }
    AD1CON1bits.DONE=0;
    while(AD1CON1bits.DONE);       //Wait for conversion to complete
    //AD1CON1bits.DONE=0;
    return ADC1BUF0;

}

void BUZZER(){
    
}
void SWITCH_LED ( int gas ){
        BUZEER_PIN38_PORT = LED_OFF;
        LED_YELLOW_PIN92_PORT = LED_OFF;
        LED_RED_PIN28_PORT = LED_OFF;
                LED_GREEN_PIN29_PORT = LED_OFF;


    if ( gas <= 10){
        LED_GREEN_PIN29_PORT = LED_ON;
       
    }
    
    else if( gas >10 && gas < 50){
        
        LED_YELLOW_PIN92_PORT = LED_ON;
       
    }
        
    else {
        LED_RED_PIN28_PORT = LED_ON;
        BUZEER_PIN38_PORT = LED_ON;
        
        
    }
    
}

int main(void) {
    
    //LED_RED_TRIS = PIN_OUTPUT;
    
    
    //*****************************************LED*****************************************
    
    LED_GREEN_TRIS = PIN_OUTPUT;
    LED_GREEN_ANSEL = DIGITAL;
    
    LED_YELLOW_TRIS = PIN_OUTPUT;
    LED_YELLOW_ANSEL = DIGITAL;

    LED_RED_TRIS  = PIN_OUTPUT;
    LED_RED_ANSEL = DIGITAL;
    
    BUZZER_TRIS = PIN_OUTPUT;
    BUZZER_ANSEL = DIGITAL;
    
    //*****************************************LCD*****************************************
    SYS_Initialize();   
    // Clear the screen 
    printf("\f"); 

     
    ADC_setConfiguration();

    //ANS5 = ANALOG;
    ANS15 = ANALOG;
     
     while ( 1 ) {
       
        
        
       //Getting ADC results
       uint16_t gas = read_10bit(ADC_CHANNEL_GAS_SENSOR);
       //int pot=read_10bit(ADC_CHANNEL_POTENTIOMETER) ;
       //int voltage = (gas*5.0)/1024;
       //int pot1 = (9.77517)*pot;

       
        SWITCH_LED(gas);

       
       gas = gas/10;
       printf("Gas:%6d\r",gas);
       //sprintf(char * str, const char * format, ...);
      // printf("gas:%d\nPot:%4d\r\n",gas,pot1);
       

       // waitie(5);
       
        //if (gas > 200){`
        //    printf("Gas:%6d\r", 0);
        //}
        
        uint32_t i = 1000000;
       while(i--);
 }
    return (0);
}
