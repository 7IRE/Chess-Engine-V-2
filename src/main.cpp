#define RLIGHTS_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION

#include "../include/gamemanager.hpp"


    int main(){
        GameManager G1;
        int A = 1;
        while(A){
            int a = G1.mainScreen();
            int b = G1.mainGame(a);
            if(b==0){break;}
            A = G1.gameOverScreen(b);
        }
        return 0;
    }
