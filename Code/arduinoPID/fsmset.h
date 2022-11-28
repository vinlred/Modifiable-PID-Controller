#ifndef FSMSET_H
#define FSMSET_H

#include <stdio.h>
#include <stdlib.h>

#define IDLE2        0
#define DEBOUNCING  1
#define HELD        2

void fsmset(int input, int *state, int *counter, int *output){
    switch (*state){
        case IDLE2:
    {
            if(input == 1){           // input rising edge
                *state = HELD;
                *output = 1;          // trigger action
            }
            break;
    }
    case HELD:
    {
      *output = 0;
      if(input == 0){           // falling edge
        *state = DEBOUNCING;
      }
      break;
    }
        case DEBOUNCING:
    {
      *counter += 1;
            if(*counter > 5){
          *state = IDLE2;
          *counter = 0;
      }
            break;
        }
        default:
            break;
    }
}

#endif
