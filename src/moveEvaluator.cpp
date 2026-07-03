#include "../include/moveEvaluator.hpp"

bool Move::isSquareAttacked(unsigned long long Cur_sq ,const BoardState& boardState){

    if(boardState.whiteToMove){
        //look for black attacks    
    }
    else{
        //look for white attacks
    }

}

Move::Move(){
    //Initialize All Bitboards
    init_magics();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);
    //rank * 8 + file
    //Initialize Knight Bitboard
    for(int i=0 ; i<=7 ; i++){
        for(int j=0 ; j<=7 ; j++){
           
            if((i-1)>=0 && (j-2)>=0){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i-1)*8 + (j-2)) ;}
            if((i-1)>=0 && (j+2)<=7){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i-1)*8 + (j+2)) ;}
            if((i+1)<=7 && (j-2)>=0){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i+1)*8 + (j-2)) ;}
            if((i+1)<=7 && (j+2)<=7){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i+1)*8 + (j+2)) ;}
            if((i-2)>=0 && (j-1)>=0){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i-2)*8 + (j-1)) ;}
            if((i-2)>=0 && (j+1)<=7){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i-2)*8 + (j+1)) ;}
            if((i+2)<=7 && (j-1)>=0){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i+2)*8 + (j-1)) ;}
            if((i+2)<=7 && (j+1)<=7){ KnightMoveBoard[i*8 + j] |=  1ULL << ((i+2)*8 + (j+1)) ;}
        }
    }
    
    //Initialize King BitBoard
    for(int i=0 ; i<=7 ; i++){
        for(int j=0 ; j<=7 ; j++){
            if((i-1)>=0 && (j-1)>=0){ KingMoveBoard[i*8 + j]   |=  1ULL << ((i-1)*8 + (j-1)) ;}
            if((i-1)>=0 && (j+1)<=7){ KingMoveBoard[i*8 + j]   |=  1ULL << ((i-1)*8 + (j+1)) ;}
            if((i+1)<=7 && (j-1)>=0){ KingMoveBoard[i*8 + j]   |=  1ULL << ((i+1)*8 + (j-1)) ;}
            if((i+1)<=7 && (j+1)<=7){ KingMoveBoard[i*8 + j]   |=  1ULL << ((i+1)*8 + (j+1)) ;}
            if((i-1)>=0            ){ KingMoveBoard[i*8 + j]   |=  1ULL << ((i-1)*8 + j)     ;}
            if((i+1)<=7            ){ KingMoveBoard[i*8 + j]   |=  1ULL << ((i+1)*8 + j)     ;}
            if((j-1)>=0            ){ KingMoveBoard[i*8 + j]   |=  1ULL << (i*8 + (j-1))     ;}
            if((j+1)<=7            ){ KingMoveBoard[i*8 + j]   |=  1ULL << (i*8 + (j+1))     ;}
        }
    }

    //Initialize Pawn Bitboard
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++){
            if((i+1)<=7){WhitePawnMoveBoard[i*8 + j][0] |= 1ULL << ((i+1)*8 + j);}
            if(i==1    ){WhitePawnMoveBoard[i*8 + j][1] |= 1ULL << ((i+2)*8 + j);}
            if((i-1)>=0){BlackPawnMoveBoard[i*8 + j][0] |= 1ULL << ((i-1)*8 + j);}
            if(i==6    ){BlackPawnMoveBoard[i*8 + j][1] |= 1ULL << ((i-2)*8 + j);}

            if(((i+1)<=7) && ((j-1)>=0)){WhitePawnAttackBoard[i*8 + j] |= 1ULL << ((i+1)*8 + j-1);}
            if(((i+1)<=7) && ((j+1)<=7)){WhitePawnAttackBoard[i*8 + j] |= 1ULL << ((i+1)*8 + j+1);}
            if(((i-1)>=0) && ((j-1)>=0)){BlackPawnAttackBoard[i*8 + j] |= 1ULL << ((i-1)*8 + j-1);}
            if(((i-1)>=0) && ((j+1)<=7)){BlackPawnAttackBoard[i*8 + j] |= 1ULL << ((i-1)*8 + j+1);}
        }
    }

    WhiteKingSidePath   = (1ULL<<5)  | (1ULL<<6);
    WhiteQueenSidePath  = (1ULL<<1)  | (1ULL<<2)  | (1ULL<<3);
    BlackKingSidePath   = (1ULL<<61) | (1ULL<<62);
    BlackQueenSidePath  = (1ULL<<57) | (1ULL<<58) | (1ULL<<59); 
}

