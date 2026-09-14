

### Credits & Resources

---

# 3D Chess Engine

A blazingly fast, bitboard-based C++ chess engine paired with a real-time 3D OpenGL renderer.

---

## Description
A robust C++ Chess Engine featuring a dedicated **3D Renderer** powered by **raylib**. Built on highly optimized bitboard architectures (utilizing both standard and magic bitboards), this engine offers a massive performance leap over traditional array-based structures. 

The front-end renders clean, 3D chess pieces and dynamic lighting, bridging low-level engine speed with a modern gameplay experience. It is designed to serve as a rock-solid foundation for further development, such as building a chess AI or a more complex chess application.

---

## Key Features
* **Bitboard Architecture:** Uses normal and magic bitboards for ultra-fast move generation and evaluation.
* **Raylib 3D Graphics:** Full 3D board, piece models, custom shaders, and dynamic lighting calculations.
* **Comprehensive Rule Enforcement:** Full implementation of standard chess rules:
  * Move validation, check, checkmate, and stalemate detection.
  * Special moves: En Passant and Castling (both kingside and queenside).
  * Pawn promotion, the 50-move rule, and draw detection.

---

## Media

![Chess Board Layout](Project_ScreenShot/Board.png)  
![Active Gameplay](Project_ScreenShot/play.png)  
![Checkmate Detection Screen](Project_ScreenShot/checkmate.png)  

---

## Controls & Gameplay

The engine reads keyboard inputs to navigate the 3D board selector and handle selections.

### Board Navigation
* **W** -> Move selector **Up** 
* **A** -> Move selector **Left**
* **S** -> Move selector **Down** 
* **D** -> Move selector **Right**

### Pawn Promotion
When a pawn reaches the final rank, input one of the following numbers to choose your promotion piece:
* **1** -> Knight
* **2** -> Bishop
* **3** -> Rook
* **4** -> Queen

---

## Compilation and Build Instructions

The project includes a pre-configured `Makefile` for clean, cross-platform builds using `g++`.

## Running the Pre-compiled Executable

If you do not want to compile the source code yourself, a pre-compiled executable is available with the project so you don't have to.

---
### Prerequisites
Ensure you have `g++`, `make`, and the `raylib` library installed on your machine. 

On Linux (Debian/Ubuntu), you can install the build essentials via:
```bash
sudo apt update
sudo apt install build-essential
```
## Building the Project

To compile the source code and generate the standalone static executable (chess_engine), run the following command in the root directory:

```bash
make
```
Cleaning Build Files

To remove the compiled binary and prepare for a clean rebuild, run:

```bash
make clean
```

---
### Credits & Resources    
* [Magic Board Implementation By Code Monkey King](https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.c)  
* [ASCII Art Core: Built using tools from ASCII-ART by 7IRE.](https://github.com/7IRE/ASCII-ART)
* [Legacy Version: Check out the original, array-based predecessor: (Chess V-1) Based on Arrays.](https://github.com/7IRE/CHESS-ENGINE) 
* [Raylib - Simple and easy-to-use library to enjoy videogames programming.](https://www.raylib.com/)

* Music by <a href="https://pixabay.com/users/samuelfjohanns-1207793/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=156750">Samuel F. Johanns</a> from <a href="https://pixabay.com//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=156750">Pixabay</a>
* Sound Effect by <a href="https://pixabay.com/users/freesound_community-46691455/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=103913">freesound_community</a> from <a href="https://pixabay.com//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=103913">Pixabay</a>