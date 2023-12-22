#include <iostream>
#include "fssimplewindow.h"
#include "GoldMiner.h"

int score;
int goldPosX, goldPosY;
int diffX, diffY;
int hookPosX, hookPosY, hookEndX, hookEndY, hookExtension;
double hookAngle;
bool hookMovingleft = true;
bool hookLaunched = false;
bool hookExtending = false;
int baseSpeed = 4;
Timer::Timer(int initialTime) : timeRemaining(initialTime) {}
void Timer::Update() {
    if (timeRemaining > 0) {
        --timeRemaining;
    }
}
bool Timer::IsTimeUp() const {
    return timeRemaining <= 0;
}


Gold::Gold() : goldValid(true), goldCaptured(false), speed(0.9f) {}

Rock::Rock() : rockValid(true), rockCaptured(false), speed(0.7f) {}

Diamond::Diamond() : diamondValid(true), diamondCaptured(false), speed(1.0f) {}

void initializeGame() {
    goldPosX = rand() % (WINDOW_WIDTH - GOLD_SIZE);
    goldPosY = WINDOW_HEIGHT - GROUND_HEIGHT - GOLD_SIZE;
    hookPosX = WINDOW_WIDTH / 2;
    hookPosY = MINER_SIZE;
    hookExtension = 0;
    hookAngle = 0;
}

void initializeGold(Gold golds[], int goldamount) {
    for (int i = 0; i < goldamount; i++) {
        int j = rand() % 5;
        golds[i].goldSize = GOLD_SIZE * j;
        golds[i].goldPosX = rand() % (WINDOW_WIDTH - golds[i].goldSize);
        golds[i].goldPosY = 30 + rand() % (570 - golds[i].goldSize);
        golds[i].goldValue = 30 + (rand() % 5) * 10;
        golds[i].speed = 0.9f - j * 0.1f;
    }
}

void initializeRock(Rock rocks[], int rockamount) {
    for (int i = 0; i < rockamount; i++) {
        int j = rand() % 5;
        rocks[i].rockSize = ROCK_SIZE * j;
        rocks[i].rockPosX = rand() % (WINDOW_WIDTH - rocks[i].rockSize);
        rocks[i].rockPosY = 30 + rand() % (570 - rocks[i].rockSize);
        rocks[i].rockValue = 10 + (rand() % 5) * 5;
        rocks[i].speed = 0.7f - j * 0.1f;
    }
}

void initializeDiamond(Diamond diamonds[], int diamondamount) {
    for (int i = 0; i < diamondamount; i++) {
        int j = rand() % 5;
        diamonds[i].diamondSize = DIAMOND_SIZE * j;
        diamonds[i].diamondPosX = rand() % (WINDOW_WIDTH - diamonds[i].diamondSize);
        diamonds[i].diamondPosY = 30 + rand() % (570 - diamonds[i].diamondSize);
        diamonds[i].diamondValue = 10 + (rand() % 5) * 30;
        diamonds[i].speed = 1.0f;
    }
}

void drawGround() {
    int undergroundHeight = 570; // Example height for the underground area

    glColor3f(0.6f, 0.4f, 0.2f); // Brown color for the underground
    glBegin(GL_QUADS);
    glVertex2i(0, WINDOW_HEIGHT - undergroundHeight);
    glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT - undergroundHeight);
    glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2i(0, WINDOW_HEIGHT);
    glEnd();

    // You can add more drawing code here for additional underground details
}

void drawGold(Gold golds[], int goldsize) {
    for (int i = 0; i < goldsize; i++) {
        if (golds[i].goldValid) {
            glColor3f(1.0f, 0.84f, 0.0f); // Gold color
            glBegin(GL_QUADS);
            glVertex2i(golds[i].goldPosX, golds[i].goldPosY);
            glVertex2i(golds[i].goldPosX + golds[i].goldSize, golds[i].goldPosY);
            glVertex2i(golds[i].goldPosX + golds[i].goldSize, golds[i].goldPosY + golds[i].goldSize);
            glVertex2i(golds[i].goldPosX, golds[i].goldPosY + golds[i].goldSize);
            glEnd();
        }
    }
}

