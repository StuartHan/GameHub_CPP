#include <time.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#define GL_SILENCE_DEPRECATION
#include "fssimplewindow.h"
#include "ysglfontdata.h"

const float PI = 3.14159265;

//parameters of TIE fighter
const double TIEradius=15;
const double TIEWingHeight=40;
const double TIEWingWidth=5;

//parameters of Star Destroyer
const double SDWidth=100;
const double SDHeight1=30;
const double SDHeight2=150;
const double SDCockpitWidth=40;
const double SDCockpitHeight=20;

//parameters of Millennium Falcon
const double MFradius=30;
const double MFHeight1=50;
const double MFHeight2=35;
const double MFHeight3=15;
const double MFWidth1=15;
const double MFWidth2=10;

//parameters of hp
const int barWidth = 200;
const int barHeight = 20;
const int barX = 10;
const int barY = 580;

//parameters of score
const int progressBarWidth = barWidth;
const int progressBarHeight = barHeight;
const int progressBarX = 800 - barX - progressBarWidth;
const int progressBarY = barY;
const int targetScore=2000;
const int TIEScore=50;
const int SDScore=300;

//parameters of generating enemies
const int TIE_SPAWN_INTERVAL = 100;
const int TIE_SPAWN_CHANCE = 30;
const int STARDESTROYER_SPAWN_INTERVAL = 200;
const int STARDESTROYER_SPAWN_CHANCE = 10;
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 100);
std::uniform_int_distribution<int> spawnXPos(0, 800 - 1);

//other parameters
const double TIEBulletV=3;
const double TIEV=1;
const double MFBulletV=5;
const double SDV=0.5;
const double LaserWidth=3;
const double LaserLength=600;


struct TIE {
    double centerX,centerY;
    int life;
    int existTime;
};

struct StarDestroyer{
    int ID;
    double centerX,centerY;
    int life;
    int existTime;
};

struct MillenniumFalcon{
    double centerX,centerY;
    int life;
};

struct TIEBullet{
    double x,y;
};

struct MFBullet{
    double x,y;
};

struct SDLaser
{
    int SD_ID;
    double x,y;
    int existTime;
};

std::vector<TIE> TIEs;
std::vector<StarDestroyer> SDs;
std::vector<TIEBullet> TIEBullets;
std::vector<SDLaser> SDLasers;
std::vector<MFBullet> MFBullets;
MillenniumFalcon mf;
int SDID=0;

void drawTIE(TIE tie)
{
//    // make sure the graph is right-side-up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800,0, 600);
    // draw cockpit
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(255, 255, 255, 50);
    glBegin(GL_POLYGON);
           for(int i = 0; i < 360; i++) {
               double angle = i * PI / 180.0;
               double x = tie.centerX + cos(angle) * TIEradius;
               double y = tie.centerY + sin(angle) * TIEradius;
               glVertex2d(x, y);
           }
    glEnd();
    //draw outline of cockpit
    glDisable(GL_BLEND);
    glLineWidth(2.0f);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; ++i)
    {
        double angle=2.0*3.14159*i/360.0;
        double x=tie.centerX+cos(angle)*TIEradius;
        double y=tie.centerY+sin(angle)*TIEradius;
        glVertex2d(x,y);
    }
    glEnd();
    //draw wings
    glColor3ub(128, 128, 128);
       glBegin(GL_QUADS);
       glVertex2i(tie.centerX+TIEradius, tie.centerY-TIEWingHeight/2);
       glVertex2i(tie.centerX+TIEradius+TIEWingWidth, tie.centerY-TIEWingHeight/2);
       glVertex2i(tie.centerX+TIEradius+TIEWingWidth, tie.centerY+TIEWingHeight/2);
       glVertex2i(tie.centerX+TIEradius, tie.centerY+TIEWingHeight/2);
       glEnd();
    glBegin(GL_QUADS);
     glVertex2i(tie.centerX-TIEradius-TIEWingWidth, tie.centerY-TIEWingHeight/2);
     glVertex2i(tie.centerX-TIEradius, tie.centerY-TIEWingHeight/2);
     glVertex2i(tie.centerX-TIEradius, tie.centerY+TIEWingHeight/2);
     glVertex2i(tie.centerX-TIEradius-TIEWingWidth, tie.centerY+TIEWingHeight/2);
    glEnd();
}

