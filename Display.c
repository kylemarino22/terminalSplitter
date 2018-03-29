#include <ncurses.h> //terminal output
#include <panel.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "Display.h"
#include <pthread.h>

int row,col;

PANEL *panels[2];

PANEL *stack_top;
WINDOW *temp_win, *old_win;
uint8_t resized_flag = 0;
#define MAX_MONITORS 6
int monitorNumber = 0;

int quitFlag = 0;
MEVENT event;

PANEL_DATA POA[MAX_MONITORS];
pthread_t tid;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
void *thread(void *vargp)
{
        sleep(1);
        writeLine(1,"HERE");
        while(quitFlag == 0) {
                pthread_mutex_lock(&m);
                if(updateDisplay(3) == 1) {
                        quitFlag = 1;
                }
                pthread_mutex_unlock(&m);
        }


        return NULL;
}

int displayActive(){
        return !quitFlag;
}

void endDisplay(){
        pthread_join(tid, NULL);
        endwin();
}

void initDisplay(int number){
        initscr();
        start_color();
        getmaxyx(stdscr,row,col);
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);

        /* Initialize all the colors */
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK);
        init_pair(4, COLOR_CYAN, COLOR_BLACK);

        temp_win = newwin(0, 0, 0, 0);
        POA[0].x = 0;
        POA[0].y = 0;
        POA[0].h = row;
        POA[0].w = col;
        POA[0].label_color = 1;
        strcpy(POA[0].label, "TEST");
        POA[0].panel = new_panel(temp_win);
        POA[0].valid = TRUE;
        POA[0].inputShift = 0;
        update_panels();
        clearLinesa(0);
        writeLine(0, "Hello World");
        pthread_create(&tid, NULL, thread, NULL);
        int i;
        for(i = 0; i < 50; i++) {
                writeLine(0, "Filler");
        }
        // writeLine(0, "a");
        // writeLine(0, "b");
        // writeLine(0, "Line3");
        // writeLine(0, "Line4");
        // writeLine(0, "Line5");
        // writeLine(0, "Line6");
        // writeLine(0, "Line7");
        // writeLine(0, "Line8");
        // writeLine(0, "Line9");
        // writeLine(0, "Line10");
        // writeLine(0, "This Line is way too long to fit into one line!!!");


        initMonitors();
        refreshMonitors();
}
int counter = 0;

void addMonitor(char *label, int color){
        monitorNumber++;

        // printf("here: %d\n", monitorNumber);
        // delay(1000);
        // printf("monitorNumber %d\n", monitorNumber);

        // while(1) ;
        POA[monitorNumber].h = row;
        POA[monitorNumber].y = 0;
        strcpy(POA[monitorNumber].label, label);
        strcpy(POA[monitorNumber].keyBoardInput, "hello");

        POA[monitorNumber].label_color = color;
        temp_win = newwin(0, 0, 0, 0);
        POA[monitorNumber].panel = new_panel(temp_win);
        POA[monitorNumber].inputShift = 0;

        POA[monitorNumber].valid = TRUE;

        resizeMonitors();

}
void resizeMonitors(){
        int i;
        for(i = 0; i<MAX_MONITORS; i++) {
                if(POA[i].valid == TRUE) {
                        POA[i].w = (int)col/(monitorNumber+1) + 1;
                        POA[i].x = (int)col/(monitorNumber+1)*i;
                }
        }
        POA[monitorNumber].w = col - POA[monitorNumber].x;
}

