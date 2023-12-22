#include <stdio.h>
#include <string>
#include <iostream>
#include <list>
#include <array>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <cmath>

const double YsPi = 3.1415927;
const int rows = 15;
const int cols = 15;

class Mark {
public:
    char state;
    void initialize(void);
    void Draw(int cx, int cy);
};

void Mark::initialize() {
    state = 'E';
}

void Mark::Draw(int cx, int cy) {
    if (state == 'B') {
        glColor3ub(0, 0, 0);
    }
    else if (state == 'W') {
        glColor3ub(255, 255, 255);
    }
    else {
        return;
    }
    glBegin(GL_POLYGON);
    for (int i = 0; i < 64; ++i)
    {
        double ang = YsPi * (double)i / 32.0;
        double s = sin(ang);
        double c = cos(ang);
        double x = cx + 15 * c;
        double y = cy + 15 * s;
        glVertex2d(x, y);
    }
    glEnd();
}

// ================================================================== //

class Board {
public: 
    Mark cell[rows][cols];
    char winner;
    int step;
    int lastBlackX, lastBlackY, lastWhiteX, lastWhiteY;
    int finalX1, finalY1, finalX2, finalY2;
    // Initialize the whole board 
    void initialize(void);
    // Convert the mouse coordinate to the cell's row and column
    int getIndex(int mouse);
    // Convert the cell's row and column to the mouse coordinate
    int getCoord(int num);
    // Draw the whole board 
    void Draw();
    void drawBackground();
    // Draw meta data
    void drawStep();
    void drawResult(char winner);
    // Check if player wins
    bool inside(int x, int y);
    bool checkWin(char curr, int x, int y);
    // AI
    bool aiMove();
    int getPositionScore(int x, int y);
    int getTupleScore(int numBlack, int numWhite);
};

void Board::initialize() {
    step = 0;
    winner = '-';
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cell[i][j].initialize();
        }
    }
}

// 20 - 60: 0, 60 - 100: 1, 100 - 140: 2
int Board::getIndex(int mouse) {
    return (mouse - 20) / 37.33;
}

int Board::getCoord(int num) {
    return (num+1) * 37.33;
}

void Board::Draw() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int x = getCoord(i); 
            int y = getCoord(j);
            cell[i][j].Draw(x, y);
        }
    }
}

void Board::drawBackground() {
    // beige background
    glColor3ub(225, 200, 170);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0, 600);
    glVertex2i(800, 600);
    glVertex2i(800, 0);
    glEnd();
    glColor3ub(0, 0, 0);
    // Draw horizontal board lines
    for (int i = 0; i <= cols; i++) {
        glBegin(GL_LINES);
        glVertex2d(37.33, 37.33 * i);
        glVertex2d(600-37.33, 37.33 * i);
        glEnd();
    }
    // Draw vertical board lines
    for (int i = 0; i <= rows; i++) {
        glBegin(GL_LINES);
        glVertex2d(37.33 * i, 37.33);
        glVertex2d(37.33 * i, 600-37.33);
        glEnd();
    }
    // Draw critical dots
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2d(getCoord(3), getCoord(3));
    glVertex2d(getCoord(3), getCoord(11));
    glVertex2d(getCoord(7), getCoord(7));
    glVertex2d(getCoord(11), getCoord(3));
    glVertex2d(getCoord(11), getCoord(11));
    glEnd();
}

void Board::drawStep() {
    std::string message = "Steps: " + std::to_string(step);
    const char* cMessage = message.c_str();
    glColor3ub(0, 0, 0);
    glRasterPos2f(580, 100);
    YsGlDrawFontBitmap16x20(cMessage);

    // Draw recent moves
    if (step > 0) {
        glColor3ub(255, 0, 0);
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex2d(getCoord(lastBlackX), getCoord(lastBlackY));
        glVertex2d(getCoord(lastWhiteX), getCoord(lastWhiteY));
        glEnd();
    }
}

void Board::drawResult(char winner) {
    if (winner == 'B') {
        glColor3ub(0, 0, 0);
        glRasterPos2f(580, 150);
        YsGlDrawFontBitmap16x20("You Win!");
    }
    else {
        glColor3ub(255, 255, 255);
        glRasterPos2f(580, 150);
        YsGlDrawFontBitmap16x20("You Lose!");
    }

    // Draw the winning line
    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(getCoord(finalX1), getCoord(finalY1));
    glVertex2d(getCoord(finalX2), getCoord(finalY2));
    glEnd();
}

