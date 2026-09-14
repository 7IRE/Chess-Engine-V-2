#include <thread>
#include "../include/gamemanager.hpp"


GameManager::GameManager(){
    InitWindow(1920,1080,"Chess Engine");
    InitAudioDevice();
    SetExitKey(KEY_ESCAPE);
    board.fenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.UpdateState();
    board.RendererState().initializeTerminalBoard();
    isAiThinking = true;
    searchDepth = 5; 
    enginePlaysBlack = true;
    gameStatus = 0;
    gameOver = true;
    SetTargetFPS(60);
    main_screen = LoadMusicStream("../audio/main_screen.mp3");
    paperRipSound = LoadSound("../audio/paper.wav");
}

GameManager::~GameManager(){
    UnloadMusicStream(main_screen);
    UnloadSound(paperRipSound);
    CloseAudioDevice();
    CloseWindow();
}

int GameManager::mainScreen() {
    PlayMusicStream(main_screen);
    int screenState = 0; 
    
    int playerColor = 0;  
    int aiDifficulty = 1; 
    bool aiDifficultyEditMode = false;
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    Color bgDark = { 15, 15, 20, 255 }; 
    Color bgLight = { 35, 40, 55, 255 };
    Color panelColor = { 0, 0, 0, 150 }; 

    float timeElapsed = 0.0f;

    while (!WindowShouldClose()) {
        UpdateMusicStream(main_screen);
        timeElapsed += GetFrameTime();
        
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;

        BeginDrawing();
        
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, bgLight, bgDark);

        DrawRectangleRounded(Rectangle{ (float)centerX - 260, (float)centerY - 250, 520, 500 }, 0.1f, 10, panelColor);
        DrawRectangleRoundedLines(Rectangle{ (float)centerX - 260, (float)centerY - 250, 520, 500 }, 0.1f, 10, ColorAlpha(RAYWHITE, 0.3f));

        if (screenState == 0) {
            float pulse = (sin(timeElapsed * 3.0f) + 1.0f) / 2.0f; 
            Color titleColor = ColorAlpha(RAYWHITE, 0.8f + (pulse * 0.2f));
            DrawText("CHESS", centerX - MeasureText("CHESS", 60) / 2, centerY - 180, 60, titleColor);
            DrawText("Select Game Mode", centerX - MeasureText("Select Game Mode", 20) / 2, centerY - 90, 20, LIGHTGRAY);
            if (GuiButton(Rectangle{ (float)centerX - 180, (float)centerY - 30, 360, 60 }, "Two Player")) {
                EndDrawing();
                return 1; 
            }
            if (GuiButton(Rectangle{ (float)centerX - 180, (float)centerY + 50, 360, 60 }, "Player vs AI")) {
                screenState = 1; 
            }
            if (GuiButton(Rectangle{ (float)centerX - 180, (float)centerY + 130, 360, 60 }, "EXIT")) {
                EndDrawing();
                return 0;
            }
        } 
        else if (screenState == 1) {
            DrawText("AI SETTINGS", centerX - MeasureText("AI SETTINGS", 40) / 2, centerY - 180, 40, RAYWHITE);
            DrawText("Choose Your Color:", centerX - 200, centerY - 90, 20, LIGHTGRAY);
            GuiToggleGroup(Rectangle{ (float)centerX - 200, (float)centerY - 60, 200, 50 }, "WHITE;BLACK", &playerColor);
            DrawText("AI Toughness:", centerX - 200, centerY + 10, 20, LIGHTGRAY);
            if (GuiButton(Rectangle{ (float)centerX - 200, (float)centerY + 140, 190, 50 }, "BACK")) {
                screenState = 0;
            }
            if (GuiButton(Rectangle{ (float)centerX + 10, (float)centerY + 140, 190, 50 }, "START GAME")) {
                EndDrawing();
                int baseValue = (playerColor == 0) ? 10 : 20; 
                return baseValue + aiDifficulty;
            }
            if (GuiDropdownBox(Rectangle{ (float)centerX - 200, (float)centerY + 40, 400, 50 }, "Easy;Medium;Hard", &aiDifficulty, aiDifficultyEditMode)) {
                aiDifficultyEditMode = !aiDifficultyEditMode;
            }
        }

        EndDrawing();
    }

    return 0; 
}






int GameManager::gameOverScreen(int result) {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    Color bgDark = { 15, 15, 20, 255 }; 
    Color bgLight = { 35, 40, 55, 255 };
    Color panelColor = { 0, 0, 0, 150 }; 
    const char* resultText = "GAME OVER";
    Color resultColor = RAYWHITE;

    switch (result) {
        case 1: 
            resultText = "Checkmate!"; 
            resultColor = { 200, 200, 255, 255 }; 
            break;
        case 2: 
            resultText = "Draw by Stalemate!"; 
            resultColor = { 255, 100, 100, 255 }; 
            break;
        case 3: 
            resultText = "Game Over: Draw by 50-move rule"; 
            resultColor = LIGHTGRAY;
            break;
        default: 
            resultText = "Game Drawn!"; 
            resultColor = LIGHTGRAY;
            break;
    }

    float timeElapsed = 0.0f;

    while (!WindowShouldClose()) {
        board.UpdateState();
        timeElapsed += GetFrameTime();
        
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;

        BeginDrawing();
        //DrawRectangleGradientV(0, 0, screenWidth, screenHeight, bgLight, bgDark);
        ClearBackground(bgDark); 
        board.RendererState().updateTerminalBoard(board.BoardState());
        EndMode3D();
        DrawRectangleRounded(Rectangle{ (float)centerX - 250, (float)centerY - 150, 500, 300 }, 0.1f, 10, panelColor);
        DrawRectangleRoundedLines(Rectangle{ (float)centerX - 250, (float)centerY - 150, 500, 300 }, 0.1f, 10, ColorAlpha(RAYWHITE, 0.3f));

        float pulse = (sin(timeElapsed * 4.0f) + 1.0f) / 2.0f; 
        Color titleColor = ColorAlpha(RAYWHITE, 0.8f + (pulse * 0.2f));
        DrawText("GAME OVER", centerX - MeasureText("GAME OVER", 50) / 2, centerY - 100, 50, titleColor);

        DrawText(resultText, centerX - MeasureText(resultText, 24) / 2, centerY - 20, 24, resultColor);

        if (GuiButton(Rectangle{ (float)centerX - 190, (float)centerY + 60, 180, 50 }, "MAIN MENU")) {
            EndDrawing();
            return 1; 
        }
        
        if (GuiButton(Rectangle{ (float)centerX + 10, (float)centerY + 60, 180, 50 }, "EXIT")) {
            EndDrawing();
            return 0;
        }

        EndDrawing();
    }

    return 0; 
}

