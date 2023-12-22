#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include "fssimplewindow.h"

// The Mark class
class Mark {
public:
    char state;
    void initialize(void);
    void Draw(int cx, int cy);
};

// The Board class
class Board {
public:
    static const int rows = 15;
    static const int cols = 15;

    Mark cell[rows][cols];
    char winner;
    int step;
    int lastBlackX, lastBlackY, lastWhiteX, lastWhiteY;
    int finalX1, finalY1, finalX2, finalY2;

    void initialize(void);
    int getIndex(int mouse);
    int getCoord(int num);
    void Draw();
    void drawBackground();
    void drawStep();
    void drawResult(char winner);
    bool inside(int x, int y);
    bool checkWin(char curr, int x, int y);
    bool aiMove();
    int getPositionScore(int x, int y);
    int getTupleScore(int numBlack, int numWhite);
};

// The run function
int run();

#endif // GOMOKU_HPP