bool Board::inside(int x, int y) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

bool Board::checkWin(char curr, int x, int y) {
    // Check horizontal
    int count = 0;
    for (int i = -4; i <= 4; i++) {
        if (inside(x+1, y)) {
            if (cell[x+i][y].state == curr) {
                count++;
                if (count >= 5) {
                    finalX1 = x+i-4;
                    finalY1 = y;
                    finalX2 = x+i;
                    finalY2 = y;
                    return true;
                }
            }
            else {
                count = 0;
            }
        }
    }

    // Check vertical
    count = 0;
    for (int i = -4; i <= 4; i++) {
        if (inside(x, y+i)) {
            if (cell[x][y+i].state == curr) {
                count++;
                if (count >= 5) {
                    finalX1 = x;
                    finalY1 = y+i-4;
                    finalX2 = x;
                    finalY2 = y+i;
                    return true;
                }
            }
            else {
                count = 0;
            }
        }
    }

    // Check diagonal
    count = 0;
    for (int i = -4; i <= 4; i++) {
        if (inside(x+i, y+i)) {
            if (cell[x+i][y+i].state == curr) {
                count++;
                if (count >= 5) {
                    finalX1 = x+i-4;
                    finalY1 = y+i-4;
                    finalX2 = x+i;
                    finalY2 = y+i;
                    return true;
                }
            }
            else {
                count = 0;
            }
        }
    }

    // Check anti-diagonal
    count = 0;
    for (int i = -4; i <= 4; i++) {
        if (inside(x+i, y-i)) {
            if (cell[x+i][y-i].state == curr) {
                count++;
                if (count >= 5) {
                    finalX1 = x+i-4;
                    finalY1 = y-i+4;
                    finalX2 = x+i;
                    finalY2 = y-i;
                    return true;
                }
            }
            else {
                count = 0;
            }
        }
    }
    
    return false;
}

bool Board::aiMove() {
    int resultX = -1, resultY = -1;
    int maxScore = -1;
    std::list<std::array<int, 2>> maxPositions;

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++){
            // Skip non-empty cells
            if (cell[x][y].state != 'E') {
                continue;
            }
            int positionScore = getPositionScore(x, y);
            if (positionScore > maxScore) {
                maxScore = positionScore;
                maxPositions.clear();
                maxPositions.push_back({x, y});
            }
            else if (positionScore == maxScore) {
                maxPositions.push_back({x, y});
            }
        }
    }

    // Randomly choose one of the max positions
    int randomIndex = rand() % maxPositions.size();
    auto it = maxPositions.begin();
    std::advance(it, randomIndex);
    resultX = (*it)[0];
    resultY = (*it)[1];
    cell[resultX][resultY].state = 'W';
    lastWhiteX = resultX;
    lastWhiteY = resultY;

    return checkWin('W', resultX, resultY);
}

int Board::getPositionScore(int x, int y) {
    int score = 0;

    // Check horizontal
    for (int i = -4; i <= 0; i++) {
        int tupleScore = 0;
        int numBlack = 0, numWhite = 0;
        for (int j = 0; j < 5; j++) {
            if (inside(x+i+j, y) && cell[x+i+j][y].state == 'B') {
                numBlack++;
            }
            else if (inside(x+i+j, y) && cell[x+i+j][y].state == 'W') {
                numWhite++;
            }
        }
        score += getTupleScore(numBlack, numWhite);
    }
    // Check vertical
    for (int i = -4; i <= 0; i++) {
        int tupleScore = 0;
        int numBlack = 0, numWhite = 0;
        for (int j = 0; j < 5; j++) {
            if (inside(x, y+i+j) && cell[x][y+i+j].state == 'B') {
                numBlack++;
            }
            else if (inside(x, y+i+j) && cell[x][y+i+j].state == 'W') {
                numWhite++;
            }
        }
        score += getTupleScore(numBlack, numWhite);
    }
    // Check diagonal
    for (int i = -4; i <= 0; i++) {
        int tupleScore = 0;
        int numBlack = 0, numWhite = 0;
        for (int j = 0; j < 5; j++) {
            if (inside(x+i+j, y+i+j) && cell[x+i+j][y+i+j].state == 'B') {
                numBlack++;
            }
            else if (inside(x+i+j, y+i+j) && cell[x+i+j][y+i+j].state == 'W') {
                numWhite++;
            }
        }
        score += getTupleScore(numBlack, numWhite);
    }
    // Check anti-diagonal
    for (int i = -4; i <= 0; i++) {
        int tupleScore = 0;
        int numBlack = 0, numWhite = 0;
        for (int j = 0; j < 5; j++) {
            if (inside(x+i+j, y-i-j) && cell[x+i+j][y-i-j].state == 'B') {
                numBlack++;
            }
            else if (inside(x+i+j, y-i-j) && cell[x+i+j][y-i-j].state == 'W') {
                numWhite++;
            }
        }
        score += getTupleScore(numBlack, numWhite);
    }

    return score;
}

