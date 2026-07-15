#include "../include/moveEvaluator.hpp"
bool Move::isSquareAttacked(unsigned long long Cur_sq ,const BoardState& boardState){
    if (Cur_sq == 0) return false;
    int square = __builtin_ctzll(Cur_sq);
    if(boardState.whiteToMove){
        //look for black attacks    
        if (KnightMoveBoard[square] & boardState.blackKnights) return true;
        if (KingMoveBoard[square] & boardState.blackKing) return true;
        if (WhitePawnAttackBoard[square] & boardState.blackPawns) return true;
        if (get_rook_attacks(square, boardState.allPieces) & (boardState.blackRooks | boardState.blackQueens)) return true;
        if (get_bishop_attacks(square, boardState.allPieces) & (boardState.blackBishops | boardState.blackQueens)) return true;
    }
    else{
        //look for white attacks
        if (KnightMoveBoard[square] & boardState.whiteKnights) return true;
        if (KingMoveBoard[square] & boardState.whiteKing) return true;
        if (BlackPawnAttackBoard[square] & boardState.whitePawns) return true;
        if (get_rook_attacks(square, boardState.allPieces) & (boardState.whiteRooks | boardState.whiteQueens)) return true;
        if (get_bishop_attacks(square, boardState.allPieces) & (boardState.whiteBishops | boardState.whiteQueens)) return true;   
    }
    return false;
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
            if (final_Sq == (1ULL << 6) && boardState.whiteCastleKing) {
                if (!(WhiteKingSidePath & boardState.allPieces)) {
                    if (!isSquareAttacked(1ULL << 4, boardState) && !isSquareAttacked(1ULL << 5, boardState) &&  !isSquareAttacked(1ULL << 6, boardState)) {   
                        return true;
                    }
                }
            }
            if (final_Sq == (1ULL << 2) && boardState.whiteCastleQueen) {
                if (!(WhiteQueenSidePath & boardState.allPieces)) {
                    if (!isSquareAttacked(1ULL << 4, boardState) && !isSquareAttacked(1ULL << 3, boardState) && !isSquareAttacked(1ULL << 2, boardState)) {  
                        return true;
                    }
                }
            }
            return false;
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
            if (final_Sq == (1ULL << 62) && boardState.blackCastleKing) {
                if (!(BlackKingSidePath & boardState.allPieces)) {
                    if (!isSquareAttacked(1ULL << 60, boardState) && !isSquareAttacked(1ULL << 61, boardState) && !isSquareAttacked(1ULL << 62, boardState)) {  
                        return true;
                    }
                }
            }
            if (final_Sq == (1ULL << 58) && boardState.blackCastleQueen) {
                if (!(BlackQueenSidePath & boardState.allPieces)) {
                    if (!isSquareAttacked(1ULL << 60, boardState) && !isSquareAttacked(1ULL << 59, boardState) && !isSquareAttacked(1ULL << 58, boardState)) {   
                        return true;
                    } 
                }
            }
            return false;        
        }
        return false; 
    }
    else{
        return false;
    }
}
    