void drawRock(Rock rocks[], int rocksize) {
    for (int i = 0; i < rocksize; i++) {
        if (rocks[i].rockValid) {
            glColor3f(0.5f, 0.5f, 0.5f); // Grey color
            glBegin(GL_QUADS);
            glVertex2i(rocks[i].rockPosX, rocks[i].rockPosY);
            glVertex2i(rocks[i].rockPosX + rocks[i].rockSize, rocks[i].rockPosY);
            glVertex2i(rocks[i].rockPosX + rocks[i].rockSize, rocks[i].rockPosY + rocks[i].rockSize);
            glVertex2i(rocks[i].rockPosX, rocks[i].rockPosY + rocks[i].rockSize);
            glEnd();
        }
    }
}

void drawDiamond(Diamond diamonds[], int diamondsize) {
    for (int i = 0; i < diamondsize; i++) {
        if (diamonds[i].diamondValid) {
            glColor3f(0.0f, 1.0f, 1.0f); // Cyan color
            glBegin(GL_QUADS);
            glVertex2i(diamonds[i].diamondPosX, diamonds[i].diamondPosY);
            glVertex2i(diamonds[i].diamondPosX + diamonds[i].diamondSize, diamonds[i].diamondPosY);
            glVertex2i(diamonds[i].diamondPosX + diamonds[i].diamondSize, diamonds[i].diamondPosY + diamonds[i].diamondSize);
            glVertex2i(diamonds[i].diamondPosX, diamonds[i].diamondPosY + diamonds[i].diamondSize);
            glEnd();
        }
    }
}

void drawMiner() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for miner
    glBegin(GL_QUADS);
    glVertex2i(hookPosX - MINER_SIZE / 2, 0);
    glVertex2i(hookPosX + MINER_SIZE / 2, 0);
    glVertex2i(hookPosX + MINER_SIZE / 2, MINER_SIZE);
    glVertex2i(hookPosX - MINER_SIZE / 2, MINER_SIZE);
    glEnd();
}

void drawHook() {
    diffX = hookEndX;
    diffY = hookEndY;
    hookEndX = hookPosX + (int)(cos(hookAngle) * (HOOK_LENGTH + hookExtension));
    hookEndY = hookPosY + (int)(sin(hookAngle) * (HOOK_LENGTH + hookExtension));
    diffX -= hookEndX;
    diffY -= hookEndY;

    glColor3f(0,0,0); // Grey color for hook
    glBegin(GL_LINES);
    glVertex2i(hookPosX, hookPosY);
    glVertex2i(hookEndX, hookEndY);
    glEnd();
}

