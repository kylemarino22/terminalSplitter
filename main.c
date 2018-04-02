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

int main(int argc,char *argv[])
{
        int counter = 0;
        int i;
        char text[10];

        initDisplay(number);
        addMonitor("Data Stream", 1);
        addMonitor("Menu", 3);
        addMonitor("Debug", 2);
        while (displayActive()) {

                sprintf(text, "%d", i);
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