void Move::generateAllMoves(MoveList &moveList ,const BoardState& boardState){
    unsigned long long friendlyPieces = boardState.whiteToMove ? boardState.whitePieces : boardState.blackPieces;
    unsigned long long enemyPieces    = boardState.whiteToMove ? boardState.blackPieces : boardState.whitePieces;
    unsigned long long allPieces      = boardState.whitePieces | boardState.blackPieces;

    unsigned long long knights = boardState.whiteToMove ? boardState.whiteKnights : boardState.blackKnights;
    while (knights) {
        int fromSquare = __builtin_ctzll(knights); 
        unsigned long long targets = KnightMoveBoard[fromSquare] & ~friendlyPieces;
        
        while (targets) {
            int toSquare = __builtin_ctzll(targets);
            moveList.push_back(MoveData(fromSquare, toSquare));
            targets &= targets - 1; 
        }
        knights &= knights - 1; 
    }

    unsigned long long king = boardState.whiteToMove ? boardState.whiteKing : boardState.blackKing;
    if (king) {
        int fromSquare = __builtin_ctzll(king);
        unsigned long long targets = KingMoveBoard[fromSquare] & ~friendlyPieces;
        
        while (targets) {
            int toSquare = __builtin_ctzll(targets);
            moveList.push_back(MoveData(fromSquare, toSquare));
            targets &= targets - 1;
        }
    }

    unsigned long long bishops = boardState.whiteToMove ? boardState.whiteBishops : boardState.blackBishops;
    while (bishops) {
        int fromSquare = __builtin_ctzll(bishops);
        unsigned long long targets = get_bishop_attacks(fromSquare, allPieces) & ~friendlyPieces;
        while (targets) {
            int toSquare = __builtin_ctzll(targets);
            moveList.push_back(MoveData(fromSquare, toSquare));
            targets &= targets - 1;
        }
        bishops &= bishops - 1;
    }

    unsigned long long rooks = boardState.whiteToMove ? boardState.whiteRooks : boardState.blackRooks;
    while (rooks) {
        int fromSquare = __builtin_ctzll(rooks);
        unsigned long long targets = get_rook_attacks(fromSquare, allPieces) & ~friendlyPieces;
        while (targets) {
            int toSquare = __builtin_ctzll(targets);
            moveList.push_back(MoveData(fromSquare, toSquare));
            targets &= targets - 1;
        }
        rooks &= rooks - 1;
    }

    unsigned long long queens = boardState.whiteToMove ? boardState.whiteQueens : boardState.blackQueens;
    while (queens) {
        int fromSquare = __builtin_ctzll(queens);
        unsigned long long targets = (get_rook_attacks(fromSquare, allPieces) | get_bishop_attacks(fromSquare, allPieces)) & ~friendlyPieces;
        while (targets) {
            int toSquare = __builtin_ctzll(targets);
            moveList.push_back(MoveData(fromSquare, toSquare));
            targets &= targets - 1;
        }
        queens &= queens - 1;
    }


    if (boardState.whiteToMove) {
        unsigned long long pawns = boardState.whitePawns;
        while (pawns) {
            int fromSquare = __builtin_ctzll(pawns);

            unsigned long long singlePush = WhitePawnMoveBoard[fromSquare][0] & ~allPieces;
            if (singlePush) {
                int toSquare = __builtin_ctzll(singlePush);
                //Promotion
                if (toSquare >= 56) {
                    moveList.push_back(MoveData(fromSquare, toSquare, 1)); // Knight Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 2)); // Bishop Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 3)); // Rook   Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 4)); // Queen  Promo
                } else {
                    moveList.push_back(MoveData(fromSquare, toSquare));
                
                    unsigned long long doublePush = WhitePawnMoveBoard[fromSquare][1] & ~allPieces;
                    if (doublePush) {
                        int doubleToSquare = __builtin_ctzll(doublePush);
                        moveList.push_back(MoveData(fromSquare, doubleToSquare, 5)); // Double push flag
                    }
                }
            }
            //Diagonal Attacks
            unsigned long long attacks = WhitePawnAttackBoard[fromSquare] & enemyPieces;
            while (attacks) {
                int toSquare = __builtin_ctzll(attacks);
                
                // Promotion Capture
                if (toSquare >= 56) {
                    moveList.push_back(MoveData(fromSquare, toSquare, 1)); // Knight Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 2)); // Bishop Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 3)); // Rook   Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 4)); // Queen  Promo
                } else {
                    moveList.push_back(MoveData(fromSquare, toSquare));
                }
                attacks &= attacks - 1;
            }

            unsigned long long epTarget = WhitePawnAttackBoard[fromSquare] & boardState.enpassant;
            if (epTarget) { 
                int toSquare = __builtin_ctzll(epTarget);
                moveList.push_back(MoveData(fromSquare, toSquare, 6)); //En Passant
            }
            
            pawns &= pawns - 1;
        }
    } 
    else {
        unsigned long long pawns = boardState.blackPawns;
        while (pawns) {
            int fromSquare = __builtin_ctzll(pawns);
            
            unsigned long long singlePush = BlackPawnMoveBoard[fromSquare][0] & ~allPieces;
            if (singlePush) {
                int toSquare = __builtin_ctzll(singlePush);
                
                //Promotion
                if (toSquare <= 7) {
                    moveList.push_back(MoveData(fromSquare, toSquare, 1)); // Knight Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 2)); // Bishop Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 3)); // Rook   Promo
                    moveList.push_back(MoveData(fromSquare, toSquare, 4)); // Queen  Promo
                } else {
                    moveList.push_back(MoveData(fromSquare, toSquare));
                    
                    
                    unsigned long long doublePush = BlackPawnMoveBoard[fromSquare][1] & ~allPieces;
                    if (doublePush) {
                        int doubleToSquare = __builtin_ctzll(doublePush);
                        moveList.push_back(MoveData(fromSquare, doubleToSquare, 5)); //Double Push Flag
                    }
                }
            }
            
            //Diagonal Attacks
            unsigned long long attacks = BlackPawnAttackBoard[fromSquare] & enemyPieces;
            while (attacks) {
                int toSquare = __builtin_ctzll(attacks);
                
                // Promotion Capture
                if (toSquare <= 7) {
                    moveList.push_back(MoveData(fromSquare, toSquare, 1));
                    moveList.push_back(MoveData(fromSquare, toSquare, 2));
                    moveList.push_back(MoveData(fromSquare, toSquare, 3));
                    moveList.push_back(MoveData(fromSquare, toSquare, 4));
                } else {
                    moveList.push_back(MoveData(fromSquare, toSquare));
                }
                attacks &= attacks - 1;
            }
            unsigned long long epTarget = BlackPawnAttackBoard[fromSquare] & boardState.enpassant;
            if (epTarget) {
                int toSquare = __builtin_ctzll(epTarget);
                moveList.push_back(MoveData(fromSquare, toSquare, 6)); // En Passant
            }
            pawns &= pawns - 1;
        }
    }

    if (boardState.whiteToMove) {
        if (boardState.whiteCastleKing && !(allPieces & WhiteKingSidePath)) {
            if (!isSquareAttacked(1ULL << 4, boardState) && !isSquareAttacked( 1ULL << 5, boardState) && !isSquareAttacked(1ULL << 6, boardState)) {
                moveList.push_back(MoveData(4, 6, 7)); // King-side Castle
            }
        }
        if (boardState.whiteCastleQueen && !(allPieces & WhiteQueenSidePath)) {
            if (!isSquareAttacked(1ULL << 4, boardState) && !isSquareAttacked(1ULL << 3, boardState) && !isSquareAttacked(1ULL << 2, boardState)) {
                moveList.push_back(MoveData(4, 2, 8)); // Queen-side Castle
            }
        }
    } 
    else { 
        if (boardState.blackCastleKing && !(allPieces & BlackKingSidePath)) {
            if (!isSquareAttacked(1ULL << 60, boardState) && !isSquareAttacked(1ULL << 61, boardState) && !isSquareAttacked(1ULL << 62, boardState)) {
                moveList.push_back(MoveData(60, 62, 7)); // King-side Castle
            }
        }
        if (boardState.blackCastleQueen && !(allPieces & BlackQueenSidePath)) {
            if (!isSquareAttacked(1ULL <<60, boardState) && !isSquareAttacked(1ULL << 59, boardState) && !isSquareAttacked(1ULL <<58, boardState)) {
                moveList.push_back(MoveData(60, 58, 8));  // Queen-side Castle
            }
        }
    }
}

