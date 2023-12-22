#ifndef MENU_IS_INCLUDED
#define MENU_IS_INCLUDED
/*{*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fstream>
#include <tuple>
#include "yspng.h"

struct Score {
    int value;
    std::string gameName;

    bool operator<(const Score& other) const {
        return value > other.value; // 分数降序排列
    }
};


class Menu
{
public:
    YsRawPngDecoder png[16]; //Change to 16 if add remainder of images

    YsSoundPlayer player;
    YsSoundPlayer::SoundData wav;

    double x, y, dt, tx1, tx2, titly1, titly2, bobv;
    int move, move2, bob;

    int level;
    int points;
    void SaveGame(int level, int points);
    void DrawPointCount(int totalScore);

    void Initialize(void);
    void drawRover(void);
    void drawBackground(int bgnum);
    void drawTitle(void);
    void drawTutorialButton(int, int);
    void drawGame1(int, int, bool);
    void drawGame2(int, int, bool);
    void drawGame3(int, int, bool);
    void drawGame4(int, int, bool);
    void drawGame5(int, int, bool);
    void drawRank(int, int);
    int drawScores(std::vector<Score>&, int, int);
    void drawTutorial();
    void LoadingGame();
    void ResetGame(bool& l1, bool& l2, bool& l3, bool& l4);

};

std::vector<Score> readScoresFromFile();
void writeScoresToFile(std::vector<Score>&);
void updateScores(std::vector<Score>&, Score&);


/*}*/
#endif
