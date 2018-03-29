#ifndef Display_h_
#define Display_h_

typedef struct {
        char line[64];
}line_t;

typedef struct {
        uint8_t valid;
        int x, y, w, h;
        char label[80];
        char inputBuffer[256];
        char keyBoardInput[80];
        line_t lines[64];
        int label_color;
        int inputShift;
        PANEL *panel;
}PANEL_DATA;



void initDisplay(int number);
int updateDisplay(int number);
void win_show(WINDOW *win, PANEL_DATA *POA, int number);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void thing(int sig);
void destroy_win(WINDOW *local_win);
void middleShow(WINDOW *win, char *label, int label_color);
void addMonitor(char *label, int color);
void refreshMonitors();
void resizeMonitors();
void initMonitors();
void clearMemory();
void append(char* s, char c, int maxLen);
void printShifter(WINDOW *win, char *str, int y, int x, int shift, int len);
void printScreen (int monitorNumber);
void clearLinesa(int monitorNumber);
void writeLine(int monitorNumber, char *line);
void *thread(void *vargp);
void shiftLines(line_t *lines);
int displayActive();
void endDisplay();


#endif
