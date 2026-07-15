#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "../include/rlights.h"
#include "raygui.h"

#include <iostream>
#include "../include/boardState.hpp"


enum PieceMeshIndex {
    MESH_ROOK   = 0,
    MESH_PAWN   = 1,
    MESH_BISHOP = 2,
    MESH_KING   = 3,
    MESH_QUEEN  = 4,
    MESH_KNIGHT = 5
};

class Renderer{
private:
    Camera3D camera;
    Model PieceModel;
    Shader lightShader;
    Model boardWhiteBox;
    Model boardBlackBox;
    Model boardGreenBox;
    Model boardYellowBox;
    Mesh boardMesh;
    Light topLight;
    float cameraPos[3];
    
public:
    Renderer();
    
    void initializeTerminalBoard();

    void Selector(int y,int x ,bool c);

    void updateTerminalBoard(BoardState boardState);

    void finalScreen(int gamestate , int x , int y );
};