void drawStarDestroyer(StarDestroyer starDestroyer)
{
    // make sure the graph is right-side-up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800,0, 600);
    //draw body
       glBegin(GL_TRIANGLES);
    glColor3ub(200, 200, 200);
       glVertex2i(starDestroyer.centerX-SDWidth/2, starDestroyer.centerY);
    glColor3ub(100, 100, 100);
       glVertex2i(starDestroyer.centerX, starDestroyer.centerY+SDHeight1);
    glColor3ub(100, 100, 100);
        glVertex2i(starDestroyer.centerX, starDestroyer.centerY-SDHeight2);
        glEnd();
    glBegin(GL_TRIANGLES);
 glColor3ub(200, 200, 200);
    glVertex2i(starDestroyer.centerX+SDWidth/2, starDestroyer.centerY);
 glColor3ub(100, 100, 100);
    glVertex2i(starDestroyer.centerX, starDestroyer.centerY+SDHeight1);
 glColor3ub(100, 100, 100);
     glVertex2i(starDestroyer.centerX, starDestroyer.centerY-SDHeight2);
     glEnd();
    //draw cockpit
    glColor3ub(0, 0, 0);
       glBegin(GL_QUADS);
       glVertex2i(starDestroyer.centerX-SDCockpitWidth/2, starDestroyer.centerY-SDCockpitHeight/2);
       glVertex2i(starDestroyer.centerX+SDCockpitWidth/2, starDestroyer.centerY-SDCockpitHeight/2);
       glVertex2i(starDestroyer.centerX+SDCockpitWidth/2, starDestroyer.centerY+SDCockpitHeight/2);
       glVertex2i(starDestroyer.centerX-SDCockpitWidth/2, starDestroyer.centerY+SDCockpitHeight/2);
    glEnd();
    //draw middle lines
    glBegin(GL_LINES);
    glVertex2i(starDestroyer.centerX, starDestroyer.centerY+SDCockpitHeight/2);
    glVertex2i(starDestroyer.centerX, starDestroyer.centerY+SDHeight1);
    glVertex2i(starDestroyer.centerX, starDestroyer.centerY-SDCockpitHeight/2);
    glVertex2i(starDestroyer.centerX, starDestroyer.centerY-SDHeight2);
    glEnd();
}

void drawMillenniumFalcon(MillenniumFalcon mf) {
    // make sure the graph is right-side-up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800,0, 600);
    //draw laser cannon
    glColor3ub(200, 200, 200);
       glBegin(GL_QUADS);
       glVertex2i(mf.centerX-MFWidth1/2, mf.centerY);
       glVertex2i(mf.centerX-MFWidth1/2, mf.centerY+MFHeight2);
       glVertex2i(mf.centerX+MFWidth1/2, mf.centerY+MFHeight2);
       glVertex2i(mf.centerX+MFWidth1/2, mf.centerY);
    glEnd();
    //draw triangles
    glColor3ub(200, 200, 200);
       glBegin(GL_POLYGON);
       glVertex2i(mf.centerX-MFradius, mf.centerY);
       glVertex2i(mf.centerX-MFWidth1/2, mf.centerY+MFHeight1);
       glVertex2i(mf.centerX-MFWidth1/2, mf.centerY);
        glEnd();
    glBegin(GL_POLYGON);
    glVertex2i(mf.centerX+MFradius, mf.centerY);
    glVertex2i(mf.centerX+MFWidth1/2, mf.centerY+MFHeight1);
    glVertex2i(mf.centerX+MFWidth1/2, mf.centerY);
     glEnd();
    //draw right cannon
    glColor3ub(200, 200, 200);
       glBegin(GL_QUADS);
       glVertex2i(mf.centerX+MFradius-MFWidth2/2, mf.centerY);
       glVertex2i(mf.centerX+MFradius-MFWidth2/2, mf.centerY+MFHeight3);
       glVertex2i(mf.centerX+MFradius+MFWidth2/2, mf.centerY+MFHeight3);
       glVertex2i(mf.centerX+MFradius+MFWidth2/2, mf.centerY);
    glEnd();
    //draw body
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(150, 150, 150);
    glVertex2i(mf.centerX, mf.centerY);
           for(int i = 0; i < 360; i++) {
               double angle = i * PI / 180.0;
               double x =mf.centerX + cos(angle) * MFradius;
               double y = mf.centerY + sin(angle) * MFradius;
               glColor3ub(230, 230, 230);
               glVertex2d(x, y);
           }
    glEnd();
}

