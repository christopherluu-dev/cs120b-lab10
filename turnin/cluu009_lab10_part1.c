/*	Author: christopherluu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Video: https://youtu.be/Nx9d6mkqh2g
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum ThreeLEDStates {THREE_START, THREE_ONE, THREE_TWO, THREE_THREE} ThreeLEDState;
enum BlinkingLEDStates {BLINK_START, BLINK_INIT, BLINK} BlinkingLEDState;
enum CombineLEDStates {COMBINE_START, COMBINE} CombineLEDState;

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

volatile unsigned char TimerFlag = 0;

void TimerISR(){
    TimerFlag = 1;
}

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff(){
    TCCR1B = 0x00;
}

ISR(TIMER1_COMPA_vect){
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0){
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M){
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

void TickThreeLEDs(){
    switch(ThreeLEDState){
        case THREE_START:
            ThreeLEDState = THREE_ONE;
            break;
        case THREE_ONE:
            ThreeLEDState = THREE_TWO;
            break;
        case THREE_TWO:
            ThreeLEDState = THREE_THREE;
            break;
        case THREE_THREE:
            ThreeLEDState = THREE_ONE;
            break;
        default:
            ThreeLEDState = THREE_START;
            break;
    }

    switch(ThreeLEDState){
        case THREE_START:
            break;
        case THREE_ONE:
            threeLEDs = 0x01;
            break;
        case THREE_TWO:
            threeLEDs = 0x02;
            break;
        case THREE_THREE:
            threeLEDs = 0x04;
            break;
        default:
            break;
    }
}

void TickBlinkingLEDs(){
    switch(BlinkingLEDState){
        case BLINK_START:
            BlinkingLEDState = BLINK_INIT;
            break;
        case BLINK_INIT:
            BlinkingLEDState = BLINK;
            break;
        case BLINK:
            BlinkingLEDState = BLINK_INIT;
            break;
        default:
            BlinkingLEDState = BLINK_START;
            break;
    }

    switch(BlinkingLEDState){
        case BLINK_START:
            break;
        case BLINK_INIT:
            blinkingLED = 0x00;
            break;
        case BLINK:
            blinkingLED = 0x01;
            break;
        default:
            break;
    }
}

void TickCombineLEDs(){
    switch(CombineLEDState){
        case COMBINE_START:
            CombineLEDState = COMBINE;
            break;
        case COMBINE:
            CombineLEDState = COMBINE;
            break;
        default:
            CombineLEDState = COMBINE_START;
            break;
    }
    
    switch(CombineLEDState){
        case COMBINE_START:
            break;
        case COMBINE:
            PORTB = threeLEDs | (blinkingLED << 3);
            break;
        default:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(1000);
    TimerOn();

    ThreeLEDState = THREE_START;
    BlinkingLEDState = BLINK_START;
    CombineLEDState = COMBINE_START;
    while (1) {
        TickThreeLEDs();
        TickBlinkingLEDs();
        TickCombineLEDs();
        while(!TimerFlag){};
        TimerFlag = 0;
    }
    return 1;
}
