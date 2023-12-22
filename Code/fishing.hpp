#ifndef FISHING_H
#define FISHING_H

#include <vector>

void drawSun();
void updateSun();
void drawWave(std::vector<std::vector<double> >& waves);
void drawMountain();
void drawFish(double& x, double& y);
void drawBomb(double& x, double& y);
void drawHook(int dx, int dy, int& dt);
void updateFish(double& x, double& y, double& speed);
void drawInit();
void drawCloud(std::vector<std::vector<double> > cloudXY);
void drawBoat(int dx);
void updateCloudXY(std::vector<std::vector<double> >& cloudXY);
void updateWaves(std::vector<std::vector<double> >& waves);
bool IsPointInEllipse(double x1, double y1, double x, double y);
double getValue(double value, double min, double max);
bool checkHit(int dx, int dy, double& x, double& y);
void printBoard(int time, int score);
int runFishing();

#endif // FISHING_H
