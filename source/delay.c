#include "delay.h"

void delayUs(int n) {
    int i, j;
    for(i = 0 ; i < n*5; i++)
    	for(j = 0; j < 5; j++);
}

void delayMs(int n) {
    int i, j;
    for(i = 0 ; i < n; i++)
        for (j = 0; j < 3500; j++);
}
