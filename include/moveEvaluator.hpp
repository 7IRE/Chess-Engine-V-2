#include "../src/magicbitboards.cpp"
#include "../include/input.hpp"
#include "../include/boardState.hpp"


class MoveData {
private:
    unsigned short data;
public:
    inline MoveData() : data(0) {}
    inline MoveData(int from, int to, int flag = 0) {data = from | (to << 6) | (flag << 12);}

    //0x3F represents 111111 in binary
    inline int getFrom() const { return data & 0x3F;}
    inline int getTo() const { return (data >> 6) & 0x3F;}
    inline int getFlag() const { return (data >> 12); }
};

class MoveList {
public:
    MoveData moves[256];
    int count = 0;     
    inline MoveList() : count(0) {}
    inline void push_back(const MoveData& move) {
        if (count < 256) {
            moves[count] = move;
            count++;
        }
    }
};

class UndoState {
    public:
    unsigned int packedData = 0;

    inline void pack(int capturedType, int castleRights, int epFile, int halfMoveClock) {
        packedData =  (unsigned int)capturedType |  ((unsigned int)castleRights << 3) |  ((unsigned int)epFile << 7) | ((unsigned int)halfMoveClock << 13);
    }

    inline int getCapturedPieceType() const { return packedData & 0x7; }
    inline int getCastleRights()      const { return (packedData >> 3) & 0xF; }
    inline int getEpFile()            const { return (packedData >> 7) & 0x3F; }
    inline int getHalfMoveClock()     const { return (packedData >> 13) & 0xFF; }
};

class Move{
private:
    //All Bitboards to check If a Validity
    unsigned long long KnightMoveBoard[64] = {0};
    unsigned long long KingMoveBoard  [64] = {0};

    unsigned long long WhitePawnAttackBoard[64]  = {0};
    unsigned long long WhitePawnMoveBoard[64][2] = {0};
    unsigned long long BlackPawnAttackBoard[64]  = {0};
    unsigned long long BlackPawnMoveBoard[64][2] = {0};
    
    unsigned long long WhiteKingSidePath ;
    unsigned long long WhiteQueenSidePath;
    unsigned long long BlackKingSidePath ;
    unsigned long long BlackQueenSidePath;

public:
    Move();
    bool MoveValidator (Position Pos ,const BoardState&  boardState);
    bool isSquareAttacked(unsigned long long Cur_sq , const BoardState& boardState);
    void generateAllMoves(MoveList &moveList ,const BoardState& boardState);

    UndoState makeMove(BoardState& state, const MoveData& moveData);
    void unmakeMove(BoardState& state, const MoveData& moveData, const UndoState& undo);
    
    bool isMoveActuallyLegal(BoardState& state, const MoveData& move);
    MoveData createMoveData(Position pos, const BoardState& state);
    int isCheckmateOrStalemate(BoardState& state);
    bool isCheck(BoardState& state);
};

