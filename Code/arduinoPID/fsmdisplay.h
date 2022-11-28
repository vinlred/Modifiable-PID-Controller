#ifndef FSMDISPLAY_H
#define FSMDISPLAY_H

#include <stdio.h>
#include <stdlib.h>

#define STATE_DS  0
#define STATE_DP  1
#define STATE_DI  2
#define STATE_DD  3

const uint8_t segP[] = {NULL, NULL, NULL, SEG_A | SEG_B | SEG_E | SEG_F | SEG_G};
const uint8_t segI[] = {NULL, NULL, NULL, SEG_E | SEG_F};
const uint8_t segD[] = {NULL, NULL, NULL, SEG_B | SEG_C | SEG_D | SEG_E | SEG_G};
const uint8_t segS[] = {SEG_A | SEG_C | SEG_D | SEG_F | SEG_G, SEG_A | SEG_B | SEG_E | SEG_F | SEG_G, SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, NULL};

int sc = 0;

void fsmdis(int *state, int statectrl, float potValue, float kp, float ki, float kd){
    switch (*state){
        case STATE_DS:
		    {  
            if(sc == 0){
            display.setSegments(segS);
            delay(1000);
            sc = 1;
            }
            if(statectrl == 1){  
				      *state = STATE_DP;
              sc = 0;
            }
            else{
              display.showNumberDecEx(potValue*100,0b01000000,true);
            }
            break;
		    }
        case STATE_DP:
		{
            if(sc == 0){
            display.setSegments(segP);
            delay(1000);
            sc = 1;
            }
            if(statectrl == 2){  
              *state = STATE_DI;
              sc = 0;
            }
            else{
             display.showNumberDecEx(kp*100,0b01000000,true);
            }
            break;
        }
        case STATE_DI:
		{
            if(sc == 0){
            display.setSegments(segI);
            delay(1000);
            sc = 1;
            }
            if(statectrl == 3){  
              *state = STATE_DD;
              sc = 0;
            }
            else{
              display.showNumberDecEx(ki*100,0b01000000,true);
            }
            break;
        }
        case STATE_DD:
		{
            if(sc == 0){
            display.setSegments(segD);
            delay(1000);
            sc = 1;
            }
            if(statectrl == 0){
				      *state = STATE_DS;
              sc = 0;
            }
            else{
              display.showNumberDecEx(kd*100,0b01000000,true);
            }
            break;
        }
        default:
            break;
    }
}

#endif
