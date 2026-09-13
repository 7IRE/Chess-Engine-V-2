#include <cmath>
#include <algorithm>
#include "../include/evaluate.hpp"



const int Evaluator::PawnPST[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
     50, 50, 50, 50, 50, 50, 50, 50,
     25, 25, 30, 40, 40, 30, 25, 25, 
     10, 10, 15, 25, 25, 15, 10, 10, 
      5,  5, 10, 20, 20, 10,  5,  5,
      5, -5,-10,  0,  0,-10, -5,  5, 
      5, 10, 10,-20,-20, 10, 10,  5, 
      0,  0,  0,  0,  0,  0,  0,  0  
};


const int Evaluator::KnightPST[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};
const int Evaluator::BishopPST[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};
const int Evaluator::RookPST[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
};
const int Evaluator::QueenPST[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
const int Evaluator::KingPST[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};

int Evaluator::evaluate(const BoardState& state) {
    int score = 0;

    unsigned long long pawns = state.whitePawns;
    while(pawns) { int sq = __builtin_ctzll(pawns); score += PAWN_VALUE + PawnPST[sq]; pawns &= pawns - 1; }
    
    unsigned long long knights = state.whiteKnights;
    while(knights) { int sq = __builtin_ctzll(knights); score += KNIGHT_VALUE + KnightPST[sq]; knights &= knights - 1; }
    
    unsigned long long bishops = state.whiteBishops;
    while(bishops) { int sq = __builtin_ctzll(bishops); score += BISHOP_VALUE + BishopPST[sq]; bishops &= bishops - 1; }
    
    unsigned long long rooks = state.whiteRooks;
    while(rooks) { int sq = __builtin_ctzll(rooks); score += ROOK_VALUE + RookPST[sq]; rooks &= rooks - 1; }
    
    unsigned long long queens = state.whiteQueens;
    while(queens) { int sq = __builtin_ctzll(queens); score += QUEEN_VALUE + QueenPST[sq]; queens &= queens - 1; }
    
    unsigned long long king = state.whiteKing;
    if(king) { int sq = __builtin_ctzll(king); score += KingPST[sq]; }
    pawns = state.blackPawns;
    while(pawns) { int sq = __builtin_ctzll(pawns); score -= (PAWN_VALUE + PawnPST[sq ^ 56]); pawns &= pawns - 1; }
    
    knights = state.blackKnights;
    while(knights) { int sq = __builtin_ctzll(knights); score -= (KNIGHT_VALUE + KnightPST[sq ^ 56]); knights &= knights - 1; }
    
    bishops = state.blackBishops;
    while(bishops) { int sq = __builtin_ctzll(bishops); score -= (BISHOP_VALUE + BishopPST[sq ^ 56]); bishops &= bishops - 1; }
    
    rooks = state.blackRooks;
    while(rooks) { int sq = __builtin_ctzll(rooks); score -= (ROOK_VALUE + RookPST[sq ^ 56]); rooks &= rooks - 1; }
    
    queens = state.blackQueens;
    while(queens) { int sq = __builtin_ctzll(queens); score -= (QUEEN_VALUE + QueenPST[sq ^ 56]); queens &= queens - 1; }
    
    king = state.blackKing;
    if(king) { int sq = __builtin_ctzll(king); score -= KingPST[sq ^ 56]; }

    if (score > 400 && state.whiteKing && state.blackKing) {
        int wKingSq = __builtin_ctzll(state.whiteKing);
        int bKingSq = __builtin_ctzll(state.blackKing);
        
        int wRank = wKingSq / 8, wFile = wKingSq % 8;
        int bRank = bKingSq / 8, bFile = bKingSq % 8;
        int centerDist = (3 - std::min(bFile, 7 - bFile)) + (3 - std::min(bRank, 7 - bRank));
        score += centerDist * 10;

        int kingDist = std::abs(wRank - bRank) + std::abs(wFile - bFile);
        score += (14 - kingDist) * 4;
    } 
    else if (score < -400 && state.whiteKing && state.blackKing) {
        int wKingSq = __builtin_ctzll(state.whiteKing);
        int bKingSq = __builtin_ctzll(state.blackKing);
        
        int wRank = wKingSq / 8, wFile = wKingSq % 8;
        int bRank = bKingSq / 8, bFile = bKingSq % 8;

        int centerDist = (3 - std::min(wFile, 7 - wFile)) + (3 - std::min(wRank, 7 - wRank));
        score -= centerDist * 10; 

        int kingDist = std::abs(wRank - bRank) + std::abs(wFile - bFile);
        score -= (14 - kingDist) * 4; 
    }
    return score;
}