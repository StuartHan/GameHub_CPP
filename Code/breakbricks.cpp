#include "fssimplewindow.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "breakbricks.hpp"
#define PI 3.14159265358979323846

ReboundBoard::ReboundBoard(double x1, double y1, double x2, double y2)
    : xLeft(x1), yBottom(y1), xRight(x2), yTop(y2) {}

// draw the board with tile
void ReboundBoard::Draw(GLuint texId) const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 1.0); glVertex2d(xLeft, yBottom);
    glTexCoord2d(1.0, 1.0); glVertex2d(xRight, yBottom);
    glTexCoord2d(1.0, 0.0); glVertex2d(xRight, yTop);
    glTexCoord2d(0.0, 0.0); glVertex2d(xLeft, yTop);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// restrict the board's position in the window
void ReboundBoard::MoveLeft(double distance) {
    xLeft -= distance;
    if (xLeft < 0) {
        xLeft = 0;
    }
    xRight = xLeft + 150.0;
}

// restrict the board's position in the window
void ReboundBoard::MoveRight(double distance) {
    xRight += distance;
    if (xRight > 800) {
        xRight = 800.0;
    }
    xLeft = xRight - 150.0;
}

Ball::Ball(double x, double y, double vx, double vy, double radius)
    : x(x), y(y), vx(vx), vy(vy), radius(radius) {}

void Ball::Move(double dt) {
    x += vx * dt;
    y += vy * dt;
}

// draw the ball with its tile
void Ball::Draw(GLuint texId) const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    glColor3f(1.0f, 1.0f, 1.0f);

    double left = x - radius;
    double right = x + radius;
    double bottom = y - radius;
    double top = y + radius;

    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(left, top);
    glTexCoord2d(1.0, 0.0); glVertex2d(right, top);
    glTexCoord2d(1.0, 1.0); glVertex2d(right, bottom);
    glTexCoord2d(0.0, 1.0); glVertex2d(left, bottom);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// calculate the distance between the center of the ball and the corner of brick or board
double Distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

Brick::Brick(double x, double y, double width, double height)
    : x(x), y(y), width(width), height(height), visible(true) {}

void Brick::Draw(GLuint texId) const {
    if (visible) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId);

        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 1.0); glVertex2d(x, y);
        glTexCoord2d(1.0, 1.0); glVertex2d(x + width, y);
        glTexCoord2d(1.0, 0.0); glVertex2d(x + width, y + height);
        glTexCoord2d(0.0, 0.0); glVertex2d(x, y + height);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void HandleBrickCollision(Brick& brick, Ball& ball, int& visibleBrickCount, int& score) {
    // top plane
    if (ball.y + ball.radius > brick.y && ball.x > brick.x && ball.x < brick.x + brick.width && ball.y < brick.y) {
        ball.vy = -ball.vy;
        ball.y = brick.y - ball.radius;
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        return;
    }
    // right plane
    else if (ball.y > brick.y && ball.y < brick.y + brick.height && ball.x > brick.x + brick.width && ball.x - ball.radius < brick.x + brick.width) {
        ball.vx = -ball.vx;
        ball.x = brick.x + brick.width + ball.radius;
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        return;
    }
    // legft plane
    else if (ball.y > brick.y && ball.y < brick.y + brick.height && ball.x < brick.x && ball.x + ball.radius > brick.x) {
        ball.vx = -ball.vx;
        ball.x = brick.x - ball.radius;
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        return;
    }
    // bottom plane
    else if (ball.y - ball.radius < brick.y + brick.height && ball.y > brick.y + brick.height && ball.x > brick.x && ball.x < brick.x + brick.width) {
        ball.vy = -ball.vy;
        ball.y = brick.y + brick.height + ball.radius;
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        return;
    }

    // hit the corner
    if (Distance(ball.x, ball.y, brick.x, brick.y) < ball.radius && ball.y < brick.y && ball.x < brick.x) {
        if (ball.vx == 0 || (ball.vx > 0 && ball.vy > 0)) {
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }
        else if (ball.vx > 0 && ball.vy < 0) {
            ball.vx = -ball.vx;
        }
        else if (ball.vx < 0 && ball.vy > 0) {
            ball.vy = -ball.vy;
        }
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        // ball.x = brick.x;
        // ball.y = brick.y;
        return;
    }
    else if (Distance(ball.x, ball.y, brick.x + brick.width, brick.y) < ball.radius && ball.y < brick.y && ball.x > brick.x + brick.width) {
        if (ball.vx > 0 && ball.vy > 0) {
            ball.vy = -ball.vy;
        }
        else if (ball.vx == 0 || (ball.vx < 0 && ball.vy > 0)) {
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }
        else if (ball.vx < 0 && ball.vy < 0) {
            ball.vx = -ball.vx;
        }
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        // ball.x = brick.x + brick.width;
        // ball.y = brick.y;
        return;
    }
    else if (Distance(ball.x, ball.y, brick.x, brick.y + brick.height) < ball.radius && ball.y > brick.y + brick.height && ball.x < brick.x) {
        if (ball.vx > 0 && ball.vy > 0) {
            ball.vx = -ball.vx;
        }
        else if (ball.vx == 0 || (ball.vx > 0 && ball.vy < 0)) {
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }
        else if (ball.vx < 0 && ball.vy < 0) {
            ball.vy = -ball.vy;
        }
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        // ball.x = brick.x;
        // ball.y = brick.y + brick.height;
        return;
    }
    else if (Distance(ball.x, ball.y, brick.x + brick.width, brick.y + brick.height) < ball.radius && ball.y > brick.y + brick.height && ball.x > brick.x + brick.width) {
        if (ball.vx > 0 && ball.vy < 0) {
            ball.vy = -ball.vy;
        }
        else if (ball.vx < 0 && ball.vy > 0) {
            ball.vx = -ball.vx;
        }
        else if (ball.vx == 0 || (ball.vx < 0 && ball.vy < 0)) {
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }
        brick.visible = false;
        score += 10;
        visibleBrickCount--;
        // ball.x = brick.x + brick.width;
        // ball.y = brick.y + brick.height;
        return;
    }

}