void drawTIEBullet(TIEBullet bullet){
    //make sure the graph is right-side-up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800,0, 600);
    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++)
    {
        double theta=2*PI*double(i)/20;
        double x=3*cos(theta);
        double y=3*sin(theta);
        glVertex2d(bullet.x + x, bullet.y + y);
    }
    glEnd();
}

void drawMFBullet(MFBullet bullet){
    //make sure the graph is right-side-up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800,0, 600);
    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++)
    {
        double theta=2*PI*double(i)/20;
        double x=3*cos(theta);
        double y=3*sin(theta);
        glVertex2d(bullet.x + x, bullet.y + y);
    }
    glEnd();
}
 
void drawSDLaser(SDLaser laser)
{
    //make sure the graph is right-side-up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800,0, 600);
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
        glVertex2i(laser.x-LaserWidth/2, laser.y);
        glVertex2i(laser.x+LaserWidth/2, laser.y);
        glVertex2i(laser.x+LaserWidth/2, laser.y-LaserLength);
        glVertex2i(laser.x-LaserWidth/2, laser.y-LaserLength);
    glEnd();
}

void drawHealthBar() {
    int currentBarWidth = (int)((double)mf.life/ 20 * barWidth);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINE_LOOP);
        glVertex2i(barX, barY);
        glVertex2i(barX + barWidth, barY);
        glVertex2i(barX + barWidth, barY - barHeight);
        glVertex2i(barX, barY - barHeight);
    glEnd();
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
        glVertex2i(barX, barY);
        glVertex2i(barX + currentBarWidth, barY);
        glVertex2i(barX + currentBarWidth, barY - barHeight);
        glVertex2i(barX, barY - barHeight);
    glEnd();
}

void drawProgressBar(int score, int maxScore) {
    int currentBarWidth = (int)((double)score / maxScore * progressBarWidth);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINE_LOOP);
        glVertex2i(progressBarX, progressBarY);
        glVertex2i(progressBarX + progressBarWidth, progressBarY);
        glVertex2i(progressBarX + progressBarWidth, progressBarY - progressBarHeight);
        glVertex2i(progressBarX, progressBarY - progressBarHeight);
    glEnd();
    glColor3ub(0, 0, 255);
    glBegin(GL_QUADS);
        glVertex2i(progressBarX, progressBarY);
        glVertex2i(progressBarX + currentBarWidth, progressBarY);
        glVertex2i(progressBarX + currentBarWidth, progressBarY - progressBarHeight);
        glVertex2i(progressBarX, progressBarY - progressBarHeight);
    glEnd();
}

void updateTIE(TIE &tie)
{
    tie.existTime++;
    tie.centerY-=TIEV;
    if(tie.existTime%100==0)
    {
        TIEBullet bullet;
        bullet.x=tie.centerX;
        bullet.y=tie.centerY-TIEradius;
        TIEBullets.push_back(bullet);
    }
}

void updateSD(StarDestroyer &sd)
{
    sd.existTime++;
    sd.centerY-=SDV;
    if(sd.existTime%200==0)
    {
        SDLaser laser;
        laser.SD_ID=sd.ID;
        laser.x=sd.centerX;
        laser.y=sd.centerY-SDHeight2;
        laser.existTime=0;
        SDLasers.push_back(laser);
    }
}

void updateTIEBullet(TIEBullet &bullet)
{
    bullet.y-=TIEBulletV;
}

void updateSDLaser(SDLaser &laser)
{
    laser.existTime++;
    laser.y-=SDV;
}

void updateMFBullet(MFBullet &bullet)
{
    bullet.y+=MFBulletV;
}