void refreshMonitors(){
        clrtoeol();
        refresh();
        int i;
        for(i = 0; i<6; i++) {
                if(POA[i].valid == TRUE) {
                        old_win = panel_window(POA[i].panel);
                        temp_win = newwin(POA[i].h, POA[i].w, POA[i].y, POA[i].x);
                        replace_panel(POA[i].panel, temp_win);
                        delwin(old_win);
                        win_show(temp_win, POA, i);
                        // mvwprintw(panel_window(POA[i].panel), POA[i].y, POA[i].x, POA[i].label);

                }
        }
        refresh();
        update_panels();
        doupdate();
}
void initMonitors(){
        addMonitor("Raspberry 2", 3);
        addMonitor("Arduino", 2);
        mousemask(ALL_MOUSE_EVENTS, NULL);
}
void clearMemory(){
        int i;
        for(i = 6; i>0; i--) {
                if(POA[i].valid == TRUE) {
                        delwin(panel_window(POA[i].panel));
                        del_panel(POA[i].panel);

                }
        }
}
int mouseX = 0;
int mouseY = 0;
int userInput = 0;
char inputBuffer[80];
int selectedMonitor;
int updateDisplay(int number){
        // initDisplay();
        // signal(SIGWINCH, thing);

        int ch = wgetch(stdscr);
        // int ch = getch();

        if(ch == KEY_MOUSE) {
                if(getmouse(&event) == OK) {
                        mouseX = event.x;
                        mouseY = event.y;
                        int i;
                        int monitorWidth = col/(monitorNumber+1);
                        for(i = 0; i<6; i++) {
                                if(POA[i].valid == TRUE) {
                                        POA[i].label_color = 0;
                                }
                        }
                        for(i = 0;; i++) {
                                if(mouseX < monitorWidth * i) {
                                        POA[i-1].label_color = 3;
                                        break;
                                }
                        }
                        //if the monitor switched
                        if(i != selectedMonitor) {
                                sprintf(POA[0].label, "Switched: %d", selectedMonitor);
                                // memset(POA[selectedMonitor - 1].keyBoardInput,0,80);
                        }
                        selectedMonitor = i;

                        //correct mouse height
                        if(mouseY >= 2  && mouseY <= 4) {
                                if(mouseX % monitorWidth > monitorWidth -7) {
                                        strcpy(POA[1].label, "Enter Clicked");
                                        // sprintf(POA[1].label, "Enter Clicked");
                                        userInput = 0;
                                        memset(POA[selectedMonitor - 1].keyBoardInput,0,80);
                                }
                                else{
                                        strcpy(POA[1].label, "Text Field Clicked");
                                        // sprintf(POA[1].label, "%s", "Text Field Clicked");
                                        userInput = 1;
                                }
                        }
                        else{
                                strcpy(POA[1].label, "Enter Not Clicked");
                                // sprintf(POA[1].label, "Enter Not Clicked");
                                userInput = 0;
                        }
                        refreshMonitors();
                }
        }

        if(userInput == 0) {
                switch(ch) {
                case 'r':
                        refreshMonitors();
                        counter++;
                        break;

                case 'q':
                        clearMemory();
                        endwin();
                        return 1;
                        break;
                }
        }
        else if(userInput == 1) {
                // append(inputBuffer, ch);
                if(ch != -1 && ch != 409) {
                        if(ch == 127) {
                                POA[selectedMonitor - 1].keyBoardInput[strlen(POA[selectedMonitor - 1].keyBoardInput) - 1] = 0;
                        }
                        else if(ch == KEY_RIGHT) {
                                if(strlen(POA[selectedMonitor - 1].keyBoardInput) - POA[selectedMonitor - 1].inputShift >
                                   POA[selectedMonitor -1].w - 10) {
                                        POA[selectedMonitor - 1].inputShift++;
                                }
                        }
                        else if(ch == KEY_LEFT) {
                                if(strlen(POA[selectedMonitor - 1].keyBoardInput) > POA[selectedMonitor -1].w - 10) {

                                        if(POA[selectedMonitor - 1].inputShift > 0) {
                                                POA[selectedMonitor - 1].inputShift--;
                                        }
                                }
                        }
                        else if(ch == 10) {
                                memset(POA[selectedMonitor - 1].keyBoardInput, 0, 80 * sizeof(char));
                        }
                        else{
                                append(POA[selectedMonitor - 1].keyBoardInput, (char)ch, 79);
                        }
                        // sprintf(POA[selectedMonitor - 1].keyBoardInput, "%s", inputBuffer);
                        refreshMonitors();
                }
        }
        return 0;
}


