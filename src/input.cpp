#include "../include/input.hpp"

char inputMove::getRawChar() { 
    #ifdef _WIN32
        return _getch(); // Direct Windows kernel call
    #else
        char buf = 0;
        struct termios old = {0};
        tcgetattr(0, &old); // Grab current terminal state
        struct termios current = old;
        current.c_lflag &= ~ICANON; // Turn off "Wait for Enter"
        current.c_lflag &= ~ECHO;   // Turn off "Show character on screen"
        tcsetattr(0, TCSANOW, &current);
        read(0, &buf, 1);
        tcsetattr(0, TCSADRAIN, &old); // Reset terminal to normal
        return buf;
    #endif
}    