UndoState Move::makeMove(BoardState& state, const MoveData& moveData) {
    int from = moveData.getFrom();
    int to = moveData.getTo();
    int flag = moveData.getFlag();  

    unsigned long long fromSq = 1ULL << from;
    unsigned long long toSq   = 1ULL << to;

    int castleRights = (state.whiteCastleKing  ? 1 : 0) | (state.whiteCastleQueen ? 2 : 0) | (state.blackCastleKing  ? 4 : 0) | (state.blackCastleQueen ? 8 : 0);
    /*
    whiteCastleKing  -1 
    whiteCastleQueen -2
    blackCastleKing  -4
    blackCastleQueen -8
    */

    int epFile = 63;
    if (state.enpassant) {
        int square = __builtin_ctzll(state.enpassant); 
        epFile = square & 7; 
    }

    int capturedType = 6; // 6 = No capture
    if (flag != 6) { // Flag 6 is en passant
        unsigned long long* enemyBBs[] = {
            state.whiteToMove ? &state.blackPawns : &state.whitePawns,
            state.whiteToMove ? &state.blackKnights : &state.whiteKnights,
            state.whiteToMove ? &state.blackBishops : &state.whiteBishops,
            state.whiteToMove ? &state.blackRooks : &state.whiteRooks,
            state.whiteToMove ? &state.blackQueens : &state.whiteQueens,
            state.whiteToMove ? &state.blackKing : &state.whiteKing
        };
        for (int i = 0; i < 6; i++) {
            if (*enemyBBs[i] & toSq) {
                *enemyBBs[i] &= ~toSq;
                capturedType = i;
                break;
            }
        }
    }

    UndoState undo;
    undo.pack(capturedType, castleRights, epFile, state.halfMoveClock);

    unsigned long long* friendlyBBs[] = {
        state.whiteToMove ? &state.whitePawns : &state.blackPawns,
        state.whiteToMove ? &state.whiteKnights : &state.blackKnights,
        state.whiteToMove ? &state.whiteBishops : &state.blackBishops,
        state.whiteToMove ? &state.whiteRooks : &state.blackRooks,
        state.whiteToMove ? &state.whiteQueens : &state.blackQueens,
        state.whiteToMove ? &state.whiteKing : &state.blackKing
    };

    unsigned long long* movingPieceBB = nullptr;
    for (auto* bb : friendlyBBs) {
        if (*bb & fromSq) { movingPieceBB = bb; break; }
    }
    
    if (!movingPieceBB) return undo;
    
    state.enpassant = 0;
    bool isPawnMove = (movingPieceBB == (state.whiteToMove ? &state.whitePawns : &state.blackPawns));
    if (capturedType != 6 || isPawnMove) state.halfMoveClock = 0;
    else state.halfMoveClock++;

    if (flag >= 1 && flag <= 4) { // Promotions
        *movingPieceBB ^= fromSq;
        unsigned long long* promoBB = nullptr;
        if (state.whiteToMove) {
            if      (flag == 1) promoBB = &state.whiteKnights;
            else if (flag == 2) promoBB = &state.whiteBishops;
            else if (flag == 3) promoBB = &state.whiteRooks;
            else                promoBB = &state.whiteQueens;
        } else {
            if      (flag == 1) promoBB = &state.blackKnights;
            else if (flag == 2) promoBB = &state.blackBishops;
            else if (flag == 3) promoBB = &state.blackRooks;
            else                promoBB = &state.blackQueens;
        }
        *promoBB |= toSq;
    } 
    else if (flag == 5) { // Double Pawn Push
        *movingPieceBB ^= fromSq; *movingPieceBB ^= toSq;
        state.enpassant = state.whiteToMove ? (fromSq << 8) : (fromSq >> 8);
    } 
    else if (flag == 6) { // En Passant Capture
        *movingPieceBB ^= fromSq; *movingPieceBB ^= toSq;
        unsigned long long capPawnSq = state.whiteToMove ? (toSq >> 8) : (toSq << 8);
        if (state.whiteToMove) state.blackPawns &= ~capPawnSq; 
        else                   state.whitePawns &= ~capPawnSq;
    } 
    else if (flag == 7) { // King-side Castle
        *movingPieceBB ^= fromSq; *movingPieceBB ^= toSq;
        if (state.whiteToMove) state.whiteRooks ^= (1ULL << 7) | (1ULL << 5);
        else                   state.blackRooks ^= (1ULL << 63) | (1ULL << 61);
    } 
    else if (flag == 8) { // Queen-side Castle
        *movingPieceBB ^= fromSq; *movingPieceBB ^= toSq;
        if (state.whiteToMove) state.whiteRooks ^= (1ULL << 0) | (1ULL << 3);
        else                   state.blackRooks ^= (1ULL << 56) | (1ULL << 59);
    } 
    else { // Standard Move
        *movingPieceBB ^= fromSq; *movingPieceBB ^= toSq;
    }

    // Update castling
    if (fromSq & (1ULL << 4) || toSq & (1ULL << 4)) { state.whiteCastleKing = false; state.whiteCastleQueen = false; }
    if (fromSq & (1ULL << 60) || toSq & (1ULL << 60)) { state.blackCastleKing = false; state.blackCastleQueen = false; }
    if (fromSq & (1ULL << 7) || toSq & (1ULL << 7)) state.whiteCastleKing = false;
    if (fromSq & (1ULL << 0) || toSq & (1ULL << 0)) state.whiteCastleQueen = false;
    if (fromSq & (1ULL << 63) || toSq & (1ULL << 63)) state.blackCastleKing = false;
    if (fromSq & (1ULL << 56) || toSq & (1ULL << 56)) state.blackCastleQueen = false;

    state.whitePieces = state.whitePawns | state.whiteKnights | state.whiteBishops | state.whiteRooks | state.whiteQueens | state.whiteKing;
    state.blackPieces = state.blackPawns | state.blackKnights | state.blackBishops | state.blackRooks | state.blackQueens | state.blackKing;
    state.allPieces   = state.whitePieces | state.blackPieces;
    
    if (!state.whiteToMove) state.fullMoveClock++;
    state.whiteToMove = !state.whiteToMove;

    return undo;
}

