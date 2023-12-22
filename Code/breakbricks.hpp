#ifndef BREAK_BRICKS_H
#define BREAK_BRICKS_H

#include "fssimplewindow.h"
#include "yspng.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.14159265358979323846

struct ReboundBoard {
    double xLeft, yBottom, xRight, yTop;

    ReboundBoard(double x1, double y1, double x2, double y2);
    void Draw(GLuint texId) const;
    void MoveLeft(double distance);
    void MoveRight(double distance);
};

struct Ball {
    double x, y;    // the center of ball
    double vx, vy;  // the velocity on x and y
    double radius;

    Ball(double x, double y, double vx, double vy, double radius);
    void Move(double dt);
    void Draw(GLuint texId) const;
};

struct Brick {
    double x, y; // coordinate at left top corner
    double width, height;
    bool visible;

    Brick(double x = 0, double y = 0, double width = 0, double height = 0);
    void Draw(GLuint texId) const;
};

double Distance(double x1, double y1, double x2, double y2);
void HandleBrickCollision(Brick& brick, Ball& ball, int& visibleBrickCount, int&score);
GLuint LoadTexture(const char* filePath);
void DrawScore(int score, GLuint texIdDigits[10]);

int runBreakBricks();

#endif

