#include <iostream>
#include "../include/TerminalRenderer.hpp"


// fg 30 -  37 , 90 - 97 
//bg 40 - 47 , 100 - 107

Renderer::Renderer(){}

void Renderer::AlignWindow(){
    printf("Please make sure all of this lines are is visible on your screen , you can resize if you want and then enter anything you want . \n");
    for(int i=0;i<320;i++){
        printf("|\n");
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    int temp;
    std::cin>>temp;
    printf("\033[2J");
}
 
void Renderer::SetTerminalSize(int x , int y ){
    printf("\e[8;%d;%dt",x,y);
}

void Renderer::Border(int c){
    int x=31,y=62;
    int gap=64*8;
    printf("\033[%d;%dH",x,y); 
    printf("\033[%dm                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ",c);   printf("\033[%d;%dH\033[0m",x+1,y);
    for(int i=1;i<=32*8;i++){
            printf("\033[%dm  \033[%d;%dH  ",c,x+i,y+gap+2);   printf("\033[%d;%dH\033[0m",x+i+1,y);
    }
    printf("\033[%dm                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ",c);   printf("\033[%d;%dH\033[0m",x+1,y);   printf("\033[%d;%dH\033[0m",x+32,y); 
}

void Renderer::initializeTerminalBoard(){
    AlignWindow();

    int n=9;
    fflush(stdin);
    // SetTerminalSize(320,640);
    Border(47);
    for(int i=1;i<n;i++){
        for(int j=1;j<n;j++){
            if((i+j)%2==0){
                white.Empty(i*32,j*64,c1);
            }
            else{
                white.Empty(i*32,j*64,c2);
            }
        }
    }
}

void Renderer::Selector(int y,int x ,int c,bool Blink){
    if(c==-1){
        if((x+y)%2==0){c = c1;}
        else          {c = c2;}
    }
    int gap=62;
    x *= 32 , y *= 64;
    if(Blink){
        printf("\033[%d;%dH",x,y); 
        printf("\033[%d;5m                                                                ",c);   printf("\033[%d;%dH\033[0m",x+1,y);
        for(int i=1;i<=30;i++){
                printf("\033[%d;5m  \033[%d;%dH  ",c,x+i,y+gap);   printf("\033[%d;%dH\033[0m",x+i+1,y);
        }
        printf("\033[%d;5m                                                                ",c);   printf("\033[%d;%dH\033[0m",x+32,y); 
    }
    else{
        printf("\033[%d;%dH",x,y); 
        printf("\033[%dm                                                                ",c);   printf("\033[%d;%dH\033[0m",x+1,y);
        for(int i=1;i<=30;i++){
                printf("\033[%dm  \033[%d;%dH  ",c,x+i,y+gap);   printf("\033[%d;%dH\033[0m",x+i+1,y);
        }
        printf("\033[%dm                                                                ",c);   printf("\033[%d;%dH\033[0m",x+32,y); 
    }
}

void Renderer::updateTerminalBoard(BoardState boardState){
    //Reset Current Board
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            currentBoard[i][j]=' ';
        }
    }
     /*Board is Numbered from left to right bottom to top
      Fen from top to  bottom   
      7 - 0 1 2 3 4 5 6 7 -> file
      6
      5
      4
      3
      2
      1
      0 -> rank
    */
    /*Array representation
      00 01 02 03 04 05 06 07 
      10
      20
      30
      40
      50
      60
      70
    */
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            unsigned long long bitSelector = 1ULL<<((7-i)*8+j);
            //WHITE
            if     (    bitSelector & boardState.whitePawns    ){   currentBoard[i][j] = 'P'; }
            else if(    bitSelector & boardState.whiteKnights  ){   currentBoard[i][j] = 'N'; }
            else if(    bitSelector & boardState.whiteBishops  ){   currentBoard[i][j] = 'B'; }
            else if(    bitSelector & boardState.whiteKing     ){   currentBoard[i][j] = 'K'; }
            else if(    bitSelector & boardState.whiteQueens   ){   currentBoard[i][j] = 'Q'; }
            else if(    bitSelector & boardState.whiteRooks    ){   currentBoard[i][j] = 'R'; }
            
            //BLACK
            else if(    bitSelector & boardState.blackPawns    ){   currentBoard[i][j] = 'p'; }
            else if(    bitSelector & boardState.blackKnights  ){   currentBoard[i][j] = 'n'; }
            else if(    bitSelector & boardState.blackBishops  ){   currentBoard[i][j] = 'b'; }
            else if(    bitSelector & boardState.blackKing     ){   currentBoard[i][j] = 'k'; }
            else if(    bitSelector & boardState.blackQueens   ){   currentBoard[i][j] = 'q'; }
            else if(    bitSelector & boardState.blackRooks    ){   currentBoard[i][j] = 'r'; }
        }
    }
    int n=9;
    fflush(stdin);
    for(int i=1;i<n;i++){
        for(int j=1;j<n;j++){
            if(currentBoard[i-1][j-1]!=prevBoard[i-1][j-1]){
                int colour=c1;
                if((i+j)%2==0){colour=c1;}
                else{colour=c2;}
                int x = i*32, y = j*64;
                //WHITE
                if     (currentBoard[i-1][j-1]=='P'){white.Pawn   (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='N'){white.Knight (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='K'){white.King   (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='Q'){white.Queen  (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='B'){white.Bishoop(x,y,colour);}
                else if(currentBoard[i-1][j-1]=='R'){white.Rook   (x,y,colour);}
                
                //BLACK
                else if(currentBoard[i-1][j-1]=='p'){black.Pawn   (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='n'){black.Knight (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='k'){black.King   (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='q'){black.Queen  (x,y,colour);}
                else if(currentBoard[i-1][j-1]=='b'){black.Bishoop(x,y,colour);}
                else if(currentBoard[i-1][j-1]=='r'){black.Rook   (x,y,colour);}
                else {black.Empty(x,y,colour);}
            }
        }
    }
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            prevBoard[i][j] = currentBoard[i][j];
        }
    }
}

// int main(){
//         int n=9;
//         White D(1);
//         fflush(stdin);
//         D.SetTerminalSize(320,640);
//         D.Border(47);
//        for(int i=1;i<n;i++){
//                 for(int j=1;j<n;j++){
//                         if((i+j)%2==0){
//                                 D.Rook(i*32,j*64,101);
                                
//                         }
//                         else{
//                                 D.Rook(i*32,j*64,40);
                                
//                         }
//                 }
//        }
//        return 0;
// }