bool checkBulletHitMF(int x, int y) {
    // Check body collision (circle)
    int dx = x - mf.centerX;
    int dy = y - mf.centerY;
    if (dx*dx + dy*dy <= MFradius*MFradius) {
        return true;
    }
    auto rectCollision = [](int x, int y, int x1, int y1, int x2, int y2) -> bool {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    };
    if (rectCollision(x, y,
                      mf.centerX - MFWidth1/2, mf.centerY,
                      mf.centerX + MFWidth1/2, mf.centerY + MFHeight2)) {
        return true;
    }
    if (rectCollision(x, y,
                      mf.centerX + MFradius - MFWidth2/2, mf.centerY,
                      mf.centerX + MFradius + MFWidth2/2, mf.centerY + MFHeight3)) {
        return true;
    }
    auto pointInTriangle = [](int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) -> bool {
        float detT = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
        float alpha = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / detT;
        float beta = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / detT;
        float gamma = 1.0f - alpha - beta;

        return alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1;
    };
    if (pointInTriangle(x, y,
                        mf.centerX - MFradius, mf.centerY,
                        mf.centerX - MFWidth1/2, mf.centerY,
                        mf.centerX - MFWidth1/2, mf.centerY + MFHeight1)) {
        return true;
    }
    if (pointInTriangle(x, y,
                        mf.centerX + MFradius, mf.centerY,
                        mf.centerX + MFWidth1/2, mf.centerY,
                        mf.centerX + MFWidth1/2, mf.centerY + MFHeight1)) {
        return true;
    }
    return false;
}

bool checkTIEHitMF(TIE tie, MillenniumFalcon mf) {
        auto distance = [](double x1, double y1, double x2, double y2) {
            return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        };
        double dist = distance(tie.centerX, tie.centerY, mf.centerX, mf.centerY);
        if (dist <= TIEradius + MFradius) {
            return true;
        }
        if (tie.centerX + TIEradius >= mf.centerX - MFWidth1/2 &&
            tie.centerX - TIEradius <= mf.centerX + MFWidth1/2 &&
            tie.centerY + TIEradius >= mf.centerY &&
            tie.centerY - TIEradius <= mf.centerY + MFHeight2) {
            return true;
        }
        if ((tie.centerX + TIEradius >= mf.centerX - MFradius &&
            tie.centerX - TIEradius <= mf.centerX - MFWidth1/2 &&
            tie.centerY + TIEradius >= mf.centerY &&
            tie.centerY - TIEradius <= mf.centerY + MFHeight1) ||
           (tie.centerX + TIEradius >= mf.centerX + MFWidth1/2 &&
            tie.centerX - TIEradius <= mf.centerX + MFradius &&
            tie.centerY + TIEradius >= mf.centerY &&
            tie.centerY - TIEradius <= mf.centerY + MFHeight1)) {
            return true;
        }
        if (tie.centerX + TIEradius >= mf.centerX + MFradius - MFWidth2/2 &&
            tie.centerX - TIEradius <= mf.centerX + MFradius + MFWidth2/2 &&
            tie.centerY + TIEradius >= mf.centerY &&
            tie.centerY - TIEradius <= mf.centerY + MFHeight3) {
            return true;
        }
        return false;
}

bool checkSDHitMF(StarDestroyer sd, MillenniumFalcon mf) {
    auto isInsideTriangle = [](double px, double py, double ax, double ay, double bx, double by, double cx, double cy) -> bool {
        double detT = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
        double w1 = ((px - ax) * (cy - ay) - (py - ay) * (cx - ax)) / detT;
        double w2 = ((bx - ax) * (py - ay) - (by - ay) * (px - ax)) / detT;
        return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
    };
    double dx = sd.centerX - mf.centerX;
    double dy = sd.centerY - mf.centerY;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance <MFradius) {
        return true;
    }
    for (int j = -1; j <= 1; j += 2) {
        for (int i = -1; i <= 1; i += 2) {
            double ax = mf.centerX + i * MFradius;
            double ay = mf.centerY;
            double bx = mf.centerX + i * MFWidth1 / 2;
            double by = mf.centerY + MFHeight1;
            double cx = sd.centerX;
            double cy = sd.centerY + j * SDHeight1;
            double dx = sd.centerX - SDWidth / 2;
            double dy = sd.centerY;
            double ex = sd.centerX;
            double ey = sd.centerY - j * SDHeight2;
            if (isInsideTriangle(ax, ay, cx, cy, dx, dy, ex, ey) ||
                isInsideTriangle(bx, by, cx, cy, dx, dy, ex, ey)) {
                return true;
            }
        }
    }
    return false;
}

bool pointInTriangle(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3) {
    float detT = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
    float alpha = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3)) / detT;
    float beta = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3)) / detT;
    float gamma = 1.0f - alpha - beta;
    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

