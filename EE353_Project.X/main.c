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

//LEDs
#define LED_GREEN_PIN91_PORT PORTAbits.RA6
//#define LED_GREEN_LAT      _LATA0
#define LED_GREEN_TRIS     _TRISA6
#define LED_GREEN_ANSEL ANSAbits.ANSA6


#define LED_YELLOW_PIN92_PORT PORTAbits.RA7
//#define LED_YELLOW_LAT      _LATA1
#define LED_YELLOW_TRIS     _TRISA7
#define LED_YELLOW_ANSEL ANSAbits.ANSA7

#define LED_RED_PIN28_PORT PORTAbits.RA9
//#define LED_RED_LAT      _LATA2
#define LED_RED_TRIS     _TRISA9
#define LED_RED_ANSEL ANSAbits.ANSA9




//ADC
#define ADC_CHANNEL_POTENTIOMETER 5
#define ANS5 ANSBbits.ANSB5
    
#define ADC_CHANNEL_GAS_SENSOR 4
#define ANS4 ANSBbits.ANSB4



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


waitie(double seconds){
    clock_t end = clock() + (seconds * CLOCKS_PER_SEC);
 
    while (clock() < end)
        ;
}



    
 //*****************************************ADC*****************************************
 // ADC_ChannelEnable ( ADC_CHANNEL_TEMPERATURE_SENSOR );
 /*ANS5 = ANALOG;
 ANS4 = ANALOG;*/
 //ADC_SetConfiguration ( ADC_CONFIGURATION_DEFAULT );
    

uint32_t read_10bit(uint32_t channel){//Reading ADC values and returning it into 10bit uint8_t 
    
    uint32_t i;//counter used in for loops
        
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

    while(!AD1CON1bits.DONE);       //Wait for conversion to complete

    return ADC1BUF0;

}

void SWITCH_LED (int i){
    if (i > 100){
        LED_GREEN_PIN91_PORT = LED_ON;}
    
    else if(i<100){
        LED_YELLOW_PIN92_PORT = LED_ON;}
    
    else{
        LED_RED_PIN28_PORT = LED_ON;
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

    //*****************************************LCD*****************************************
    SYS_Initialize();   
     // Clear the screen 
     printf("\f" ); 

     
    ADC_setConfiguration();

    ANS5 = ANALOG;
    ANS4 = ANALOG;
     
     while ( 1 ) {
       
        
        
       //Getting ADC results
       int gas=read_10bit(ADC_CHANNEL_GAS_SENSOR);
       int pot=read_10bit(ADC_CHANNEL_POTENTIOMETER) ;

       int pot1 = (9.77517)*pot;

       
       
       
       
       printf("Gas:%6d\r",gas);
       
      // printf("gas:%d\nPot:%4d\r\n",gas,pot1);
       

        waitie(0.5);
        
        SWITCH_LED (gas);
 }
    return (EXIT_SUCCESS);
}