void Move::unmakeMove(BoardState& state, const MoveData& moveData, const UndoState& undo) {
    state.whiteToMove = !state.whiteToMove; 
    if (!state.whiteToMove) state.fullMoveClock--;

    int from = moveData.getFrom();
    int to = moveData.getTo();
    int flag = moveData.getFlag();

    unsigned long long fromSq = 1ULL << from;
    unsigned long long toSq   = 1ULL << to;

    int capturedPieceType = undo.getCapturedPieceType();
    int castleRights      = undo.getCastleRights();
    int epFile            = undo.getEpFile();
    state.halfMoveClock   = undo.getHalfMoveClock();


    if (epFile == 63) {
        state.enpassant = 0;
    } else {
        int rank = state.whiteToMove ? 2 : 5; 
        state.enpassant = 1ULL << (rank * 8 + epFile);
    }

    state.whiteCastleKing  = (castleRights & 1);
    state.whiteCastleQueen = (castleRights & 2);
    state.blackCastleKing  = (castleRights & 4);
    state.blackCastleQueen = (castleRights & 8);

    unsigned long long* friendlyBBs[] = {
        state.whiteToMove ? &state.whitePawns : &state.blackPawns,
        state.whiteToMove ? &state.whiteKnights : &state.blackKnights,
        state.whiteToMove ? &state.whiteBishops : &state.blackBishops,
        state.whiteToMove ? &state.whiteRooks : &state.blackRooks,
        state.whiteToMove ? &state.whiteQueens : &state.blackQueens,
        state.whiteToMove ? &state.whiteKing : &state.blackKing
    };

    
    if (flag >= 1 && flag <= 4) { // Revert Promotions
        unsigned long long* promoBB = nullptr;
        if (state.whiteToMove) {
            if      (flag == 1) promoBB = &state.whiteKnights;
            else if (flag == 2) promoBB = &state.whiteBishops;
            else if (flag == 3) promoBB = &state.whiteRooks;
            else                promoBB = &state.whiteQueens;
        } else {
            if      (flag == 1) promoBB = &state.blackKnights;
            else if (flag == 2) promoBB = &state.blackBishops;
            else if (flag == 3) promoBB = &state.blackRooks;
            else                promoBB = &state.blackQueens;
        }
        *promoBB &= ~toSq;
        *friendlyBBs[0] |= fromSq; // Revert base pawn
    } 
    else if (flag == 7) { // King-Side Castle
        *friendlyBBs[5] ^= fromSq; *friendlyBBs[5] ^= toSq;
        if (state.whiteToMove) state.whiteRooks ^= (1ULL << 7) | (1ULL << 5);
        else                   state.blackRooks ^= (1ULL << 63) | (1ULL << 61);
    } 
    else if (flag == 8) { // Queen-Side Castle
        *friendlyBBs[5] ^= fromSq; *friendlyBBs[5] ^= toSq;
        if (state.whiteToMove) state.whiteRooks ^= (1ULL << 0) | (1ULL << 3);
        else                   state.blackRooks ^= (1ULL << 56) | (1ULL << 59);
    } 
    else if (flag == 6) { // En Passant
        *friendlyBBs[0] ^= fromSq; *friendlyBBs[0] ^= toSq; 
        unsigned long long capPawnSq = state.whiteToMove ? (toSq >> 8) : (toSq << 8);
        if (state.whiteToMove) state.blackPawns |= capPawnSq;
        else                   state.whitePawns |= capPawnSq;
    } 
    else { // Standard moves
        for (auto* bb : friendlyBBs) {
            if (*bb & toSq) { *bb ^= toSq; *bb ^= fromSq; break; }
        }
    }

    // Re-insert captured targets
    if (capturedPieceType != 6) {
        unsigned long long* enemyBBs[] = {
            state.whiteToMove ? &state.blackPawns : &state.whitePawns,
            state.whiteToMove ? &state.blackKnights : &state.whiteKnights,
            state.whiteToMove ? &state.blackBishops : &state.whiteBishops,
            state.whiteToMove ? &state.blackRooks : &state.whiteRooks,
            state.whiteToMove ? &state.blackQueens : &state.whiteQueens,
            state.whiteToMove ? &state.blackKing : &state.whiteKing
        };
        *enemyBBs[capturedPieceType] |= toSq;
    }

    state.whitePieces = state.whitePawns | state.whiteKnights | state.whiteBishops | state.whiteRooks | state.whiteQueens | state.whiteKing;
    state.blackPieces = state.blackPawns | state.blackKnights | state.blackBishops | state.blackRooks | state.blackQueens | state.blackKing;
    state.allPieces   = state.whitePieces | state.blackPieces;
}