void checkMFBulletHit() {
    for (int i=0;i<MFBullets.size();i++) {
       MFBullet &bullet=MFBullets[i];
    for(auto &tie:TIEs)
    {
        double dx = bullet.x - tie.centerX;
        double dy = bullet.y - tie.centerY;
        bool hit1=dx * dx + dy * dy <= TIEradius * TIEradius;
        bool hit2=(bullet.x >= tie.centerX - TIEradius - TIEWingWidth && bullet.x <= tie.centerX - TIEradius &&
                   bullet.y >= tie.centerY - TIEWingHeight / 2 && bullet.y <= tie.centerY + TIEWingHeight / 2);
        bool hit3=(bullet.x >= tie.centerX + TIEradius && bullet.x <= tie.centerX + TIEradius + TIEWingWidth &&
                   bullet.y >= tie.centerY - TIEWingHeight / 2 && bullet.y <= tie.centerY + TIEWingHeight / 2);
        if(hit1||hit2||hit3)
        {
            tie.life-=1;
            MFBullets.erase(MFBullets.begin()+i);
            break;
        }
    }
    for(auto &sd:SDs)
    {
        bool inTriangle1 = pointInTriangle(bullet.x, bullet.y,
            sd.centerX - SDWidth/2, sd.centerY,
            sd.centerX, sd.centerY + SDHeight1,
            sd.centerX, sd.centerY - SDHeight2);
        bool inTriangle2 = pointInTriangle(bullet.x, bullet.y,
            sd.centerX + SDWidth/2, sd.centerY,
           sd.centerX, sd.centerY + SDHeight1,
            sd.centerX, sd.centerY - SDHeight2);
        if(inTriangle1||inTriangle2)
        {
            sd.life-=1;
            MFBullets.erase(MFBullets.begin()+i);
            break;
        }
    }
    }
}

bool checkLaserHitMF(int rectMidX, int rectTopY, int rectWidth, int rectHeight) {
    int rectX1 = rectMidX - rectWidth/2;
    int rectY1 = rectTopY - rectHeight;
    int rectX2 = rectMidX + rectWidth/2;
    int rectY2 = rectTopY;
    if (rectX1 < mf.centerX + MFWidth1/2 && rectX2 > mf.centerX - MFWidth1/2 &&
        rectY1 < mf.centerY + MFHeight2 && rectY2 > mf.centerY) {
        return true;
    }
    if (rectX1 < mf.centerX - MFWidth1/2 && rectX2 > mf.centerX - MFradius &&
        rectY1 < mf.centerY + MFHeight1 && rectY2 > mf.centerY) {
        return true;
    }
    if (rectX1 < mf.centerX + MFradius && rectX2 > mf.centerX + MFWidth1/2 &&
        rectY1 < mf.centerY + MFHeight1 && rectY2 > mf.centerY) {
        return true;
    }
    if (rectX1 < mf.centerX + MFradius + MFWidth2/2 && rectX2 > mf.centerX + MFradius - MFWidth2/2 &&
        rectY1 < mf.centerY + MFHeight3 && rectY2 > mf.centerY) {
        return true;
    }
    double distance = sqrt(pow(rectMidX - mf.centerX, 2) + pow(rectTopY - rectHeight/2 - mf.centerY, 2));
    if (distance < MFradius) {
        return true;
    }
    return false;
}

void drawResult(bool win) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2i(350,300);
    if(win){
        YsGlDrawFontBitmap16x20("You Win!");
    }else{
        YsGlDrawFontBitmap16x20("You Lose!");
    }
}

