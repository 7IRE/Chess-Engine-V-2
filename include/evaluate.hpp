#pragma once
#include "boardState.hpp"

class Evaluator {
private:
    const int PAWN_VALUE = 100;
    const int KNIGHT_VALUE = 320;
    const int BISHOP_VALUE = 330;
    const int ROOK_VALUE = 500;
    const int QUEEN_VALUE = 900;

    static const int PawnPST[64];
    static const int KnightPST[64];
    static const int BishopPST[64];
    static const int RookPST[64];
    static const int QueenPST[64];
    static const int KingPST[64];

public:
    int evaluate(const BoardState& state);
};