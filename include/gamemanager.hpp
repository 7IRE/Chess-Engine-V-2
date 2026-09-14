
#include "../include/logic.hpp"
#include "../include/engine.hpp"
#include <atomic>

class GameManager{
    Engine aiEngine;
    Board board;
    int searchDepth ; 
    bool enginePlaysBlack ;
    int gameStatus ;
    bool gameOver ;
    std::atomic<bool> isAiThinking;
    MoveData backgroundAiMove;
    Music main_screen;
    Sound paperRipSound;
    Sound ClickSound;
    Sound HitSound;
    public:
    
    GameManager();
    int mainScreen();
    int mainGame(int options);
    int gameOverScreen(int result);
    int twoPlayer();
    int playerVsAi();
    ~GameManager();
};


    
   
    