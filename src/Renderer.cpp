    #include "../include/Renderer.hpp"
    
    Renderer::Renderer(){};
    
    void Renderer::initializeTerminalBoard(){
        
  
        camera.position = Vector3{ 7.0f, 18.0f, 20.0f };   
        camera.target = Vector3{ 7.0f, 0.0f, 7.0f };    
        camera.up = Vector3{0,1,0};
        camera.fovy = 45;
        camera.projection = CAMERA_PERSPECTIVE;
        PieceModel = LoadModel("../models/BoardPieces.glb");
        lightShader = LoadShader("../shaders/lighting.vs", "../shaders/lighting.fs");
        lightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightShader, "viewPos");
        PieceModel.materials[0].shader = lightShader;
        topLight = CreateLight(LIGHT_DIRECTIONAL, Vector3{ 0.0f, 10.0f, 2.0f }, Vector3Zero(), WHITE, lightShader);
        boardMesh = GenMeshCube(2, 0.2f, 2);
        boardWhiteBox = LoadModelFromMesh(boardMesh);
        boardBlackBox = LoadModelFromMesh(boardMesh);
        boardGreenBox = LoadModelFromMesh(boardMesh);
        boardYellowBox = LoadModelFromMesh(boardMesh);
        boardWhiteBox.materials[0].shader = lightShader;
        boardBlackBox.materials[0].shader = lightShader;
        boardGreenBox.materials[0].shader = lightShader;
        boardYellowBox.materials[0].shader = lightShader;
        UpdateLightValues(lightShader, topLight);
        cameraPos[0] = camera.position.x; 
        cameraPos[1] = camera.position.y; 
        cameraPos[2] = camera.position.z;  
        SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        boardWhiteBox.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
        boardBlackBox.materials[0].maps[MATERIAL_MAP_ALBEDO].color = BLACK;
        boardGreenBox.materials[0].maps[MATERIAL_MAP_ALBEDO].color = GREEN;
        boardYellowBox.materials[0].maps[MATERIAL_MAP_ALBEDO].color = YELLOW;

    }

    void Renderer::Selector(int y,int x ,bool c){
        if(c){
            DrawModel(boardGreenBox, Vector3{ x*2.0f, -0.04f, y*2.0f }, 1.0f, WHITE);
        }
        else{
            DrawModel(boardYellowBox, Vector3{ x*2.0f, -0.05f, y*2.0f }, 1.0f, WHITE);
        }
    }


    
