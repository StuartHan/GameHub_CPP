#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include<iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "fishing.hpp"

using namespace std;
const double YS_PI = 3.1415927;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float SUN_RADIUS = 50.0f;
const float SUN_SPEED = 0.005f;
const int WAVE_AMPLITUDE = 10;   // wave's amplitude
const float WAVE_FREQUENCY = 0.05f;  // wave's frequency

float sunX = 0;
float sunY = 300;

int totalScore = 0;

void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < 64; i ++) {
        double angle = (double) i * YS_PI / 32.0;
        double newX = (double) sunX + cos(angle) * (double) SUN_RADIUS;
        double newY = (double) sunY + sin(angle) * (double) SUN_RADIUS;
        glVertex2d(newX, newY);
    }

    glEnd();
}

void updateSun() {
    static float angle = 9.8;
    float a = WINDOW_WIDTH / 2.0;
    float b = WINDOW_HEIGHT / 4.0;
    float centerX = WINDOW_WIDTH / 2.0;
    float centerY = WINDOW_HEIGHT / 2.0;

    sunX = centerX + a * cos(angle);
    sunY = centerY + b * sin(angle);

    angle += SUN_SPEED;

    if (angle >= 2 * YS_PI) {
        angle -= 2 * YS_PI;
    }
}

void drawWave(vector<vector<double>>& waves) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(5);
    for(int i = 0; i < waves.size(); i ++) {
        int x = waves[i][0];
        int y = waves[i][1];
        glBegin(GL_LINE_STRIP);
        for (int j = x; j < x + 150; j ++) {
            double dy = WAVE_AMPLITUDE * sinf(WAVE_FREQUENCY * j);
            glVertex2f(j,  y + dy);
        }
        glEnd();
    }
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2i(0, 600);
    glVertex2i(100, 600);
    glVertex2i(100, 300);
    glVertex2i(0, 300);
    glEnd();
}

void drawMountain() {
    // draw mountain
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    glVertex2i(700, 400);
    glVertex2i(800, 400);
    glVertex2i(750, 350);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2i(650, 400);
    glVertex2i(750, 400);
    glVertex2i(700, 280);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2i(600, 400);
    glVertex2i(700, 400);
    glVertex2i(650, 360);
    glEnd();
}

void drawFish(double& x, double& y) {
    // draw fish's body
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x, y); // center

    double radiusX = 20.0;
    double radiusY = 10.0;

    for (int i = 0; i <= 100; i++) {
        double angle = 2 * YS_PI * i / 100;
        double newX = x + radiusX * cosf(angle);
        double newY = y + radiusY * sinf(angle);
        glVertex2d(newX, newY);
    }
    glEnd();

    // draw fish's eye
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int j = 0; j < 64; j ++) {
        double angle = (double) j * YS_PI / 32.0;
        double newX = (double) x + 12 + cos(angle) * (double) 3;
        double newY = (double) y + sin(angle) * (double) 3;
        glVertex2d(newX, newY);
    }
    glEnd();


    // draw fish's tail
    glColor3f(1.0f, 0.5f, 0.0f);

    glBegin(GL_TRIANGLES);
    glVertex2d(x - 20, y);
    glVertex2d(x - 40, y + 10);
    glVertex2d(x - 40,  y- 10);
    glEnd();
}


void drawBomb(double& x, double& y) {
    // draw fish's body
    glColor3f(0.0f, 0.0f, 0.0f);
    // draw fish's eye
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int j = 0; j < 64; j ++) {
        double angle = (double) j * YS_PI / 32.0;
        double newX = (double) x + 12 + cos(angle) * (double) 10;
        double newY = (double) y + sin(angle) * (double) 10;
        glVertex2d(newX, newY);
    }
    glEnd();
}




void drawHook(int dx, int dy, int& dt) {
    glLineWidth(3);
    //  draw hook's line
    glColor3f(0.2f, 0.2f, 0.2f);

    if(dy + 420 >= 600 || (400 + dy < 400) ){
        dt = -dt;
    }

    // draw hook
    glBegin(GL_LINE_STRIP);
    glVertex2d(195 + dx, 400);
    glVertex2d(195 + dx, 410 + dy);
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 270; i++) {
        float angle = i * YS_PI / 180;
        float newX = 195 + dx + 5 * cosf(angle);
        float newY = 415 + dy + 5 * sinf(angle);
        glVertex2f(newX, newY);
    }
    glEnd();
}

void updateFish(double& x, double& y, double& speed) {
    x += speed;
    if(x > 880) {
        x = -20;
        y =  rand() % 240 + 440;
    }
    while(y > 590) {
        y =  rand() % 240 + 440;
    }
}