int Move::isCheckmateOrStalemate(BoardState& state) {

    if (state.halfMoveClock >= 100) {
        return 3; // Game Over: Draw by 50-move rule
    }
    
    MoveList moveList;
    generateAllMoves(moveList, state);

    int legalMovesCount = 0;

    for (int i = 0; i < moveList.count; i++) {
        MoveData candidateMove = moveList.moves[i];

        UndoState undo = makeMove(state, candidateMove);

        state.whiteToMove = !state.whiteToMove; 
        bool kingSafe = !isCheck(state);    
        state.whiteToMove = !state.whiteToMove;

        unmakeMove(state, candidateMove, undo);

        if (kingSafe) {
            legalMovesCount++;
            break; 
        }
    }

    if (legalMovesCount == 0) {
        unsigned long long activeKingBB = state.whiteToMove ? state.whiteKing : state.blackKing;
        if (isSquareAttacked(activeKingBB, state)) {
            //Game Over: CHECKMATE!
            return 1;
        } else {
            //Game Over: STALEMATE!
            return 2;
        }
        
    }
    return 0;
}


bool Move::isCheck(BoardState& state){
    if(state.whiteToMove){
        return isSquareAttacked(state.whiteKing,state);
    }   
       return isSquareAttacked(state.blackKing,state);
}

