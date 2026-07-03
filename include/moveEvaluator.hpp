#include "../src/magicbitboards.cpp"
#include "../include/input.hpp"
#include "../include/boardState.hpp"

class Move{
private:
    unsigned long long KnightMoveBoard[64] = {0};
    unsigned long long KingMoveBoard  [64] = {0};

    unsigned long long WhitePawnAttackBoard[64]  = {0};
    unsigned long long WhitePawnMoveBoard[64][2] = {0};
    unsigned long long BlackPawnAttackBoard[64]  = {0};
    unsigned long long BlackPawnMoveBoard[64][2] = {0};
    
    unsigned long long WhiteKingSidePath ;
    unsigned long long WhiteQueenSidePath;
    unsigned long long BlackKingSidePath ;
    unsigned long long BlackQueenSidePath;


public:
    Move();
    bool MoveValidator (Position Pos ,const BoardState&  boardState);
    bool isSquareAttacked(unsigned long long Cur_sq , const BoardState& boardState);
};