void drawInit() {
    // enable alpha
    // background color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    double bgColor = (sunY - 150) / 300;
    glColor4f(0, 0.45 - bgColor/2, 0.9 - bgColor, bgColor + 0.1);
    glBegin(GL_POLYGON);
    glVertex2i(0, 600);
    glVertex2i(800, 600);
    glVertex2i(800, 0);
    glVertex2i(0, 0);
    glEnd();

    // disable alpha
    glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw sea
    glShadeModel(GL_SMOOTH);
    glBegin(GL_POLYGON);
    glColor3ub(135,206,250);
    glVertex2i(100, 600);
    glColor3ub(0,191,255);
    glVertex2i(800, 600);
    glColor3ub(240,248,255);
    glVertex2i(800, 400);
    glColor3ub(240,248,255);
    glVertex2i(100, 400);
    glEnd();

    glDisable(GL_BLEND);
}

void drawCloud(vector<vector<double>> cloudXY) {
    glColor3f(0.0f, 1.0f, 1.0f);

    for (int i = 0; i < cloudXY.size(); i++) {
        double x = cloudXY[i][0];
        double y = cloudXY[i][1];
        for(int k = 0; k <= 3; k ++) {
            for (int j = 0; j < 72; j++) {
                glBegin(GL_POLYGON);
                double angle = (double) j * YS_PI / 32.0;
                double newX = (double) x + 15 * k + cos(angle) * (double) 20;
                double newY = (double) y + sin(angle) * (double) 20;
                glVertex2d(newX, newY);
            }
        }
        for(int k = 1; k <= 3; k ++) {
            for (int j = 0; j < 72; j++) {
                glBegin(GL_POLYGON);
                double angle = (double) j * YS_PI / 32.0;
                double newX = (double) x + 10 * k  + cos(angle) * (double) 20;
                double newY = (double) y - 15 + sin(angle) * (double) 20;
                glVertex2d(newX, newY);
            }
        }
        glEnd();
    }
}