int GameManager::twoPlayer(){
       
    while(!gameOver && !WindowShouldClose()){
        board.UpdateState();
        auto& state = board.BoardState();
        Position Selection={-1,-1,-1,-1};
        BeginDrawing();
        board.RendererState().updateTerminalBoard(board.BoardState());
        if(board.getisPromoting() == true){
            board.handlePromotionInput(); 
        }
        else {
            Selection = board.selection();
        }
        EndMode3D();
        if(board.getWhiteToMove()){
            DrawText("WHITE TO MOVE", 10 , 1000, 20, WHITE);
        }
        else{
            DrawText("BLACK TO MOVE", 10 , 20, 20, BLACK);
        }
        EndDrawing();
        
        if (Selection.finalRank != -1) {
            auto& stateRef = board.BoardState();
            auto& evaluator = board.MoveEvaluatorState();
            if (evaluator.MoveValidator(Selection, stateRef) && 
                evaluator.isMoveActuallyLegal(stateRef, evaluator.createMoveData(Selection, stateRef))) {
                board.movePiece(Selection);
                board.UpdateState(); 
                PlaySound(paperRipSound);
                gameStatus = evaluator.isCheckmateOrStalemate(board.BoardState());
                if (gameStatus != 0) {
                    gameOver = true;
                }
            }
        }
        
    }
    board.UpdateState();
    auto& state = board.BoardState();
    BeginDrawing();
    board.RendererState().updateTerminalBoard(board.BoardState());
    EndMode3D();
    EndDrawing();
    return gameStatus;
}


int GameManager::playerVsAi(){
    
    while(!gameOver && !WindowShouldClose()){
        board.UpdateState();
        auto& state = board.BoardState();   
        Position Selection={-1,-1,-1,-1};
        BeginDrawing();
        board.RendererState().updateTerminalBoard(board.BoardState());  
        bool isEngineTurn = (board.getWhiteToMove() != enginePlaysBlack);
        if(board.getisPromoting() == true){
            if (isEngineTurn) board.setPromotionPiece('q'); 
            else board.handlePromotionInput();   
        }
        else {
            if (isEngineTurn) {
                if (backgroundAiMove.getFrom() != backgroundAiMove.getTo()) {
                    int from = backgroundAiMove.getFrom();
                    int to = backgroundAiMove.getTo();

                    Selection.initFile = from % 8;
                    Selection.initRank = from / 8;
                    Selection.finalFile = to % 8;
                    Selection.finalRank = to / 8;
                    backgroundAiMove = MoveData(); 

                } 
                else if (!isAiThinking) {
                    isAiThinking = true;

                    std::thread([this, state]() {
                        MoveData calculatedMove = aiEngine.getBestMove(state, searchDepth);
                        backgroundAiMove = calculatedMove;
                        isAiThinking = false; 
                    }).detach(); 
                }
            } 
            else {
                Selection = board.selection();
            }
        }
        EndMode3D();
        if(board.getWhiteToMove()){
            DrawText("WHITE TO MOVE", 10 , 1000, 20, WHITE);
        }
        else{
            DrawText("BLACK TO MOVE", 10 , 20, 20, BLACK);
        }
        EndDrawing();
        
        if (Selection.finalRank != -1) {
            auto& stateRef = board.BoardState();
            auto& evaluator = board.MoveEvaluatorState();
            if (evaluator.MoveValidator(Selection, stateRef) && 
                evaluator.isMoveActuallyLegal(stateRef, evaluator.createMoveData(Selection, stateRef))) {
                board.movePiece(Selection);
                board.UpdateState(); 
                PlaySound(paperRipSound);
                gameStatus = evaluator.isCheckmateOrStalemate(board.BoardState());
                if (gameStatus != 0) {
                    gameOver = true;
                }
            }
        }
        
    }
    board.UpdateState();
    auto& state = board.BoardState();
    BeginDrawing();
    board.RendererState().updateTerminalBoard(board.BoardState());
    EndMode3D();
    EndDrawing();
    return gameStatus;
}


int GameManager::mainGame(int options){

    gameOver = false;
    gameStatus = 0;
    isAiThinking = false;

    board.fenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.UpdateState();
    board.RendererState().initializeTerminalBoard();

    if(options/10!=0 && options/10 !=0){searchDepth = ((options%10) )+2;}
    std::thread t1;
    if(options == 1){
       return twoPlayer();
    }
    else if(options/10 == 1 || options/10 == 2){
        if(options/10==1) enginePlaysBlack = true;
        else enginePlaysBlack = false;
        return playerVsAi();
    }
    else return 0;
}