void updateHook(Gold golds[], int goldsize, Rock rocks[], int rocksize, Diamond diamonds[], int diamondsize) {
    if (!hookLaunched) {
        if (hookMovingleft) {
            hookAngle += 0.05;
            if (hookAngle >= M_PI) {
                hookMovingleft = false;
            }
        }
        else {
            hookAngle -= 0.05;
            if (hookAngle <= 0) {
                hookMovingleft = true;
            }
        }
        return;
    }
    if (hookExtending) {
        // Extend the hook in the direction it is pointing
        hookExtension += baseSpeed;
    }
    else {
        hookExtension -= baseSpeed * 2;
    }

    if (hookEndX < 0 || hookEndX > WINDOW_WIDTH || hookEndY < 0 || hookEndY > WINDOW_HEIGHT) {
        hookExtending = false;
    }
    if (hookExtending){
        for (int i = 0; i < diamondsize; i++) {
            if (diamonds[i].diamondValid &&hookEndX >= diamonds[i].diamondPosX && hookEndX <= diamonds[i].diamondPosX + diamonds[i].diamondSize && hookEndY >= diamonds[i].diamondPosY && hookEndY <= diamonds[i].diamondPosY + diamonds[i].diamondSize) {
                diamonds[i].diamondCaptured = true;
                score += diamonds[i].diamondValue;
                hookExtending = false;
                break;
            }
        }
        if (!hookExtending) return;
        for (int i = 0; i < goldsize; i++) {
            if (golds[i].goldValid && hookEndX >= golds[i].goldPosX && hookEndX <= golds[i].goldPosX + golds[i].goldSize && hookEndY >= golds[i].goldPosY && hookEndY <= golds[i].goldPosY + golds[i].goldSize) {
                golds[i].goldCaptured = true;
                score += golds[i].goldValue;
                hookExtending = false;
                break;
            }
        }
        if (!hookExtending) return;
        for (int i = 0; i < rocksize; i++) {
            if (rocks[i].rockValid && hookEndX >= rocks[i].rockPosX && hookEndX <= rocks[i].rockPosX + rocks[i].rockSize && hookEndY >= rocks[i].rockPosY && hookEndY <= rocks[i].rockPosY + rocks[i].rockSize) {
                rocks[i].rockCaptured = true;
                score += rocks[i].rockValue;
                hookExtending = false;
                break;
            }
        }
    }
    for (int i = 0; i < diamondsize; i++) {
        if (diamonds[i].diamondCaptured && diamonds[i].diamondValid) {
            if (hookExtension <= 0) {
                hookLaunched = false;
                diamonds[i].diamondValid = false;
            }
            else {
                diamonds[i].diamondPosX -= diffX;
                diamonds[i].diamondPosY -= diffY;
            }
        }
    }
    for (int i = 0; i < goldsize; i++) {
        if (golds[i].goldCaptured && golds[i].goldValid) {
            if (hookExtension <= 0) {
                hookLaunched = false;
                golds[i].goldValid = false;
            }
            else {
                golds[i].goldPosX -= diffX;
                golds[i].goldPosY -= diffY;
            }
        }
    }
    for (int i = 0; i < rocksize; i++) {
        if (rocks[i].rockCaptured && rocks[i].rockValid) {
            if (hookExtension <= 0) {
                hookLaunched = false;
                rocks[i].rockValid = false;
            }
            else {
                rocks[i].rockPosX -= diffX;
                rocks[i].rockPosY -= diffY;
            }
        }
    }


    if (hookExtension <= 0) {
        hookLaunched = false;
    }
}

void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);  // 设置文本颜色为白色
    glRasterPos2i(10, WINDOW_HEIGHT - 20);  // 设置文本位置
    
    std::string scoreText = "Score: " + std::to_string(score);
    const char* text = scoreText.c_str();
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *text);
        ++text;
    }
}

void updateGame(Gold golds[], int goldsize, Rock rocks[], int rocksize, Diamond diamonds[], int diamondsize) {
    // Launch the hook when the Space key is pressed
    if (FsGetKeyState(FSKEY_SPACE) && !hookLaunched) {
        hookLaunched = true;
        hookExtending = true;
        return;
    }

    updateHook(golds, goldsize, rocks, rocksize, diamonds, diamondsize);

}

int runGoldMiner() {
    srand(time(nullptr));
    initializeGame();
    
    Timer gameTimer(60 * 50);

    
    Gold golds[5];
    Rock rocks[5];
    Diamond diamonds[3];
    initializeGold(golds, 5);
    initializeRock(rocks, 5);
    initializeDiamond(diamonds, 3);

    while (true) {
        FsPollDevice();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        int key = FsInkey();
        switch(key) {
            case FSKEY_ESC:
            
                int resScore = score;
                score = 0;
                return  resScore;
                
        }
        drawGround();
        drawGold(golds, 5);
        drawRock(rocks, 5);
        drawDiamond(diamonds, 3);
        drawMiner();
        drawHook();
        drawScore();
        updateGame(golds, 5, rocks, 5, diamonds, 3);
        
        gameTimer.Update();
        if (gameTimer.IsTimeUp()) {
            std::cout << "Time's up! Game Over!" << std::endl;
            break;
        }

        
        FsSwapBuffers();
        FsSleep(20);
    }

    std::cout << "Gold Captured! Game Over!" << std::endl;
   
    int resScore = score;
    score = 0;
    return resScore;
}

