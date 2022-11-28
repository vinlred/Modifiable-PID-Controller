#ifndef FSMBUTTON_H
#define FSMBUTTON_H

#include <stdio.h>
#include <stdlib.h>

#define IDLE1       0
#define WAIT        1
#define HELD        2
#define DEBOUNCING  3

void fsmbutton(int input, int *state, int *counter, int *output){
    switch (*state){
        case IDLE1:
    {
            if(input == 1){           // input rising edge
                *state = WAIT;
                *output = 1;          // trigger action
            }
            break;
    }
    case WAIT:
    {
      *output = 0;
      *counter += 1;
      if(*counter > 7 && input == 1){           // falling edge
        *state = HELD;
        *counter = 0;
      }
      else if(input == 0){
        *state = DEBOUNCING;
      }
      break;
    }
        case DEBOUNCING:
    {
      *counter += 1;
            if(*counter > 5){
          *state = IDLE1;
          *counter = 0;
      }
            break;
        }
        case HELD:
        {
          if(input == 1){
            *output = 1;
          }
          else{
            *state = DEBOUNCING;
      *output = 0;
          }
          break;
        }
        default:
            break;
    }
}

#endif
