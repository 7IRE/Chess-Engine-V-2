#include "../include/logic.hpp"


int main(){
    

    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.UpdateState();
    BoardState state = board.BoardState();
    int gameStatus;
    bool gameOver = false;
    while(!gameOver){
        auto Selection = board.selection();
        if(board.MoveEvaluatorState().MoveValidator(Selection, state) && board.MoveEvaluatorState().isMoveActuallyLegal(state,board.MoveEvaluatorState().createMoveData(Selection,state))){
            board.movePiece(Selection);
            board.UpdateState();
            state = board.BoardState();   

        
            gameStatus = board.MoveEvaluatorState().isCheckmateOrStalemate(state);
            if (gameStatus != 0) {
                gameOver = true;
            }
        }
    }
    board.RendererState().finalScreen(gameStatus,128,192);
   
    while(1){}
    return 0;
}