void Renderer::updateTerminalBoard(BoardState boardState) {
    char currentBoard[8][8] = {0};
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            unsigned long long bitSelector = 1ULL<<((7-j)*8+i);
            //WHITE
            if     (    bitSelector & boardState.whitePawns    ){   currentBoard[j][i] = 'P'; }
            else if(    bitSelector & boardState.whiteKnights  ){   currentBoard[j][i] = 'N'; }
            else if(    bitSelector & boardState.whiteBishops  ){   currentBoard[j][i] = 'B'; }
            else if(    bitSelector & boardState.whiteKing     ){   currentBoard[j][i] = 'K'; }
            else if(    bitSelector & boardState.whiteQueens   ){   currentBoard[j][i] = 'Q'; }
            else if(    bitSelector & boardState.whiteRooks    ){   currentBoard[j][i] = 'R'; }
            
            //BLACK
            else if(    bitSelector & boardState.blackPawns    ){   currentBoard[j][i] = 'p'; }
            else if(    bitSelector & boardState.blackKnights  ){   currentBoard[j][i] = 'n'; }
            else if(    bitSelector & boardState.blackBishops  ){   currentBoard[j][i] = 'b'; }
            else if(    bitSelector & boardState.blackKing     ){   currentBoard[j][i] = 'k'; }
            else if(    bitSelector & boardState.blackQueens   ){   currentBoard[j][i] = 'q'; }
            else if(    bitSelector & boardState.blackRooks    ){   currentBoard[j][i] = 'r'; }
        }
    }

    if (hasPreviousBoard == false) {
        for(int i=0; i<8; i++) for(int j=0; j<8; j++) previousBoard[i][j] = currentBoard[i][j];
        hasPreviousBoard = true;
    } else {
        bool boardChanged = false;
        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                if (previousBoard[i][j] != currentBoard[i][j]) boardChanged = true;
            }
        }
        if (boardChanged) { 
            int fR = -1, fC = -1, tR = -1, tC = -1;
            char mPiece = 0;

            // Find TO square
            for(int i=0; i<8; i++) {
                for(int j=0; j<8; j++) {
                    if (previousBoard[i][j] != currentBoard[i][j] && currentBoard[i][j] != 0) {
                        if (tR == -1 || currentBoard[i][j] == 'k' || currentBoard[i][j] == 'K') {
                            tR = i; tC = j; mPiece = currentBoard[i][j];
                        }
                    }
                }
            }
            // Find FROM square
            if (tR != -1) {
                bool isBlack = (mPiece >= 'a' && mPiece <= 'z');
                for(int i=0; i<8; i++) {
                    for(int j=0; j<8; j++) {
                        if (previousBoard[i][j] != currentBoard[i][j] && currentBoard[i][j] == 0) {
                            bool wasBlack = (previousBoard[i][j] >= 'a' && previousBoard[i][j] <= 'z');
                            if (isBlack == wasBlack) { fR = i; fC = j; }
                        }
                    }
                }
            }

            if (fR != -1 && tR != -1) {
                currentAnim.active = true;
                currentAnim.fromRow = fR; currentAnim.fromCol = fC;
                currentAnim.toRow = tR;   currentAnim.toCol = tC;
                currentAnim.piece = mPiece;
                currentAnim.progress = 0.0f;
            }
            for(int i=0; i<8; i++) for(int j=0; j<8; j++) previousBoard[i][j] = currentBoard[i][j];
        }
    }
    if (currentAnim.active) {
        currentAnim.progress += GetFrameTime() * currentAnim.speed;
        if (currentAnim.progress >= 1.0f) {
            currentAnim.active = false;
            currentAnim.progress = 1.0f;
        }
    }

    ClearBackground(DARKGRAY);
    BeginMode3D(camera);
    rlEnableDepthTest();
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if ((i+j)% 2 == 0) DrawModel(boardWhiteBox, Vector3{ i*2.0f, -0.1f, j*2.0f }, 1.0f, WHITE);
            else DrawModel(boardBlackBox, Vector3{ i*2.0f, -0.1f, j*2.0f }, 1.0f, WHITE);
        }   
    }
    
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            char piece = currentBoard[i][j];
            if (piece == 0) continue;
            if (currentAnim.active && i == currentAnim.toRow && j == currentAnim.toCol) continue;
            
            unsigned long long bitSelector = 1ULL<<((7-i)*8+j);
            Matrix rotationMat = MatrixRotateY(180.0f * DEG2RAD);
            PieceModel.materials[1].maps[MATERIAL_MAP_ALBEDO].color = Color{ 40, 40, 40, 255 };
            if (boardState.blackPieces & bitSelector) {
                PieceModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = Color{ 35, 35, 35, 255 };
            } else {
                PieceModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
            }
            
            Matrix translationMat = MatrixTranslate( j*2 , 0, i*2);
            if(currentBoard[i][j]=='n') translationMat = MatrixMultiply(rotationMat, translationMat);
            
            if     (currentBoard[i][j]=='P' || currentBoard[i][j]=='p') DrawMesh(PieceModel.meshes[MESH_PAWN  ], PieceModel.materials[0], translationMat);
            else if(currentBoard[i][j]=='N' || currentBoard[i][j]=='n') DrawMesh(PieceModel.meshes[MESH_KNIGHT], PieceModel.materials[0], translationMat);
            else if(currentBoard[i][j]=='K' || currentBoard[i][j]=='k') DrawMesh(PieceModel.meshes[MESH_KING  ], PieceModel.materials[0], translationMat);
            else if(currentBoard[i][j]=='Q' || currentBoard[i][j]=='q') DrawMesh(PieceModel.meshes[MESH_QUEEN ], PieceModel.materials[0], translationMat);
            else if(currentBoard[i][j]=='B' || currentBoard[i][j]=='b') DrawMesh(PieceModel.meshes[MESH_BISHOP], PieceModel.materials[0], translationMat);
            else if(currentBoard[i][j]=='R' || currentBoard[i][j]=='r') DrawMesh(PieceModel.meshes[MESH_ROOK  ], PieceModel.materials[0], translationMat);
        }   
    }

    if (currentAnim.active) {
        Vector3 startPos = { currentAnim.fromCol * 2.0f, 0.0f, currentAnim.fromRow * 2.0f };
        Vector3 endPos   = { currentAnim.toCol * 2.0f,   0.0f, currentAnim.toRow * 2.0f };
        Vector3 currentPos = Vector3Lerp(startPos, endPos, currentAnim.progress);
        
        PieceModel.materials[1].maps[MATERIAL_MAP_ALBEDO].color = Color{ 40, 40, 40, 255 };
        if (currentAnim.piece >= 'a' && currentAnim.piece <= 'z') { 
            PieceModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = Color{ 35, 35, 35, 255 };
        } else {
            PieceModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
        }
        
        Matrix translationMat = MatrixTranslate(currentPos.x, currentPos.y, currentPos.z);
        if (currentAnim.piece == 'n') { 
            Matrix rotationMat = MatrixRotateY(180.0f * DEG2RAD);
            translationMat = MatrixMultiply(rotationMat, translationMat);
        }
        
        if      (currentAnim.piece == 'P' || currentAnim.piece == 'p') DrawMesh(PieceModel.meshes[MESH_PAWN],   PieceModel.materials[0], translationMat);
        else if (currentAnim.piece == 'N' || currentAnim.piece == 'n') DrawMesh(PieceModel.meshes[MESH_KNIGHT], PieceModel.materials[0], translationMat);
        else if (currentAnim.piece == 'B' || currentAnim.piece == 'b') DrawMesh(PieceModel.meshes[MESH_BISHOP], PieceModel.materials[0], translationMat);
        else if (currentAnim.piece == 'R' || currentAnim.piece == 'r') DrawMesh(PieceModel.meshes[MESH_ROOK],   PieceModel.materials[0], translationMat);
        else if (currentAnim.piece == 'Q' || currentAnim.piece == 'q') DrawMesh(PieceModel.meshes[MESH_QUEEN],  PieceModel.materials[0], translationMat);
        else if (currentAnim.piece == 'K' || currentAnim.piece == 'k') DrawMesh(PieceModel.meshes[MESH_KING],   PieceModel.materials[0], translationMat);
    }
}