int runStarWar() {
    int frameCounter=0;
    mf.centerX=400;
    mf.centerY=50;
    mf.life=20;
    int score=0;
    int endTime=0;
    while(endTime<200){
    if(mf.life>0&&score<targetScore){
        FsPollDevice();
    int key = FsInkey();
        switch (key) {
            case FSKEY_LEFT:
                if(mf.centerX>10+MFradius)
                mf.centerX-=10;
                break;
            case FSKEY_RIGHT:
                if(mf.centerX<800-10-MFradius)
                mf.centerX+=10;
                break;
            case FSKEY_UP:
                if(mf.centerY<600-10-MFHeight1)
                mf.centerY+=10;
                break;
            case FSKEY_DOWN:
                if(mf.centerY>10+MFradius)
                mf.centerY-=10;
                break;
            case FSKEY_SPACE:
               MFBullet mfbullet;
                mfbullet.x = mf.centerX;
                mfbullet.y = mf.centerY+MFHeight2;
               MFBullets.push_back(mfbullet);
                break;
            case FSKEY_ESC:
                endTime=200;
                
                break;
        }
        //generate enemies
        frameCounter++;
        if (frameCounter % TIE_SPAWN_INTERVAL == 0) {
               int randomValue = distribution(generator);
               if (randomValue < TIE_SPAWN_CHANCE) {
                   int xPos = spawnXPos(generator);
                   TIE tie;
                   tie.centerX=xPos;
                   tie.centerY=600+TIEradius;
                   tie.life=3;
                   TIEs.push_back(tie);
               }
           }
        if (frameCounter % STARDESTROYER_SPAWN_INTERVAL == 0) {
              int randomValue = distribution(generator);
              if (randomValue < STARDESTROYER_SPAWN_CHANCE) {
                  int xPos = spawnXPos(generator);

                  StarDestroyer starDestroyer;
                  SDID++;
                  starDestroyer.ID=SDID;
                  starDestroyer.centerX=xPos;
                  starDestroyer.centerY=600+SDHeight2;
                  starDestroyer.life=20;
                  SDs.push_back(starDestroyer);
              }
          }
        glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //check if MF bullets hit any enemy
        checkMFBulletHit();
        //deal with TIEs
        for (int i=0;i<TIEs.size();i++) {
            TIE &tie=TIEs[i];
            if(tie.life<=0)
            {
                score+=TIEScore;
                TIEs.erase(TIEs.begin()+i);
            }
            if(tie.centerY<0-TIEWingHeight/2)
            {

                TIEs.erase(TIEs.begin()+i);
            }
            if(checkTIEHitMF(tie, mf)){
                mf.life-=5;
                TIEs.erase(TIEs.begin()+i);
            }
            updateTIE(tie);
            drawTIE(tie);
        }
        //deal with SDs
        for (int i=0;i<SDs.size();i++) {
           StarDestroyer &sd=SDs[i];
            if(sd.centerY<0-SDHeight1||sd.life<=0)
            {
                if(sd.life<=0)
                {
                    score+=SDScore;
                }
                for (int i=0;i<SDLasers.size();i++) {
                    SDLaser &laser=SDLasers[i];
                    if(laser.SD_ID==sd.ID)
                    {
                        SDLasers.erase( SDLasers.begin()+i);
                    }
                    }
                SDs.erase(SDs.begin()+i);
            }
            if(checkSDHitMF(sd, mf)){
                mf.life-=mf.life;
                SDs.erase(SDs.begin()+i);
            }
            updateSD(sd);
            drawStarDestroyer(sd);
        }
        drawMillenniumFalcon(mf);
        drawHealthBar();
        drawProgressBar(score,targetScore);
        for (int i=0;i<TIEBullets.size();i++) {
            TIEBullet &bullet=TIEBullets[i];
            if(bullet.y<0)
            {
                TIEBullets.erase(TIEBullets.begin()+i);
            }
            if(checkBulletHitMF(bullet.x, bullet.y))
            {
                mf.life-=1;
                TIEBullets.erase(TIEBullets.begin()+i);
            }
            updateTIEBullet(bullet);
            drawTIEBullet(bullet);
            }
        for (int i=0;i<SDLasers.size();i++) {
            SDLaser &laser=SDLasers[i];

            if(laser.existTime>=100)
            {
                SDLasers.erase( SDLasers.begin()+i);
            }
            if(checkLaserHitMF(laser.x, laser.y,LaserWidth,LaserLength))
            {
                mf.life-=10;
                SDLasers.erase(SDLasers.begin()+i);
            }
            updateSDLaser(laser);
            drawSDLaser(laser);
            }
        for (int i=0;i<MFBullets.size();i++) {
            MFBullet &bullet=MFBullets[i];
            if(bullet.y>600)
            {
                MFBullets.erase(MFBullets.begin()+i);
            }
            updateMFBullet(bullet);
            drawMFBullet(bullet);
        }
        FsSwapBuffers();
        FsSleep(20);
        }
        else
        {
            FsPollDevice();
            glClear(GL_COLOR_BUFFER_BIT);
            endTime++;
            if(mf.life<=0){
                drawResult(false);
                }
            else if(score>=targetScore){
                drawResult(true);
                }
            FsSwapBuffers();
            FsSleep(20);
        }
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return score;
}









