
#include "../include/logic.hpp"
#include "../include/engine.hpp"

class GameManager{
    Engine aiEngine;
    int searchDepth ; 
    bool enginePlaysBlack ;
    int gameStatus ;
    bool gameOver ;
    public:
    
    GameManager();
    int mainScreen();
    int mainGame();
    int gameOverScreen(int result);
    ~GameManager();
};


/*
int main(){
    
    
   
    
    while(!gameOver && !WindowShouldClose()){
        board.UpdateState();
        auto& state = board.BoardState();
        Position Selection={-1,-1,-1,-1};
        BeginDrawing();
        board.RendererState().updateTerminalBoard(board.BoardState());
        if(board.getisPromoting() == true){
            board.handlePromotionInput(); 
        }
        else {
            bool isEngineTurn = (board.getWhiteToMove() != enginePlaysBlack);
            if (isEngineTurn) {
                MoveData engineMove = aiEngine.getBestMove(board.BoardState(), searchDepth);
                int from = engineMove.getFrom();
                int to = engineMove.getTo();
                Selection.initFile = from % 8;
                Selection.initRank = from / 8;
                Selection.finalFile = to % 8;
                Selection.finalRank = to / 8;
            } 
            else {
                Selection = board.selection();
            }
        }
        EndMode3D();
        if(board.getWhiteToMove()){
            DrawText("WHITE TO MOVE", 10 , 1000, 20, WHITE);
        }
        else{
            DrawText("BLACK TO MOVE", 10 , 20, 20, BLACK);
        }
        EndDrawing();
        
        if (Selection.finalRank != -1) {
            auto& stateRef = board.BoardState();
            auto& evaluator = board.MoveEvaluatorState();
            if (evaluator.MoveValidator(Selection, stateRef) && 
                evaluator.isMoveActuallyLegal(stateRef, evaluator.createMoveData(Selection, stateRef))) {
                board.movePiece(Selection);
                board.UpdateState(); 
                
                gameStatus = evaluator.isCheckmateOrStalemate(board.BoardState());
                if (gameStatus != 0) {
                    gameOver = true;
                }
            }
        }
        
    }
    board.RendererState().finalScreen(gameStatus,128,192);
    return 0;
}

*/