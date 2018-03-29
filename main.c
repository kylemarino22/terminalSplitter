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
        while (displayActive()) {


                // if(updateDisplay(number) == 1) {

                sprintf(text, "Hello %d", i);
                writeLine(0, text);
                i++;
                sleep(1);
        }
        endDisplay();

        printf("were done\n");
        return 0;
}
