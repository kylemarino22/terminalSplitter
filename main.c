#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> /* memcpy, memset */
#include <unistd.h>
#include <ncurses.h> //terminal output
#include <panel.h>
#include "Display.h"
#include <pthread.h>


int number = 3;

char test[20];
int main(int argc,char *argv[])
{
        int counter = 0;
        int i;
        char text[10];

        initDisplay(number);
        addMonitor("Raspberry 1", 1);
        addMonitor("Raspberry 2", 3);
        addMonitor("Arduino", 2);
        while (displayActive()) {


                // if(updateDisplay(number) == 1) {

                sprintf(text, "Hello %d", i);
                writeLine(0, text,-1);
                writeLine(1, readInput(1), -1);
                writeLine(2, readInput(2), -1);
                i++;
                sleep(1);
        }
        endDisplay();

        printf("were done\n");
        return 0;
}
