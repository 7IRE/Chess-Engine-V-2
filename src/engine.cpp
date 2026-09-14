#include "../include/engine.hpp"
#include <algorithm>
int Engine::alphaBeta(BoardState& state, int depth, int alpha, int beta) {
    if (depth == 0) {
        return quiescenceSearch(state, alpha, beta);
    }

    MoveList moveList;
    moveGenerator.generateAllMoves(moveList, state);
    orderMoves(moveList, state);
    bool hasLegalMoves = false;

    for (int i = 0; i < moveList.count; i++) {
        MoveData candidateMove = moveList.moves[i];
        UndoState undo = moveGenerator.makeMove(state, candidateMove);

        state.whiteToMove = !state.whiteToMove;
        bool kingInCheck = moveGenerator.isCheck(state); 
        state.whiteToMove = !state.whiteToMove;

        if (kingInCheck) {
            moveGenerator.unmakeMove(state, candidateMove, undo);
            continue;
        }

        hasLegalMoves = true;

        int score = -alphaBeta(state, depth - 1, -beta, -alpha);
        
        moveGenerator.unmakeMove(state, candidateMove, undo);

        if (score >= beta) return beta; 
        if (score > alpha) alpha = score;
    }

    if (!hasLegalMoves) {
        if (moveGenerator.isCheck(state)) return -INFINITY_SCORE + (100 - depth);
        return 0; 
    }

    return alpha;
}

MoveData Engine::getBestMove(BoardState state, int maxDepth) {
    MoveList moveList;
    moveGenerator.generateAllMoves(moveList, state);
    orderMoves(moveList, state);
    int bestScore = -INFINITY_SCORE;
    MoveData bestMove;

    for (int i = 0; i < moveList.count; i++) {
        MoveData candidateMove = moveList.moves[i];
        UndoState undo = moveGenerator.makeMove(state, candidateMove);

        state.whiteToMove = !state.whiteToMove;
        bool kingInCheck = moveGenerator.isCheck(state);
        state.whiteToMove = !state.whiteToMove;

        if (kingInCheck) {
            moveGenerator.unmakeMove(state, candidateMove, undo); 
            continue;
        }

        int score = -alphaBeta(state, maxDepth - 1, -INFINITY_SCORE, INFINITY_SCORE);
        moveGenerator.unmakeMove(state, candidateMove, undo);

        if (score > bestScore || bestMove.getFrom() == bestMove.getTo()) {
            bestScore = score;
            bestMove = candidateMove;
        }
    }

    return bestMove;
}



int Engine::quiescenceSearch(BoardState& state, int alpha, int beta) {
    int eval = evaluator.evaluate(state);
    int standPat = state.whiteToMove ? eval : -eval;

    if (standPat >= beta) return beta;
    if (alpha < standPat) alpha = standPat;

    MoveList moveList;
    moveGenerator.generateAllMoves(moveList, state);
    orderMoves(moveList, state);
    unsigned long long enemyPieces = state.whiteToMove ? state.blackPieces : state.whitePieces;
    for (int i = 0; i < moveList.count; i++) {
        MoveData candidateMove = moveList.moves[i];
        int toSq = candidateMove.getTo();
        int flag = candidateMove.getFlag();

        bool isCapture = ((1ULL << toSq) & enemyPieces) != 0;
        bool isEnPassant = (flag == 6);
        bool isQueenPromotion = (flag == 4); 

        if (!isCapture && !isEnPassant && !isQueenPromotion) {
            continue;
        }

        UndoState undo = moveGenerator.makeMove(state, candidateMove);

        state.whiteToMove = !state.whiteToMove;
        bool kingInCheck = moveGenerator.isCheck(state);
        state.whiteToMove = !state.whiteToMove;

        if (kingInCheck) {
            moveGenerator.unmakeMove(state, candidateMove, undo);
            continue;
        }

        int score = -quiescenceSearch(state, -beta, -alpha);
        
        moveGenerator.unmakeMove(state, candidateMove, undo);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}


int Engine::getPieceValue(int sq, const BoardState& state) {
    unsigned long long mask = 1ULL << sq;
    if (mask & (state.whitePawns | state.blackPawns)) return 100;
    if (mask & (state.whiteKnights | state.blackKnights)) return 200;
    if (mask & (state.whiteBishops | state.blackBishops)) return 200;
    if (mask & (state.whiteRooks | state.blackRooks)) return 300;
    if (mask & (state.whiteQueens | state.blackQueens)) return 400;
    if (mask & (state.whiteKing | state.blackKing)) return 500;
    return 0;
}

int Engine::scoreMove(const MoveData& move, const BoardState& state) {
    int to = move.getTo();
    int from = move.getFrom();
    int flag = move.getFlag();

    int victimVal = getPieceValue(to, state);
    int attackerVal = getPieceValue(from, state);

    if (flag == 6) { 
        victimVal = 100;
    }

    if (victimVal > 0) {
        return 1000 + (victimVal * 10) - attackerVal; 
    }

    if (flag == 4) {
        return 900; 
    }

    return 0; 
}

void Engine::orderMoves(MoveList& moveList, const BoardState& state) {
    int moveScores[256];
    for (int i = 0; i < moveList.count; i++) {
        moveScores[i] = scoreMove(moveList.moves[i], state);
    }

    for (int i = 0; i < moveList.count - 1; i++) {
        int bestScore = moveScores[i];
        int bestIndex = i;
        for (int j = i + 1; j < moveList.count; j++) {
            if (moveScores[j] > bestScore) {
                bestScore = moveScores[j];
                bestIndex = j;
            }
        }
        std::swap(moveList.moves[i], moveList.moves[bestIndex]);
        std::swap(moveScores[i], moveScores[bestIndex]);
    }
}