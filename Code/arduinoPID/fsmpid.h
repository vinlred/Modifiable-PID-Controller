#ifndef FSMPID_H
#define FSMPID_H

#include <stdio.h>
#include <stdlib.h>

#define STATE_SPEED  0
#define STATE_P      1
#define STATE_I      2
#define STATE_D      3


void fsmpid(int inset, int input1, int input2, int *state, float *kp, float *ki, float* kd){
    switch (*state){
        case STATE_SPEED:
		{  
            if(inset == 1){  
				*state = STATE_P;
            }
            break;
		}
        case STATE_P:
		{
            if(inset == 1){
				*state = STATE_I;
            }
			else if(input1 == 1){
        if(*kp >= 10.0){
          *kp = 9.9;
        }
				*kp += 0.01;
			}
			else if(input2 == 1){
        if(*kp <= 0){
          *kp = 0.01;
        }
				*kp -= 0.01;
			}
            break;
        }
        case STATE_I:
		{
            if(inset == 1){
				*state = STATE_D;
            }
			else if(input1 == 1){
        if(*ki >= 10.0){
          *ki = 9.9;
        }
				*ki += 0.01;
			}
			else if(input2 == 1){
        if(*ki <= 0){
          *ki = 0.01;
        }
				*ki -= 0.01;
			}
            break;
        }
        case STATE_D:
		{
            if(inset == 1){
				*state = STATE_SPEED;
        
  preferences.putFloat("kp",*kp);
  preferences.putFloat("ki",*ki);
  preferences.putFloat("kd",*kd);
            }
			else if(input1 == 1){
        if(*kd >= 10.0){
          *kd = 9.9;
        }
				*kd += 0.01;
			}
			else if(input2 == 1){
        if(*kd <= 0){
          *kd = 0.01;
        }
				*kd -= 0.01;
			}
            break;
        }
        default:
            break;
    }
}

#endif
