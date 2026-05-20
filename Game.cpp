#include "Game.h"
#include "Platform.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <fstream>
#include <iostream>

Game::Game() 
    : score(0), currentBlock(rand() % 7, 5, 0), nextBlock(rand() % 7, 5, 0), isRunning(true) {
    board.init();
    Input::setupConsole();
    // lần rơi cuối là hiện tại
    lastFallTime = chrono::steady_clock::now();
}

Game::~Game() {
    Input::restoreConsole();
}
int Game::getRandomBlockType() const {
    return rand() % 7;
}

void Game::spawnNewBlock() {
    currentBlock = nextBlock;
    currentBlock.setPosition(5, 0);
    nextBlock = Block(getRandomBlockType(), 5, 0);
}

void Game::handleInput() {
    while (Input::isKeyPressed()) {
        char c = Input::getChar();
        if (c == 'w' || c == 'W') {
            Block rotatedBlock = currentBlock;
            rotatedBlock.rotate();
            if (board.canMove(rotatedBlock, 0, 0))
                currentBlock.rotate();
        }
        if ((c == 'a' || c == 'A') && board.canMove(currentBlock, -1, 0)) 
            currentBlock.moveX(-1);
        if ((c == 'd' || c == 'D') && board.canMove(currentBlock, 1, 0)) 
            currentBlock.moveX(1);
        if ((c == 's' || c == 'S') && board.canMove(currentBlock, 0, 1)) 
            currentBlock.moveY(1);
        if ((c == 'q' || c == 'Q')) 
            isRunning = false;
    }
}

void Game::update() {
    // Remove old block from board
    board.removeBlock(currentBlock);
    
    // Handle input (key board)
    handleInput();
    if (!isRunning) return;
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - lastFallTime).count();
    if (elapsed >= 500)
    {
        if (board.canMove(currentBlock, 0, 1)) {
        currentBlock.moveY(1);
        } else {
            // Place block and spawn new one
            board.placeBlock(currentBlock);
            int cleared = board.removeLine();

            switch (cleared) {
                case 1: score += 100;
            break;
                case 2: score += 300;
            break;
                case 3: score += 500;
            break;
                case 4: score += 800;
            break;
            }
            spawnNewBlock();
            if (!board.canMove(currentBlock, 0, 0)) {
                isRunning = false;
                return;
            }
        }
        lastFallTime = now;
    }

    // Apply gravity
    
    
    // Place current block on board
    board.placeBlock(currentBlock);
    
    // Draw
    board.draw(nextBlock);
    cout << "Score: " << score << endl;
    
    // Game loop speed
    this_thread::sleep_for(chrono::milliseconds(30));
}

void Game::start() {
    board.draw(nextBlock);
    
    while (isRunning) {
        update();
    }
    cout << "Game Over! Your score: " << score << endl;
    saveScore();
    
    this_thread::sleep_for(chrono::seconds(3));
}

bool Game::isGameOver() const {
    return !isRunning;
}
int Game::getScore() const {
    return score;
}
void Game::saveScore() {
    ofstream file("score.txt", ios::app);

    if (!file) {
        cout << "Cannot open file!" << endl;
        return;
    }
    cout << "Saving score..." << endl;

    file << "Score: " << score << endl;
    file.close();
}
