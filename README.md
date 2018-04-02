# terminalSplitter
A c library that creates a more useful GUI for the terminal by splitting print statements into different parts of the screen.

### About 

Terminal output is usually limited to a single screen, and to have different outputs from one program can be messy with several terminal windows. terminalSplitter solves this issue, allowing the user to view several output consoles from the terminal. Uses for this library range from monitoring several processors, displaying menus, or simply an organized way of displaying debug statements. 

### Usage
Using terminalSplitter is very simple. To enter typing mode, click on the box to the left of any Enter button to select the keyboard input. You can type here and to send characters to the input buffer, either press the enter key or click the enter button on the screen. This input buffer can later be read with the function ```readInput();``` Click anywhere else on the screen to leave typing mode. When not in typing mode, ressing ```r``` will refresh the page, and pressing ```q``` will exit the program. 

### Example Layout
![Demo of Program] (https://i.imgur.com/3pE5aU2.png)

(I am using the agnoster theme for Oh My Zsh. Your terminal output may look different.)

### Implementation
##### Compilation
To integrate terminalSplitter into your program is very simple. Add the dislplay.c and display.h files into your c program and link them as you normally would into the compiler. 

##### terminalSplitter functions
To start terminalSplitter, run the code:
```
initDisplay(3);
addMonitor("Data Stream", 1);
addMonitor("Menu", 3);
addMonitor("Debug", 2);
```
```initDisplay()``` takes in the number of desired terminal ouputs as a parameter and initializes the library.

```addMonitor()``` takes in the name of a monitor, and the color of the name label. This should be run directly after ```initDisplay();``` The colors are as follows:
```
0 White
1 Red
2 Green
3 Blue
4 Teal
```

To print to a monitor, use the function:
```
writeLine(0, text, -1);
```
```writeLine()``` takes in the monitor number to print in, the text to print, and the line number to print on. -1 for the line number will print a new line. 

To read from the input buffer, use the function:
```
readInput(1);
```
```readInput()``` takes in the monitor number to read from, and returns the character input buffer. The buffer is cleared once read from. 

To check if the display is still active, use the function: 
```
dislpayActive();
```
This returns a 1 if the display is active and 0 if it has quit. 

End terminalSplitter with the function:
```
endDisplay();
```
This ends the multi-threading and cleans up before quitting.
##### Example main.c
The included main.c file uses all of these functions to create a simple program:
```
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
```
This will simulate a data stream in monitor 0, and in monitors 1 and 2, if text is inputted it will appear under the respective monitor. 