/*
    void Renderer::updateTerminalBoard(BoardState boardState){
        char currentBoard[8][8]={0};
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                unsigned long long bitSelector = 1ULL<<((7-j)*8+i);
                //WHITE
                if     (    bitSelector & boardState.whitePawns    ){   currentBoard[j][i] = 'P'; }
                else if(    bitSelector & boardState.whiteKnights  ){   currentBoard[j][i] = 'N'; }
                else if(    bitSelector & boardState.whiteBishops  ){   currentBoard[j][i] = 'B'; }
                else if(    bitSelector & boardState.whiteKing     ){   currentBoard[j][i] = 'K'; }
                else if(    bitSelector & boardState.whiteQueens   ){   currentBoard[j][i] = 'Q'; }
                else if(    bitSelector & boardState.whiteRooks    ){   currentBoard[j][i] = 'R'; }
                
                //BLACK
                else if(    bitSelector & boardState.blackPawns    ){   currentBoard[j][i] = 'p'; }
                else if(    bitSelector & boardState.blackKnights  ){   currentBoard[j][i] = 'n'; }
                else if(    bitSelector & boardState.blackBishops  ){   currentBoard[j][i] = 'b'; }
                else if(    bitSelector & boardState.blackKing     ){   currentBoard[j][i] = 'k'; }
                else if(    bitSelector & boardState.blackQueens   ){   currentBoard[j][i] = 'q'; }
                else if(    bitSelector & boardState.blackRooks    ){   currentBoard[j][i] = 'r'; }
            }
        }
        
        ClearBackground(DARKGRAY);
        BeginMode3D(camera);
        rlEnableDepthTest();
            
           
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if ((i+j)% 2 == 0) {
                    DrawModel(boardWhiteBox, Vector3{ i*2.0f, -0.1f, j*2.0f }, 1.0f, WHITE);
                } 
                else {
                    DrawModel(boardBlackBox, Vector3{ i*2.0f, -0.1f, j*2.0f }, 1.0f, WHITE);
                }
            }   
        }
        
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                char piece = currentBoard[i][j];
                if (piece == 0) continue;
                unsigned long long bitSelector = 1ULL<<((7-i)*8+j);
                Matrix rotationMat = MatrixRotateY(180.0f * DEG2RAD);
                PieceModel.materials[1].maps[MATERIAL_MAP_ALBEDO].color = Color{ 40, 40, 40, 255 };
                if (boardState.blackPieces & bitSelector) {
                    PieceModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = Color{ 35, 35, 35, 255 };
                }  
                else {
                    PieceModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
                }
                //Rotate Black Knight
                Matrix translationMat = MatrixTranslate( j*2 , 0, i*2);
                if(currentBoard[i][j]=='n'){
                translationMat = MatrixMultiply(rotationMat, translationMat);
                }
                if     (currentBoard[i][j]=='P' || currentBoard[i][j]=='p'){DrawMesh(PieceModel.meshes[MESH_PAWN  ], PieceModel.materials[0], translationMat);}
                else if(currentBoard[i][j]=='N' || currentBoard[i][j]=='n'){DrawMesh(PieceModel.meshes[MESH_KNIGHT], PieceModel.materials[0], translationMat);}
                else if(currentBoard[i][j]=='K' || currentBoard[i][j]=='k'){DrawMesh(PieceModel.meshes[MESH_KING  ], PieceModel.materials[0], translationMat);}
                else if(currentBoard[i][j]=='Q' || currentBoard[i][j]=='q'){DrawMesh(PieceModel.meshes[MESH_QUEEN ], PieceModel.materials[0], translationMat);}
                else if(currentBoard[i][j]=='B' || currentBoard[i][j]=='b'){DrawMesh(PieceModel.meshes[MESH_BISHOP], PieceModel.materials[0], translationMat);}
                else if(currentBoard[i][j]=='R' || currentBoard[i][j]=='r'){DrawMesh(PieceModel.meshes[MESH_ROOK  ], PieceModel.materials[0], translationMat);}
                
            }   
        }
      
    };

    */
    void Renderer::finalScreen(int gamestate , int x , int y ){
        if(gamestate==3 && !WindowShouldClose()){
            //Game Over: Draw by 50-move rule
            while(!WindowShouldClose()){
                BeginDrawing();
                DrawText("Draw by 50-move rule", 820, 540, 40, DARKGRAY);
                if(GuiButton(Rectangle{ 860, 600, 160, 80 }, "EXIT")){
                    break;
                }
                EndDrawing();
            }
        }
        else if(gamestate==2 && !WindowShouldClose()){
            //Game Over: STALEMATE!
            while(!WindowShouldClose()){
                BeginDrawing();
                DrawText("Stalemate", 860, 540, 40, RED);
                if(GuiButton(Rectangle{ 860, 600, 160, 80 }, "EXIT")){
                    break;
                }
                EndDrawing();
            }
        }
        else if(gamestate==1 && !WindowShouldClose()){
            //Game Over: CHECKMATE!
            while(!WindowShouldClose()){
                BeginDrawing();
                DrawText("Checkmate", 860, 540, 40, GREEN);
                if(GuiButton(Rectangle{ 860, 600, 160, 80 }, "EXIT")){
                    break;
                }
                EndDrawing();
            }
        }
        UnloadShader(lightShader);
        UnloadModel(boardWhiteBox);
        UnloadModel(boardBlackBox);
        UnloadModel(boardGreenBox);  
        UnloadModel(boardYellowBox);
        UnloadModel(PieceModel);
        CloseWindow();
    };


    Renderer::~Renderer(){
        UnloadShader(lightShader);
        UnloadModel(boardWhiteBox);
        UnloadModel(boardBlackBox);
        UnloadModel(boardGreenBox);  
        UnloadModel(boardYellowBox);
        UnloadModel(PieceModel);
    }