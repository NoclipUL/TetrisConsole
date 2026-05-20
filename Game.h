#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Block.h"
#include <chrono>

using namespace std;

class Game {
private:
    int score;
    int totalLinesRemoved;
    Board board;
    Block currentBlock;
    Block nextBlock;
    bool isRunning;
    // Thêm 1 biến thời gian cho lần rơi block cuối cùng. Fix issu #1
    chrono::steady_clock::time_point lastFallTime;
public:
    Game();
    ~Game();
    
    void start();
    void update();
    void handleInput();
    void spawnNewBlock();
    bool isGameOver() const;
    int getScore() const;
    int getLevel() const;
    void saveScore();
private:
    int getRandomBlockType() const;
    int getDropInterval() const;
};

#endif
