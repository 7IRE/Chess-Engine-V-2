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


Board::Board( std::string boardFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"     ){ renderer.initializeTerminalBoard(); fenToBoard(boardFen);   }

BoardState Board::BoardState(){return boardState;}
Move Board::MoveEvaluatorState(){return move;}


void Board::UpdateState(){
    boardState.whitePawns = whitePawns , boardState.whiteKnights = whiteKnights , boardState.whiteBishops =  whiteBishops , boardState.whiteRooks = whiteRooks , boardState.whiteQueens = whiteQueens , boardState.whiteKing = whiteKing ;
    boardState.blackPawns = blackPawns , boardState.blackKnights = blackKnights , boardState.blackBishops =  blackBishops , boardState.blackRooks = blackRooks , boardState.blackQueens = blackQueens , boardState.blackKing = blackKing ;
    boardState.whitePieces = whitePieces , boardState.blackPieces =  blackPieces , boardState.allPieces =  allPieces ; 
    boardState.enpassant = enpassant ; 
    boardState.whiteToMove = whiteToMove ; 
    boardState.halfMoveClock = halfMoveClock ,boardState.fullMoveClock = fullMoveClock ;
    boardState.whiteCastleKing = whiteCastleKing ,boardState.whiteCastleQueen = whiteCastleQueen,boardState.blackCastleKing = blackCastleKing , boardState.blackCastleQueen =  blackCastleQueen ;  

    renderer.updateTerminalBoard(boardState);
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
    Position Selection;
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
    char Input ;
    int file,rank,colour;
    int prevFile,prevRank;

    bool ck;
    do{
        do{
            ck = false;
            Input = ' ';
            if(whiteToMove){    file = prevFile = 7 , rank = prevRank = 0 ,colour = 107; }
            else           {    file = prevFile = 0 , rank = prevRank = 7 ,colour = 40; }

            while(Input!='y'){
                Input = input.getRawChar();
                if(Input>=65 && Input <=90){Input = Input + 32;}

                if      (Input=='w'){rank++;}
                else if (Input=='a'){file--;}
                else if (Input=='s'){rank--;}
                else if (Input=='d'){file++;}

                if(rank<0){rank=7;}
                else if(rank>7){rank=0;}

                if(file<0){file=7;}
                else if(file>7){file=0;}

                renderer.Selector(prevFile+1,8-prevRank);
                
                renderer.Selector(file+1,8-rank,colour);
                prevFile=file,prevRank=rank;
            }
            unsigned long long bitSelected = 1ULL <<(8*rank)+file;
            if(whiteToMove){ ck = (bitSelected & whitePieces);}
            else { ck = (bitSelected & blackPieces);}
            if(ck){
                renderer.Selector(prevFile+1,8-prevRank);
                renderer.Selector(file+1,8-rank,43,true);
            }
            else{
                renderer.Selector(prevFile+1,8-prevRank);
                renderer.Selector(file+1,8-rank,41); 
                renderer.Selector(file+1,8-rank); 
            }
        }while(!ck);
        Selection.initFile=file , Selection.initRank = rank;


            ck = false;
            Input = ' ';


            while(Input!='y'){
                Input = input.getRawChar();
                if(Input>=65 && Input <=90){Input = Input + 32;}

                if      (Input=='w'){rank++;}
                else if (Input=='a'){file--;}
                else if (Input=='s'){rank--;}
                else if (Input=='d'){file++;}

                if(rank<0){rank=7;}
                else if(rank>7){rank=0;}

                if(file<0){file=7;}
                else if(file>7){file=0;}

                if(prevFile==Selection.initFile && prevRank==Selection.initRank){renderer.Selector(prevFile+1,8-prevRank,43,true);}
                else{renderer.Selector(prevFile+1,8-prevRank);}
                renderer.Selector(file+1,8-rank,colour);
                prevFile=file,prevRank=rank;
            }

            renderer.Selector(file+1,8-rank);
            renderer.Selector(Selection.initFile+1,8-Selection.initRank);
            Selection.finalFile=file , Selection.finalRank=rank;
            unsigned long long bitSelected = 1ULL <<(8*rank)+file;
            if(whiteToMove){ck = (whitePieces & bitSelected);}
            else{ck = (blackPieces & bitSelected);}
            if(Selection.initFile==Selection.finalFile && Selection.initRank==Selection.finalRank){
                ck = true;
        }
    }while(ck);

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

        if (final_Sq & allPieces) {
            blackPawns   &= ~final_Sq;
            blackKnights &= ~final_Sq;
            blackBishops &= ~final_Sq;
            blackRooks   &= ~final_Sq;
            blackQueens  &= ~final_Sq;
            blackKing    &= ~final_Sq;
        }
        
        if (isCapture || isPawnMove) {
            halfMoveClock = 0;
        }
         else {
            halfMoveClock++;
        }

        *movingPieceBB ^= Curr_Sq;
        *movingPieceBB ^= final_Sq;

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
        bool isPawnMove = (movingPieceBB == &whitePawns);

        if (final_Sq & allPieces) {
            whitePawns   &= ~final_Sq;
            whiteKnights &= ~final_Sq;
            whiteBishops &= ~final_Sq;
            whiteRooks   &= ~final_Sq;
            whiteQueens  &= ~final_Sq;
            whiteKing    &= ~final_Sq;
        }

         
        if (isCapture || isPawnMove) {
            halfMoveClock = 0;
        }
         else {
            halfMoveClock++;
        }

        *movingPieceBB ^= Curr_Sq;
        *movingPieceBB ^= final_Sq;

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

int main(){
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.UpdateState();
    
    while(1){
        auto Selection = board.selection();
        if(board.MoveEvaluatorState().MoveValidator(Selection,board.BoardState())){
            board.movePiece(Selection);
            board.UpdateState();
        }
    }
    return 0 ;
}