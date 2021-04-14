/* 
 * File:   prograC.c
 * Author: Juan Diego Villafuerte Pazos
 *
 * Created on 13 de abril de 2021, 11:28 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
#include <xc.h>
#pragma config FOSC=INTRC_NOCLKOUT //Oscilador interno sin salida
#pragma config WDTE=OFF           //Reinicio repetitivo del pic
#pragma config PWRTE=ON           //espera de 72 ms al iniciar el pic
#pragma config MCLRE=OFF          //El pin MCLR se utiliza como entrada/salida
#pragma config CP=OFF             //Sin protecci?n de c?digo
#pragma config CPD=OFF            //Sin protecci?n de datos
    
#pragma config BOREN=OFF //Sin reinicio cuando el input voltage es inferior a 4V
#pragma config IESO=OFF  //Reinicio sin cambio de reloj de interno a externo
#pragma config FCMEN=OFF //Cambio de reloj externo a interno en caso de fallas
#pragma config LVP=ON    //Programaci?n en low voltage permitida
    
//CONFIGURATION WORD 2
#pragma config WRT=OFF //Proteccion de autoescritura por el programa desactivada
#pragma config BOR4V=BOR40V //Reinicio abajo de 4V 

char banderas;
char centenas;
char decenas;
char unidades;

char tabla(char display){
              
    switch(display){
        case 0:
        return 0b00111111;
        break; 
        case 1:
        return 0b00000110;
        break;  
        case 2:
        return 0b01011011;
        break;  
        case 3:
        return 0b01001111;
        break; 
        case 4:
        return 0b01100110;
        break;  
        case 5:
        return 0b01101101;
        break;  
        case 6:
        return 0b01111101;
        case 7:
        return 0b00000111;
        break;  
        case 8:
        return 0b01111111;
        break;  
        case 9:
        return 0b01100111;
        break;    
    }
}
void __interrupt() isr(void){    // only process timer-triggered interrupts
    
    if(RBIF == 1)  {
        if (PORTBbits.RB0 == 0) {
            PORTA++; 
        }
        if (PORTBbits.RB1 == 0) {
            PORTA--; 
        } 
    }
    if (T0IF == 1) {
        
        PORTC++;
        
        TMR0 = 125;
        INTCONbits.T0IF = 0;
        
        PORTBbits.RB5 = 0;
        PORTBbits.RB6 = 0;
        PORTBbits.RB7 = 0;
        
        PORTD = 0;
        
        switch(banderas) {
            case 0:
                PORTD = tabla(centenas);
                
                PORTBbits.RB5 = 1;
                PORTBbits.RB6 = 0;
                PORTBbits.RB7 = 0;
                banderas++;
                break;
            case 1:
                PORTD = tabla(decenas);
                
                PORTBbits.RB5 = 0;
                PORTBbits.RB6 = 1;
                PORTBbits.RB7 = 0;
                banderas++;
                break;
            case 2:
                PORTD = tabla(unidades);
                
                PORTBbits.RB5 = 0;
                PORTBbits.RB6 = 0;
                PORTBbits.RB7 = 1;
                banderas = 0;
                break;
        }
    }    
       //PORTA = 0;        
       //TMR0  =  125;
       INTCONbits.RBIF = 0; //clear this interrupt condition    
}
void main(void) {
    //configuraciones
    
    //configuracion reloj
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS   = 1;
    
    //configuracion in out
    ANSELH = 0;
    ANSEL  = 0;
    
    TRISB  = 3;
    TRISA  = 0;
    TRISC  = 0;
    TRISD  = 0;
    
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    
    PORTA  = 0;
    PORTB  = 0;
    PORTC  = 0;
    PORTD  = 0;
    
    //configuracion interrupt on change b
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    //configuracion tmr0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS2  = 0;
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 1;
    
    //reset tmr0
    TMR0 = 100;
    INTCONbits.T0IF = 0;
    
    //configuracion interrupciones
    INTCONbits.GIE  = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.T0IE = 1;
    
    //banderas = 0;
    while (1)
    {
        centenas = PORTA/100;
        decenas = PORTA%100/10;
        unidades = PORTA%100%10;
     }
}