// numBlack: user, numWhite: AI
int Board::getTupleScore(int numBlack, int numWhite) {
    // Atack on this tuple
    if (numBlack == 0 && numWhite == 0)     return 10;
    if (numBlack == 0 && numWhite == 1)     return 35;
    if (numBlack == 0 && numWhite == 2)     return 1500;
    if (numBlack == 0 && numWhite == 3)     return 18000;
    if (numBlack == 0 && numWhite == 4)     return 1000000;
    // Defense on this tuple
    if (numBlack == 1 && numWhite == 0)     return 15;
    if (numBlack == 2 && numWhite == 0)     return 400;
    if (numBlack == 3 && numWhite == 0)     return 6000;
    if (numBlack == 4 && numWhite == 0)     return 150000;
    // No one can win on this tuple
    return 0;
}

// ================================================================== //

int run() {
    // Initialize random seed
    srand(time(NULL));

    Board board;
    board.initialize();
    // FsOpenWindow(0, 0, 800, 600, 1); 
    char curr = 'B';
    
    while (FSKEY_NULL == FsInkey()) {
        // Check tie
        if (board.step == 15 * 15 / 2 + 1) {
            printf("Tie!\n");
            break;
        }

        FsPollDevice();
        auto key = FsInkey();
        if (FSKEY_ESC == key) {
            break;
        }

        // Clear window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw
        board.drawBackground();
        board.Draw();
        board.drawStep();
        FsSwapBuffers();

        bool click = false, move = false;
        int mouseEvent;
        int lb = -1, mb = -1, rb = -1, mx = -1, my = -1;
        mouseEvent = FsGetMouseEvent(lb, mb, rb, mx, my);
        switch (mouseEvent) {
        case FSMOUSEEVENT_LBUTTONDOWN:
            click = true;
            printf("The x is %d\n", mx); 
            printf("The y is %d\n", my);
            break;
        }

        if (click == true && curr == 'B') {
            int row = board.getIndex(mx);
            int col = board.getIndex(my);
            printf("The row is %d\n", row);
            printf("The col is %d\n", col);
            if (board.cell[row][col].state != 'E') {
                printf("Already Taken.\n");
                continue;
            }
            board.cell[row][col].state = curr;
            board.step++;
            board.lastBlackX = row;
            board.lastBlackY = col;
            curr = 'W';

            // Clear window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Draw
            board.drawBackground();
            board.Draw();
            board.drawStep();
            if (board.checkWin('B', row, col)) {
                printf("You win!\n");
                // Clear window
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // Draw
                board.drawBackground();
                board.Draw();
                board.drawStep();
                board.drawResult('B');
                FsSwapBuffers();
                FsSleep(1500);
                board.winner = 'B';
                break;
            }
            FsSwapBuffers();
            
            // AI's move
            if (board.aiMove()) {
                printf("AI win!\n");
                // Clear window
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // Draw
                board.drawBackground();
                board.Draw();
                board.drawStep();
                board.drawResult('W');
                FsSwapBuffers();
                FsSleep(1500);
                break;
            }
            curr = 'B';
        }
        FsSleep(50);
    }

    // Return
    if (board.winner == 'B') {
        return board.step;
    }
    
    return -1;
}