void drawBoat(int dx) {
    // draw first layer
    glColor3f(1, 1, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2i(150 + dx, 400);
    glVertex2i(240 + dx, 400);
    glVertex2i(270 + dx, 350);
    glVertex2i(120 + dx, 350);
    glEnd();

    // draw second layer
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2i(150 + dx, 350);
    glVertex2i(240 + dx, 350);
    glVertex2i(240 + dx, 350);
    glVertex2i(240 + dx, 330);
    glVertex2i(150 + dx, 330);
    glEnd();


    glBegin(GL_QUADS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw window
    glColor4f(0.0f, 0.5f, 1.0f, 0.8f);
    for(int i = 0; i < 4; i ++) {
        glVertex2i(160 + 20 * i + dx , 345);
        glVertex2i(170  + 20 * i + dx, 345);
        glVertex2i(170  + 20 * i + dx, 335);
        glVertex2i(160 + 20 * i + dx, 335);
    }
    glEnd();
    glDisable(GL_BLEND);


    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POLYGON);
    glVertex2i(170 + dx, 330);
    glVertex2i(230 + dx, 330);
    glVertex2i(230 + dx, 320);
    glVertex2i(170 + dx, 320);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2i(190 + dx, 320);
    glVertex2i(205 + dx, 320);
    glVertex2i(205 + dx, 290);
    glVertex2i(190 + dx, 290);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2i(190 + dx, 320);
    glVertex2i(205 + dx, 320);
    glVertex2i(205 + dx, 290);
    glVertex2i(190 + dx, 290);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2i(208 + dx, 320);
    glVertex2i(218 + dx, 320);
    glVertex2i(218 + dx, 300);
    glVertex2i(208 + dx, 300);
    glEnd();
}

void updateCloudXY(vector<vector<double>>& cloudXY) {
    for(int i = 0; i < cloudXY.size(); i ++) {
        if(cloudXY[i][0] > 800) {
            cloudXY.erase(cloudXY.cbegin() + i);
            double y = rand() % 350;
            double speed = (rand() % 100) / 100.0 + 0.5;
            cloudXY.push_back({0, y, speed});
        }
        cloudXY[i][0] += cloudXY[i][2];
    }
}

void updateWaves(vector<vector<double>>& waves) {
    for(int i = 0; i < waves.size(); i ++) {
        if(waves[i][0] > 800) {
            waves.erase(waves.begin() + i);
            double y = rand() % 200 + 410;
            double speed = (rand() % 100) / 100.0 + 0.5;
            waves.push_back({-200, y, speed, 1});
        }
        waves[i][0] +=  waves[i][2];
    }
}

bool IsPointInEllipse(double x1, double y1, double x, double y) {
    int radiusX = 20;
    int radiusY = 10;
    double newX = x1 - x;
    double newY = y1 - y;
    return (newX * newX) / (radiusX * radiusX) +
           (newY * newY) / (radiusY * radiusY) <= 1;
}

double getValue(double value, double min, double max) {
    return (value < min) ? min : (value > max) ? max : value;
}

// check the fish and hook if hitted
bool checkHit(int dx, int dy, double& x, double& y) {
    
        int radiusX = 20;
        int radiusY = 10;
        int radius = 5;
        double circleX  = 195 + dx;
        double circleY = 415 + dy;
        if (IsPointInEllipse(circleX, circleY, x, y)) {
            return true;
        }
        double closestX = getValue(circleX, x - radiusX, x + radiusX);
        double closestY = getValue(circleY, y - radiusY, y + radiusY);
        double distanceX = circleX - closestX;
        double distanceY = circleY - closestY;
        double distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
        bool result = distanceSquared <= (radius * radius);
        if(result == true) {
            x = -20;
            y =  rand() % 200 + 440;
        }
        while(y > 590) {
            y = rand() % 240 + 440;
        }
        return result;
        
    
}

void printBoard(int time, int score){
    glColor3d(1.0, 0.5, 0.0);
    glRasterPos2d(20,60);
    
    std::string printChar = "Remaining Time:";
    printChar += std::to_string(time);
    YsGlDrawFontBitmap12x16(printChar.c_str());
    glRasterPos2d(200.0,370);
    glColor3d(1.0, 0.5, 0.0);
    glRasterPos2d(20,20);
    std::string printChar2 = "Total Score: ";
    printChar2 += std::to_string(score);
    YsGlDrawFontBitmap12x16(printChar2.c_str());
    glFlush();
}



int runFishing() {
    auto t0=std::chrono::high_resolution_clock::now();
    
    // init window
    vector<vector<double>> cloudXY;
    for(int i = 0; i < 10; i ++){
        double x = rand() % 800;
        double y = rand() % 400 - 50;
        double speed = (rand() % 100) / 100.0 + 0.5;
        cloudXY.push_back({x, y, speed});
    }
    vector<vector<double>> waves;
    
    for(int i = 0; i < 5; i ++){
        double x = rand() % 800;
        double y = rand() % 200 + 410;
        double speed = (rand() % 100) / 100.0 + 0.5;
        waves.push_back({x, y, speed});
    }
    
    int fishNumber = 3;
    double fishX[fishNumber], fishY[fishNumber];
    for(int i = 0; i < fishNumber; i++){
        fishX[i] = rand()%100 + 200;
        fishY[i] = rand() % 200 + 410;
    }
    
    double bombX = 200;
    double bombY = rand() % 200 + 410;
    
    
    
    int dx = 0;
    int dy = 0, dt = 1;
    
    
    int terminate = 0;
    int controlHook = 0;
    int meter = 0;

    int remainTime = 60;
    
    while(terminate == 0)
    {
        auto t1=std::chrono::high_resolution_clock::now();
        auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        int currentTime = timePassed /1000;
//        std::cout << "current Time is " << currentTime << std::endl;
        
        FsPollDevice();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        int key = FsInkey();
        switch(key) {
            case FSKEY_RIGHT:
                if(240 + dx <= 580 && controlHook != 1) {
                    dx += 20;
                }
                break;
            case FSKEY_LEFT:
                if(120 + dx >= 120 && controlHook != 1) {
                    dx -= 20;
                }
                break;
            case FSKEY_SPACE:
                controlHook = 1;
                break;
            case FSKEY_ESC:
                terminate = 1;
                
                int resScore = totalScore;
                totalScore = 0;
                return  resScore;
//                break;
        }
        if(controlHook == 1) {
            dy += dt;
            meter ++;
            if(meter > 360) {
                controlHook = 0;
                meter = 0;
                dy = 0;
            }
        }
        double speed = (rand() % 100) / 100.0 + 0.5;
        
        drawSun();
    
        drawInit();

        drawCloud(cloudXY);
        
        for(int i = 0; i < fishNumber; i++){
            drawFish(fishX[i], fishY[i]);
        }
        
        drawBomb(bombX, bombY);
        
        drawCloud(cloudXY);
        
        drawBoat(dx);
        
        drawMountain();
        
        drawWave(waves);

        drawHook(dx, dy, dt);
        
        printBoard( remainTime - currentTime, totalScore);
        
        for(int i = 0; i < fishNumber; i++){
            updateFish(fishX[i], fishY[i], speed);
        }
        
        updateFish(bombX, bombY, speed);
        
        for(int i = 0; i < fishNumber; i++){
            if (checkHit(dx, dy, fishX[i], fishY[i])){
                totalScore+=10;
            }
        }
    
        // check whether the hook hits a bomb
        if(checkHit(dx, dy, bombX, bombY)){
            totalScore = 0;
            std::cout << "A bomb is caught! All scores will be lost" << std::endl;
        }
    
        
        if(remainTime - currentTime == 0){
            std::cout << "The game is over. The total score is " << totalScore << std::endl;
            
            
            int resScore = totalScore;
            totalScore = 0;
            return resScore;
//            break;
        }
        
        updateSun();

        updateCloudXY(cloudXY);

        updateWaves(waves);

        FsSwapBuffers();

        FsSleep(10);
    }
    return 0;
}




