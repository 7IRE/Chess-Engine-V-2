#ifndef BOARD_STATE
#define BOARD_STATE

class BoardState{
public:
    unsigned long long whitePawns , whiteKnights , whiteBishops , whiteRooks , whiteQueens , whiteKing ;
    unsigned long long blackPawns , blackKnights , blackBishops , blackRooks , blackQueens , blackKing ;
    unsigned long long whitePieces , blackPieces , allPieces ; 
    unsigned long long enpassant ; 
    bool whiteToMove ; 
    int halfMoveClock , fullMoveClock ;
    bool whiteCastleKing , whiteCastleQueen,    blackCastleKing ,   blackCastleQueen ;  
};

#endif