MoveData Move::createMoveData(Position pos, const BoardState& state) {
    int fromSquare = pos.initRank * 8 + pos.initFile;
    int toSquare = pos.finalRank * 8 + pos.finalFile;
    int flag = 0; 
    unsigned long long fromSq = 1ULL << fromSquare;
    if (fromSq & state.whitePawns) {
        if (toSquare >= 56) flag = 4; 
        else if (toSquare - fromSquare == 16) flag = 5; 
    } else if (fromSq & state.blackPawns) {
        if (toSquare <= 7) flag = 4;  
        else if (fromSquare - toSquare == 16) flag = 5; 
    }
    if (state.enpassant == (1ULL << toSquare) && (fromSq & (state.whitePawns | state.blackPawns))) {
        flag = 6;
    }
    if (fromSq & (state.whiteKing | state.blackKing)) {
        if (toSquare - fromSquare == 2) flag = 7;       
        else if (fromSquare - toSquare == 2) flag = 8;  
    }

    return MoveData(fromSquare, toSquare, flag);
}

bool Move::isMoveActuallyLegal(BoardState& state, const MoveData& move) {
    UndoState undo = makeMove(state, move);
    state.whiteToMove = !state.whiteToMove; 
    bool kingInCheck = isCheck(state);
    state.whiteToMove = !state.whiteToMove;
    unmakeMove(state, move, undo);

    return !kingInCheck;
}