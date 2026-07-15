SHELL = cmd.exe

CXX = C:/msys64/ucrt64/bin/g++

RAYLIB_DIR = C:/raylib/raylib/src

INCLUDES = -Iinclude -I$(RAYLIB_DIR)
LDFLAGS = -L$(RAYLIB_DIR)

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm 

SRC = src/logic.cpp \
      src/moveEvaluator.cpp \
      src/main.cpp \
      src/Renderer.cpp

BUILD_DIR = build
TARGET = $(BUILD_DIR)/chess_engine.exe

all: build copy_assets

build:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(SRC) -o $(TARGET) $(INCLUDES) $(LDFLAGS) $(LIBS)

copy_assets:
	@if not exist $(BUILD_DIR)\models mkdir $(BUILD_DIR)\models
	@if not exist $(BUILD_DIR)\shaders mkdir $(BUILD_DIR)\shaders
	@xcopy /E /I /Y /Q models $(BUILD_DIR)\models
	@xcopy /E /I /Y /Q shaders $(BUILD_DIR)\shaders

clean:
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)