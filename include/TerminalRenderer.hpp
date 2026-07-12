#include <iostream>
#include "../include/TerminalDrawer.hpp"
#include "../include/boardState.hpp"

class Renderer{
private:
    int c1 = 100 , c2 = 42;
    White white;
    Black black;
    char currentBoard[8][8];
    char prevBoard[8][8] = {{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' '}};

    void AlignWindow();

    void SetTerminalSize(int x , int y );

    void Border(int c);

public:
    Renderer();
    
    void initializeTerminalBoard();

    void Selector(int y,int x ,int c=-1,bool Blink=false);

    void updateTerminalBoard(BoardState boardState);

    void finalScreen(int gamestate , int x , int y );
};