// load the texture
GLuint LoadTexture(const char* filePath) {
    YsRawPngDecoder decoder;
    if (YSOK != decoder.Decode(filePath)) {
        printf("Cannot open image file %s.\n", filePath);
        return 0;
    }
    decoder.Flip();

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, decoder.wid, decoder.hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, decoder.rgba);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texId;
}

// main function
int runBreakBricks(void) {
//    FsOpenWindow(16, 16, 800, 600, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int score = 0;

    // initial the board at the middle of the window
    ReboundBoard board(325, 500, 475, 520);
    double movementDistance = 35.0; // board's speed

    // initial the ball at the middle of the window, on the board
    // its flying angle is initialized randomly between 60 - 120 degree
    double ballSpeed = 200; // ball's speed
    srand(time(NULL));

    double ballAngle;
    do {
        ballAngle = (rand() % 61 + 60) * PI / 180.0;
    } while (fabs(ballAngle - PI / 2) < 1e-6); // make sure it is not 90 degree
    Ball ball(400, 490, ballSpeed * cos(ballAngle), -ballSpeed * sin(ballAngle), 10);

    const int NUM_BRICKS = 36;
    Brick bricks[NUM_BRICKS];

    // counter for recording remaining bricks
    int visibleBrickCount = NUM_BRICKS;

    // initialize the bricks
    for (int i = 0; i < NUM_BRICKS; ++i) {
        double x = 100 + (i % 6) * 100;
        double y = 50 + (i / 6) * 25;
        bricks[i] = Brick(x, y, 100, 25);
    }

    GLuint ballTexId = LoadTexture("media/ball.png");
    GLuint boardTexId = LoadTexture("media/board.png");
    GLuint orange_texId = LoadTexture("media/orange_brick.png");
    GLuint red_texId = LoadTexture("media/red_brick.png");
    GLuint blue_texId = LoadTexture("media/blue_brick.png");
    GLuint green_texId = LoadTexture("media/green_brick.png");
    GLuint yellow_texId = LoadTexture("media/yellow_brick.png");
    GLuint purple_texId = LoadTexture("media/purple_brick.png");
    GLuint universeTexId = LoadTexture("media/universe.png");
    GLuint readyTexId = LoadTexture("media/ready.png");
    GLuint goTexId = LoadTexture("media/go.png");
    GLuint winTexId = LoadTexture("media/youwin.png");
    GLuint loseTexId = LoadTexture("media/youlose.png");
    GLuint oneTexId = LoadTexture("media/1.png");
    GLuint twoTexId = LoadTexture("media/2.png");
    GLuint threeTexId = LoadTexture("media/3.png");
    GLuint fourTexId = LoadTexture("media/4.png");
    GLuint fiveTexId = LoadTexture("media/5.png");
    GLuint sixTexId = LoadTexture("media/6.png");
    GLuint sevenTexId = LoadTexture("media/7.png");
    GLuint eightTexId = LoadTexture("media/8.png");
    GLuint nineTexId = LoadTexture("media/9.png");
    GLuint zeroTexId = LoadTexture("media/0.png");


    double waitTimeStart = FsSubSecondTimer();
    while (FsSubSecondTimer() - waitTimeStart < 2000) {
        FsPollDevice();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, universeTexId);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2d(0.0, 1.0); glVertex2d(0, 0);
        glTexCoord2d(1.0, 1.0); glVertex2d(800, 0);
        glTexCoord2d(1.0, 0.0); glVertex2d(800, 600);
        glTexCoord2d(0.0, 0.0); glVertex2d(0, 600);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // draw Ready
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, readyTexId);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2d(0.0, 1.0); glVertex2d(175, 150);
        glTexCoord2d(1.0, 1.0); glVertex2d(625, 150);
        glTexCoord2d(1.0, 0.0); glVertex2d(625, 300);
        glTexCoord2d(0.0, 0.0); glVertex2d(175, 300);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        FsSwapBuffers();
        FsSleep(10);
    }

    waitTimeStart = FsSubSecondTimer();
    while (FsSubSecondTimer() - waitTimeStart < 500) {
        FsPollDevice();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, universeTexId);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2d(0.0, 1.0); glVertex2d(0, 0);
        glTexCoord2d(1.0, 1.0); glVertex2d(800, 0);
        glTexCoord2d(1.0, 0.0); glVertex2d(800, 600);
        glTexCoord2d(0.0, 0.0); glVertex2d(0, 600);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // draw Ready
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, goTexId);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2d(0.0, 1.0); glVertex2d(275, 150);
        glTexCoord2d(1.0, 1.0); glVertex2d(525, 150);
        glTexCoord2d(1.0, 0.0); glVertex2d(525, 300);
        glTexCoord2d(0.0, 0.0); glVertex2d(275, 300);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        FsSwapBuffers();
        FsSleep(10);
    }

    double lastTime = FsSubSecondTimer();
    bool won = false;
    int terminate = 0;
    

    
        while(terminate == 0)
        {
        
        FsPollDevice();

            
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, universeTexId);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 1.0); glVertex2d(0, 0);
        glTexCoord2d(1.0, 1.0); glVertex2d(800, 0);
        glTexCoord2d(1.0, 0.0); glVertex2d(800, 600);
        glTexCoord2d(0.0, 0.0); glVertex2d(0, 600);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // update ball's position
        double currentTime = FsSubSecondTimer();
        double dt = (currentTime - lastTime) / 1000.0; // transfer to second
        lastTime = currentTime;
        ball.Move(dt);

        int key = FsInkey();
            
            
        switch(key) {
            case FSKEY_LEFT:
                board.MoveLeft(movementDistance);
                break;
            case  FSKEY_RIGHT:

                board.MoveRight(movementDistance);
                break;
            case  FSKEY_ESC:
                terminate = 1;
                    int resScore = score;
                    score = 0;
                    return  resScore;
        }
        
            
        if (visibleBrickCount == 0) {
            printf("YOU WIN\n");
            won = true;
            break;
        }

        // collision between ball and walls
        if (ball.x - ball.radius < 0 || ball.x + ball.radius > 800) {
            ball.vx = -ball.vx;
            ball.x += ball.vx * dt;
        }
        if (ball.y - ball.radius < 0) {
            ball.vy = -ball.vy;
            ball.y += ball.vy * dt;
        }

        // collision between ball and board
        else if (ball.y + ball.radius > board.yBottom && ball.x > board.xLeft + 65 && ball.x < board.xRight - 65 && ball.y < board.yBottom) {
            ball.vy = -ball.vy; // obey incidence and reflection law
            ball.y = board.yBottom - ball.radius;
        }

        // left section of board's top face
        else if (ball.y + ball.radius > board.yBottom && ball.x > board.xLeft && ball.x <= board.xLeft + 65 && ball.y < board.yBottom) {
            double speed = sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
            double randomAngle = (rand() % 61 + 30) * PI / 180.0; // give a random angel between 30-90
            ball.vx = speed * cos(randomAngle) * (ball.vx >= 0 ? 1 : -1);
            ball.vy = -speed * sin(randomAngle);
            ball.y = board.yBottom - ball.radius;
        }

        // right section of board's top face
        else if (ball.y + ball.radius > board.yBottom && ball.x >= board.xRight - 65 && ball.x < board.xRight && ball.y < board.yBottom) {
            double speed = sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
            double randomAngle = (rand() % 61 + 30) * PI / 180.0; // give a random angel between 30-90
            ball.vx = speed * cos(randomAngle) * (ball.vx >= 0 ? 1 : -1);
            ball.vy = -speed * sin(randomAngle);
            ball.y = board.yBottom - ball.radius;
        }

        // right side face of board
        else if (ball.y > board.yBottom && ball.y < board.yTop && ball.x > board.xRight && ball.x - ball.radius < board.xRight) {
            ball.vx = -ball.vx;
            ball.x = board.xRight + ball.radius;
        }
        // left side face of board
        else if (ball.y > board.yBottom && ball.y < board.yTop && ball.x < board.xLeft && ball.x + ball.radius > board.xLeft) {
            ball.vx = -ball.vx;
            ball.x = board.xLeft - ball.radius;
        }
        // miss the ball
        else if (ball.y + ball.radius > 600) {
            won = false;
            break; // end the game
        }

        // hit the corner of board
        if (Distance(ball.x, ball.y, board.xLeft, board.yBottom) < ball.radius && ball.y < board.yBottom && ball.x < board.xLeft ||
            Distance(ball.x, ball.y, board.xRight, board.yBottom) < ball.radius && ball.y < board.yBottom && ball.x > board.xRight ||
            Distance(ball.x, ball.y, board.xLeft, board.yTop) < ball.radius && ball.y > board.yBottom && ball.x < board.xLeft ||
            Distance(ball.x, ball.y, board.xRight, board.yTop) < ball.radius && ball.y > board.yBottom && ball.x > board.xRight
            ) {
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }

        // check if hits the bricks
        for (int i = 0; i < NUM_BRICKS; ++i) {
            if (bricks[i].visible) {
                HandleBrickCollision(bricks[i], ball, visibleBrickCount, score);
            }
        }

        // printf("Score: %d\n", score);

        board.Draw(boardTexId);
        ball.Draw(ballTexId);

        // draw the bricks
        for (int i = 0; i < NUM_BRICKS; ++i) {

            GLuint texIdForBrick;
            int row = i / 6;
            switch (row) {
            case 0:
                texIdForBrick = red_texId; // red
                break;
            case 1:
                texIdForBrick = orange_texId; // orange
                break;
            case 2:
                texIdForBrick = yellow_texId; // yellow
                break;
            case 3:
                texIdForBrick = green_texId; // green
                break;
            case 4:
                texIdForBrick = blue_texId; // blue
                break;
            case 5:
                texIdForBrick = purple_texId; // purple
                break;
            }
            bricks[i].Draw(texIdForBrick);
        }

        char scoreStr[10];
        sprintf(scoreStr, "%d", score); // transfer the score to string

        const int numWidth = 25;
        const int numHeight = 50;

        // draw the number
        int drawPosX = 700;
        int drawPosY = 0;

        for (int i = 0; scoreStr[i] != '\0'; ++i) {
            GLuint texIdForNum;
            switch (scoreStr[i]) {
            case '0': texIdForNum = zeroTexId; break;
            case '1': texIdForNum = oneTexId; break;
            case '2': texIdForNum = twoTexId; break;
            case '3': texIdForNum = threeTexId; break;
            case '4': texIdForNum = fourTexId; break;
            case '5': texIdForNum = fiveTexId; break;
            case '6': texIdForNum = sixTexId; break;
            case '7': texIdForNum = sevenTexId; break;
            case '8': texIdForNum = eightTexId; break;
            case '9': texIdForNum = nineTexId; break;
            default: continue;
            }

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texIdForNum);
            glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 1.0f);
            glTexCoord2d(0.0, 1.0); glVertex2d(drawPosX, drawPosY);
            glTexCoord2d(1.0, 1.0); glVertex2d(drawPosX + numWidth, drawPosY);
            glTexCoord2d(1.0, 0.0); glVertex2d(drawPosX + numWidth, drawPosY + numHeight);
            glTexCoord2d(0.0, 0.0); glVertex2d(drawPosX, drawPosY + numHeight);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            drawPosX += numWidth; // move to the next number's location

            if (drawPosX >= 800) {
                drawPosX = 700;
                drawPosY += numHeight;
            }
        }

        FsSwapBuffers();
        FsSleep(10);
    }

    // game ends
    double endWaitStart = FsSubSecondTimer();
    while (FsSubSecondTimer() - endWaitStart < 3000) {
        FsPollDevice();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, universeTexId);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 1.0); glVertex2d(0, 0);
        glTexCoord2d(1.0, 1.0); glVertex2d(800, 0);
        glTexCoord2d(1.0, 0.0); glVertex2d(800, 600);
        glTexCoord2d(0.0, 0.0); glVertex2d(0, 600);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // show the result
        if (won) {
            glBindTexture(GL_TEXTURE_2D, winTexId);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, loseTexId);
        }

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 1.0); glVertex2d(200, 200);
        glTexCoord2d(1.0, 1.0); glVertex2d(600, 200);
        glTexCoord2d(1.0, 0.0); glVertex2d(600, 400);
        glTexCoord2d(0.0, 0.0); glVertex2d(200, 400);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        FsSwapBuffers();
        FsSleep(10);
    }
    
    
    int resScore = score;
    score = 0;
    return  resScore;
}
