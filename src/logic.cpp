#include <iostream>
#include <string>
#include "../include/logic.hpp"


       
    //whiteToMove 1 for white , 0 for black
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1


        /*Board is Numbered from left to right bottom to top
          Fen from top to  bottom   

          7 - 0 1 2 3 4 5 6 7
          6
          5
          4
          3
          2
          1
          0
        */

Board::Board(){std::string boardFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ;  fenToBoard(boardFen);  file = 4;    rank = 1; }
Board::Board( std::string boardFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"     ){  fenToBoard(boardFen);  file = 4;    rank = 1; }

BoardState& Board::BoardState(){return boardState;}
Move& Board::MoveEvaluatorState(){return move;}
Renderer& Board::RendererState(){return renderer;}
bool Board::getisPromoting(){return isPromoting;}
bool Board::getWhiteToMove(){return whiteToMove;}

void Board::UpdateState(){
    boardState.whitePawns = whitePawns , boardState.whiteKnights = whiteKnights , boardState.whiteBishops =  whiteBishops , boardState.whiteRooks = whiteRooks , boardState.whiteQueens = whiteQueens , boardState.whiteKing = whiteKing ;
    boardState.blackPawns = blackPawns , boardState.blackKnights = blackKnights , boardState.blackBishops =  blackBishops , boardState.blackRooks = blackRooks , boardState.blackQueens = blackQueens , boardState.blackKing = blackKing ;
    boardState.whitePieces = whitePieces , boardState.blackPieces =  blackPieces , boardState.allPieces =  allPieces ; 
    boardState.enpassant = enpassant ; 
    boardState.whiteToMove = whiteToMove ; 
    boardState.halfMoveClock = halfMoveClock ,boardState.fullMoveClock = fullMoveClock ;
    boardState.whiteCastleKing = whiteCastleKing ,boardState.whiteCastleQueen = whiteCastleQueen,boardState.blackCastleKing = blackCastleKing , boardState.blackCastleQueen =  blackCastleQueen ;  

}

void Board::fenToBoard(std::string FenString){    
    whitePawns = 0, whiteKnights = 0, whiteBishops = 0, whiteRooks = 0, whiteQueens = 0, whiteKing = 0 ;
    blackPawns = 0, blackKnights = 0, blackBishops = 0, blackRooks = 0, blackQueens = 0, blackKing = 0 ;
    whitePieces = 0, blackPieces = 0, allPieces = 0; 
    enpassant = 0;
    whiteToMove = true; 
    halfMoveClock =0, fullMoveClock=0;
    whiteCastleKing = false , whiteCastleQueen = false ,    blackCastleKing = false ,   blackCastleQueen = false; 
        
    int stringIterator=0;      
    for(int rank = 7;rank>=0;){
        for(int file = 0 ; file <=8 ;){
            auto bit_selector = 1ULL << (rank*8 + file); 
            
            //BLACK
            if      (FenString[stringIterator]=='r'){   blackRooks   = blackRooks   |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='n'){   blackKnights = blackKnights |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='b'){   blackBishops = blackBishops |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='q'){   blackQueens  = blackQueens  |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='k'){   blackKing    = blackKing    |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='p'){   blackPawns   = blackPawns   |   bit_selector ;  file++; }
            
            //WHITE
            else if (FenString[stringIterator]=='R'){   whiteRooks   = whiteRooks   |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='N'){   whiteKnights = whiteKnights |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='B'){   whiteBishops = whiteBishops |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='Q'){   whiteQueens  = whiteQueens  |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='K'){   whiteKing    = whiteKing    |   bit_selector ;  file++; }
            else if (FenString[stringIterator]=='P'){   whitePawns   = whitePawns   |   bit_selector ;  file++; }
            
            else if(FenString[stringIterator]>'0' && FenString[stringIterator]<'9'){    file += FenString[stringIterator] - '0';}
            else if(FenString[stringIterator]=='/'|| FenString[stringIterator]==' '){    stringIterator++;   break;  }
            stringIterator++;
        }
        rank--;
    }
    if(FenString[stringIterator]==' '){  stringIterator++;  }
    
    if(FenString[stringIterator]=='w'){whiteToMove=true;}
    else{whiteToMove=false;}
    stringIterator+=2;
    
    while(FenString[stringIterator]!=' '){
        if     (FenString[stringIterator] == 'K' ){    whiteCastleKing=true;   }
        else if(FenString[stringIterator] == 'Q' ){    whiteCastleQueen=true;  }
        else if(FenString[stringIterator] == 'k' ){    blackCastleKing=true;   }
        else if(FenString[stringIterator] == 'q' ){    blackCastleQueen=true;  }
        stringIterator++;
    }
    stringIterator++;
    if(FenString[stringIterator]=='-'){}
    else if(FenString[stringIterator]>='a' && FenString[stringIterator]<='h'){
        enpassant= 1ULL << ( 8*( FenString[stringIterator+1]-'1' )  + FenString[stringIterator]-'a');
        stringIterator++;
    }
    stringIterator -=- 2; // Add 2 to string iterator
    while(FenString[stringIterator]==' '){stringIterator++;}
    while(FenString[stringIterator]!=' '){
        halfMoveClock *= 10;
        halfMoveClock += FenString[stringIterator++]-'0';
    }
    stringIterator++;
    while(stringIterator<FenString.length() && FenString[stringIterator]!=' '){
        fullMoveClock *= 10;
        fullMoveClock += FenString[stringIterator++]-'0';
    }
    whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
    blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
    allPieces = whitePieces | blackPieces;

} 

Position Board::selection(){
    
    /*Board is Numbered from left to right bottom to top
        Fen from top to  bottom   

          7 - 0 1 2 3 4 5 6 7
          6
          5
          4
          3
          2
          1
          0
    */

    if(Selection.finalRank!=-1){
        Selection.initFile=-1;
        Selection.initRank=-1;
        Selection.finalFile=-1;
        Selection.finalRank=-1;
    }

    int keyPressed = GetKeyPressed();
    if (keyPressed >= 65 && keyPressed <= 90) keyPressed += 32;
    if      (keyPressed == 'w') { rank++; }
    else if (keyPressed == 'a') { file--; }
    else if (keyPressed == 's') { rank--; }
    else if (keyPressed == 'd') { file++; }
    if(file<0){file=7;}
    if(rank<0){rank=7;}
    if(file>7){file=0;}
    if(rank>7){rank=0;}
    
    if(keyPressed == 'y'){
        if(Selection.initFile==-1){
            Selection.initFile=file;
            Selection.initRank=rank;
        }
        else if(Selection.finalRank==-1){
            Selection.finalFile=file;
            Selection.finalRank=rank;
        }
    }
    if(Selection.initFile!=-1){
            renderer.Selector(7-Selection.initRank,Selection.initFile,true);
    }
    renderer.Selector(7-rank,file,false);
    return Selection;
}

bool Board::movePiece(Position Pos){
    //NOTE : DOES NOT CHECK IF A MOVE IS VALID OR NOT 
    //Return False on Move Failure
    unsigned long long Curr_Sq  = 1ULL << (Pos.initFile +  8*Pos.initRank);
    unsigned long long final_Sq = 1ULL << (Pos.finalFile +  8*Pos.finalRank);   
    if((Curr_Sq & whitePieces ) && whiteToMove){
        if (final_Sq & whitePieces) return false;
        
        unsigned long long* whiteBitboards[] = {
            &whitePawns, &whiteKnights, &whiteBishops, &whiteRooks, &whiteQueens, &whiteKing
        };

        unsigned long long* movingPieceBB = nullptr;
        for (auto* bb : whiteBitboards) {
            if (Curr_Sq & *bb) {
                movingPieceBB = bb;
                break; 
            }
        }


        if (!movingPieceBB) return false;

        bool isCapture = (final_Sq & allPieces);
        bool isPawnMove = (movingPieceBB == &whitePawns);

        if (isPawnMove && (final_Sq == enpassant)) {
            unsigned long long capturedPawn = enpassant >> 8; 
            blackPawns &= ~capturedPawn;
            isCapture = true; 
        }


        if (final_Sq & allPieces) {
            blackPawns   &= ~final_Sq;
            blackKnights &= ~final_Sq;
            blackBishops &= ~final_Sq;
            blackRooks   &= ~final_Sq;
            blackQueens  &= ~final_Sq;
            blackKing    &= ~final_Sq;

            if (final_Sq == (1ULL << 56)) blackCastleQueen = false;
            if (final_Sq == (1ULL << 63)) blackCastleKing = false;
        }

        if (movingPieceBB == &whiteKing) {
            if (Pos.initFile == 4 && Pos.finalFile == 6) {        
                whiteRooks ^= (1ULL << 7) | (1ULL << 5);          
            } else if (Pos.initFile == 4 && Pos.finalFile == 2) { 
                whiteRooks ^= (1ULL << 0) | (1ULL << 3);          
            }
            whiteCastleKing = false;
            whiteCastleQueen = false;
        }
        if (movingPieceBB == &whiteRooks) {
            if (Curr_Sq == (1ULL << 0)) whiteCastleQueen = false; 
            if (Curr_Sq == (1ULL << 7)) whiteCastleKing = false;  
        }
        
        if (isPawnMove && (Pos.finalRank - Pos.initRank == 2)) {
            enpassant = 1ULL << (Pos.initFile + 8 * (Pos.initRank + 1));
        } else {
            enpassant = 0; 
        }

        if (isCapture || isPawnMove) {
            halfMoveClock = 0;
        }
         else {
            halfMoveClock++;
        }
        
        if(isPawnMove && Pos.finalRank == 7){
            // char Input='x';
            // while(Input<'1' || Input>'4'){
            //     Input = input.getRawChar();
            // }
            // if(Input=='1') {whiteKnights ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Knight Promo
            // if(Input=='2') {whiteBishops ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Bishop Promo
            // if(Input=='3') {whiteRooks   ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Rook   Promo
            // if(Input=='4') {whiteQueens  ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Queen  Promo
            *movingPieceBB ^= Curr_Sq;
            isPromoting = true;
            promotionPendingMove = Pos; 
        }
        else{
            *movingPieceBB ^= Curr_Sq;
            *movingPieceBB ^= final_Sq;
        }

        whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
        blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
        allPieces   = whitePieces | blackPieces;
        whiteToMove =!whiteToMove;
        return true;
    }
    else if((Curr_Sq & blackPieces) && !whiteToMove){
        if (final_Sq & blackPieces) return false;
        unsigned long long* blackBitboards[] = {
            &blackPawns, &blackKnights, &blackBishops, &blackRooks, &blackQueens, &blackKing
        };

        unsigned long long* movingPieceBB = nullptr;
        for(auto* bb : blackBitboards){
            if(Curr_Sq & *bb){
                movingPieceBB = bb;
                break;
            }
        }

        if (!movingPieceBB) return false;

        bool isCapture = (final_Sq & allPieces);
        bool isPawnMove = (movingPieceBB == &blackPawns);

        if (isPawnMove && (final_Sq == enpassant)) {
            unsigned long long capturedPawn = enpassant << 8; 
            whitePawns &= ~capturedPawn;
            isCapture = true; 
        }

        if (final_Sq & allPieces) {
            whitePawns   &= ~final_Sq;
            whiteKnights &= ~final_Sq;
            whiteBishops &= ~final_Sq;
            whiteRooks   &= ~final_Sq;
            whiteQueens  &= ~final_Sq;
            whiteKing    &= ~final_Sq;

            if (final_Sq == (1ULL << 0)) whiteCastleQueen = false; 
            if (final_Sq == (1ULL << 7)) whiteCastleKing = false;
        }


        if (movingPieceBB == &blackKing) {
            if (Pos.initFile == 4 && Pos.finalFile == 6) {        
                blackRooks ^= (1ULL << 63) | (1ULL << 61);        
            } else if (Pos.initFile == 4 && Pos.finalFile == 2) { 
                blackRooks ^= (1ULL << 56) | (1ULL << 59);        
            }
            blackCastleKing = false;
            blackCastleQueen = false;
        }
        if (movingPieceBB == &blackRooks) {
            if (Curr_Sq == (1ULL << 56)) blackCastleQueen = false; 
            if (Curr_Sq == (1ULL << 63)) blackCastleKing = false;  
        }

        if (isPawnMove && (Pos.initRank - Pos.finalRank == 2)) {
            enpassant = 1ULL << (Pos.initFile + 8 * (Pos.initRank - 1));
        } else {
            enpassant = 0;
        }
    
        if (isCapture || isPawnMove) {
            halfMoveClock = 0;
        }
         else {
            halfMoveClock++;
        }


        if(isPawnMove && Pos.finalRank == 0){
            // char Input='x';
            // while(Input<'1' || Input>'4'){
            //     Input = input.getRawChar();
            // }
            // if(Input=='1') {blackKnights ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Knight Promo
            // if(Input=='2') {blackBishops ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Bishop Promo
            // if(Input=='3') {blackRooks   ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Rook   Promo
            // if(Input=='4') {blackQueens  ^= final_Sq;  *movingPieceBB ^= Curr_Sq;}// Queen  Promo
            *movingPieceBB ^= Curr_Sq;
            isPromoting = true;
            promotionPendingMove = Pos; 
        }
        else{
            *movingPieceBB ^= Curr_Sq;
            *movingPieceBB ^= final_Sq;
        }
        whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
        blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
        allPieces   = whitePieces | blackPieces;

        fullMoveClock++;
        whiteToMove =!whiteToMove;
        return true;
    }
    else{
        return false;
    }
}

void Board::setPromotionPiece(char promopiece){
    int Input;
    if(promopiece=='q')Input = 4;
    if(promopiece=='r')Input = 3;
    
    if(promopiece=='b')Input = 2;
    if(promopiece=='k')Input = 1;
    if (Input < '1' || Input > '4') return; 

    unsigned long long final_Sq = 1ULL << (promotionPendingMove.finalFile + 8 * promotionPendingMove.finalRank);

    if (!whiteToMove) { 
        if      (Input == '1') whiteKnights |= final_Sq;
        else if (Input == '2') whiteBishops |= final_Sq;
        else if (Input == '3') whiteRooks   |= final_Sq;
        else if (Input == '4') whiteQueens  |= final_Sq;
    } 
    else { 
        if      (Input == '1') blackKnights |= final_Sq;
        else if (Input == '2') blackBishops |= final_Sq;
        else if (Input == '3') blackRooks   |= final_Sq;
        else if (Input == '4') blackQueens  |= final_Sq;
    }

    whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
    blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
    allPieces   = whitePieces | blackPieces;
}
void Board::handlePromotionInput() {
    if (!isPromoting) return;

    int Input = GetKeyPressed();
    if (Input < '1' || Input > '4') return; 

    unsigned long long final_Sq = 1ULL << (promotionPendingMove.finalFile + 8 * promotionPendingMove.finalRank);

    if (!whiteToMove) { 
        if      (Input == '1') whiteKnights |= final_Sq;
        else if (Input == '2') whiteBishops |= final_Sq;
        else if (Input == '3') whiteRooks   |= final_Sq;
        else if (Input == '4') whiteQueens  |= final_Sq;
    } 
    else { 
        if      (Input == '1') blackKnights |= final_Sq;
        else if (Input == '2') blackBishops |= final_Sq;
        else if (Input == '3') blackRooks   |= final_Sq;
        else if (Input == '4') blackQueens  |= final_Sq;
    }

    whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
    blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
    allPieces   = whitePieces | blackPieces;

    isPromoting = false; 
}