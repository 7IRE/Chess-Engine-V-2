#pragma once
#include "boardState.hpp"
#include "moveEvaluator.hpp"
#include "evaluate.hpp"

class Engine {
private:
    Move moveGenerator;
    Evaluator evaluator;
    const int INFINITY_SCORE = 30000;

    int alphaBeta(BoardState& state, int depth, int alpha, int beta);
    int quiescenceSearch(BoardState& state, int alpha, int beta);

    int getPieceValue(int sq, const BoardState& state);
    int scoreMove(const MoveData& move, const BoardState& state);
    void orderMoves(MoveList& moveList, const BoardState& state);
public:
    MoveData getBestMove(BoardState state, int maxDepth);
};