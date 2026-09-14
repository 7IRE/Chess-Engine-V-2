#pragma once
#include <iostream>
#include <string>
#include "../include/Renderer.hpp"
#include "../include/moveEvaluator.hpp"


class Board{
    
private:
    unsigned long long whitePawns = 0, whiteKnights = 0, whiteBishops = 0, whiteRooks = 0, whiteQueens = 0, whiteKing = 0 ;
    unsigned long long blackPawns = 0, blackKnights = 0, blackBishops = 0, blackRooks = 0, blackQueens = 0, blackKing = 0 ;
    unsigned long long whitePieces = 0, blackPieces = 0, allPieces = 0; 
    unsigned long long enpassant = 0; 
    bool whiteToMove ; 
    int halfMoveClock = 0, fullMoveClock = 0;
    bool whiteCastleKing , whiteCastleQueen,    blackCastleKing ,   blackCastleQueen ;  

    Renderer renderer;

    Move move;
    BoardState boardState;

    //Selector Variables
    Position Selection={-1,-1,-1,-1};
    char Input ;
    int file=0,rank=0,colour;

    bool isPromoting = false;
    Position promotionPendingMove; 
           
public:
    Board();
    Board( std::string boardFen);
    void fenToBoard(std::string FenString);
    Position selection();
    bool movePiece(Position Pos);
    void UpdateState();
    void handlePromotionInput();
    BoardState& BoardState();
    Move& MoveEvaluatorState();
    Renderer& RendererState();
    void setPromotionPiece(char promopiece);
    bool getisPromoting();
    bool getWhiteToMove();

};


