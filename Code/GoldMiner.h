//
//  GoldMiner.h
//  GoldMiner
//
//  Created by 聂羽丞 on 11/27/23.
//

#ifndef GoldMiner_h
#define GoldMiner_h


#include <cstdlib>
#include <cmath>
#include <ctime>
#include <GLUT/GLUT.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GROUND_HEIGHT = 50;
const int GOLD_SIZE = 30;
const int ROCK_SIZE = 20;
const int DIAMOND_SIZE = 10;
const int HOOK_SIZE = 10;
const int MINER_SIZE = 30;
const int HOOK_LENGTH = 50;

extern int score;
extern int goldPosX, goldPosY;
extern int diffX, diffY;
extern int hookPosX, hookPosY, hookEndX, hookEndY, hookExtension;
extern double hookAngle;
extern bool hookMovingleft;
extern bool hookLaunched;
extern bool hookExtending;
extern int baseSpeed;
class Timer {
public:
    int timeRemaining;
    Timer(int initialTime);
    void Update();
    bool IsTimeUp() const;
};


class Gold {
public:
    int goldPosX, goldPosY;
    int goldSize;
    int goldValue;
    bool goldValid;
    bool goldCaptured;
    float speed;

    Gold();
};

class Rock {
public:
    int rockPosX, rockPosY;
    int rockSize;
    int rockValue;
    bool rockValid;
    bool rockCaptured;
    float speed;

    Rock();
};

class Diamond {
public:
    int diamondPosX, diamondPosY;
    int diamondSize;
    int diamondValue;
    bool diamondValid;
    bool diamondCaptured;
    float speed;

    Diamond();
};

void initializeGame();
void initializeGold(Gold golds[], int goldamount);
void initializeRock(Rock rocks[], int rockamount);
void initializeDiamond(Diamond diamonds[], int diamondamount);
void drawGround();
void drawGold(Gold golds[], int goldsize);
void drawRock(Rock rocks[], int rocksize);
void drawDiamond(Diamond diamonds[], int diamondsize);
void drawMiner();
void drawHook();
void updateHook(Gold golds[], int goldsize, Rock rocks[], int rocksize, Diamond diamonds[], int diamondsize);
void drawScore();
void updateGame(Gold golds[], int goldsize, Rock rocks[], int rocksize, Diamond diamonds[], int diamondsize);
int runGoldMiner();


#endif /* GoldMiner_h */