void win_show(WINDOW *win, PANEL_DATA *POA, int number)
{
        int startx, starty, height, width;

        // getbegyx(win, starty, startx);
        getmaxyx(win, height, width);

        box(win, 0, 0);
        mvwaddch(win, 2, 0, ACS_LTEE);
        mvwaddch(win, 4, 0, ACS_LTEE);
        mvwprintw(win, 3, width-7, "Enter");
        mvwhline(win, 2, 1, ACS_HLINE, width - 2);
        mvwhline(win, 4, 1, ACS_HLINE, width - 2);
        mvwaddch(win, 2, width - 1, ACS_RTEE);
        mvwaddch(win, 4, width - 1, ACS_RTEE);

        mvwaddch(win, 2, width-9, ACS_TTEE);
        mvwaddch(win, 3, width-9, ACS_VLINE);
        mvwaddch(win, 4, width-9, ACS_BTEE);
        mvwaddch(win, 1, 1, ACS_HLINE);
        printShifter(win, POA[number].keyBoardInput, 3, 1, POA[number].inputShift, width-10);
        printScreen(number);

        print_in_middle(win, 1, 0, POA[number].w, POA[number].label, COLOR_PAIR(POA[number].label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
        int length, x, y;
        float temp;

        if(win == NULL)
                win = stdscr;
        getyx(win, y, x);
        if(startx != 0)
                x = startx;
        if(starty != 0)
                y = starty;
        if(width == 0)
                width = 80;

        length = strlen(string);
        temp = (width - length)/ 2;
        x = startx + (int)temp;
        wattron(win, color);
        mvwprintw(win, y, x, "%s", string);
        wattroff(win, color);
        refresh();
}

void destroy_win(WINDOW *local_win)
{
        wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        wrefresh(local_win);
        delwin(local_win);
}
void append(char* s, char c, int maxLen) {
        int len = strlen(s);

        if(len <= maxLen - 1) {
                s[len] = c;
                s[len+1] = '\0';
        }
}

void printShifter(WINDOW *win, char *str, int y, int x, int shift, int len){
        int i;
        char newStr[len];
        for(i = 0; i < len; i++) {
                newStr[i] = str[i + shift];
        }
        newStr[i]='\0';
        mvwprintw(win, y, x, newStr);
}
void writeLine(int monitorNumber, char *line){
        //get current available line
        int i;
        int j;
        int k;
        int lineLength = POA[monitorNumber].w -1;

        // POA[monitorNumber].lines[0].line;
        // strlen(POA[monitorNumber].lines[0].line);
        for(i = 0; i < 64; i++) {
                if(strlen(POA[monitorNumber].lines[i].line) == 0) {
                        break;
                }
        }

        if(i > 63) {
                shiftLines(POA[monitorNumber].lines);
                i--;
        }
        for(k = 0;; k++) {
                for(j=0; (j < lineLength && j < strlen(line)); j++) {
                        POA[monitorNumber].lines[i + k].line[j] = line[j + k * lineLength];
                }
                //end if remaining line has less than line length number of characters
                if((k+1) * lineLength > strlen(line)) {
                        break;
                }

                // if(i == 63) {
                //         shiftLines(POA[monitorNumber].lines);
                // }

        }
        printScreen(monitorNumber);
        refresh();
        update_panels();
        doupdate();
        usleep(1000);

}

void shiftLines(line_t *lines){
        int i;
        for(i = 0; i < 63; i++) {
                lines[i] = lines[i+ 1];
        }
        memset(lines[63].line, '\0', 64);
}

void clearLinesa(int monitorNumber){
        int i;
        for(i = 0; i < 64; i++) {
                memset(POA[monitorNumber].lines[i].line, '\0', 64);
        }
}

void printScreen(int monitorNumber){
        int width = POA[monitorNumber].w;
        //position of cursor in the output buffer
        int pos = 0;

        //gets current line
        int i;
        int j;
        for(i = 0; i < 64; i++) {
                if(strlen(POA[monitorNumber].lines[i].line) == 0) {
                        break;
                }
        }
        //if number of lines in buffer is less/greater than screen lines

        if(i < POA[monitorNumber].h - 5) {
                for(j = 0; j < i; j++) {
                        // refreshMonitors();
                        mvwprintw(panel_window(POA[monitorNumber].panel), j + 5, 1,  "%s", POA[monitorNumber].lines[j].line);

                }
        }
        else{
                for(j = POA[monitorNumber].h-6; j > 0; j--) {
                        mvwprintw(panel_window(POA[monitorNumber].panel), j+4, 1, "%s", POA[monitorNumber].lines[j+i - POA[monitorNumber].h +4].line);
                }
        }
}