bool Move::MoveValidator (Position Pos , const BoardState& boardState){
    unsigned long long Curr_Sq  = 1ULL << (Pos.initFile +  8*Pos.initRank);
    unsigned long long final_Sq = 1ULL << (Pos.finalFile +  8*Pos.finalRank);  
    int native_square = Pos.initRank*8 + Pos.initFile;
    
    if((boardState.whitePieces & Curr_Sq) && boardState.whiteToMove){
        if     (Curr_Sq & boardState.whitePawns  ){ 
            if(final_Sq & WhitePawnAttackBoard[native_square]){
                return ((final_Sq & boardState.blackPieces) || (final_Sq == boardState.enpassant));
            }
            else if (final_Sq & WhitePawnMoveBoard[native_square][0]) {
                return !(final_Sq & boardState.allPieces);
            }
            else if((final_Sq & WhitePawnMoveBoard[native_square][1]) && (!(WhitePawnMoveBoard[native_square][0] & boardState.allPieces)) && (!(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.whiteKnights){  
            if((    (final_Sq & (KnightMoveBoard[native_square]))   ) && (  ( final_Sq & boardState.blackPieces ) || !( final_Sq & boardState.allPieces ) )){
                return true;
            } 
            return false;
        }
        else if(Curr_Sq & boardState.whiteBishops){
            if(((get_bishop_attacks(native_square , boardState.allPieces)) & final_Sq) && ( (final_Sq & boardState.blackPieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.whiteRooks  ){
            if(((get_rook_attacks(native_square , boardState.allPieces)) & final_Sq) && ( (final_Sq & boardState.blackPieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.whiteQueens ){
            if((((  get_bishop_attacks(native_square , boardState.allPieces)  | get_rook_attacks(native_square , boardState.allPieces) )) & final_Sq) && ( (final_Sq & boardState.blackPieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.whiteKing   ){
            if(((final_Sq & (KingMoveBoard[native_square]))) && ((final_Sq & boardState.blackPieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
            //Further Implementation Required
        }
        return false;
    }
    else if((boardState.blackPieces & Curr_Sq) && !boardState.whiteToMove){
            if     (Curr_Sq & boardState.blackPawns  ){
                if(final_Sq & BlackPawnAttackBoard[native_square]){
                    return ((final_Sq & boardState.whitePieces) || (final_Sq == boardState.enpassant));
                }
                else if(final_Sq & BlackPawnMoveBoard[native_square][0]){
                    return !(final_Sq & boardState.allPieces);
                }
                else if((final_Sq & BlackPawnMoveBoard[native_square][1]) && (!(BlackPawnMoveBoard[native_square][0] & boardState.allPieces)) && (!(final_Sq & boardState.allPieces))){
                    return true;
                }   
                return false;
        }
        else if(Curr_Sq & boardState.blackKnights){  
            if((    (final_Sq & (KnightMoveBoard[native_square]))   ) && (  ( final_Sq & boardState.whitePieces ) || !( final_Sq & boardState.allPieces ) )){
                return true;
            } 
            return false;
        }
        else if(Curr_Sq & boardState.blackBishops){
            if(((get_bishop_attacks(native_square , boardState.allPieces)) & final_Sq) && ( (final_Sq & boardState.whitePieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.blackRooks  ){
            if(((get_rook_attacks(native_square , boardState.allPieces)) & final_Sq) && ( (final_Sq & boardState.whitePieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.blackQueens ){
            if((((  get_bishop_attacks(native_square , boardState.allPieces)  | get_rook_attacks(native_square , boardState.allPieces) )) & final_Sq) && ( (final_Sq & boardState.whitePieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
        }
        else if(Curr_Sq & boardState.blackKing   ){
            if(((final_Sq & (KingMoveBoard[native_square]))) && ((final_Sq & boardState.whitePieces) || !(final_Sq & boardState.allPieces))){
                return true;
            }
            return false;
            //Further Implementation Required
        }
        return false; 
    }
    else{
        return false;
    }
}

