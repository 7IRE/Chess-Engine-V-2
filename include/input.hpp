#ifndef INPUT_HPP
#define INPUT_HPP 

#include <iostream>

//Specific Libraries for input without Enter
#ifdef _WIN32
    #include <conio.h>  // Built-in to Windows compilers
#else
    #include <termios.h> // Built-in to Linux/macOS
    #include <unistd.h>
#endif

class Position{
public:
    int initRank;
    int initFile;
    int finalRank;
    int finalFile;
};

class inputMove{
public:
    char getRawChar();
};

#endif