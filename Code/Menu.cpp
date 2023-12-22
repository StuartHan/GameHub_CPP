#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "Menu.h"
#include "textinput.h"
#include "textstring.h"
#include "ysglfontdata.h"
#include "gomoku.hpp"
#include "fishing.hpp"
#include "breakbricks.hpp"
#include "GoldMiner.h"
#include "starwar.hpp"
#include <iostream>
#include <map>
using namespace std;

//Menu Program
GLuint texId[14];
enum State {
    IN_GAME,
    IN_MENU
};

vector<Score> readScoresFromFile() {
    FsChangeToProgramDir();
    std::vector<Score> scores;
    std::ifstream file("scores.txt");

    Score temp;
    while (file >> temp.value >> temp.gameName) {
        scores.push_back(temp);
    }

    file.close();
    return scores;
}

void writeScoresToFile(vector<Score>& scores) {
    std::ofstream file("scores.txt");
    
    if(!file) {
        cout << "---------------";
    }

    for (const auto& score : scores) {
        file << score.gameName << " " << score.value << std::endl;
        cout << "写" << score.value;
    }

    file.close();
}

void updateScores(vector<Score>& scores, Score& newScore) {
    scores.push_back(newScore);
    std::sort(scores.begin(), scores.end());
    std::cout<< "55 size is : " << scores.size() << std::endl;
   
    if (scores.size() > 3) {
        scores.pop_back();
    }
}

void Menu::ResetGame(bool& l1, bool& l2, bool& l3, bool& l4)
{
    l1 = 0;
    l2 = 0;
    l3 = 0;
    l4 = 0;
}

void Menu::Initialize(void)
{
    x = 100, y = 550;
    tx1 = 65, tx2 = 325;
    titly1 = 150, titly2 = 235;
    move = 1, move2 = -1, bob = -1, bobv = 0.3;
    dt = 0.1;
    level = 0;
    points = 0;
    png[0].Decode("media/background.png");
    png[0].Flip();
    png[1].Decode("media/title.png"); // Rover
    png[1].Flip();
    png[2].Decode("media/gomoku.png"); //1
    png[2].Flip();
    png[3].Decode("media/fishing.png"); //2
    png[3].Flip();
    png[4].Decode("media/goldminer.png"); //3
    png[4].Flip();
    png[5].Decode("media/breakbrick.png"); //4
    png[5].Flip();
    png[6].Decode("media/starwar.png");
    png[6].Flip();
    png[7].Decode("media/mars_rover_rotated.png");
    png[7].Flip();
    png[8].Decode("media/tutorial.png");
    png[8].Flip();
    png[9].Decode("media/rank.png");
    png[9].Flip();
}

void Menu::drawBackground(int bgnum)
{

    glShadeModel(GL_SMOOTH); // gradient background
    glBegin(GL_QUADS);

    glColor3ub(255, 235, 198);
    glVertex2i(0, 0);

    glColor3ub(255, 235, 198);
    glVertex2i(800, 0);

    glColor3ub(255, 248, 235);
    glVertex2i(800, 400);

    glColor3ub(255, 248, 235);
    glVertex2i(0, 400);

    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(0, 600 - 1);
    glDrawPixels(png[0].wid, png[0].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[0].rgba);
    glDisable(GL_BLEND);
}

void Menu::drawRover(void)
{
    if (x > 700 || x < 50)
    {
        move = move * -1;
    }
    srand(time(NULL));
    int ran = rand() % 10;
    if (y < 500 || y > 550)
    {
        move2 = move2 * -1;
    }
    y += move2 * (0.1 * ran);
    x += move;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(x, y);
    glDrawPixels(png[7].wid, png[7].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[7].rgba);
    glDisable(GL_BLEND);
}

void Menu::drawTitle()
{

    if (titly1 <= 140 || titly1 >= 160)
    {
        bob = bob * -1;
    }
    titly1 += bobv * bob;
    titly2 += bobv * bob;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(tx1, titly1);
    glDrawPixels(png[1].wid, png[1].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[1].rgba);
    glDisable(GL_BLEND);
}

void Menu::drawGame1(int posx, int posy, bool hover)
{

        if (hover)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            glDrawPixels(png[2].wid, png[2].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[2].rgba);
            glDisable(GL_BLEND);

        }

        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            //glDrawPixels(png[4].wid, png[4].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[4].rgba);
            YsGlDrawFontBitmap16x20("Gomoku");
            glDisable(GL_BLEND);

    }
}

void Menu::drawGame2(int posx, int posy, bool hover)
{

        if (hover)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            glDrawPixels(png[3].wid, png[3].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[3].rgba);
            glDisable(GL_BLEND);

        }

        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            // glDrawPixels(png[5].wid, png[5].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[5].rgba);
            YsGlDrawFontBitmap16x20("Fishing");
            glDisable(GL_BLEND);

        }
}

void Menu::drawGame3(int posx, int posy, bool hover)
{
    if (hover)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            glDrawPixels(png[4].wid, png[4].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[4].rgba);
            glDisable(GL_BLEND);

        }

        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            //glDrawPixels(png[6].wid, png[6].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[6].rgba);
            YsGlDrawFontBitmap16x20("GoldMiner");
            glDisable(GL_BLEND);
    }
}

void Menu::drawGame4(int posx, int posy, bool hover)
{

        if (hover)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            glDrawPixels(png[5].wid, png[5].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[5].rgba);
            glDisable(GL_BLEND);

        }

        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            YsGlDrawFontBitmap16x20("BreakBricks");
            glDisable(GL_BLEND);

        }
}

void Menu::drawGame5(int posx, int posy, bool hover)
{

        if (hover)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            glDrawPixels(png[6].wid, png[6].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[6].rgba);
            glDisable(GL_BLEND);

        }

        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(posx, posy);
            YsGlDrawFontBitmap16x20("StarWar");
            glDisable(GL_BLEND);
        }
}


void Menu::drawTutorialButton(int posx, int posy)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(posx, posy);
    glDrawPixels(png[8].wid, png[8].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[8].rgba);
    glDisable(GL_BLEND);
}

void Menu::drawRank(int posx, int posy)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos2i(posx, posy);
    glDrawPixels(png[9].wid, png[9].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[9].rgba);
    glDisable(GL_BLEND);
}


void Menu::LoadingGame() {
    std::string loadingStr = "Loading game";

    for (int i = 0; i <= loadingStr.length(); ++i) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(0);

        glColor3f(1, 1, 1);
        glRasterPos2i(82, 112);

        YsGlDrawFontBitmap16x24(loadingStr.substr(0, i).c_str());

        FsSwapBuffers();
        FsSleep(100);
    }

    FsSleep(500);
}

void DrawText(int x, int y, const std::string &str) {
    glColor3ub(0, 0, 0);
    glRasterPos2i(x, y);
    YsGlDrawFontBitmap16x20(str.c_str());
}

bool compareScores(const Score& a, const Score& b) {
    return a.value > b.value;
}


void Menu::drawTutorial() {
    while(true) {
        FsPollDevice();
    

        int key = FsInkey();
        if (FSKEY_ESC == key) {
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawBackground(0);
        string introduction[5][4] = {{
            "The user will be the black one in the game",
            "and the computer will be the white one.",
            "The one who places five pieces of chess in a continuous line will win",
            ""
        }, {
            "Users can use left and right keys to control the movement",
            "of the boat. The boat can't be moved out its scope",
            "and press space to release the hook",
            ""
        },
            {
                "The player is located at the top of the screen as a dark blue cube, and ",
                "your hook is swinging at a certain speed. Once the spacebar is pressed",
                "the hook will launch in the current direction, until it gets any targets or out of range",
                ""
            },
            {   "There are six layers' bricks in the air",
                "and the user should use the ball to break each brick.",
                "when the ball hits the bricks or the walls",
                "the ball will rebound obeying the law of reflection"
            },
            {
                "a classic space shooting game where the player",
                "controls the Millennium Falcon in space to dodge enemy",
                "bullets and strives to destroy enemy warships",
                ""
            }
        };

        glColor3f(1, 1, 1);
        int yOffset = 0;
        int count = 0;
        for (int i = 2; i <= 6; i ++) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glRasterPos2i(50, 130 + yOffset);
                glDrawPixels(png[i].wid, png[i].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[i].rgba);
                glDisable(GL_BLEND);

                glColor3f(0, 0, 0);
               // glRasterPos2i(50 + png[i].wid + 10, 130 + yOffset + png[i].hei /2);
                int txtOffset = 0;
                for (const auto& line : introduction[i - 2]) {
                    glRasterPos2i(50 + png[i].wid + 10,  yOffset + png[i].hei /2 + txtOffset);
                    YsGlDrawFontBitmap10x14(line.c_str());
                    txtOffset += 18;
                }
                yOffset += png[i].hei + 5;
            }

        FsSwapBuffers();
        FsSleep(10);
    }
}


int Menu::drawScores(std::vector<Score>& scores, int startX, int startY) {
    std::cout << "Number of Scores: " << scores.size() << std::endl;
    int totalScore = 0;

    std::map<std::string, std::vector<Score> > groupedScores;

    for (const auto& score : scores) {
        groupedScores[score.gameName].push_back(score);
        totalScore += score.value;
    }

    for (auto& pair : groupedScores) {
        std::sort(pair.second.begin(), pair.second.end(),compareScores);
    }

    while(true) {
        FsPollDevice();
    
        int key = FsInkey();
        if (FSKEY_ESC == key) {
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawBackground(0);

        glColor3f(1, 1, 1);
        int yOffset = 0;
        int count = 0;
      
        
        for (const auto& pair : groupedScores) {
            int printCount = 0;
            count ++;
            DrawText(startX + count / 4 * 400, startY + yOffset, pair.first);
            yOffset += 30;

            for (const auto& score : pair.second) {
                printCount ++;
                std::string scoreText =  std::to_string(score.value);
                DrawText(startY + count / 4 * 400, startY + yOffset, scoreText);
                yOffset += 30;
                if (printCount == 3) {
                    break;
                }
            }
            if (printCount < 3) {
                for (int i = 0; i < 3 - printCount; ++i) {
                    DrawText(startX + count / 4 * 400, startY + yOffset, "0");
                    yOffset += 30;
                }
            }
            std::cout << std::endl;
            if(count == 3) {
                yOffset = 0;
            }
            else {
                yOffset += 30;
            }
    
        }
        FsSwapBuffers();
        FsSleep(10);
    }
    
    
    return totalScore;
}

void Menu::DrawPointCount(int totalScore)
{
    glRasterPos2i(600, 90);
//    char pointstxt[256];
//    std::sprintf(pointstxt, "%d", points);
    YsGlDrawFontBitmap16x20("Points: ");

    std::string countString = std::to_string(totalScore);
    glRasterPos2i(720, 90);
    YsGlDrawFontBitmap16x20(countString.c_str());
}

int main(void)
{
    
    int totalScore = 0;
    
    FsOpenWindow(0, 0, 800, 600, 1);
    FsChangeToProgramDir();
    vector<Score> scores = readScoresFromFile();

    Menu menu;
    menu.Initialize();
    State appState = IN_MENU;
    
    bool h1 = 0, h2 = 0, h3 = 0, h4 = 0, h5 = 0;
    int px1, px2, px3, px4, px5, px6, px7, px8;
    int py1, py2, py3, py4, py5, py6, py7, py8;

    px1 = 170, px2 = 430, px3 = 195, px4 = 456, px5 = 300, px6 = 20, px7 = 645, px8 = 50;
    py1 = 300, py2 = 325, py3 = 405, py4 = 425, py5 = 220, py6 = 525, py7 = 55, py8 = 500;

    
    while(true)
    {
        
        FsPollDevice();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto key = FsInkey();

        int lb, mb, rb, mx, my;
        int evt = FsGetMouseEvent(lb, mb, rb, mx, my);

        if (FSMOUSEEVENT_MOVE == FsGetMouseEvent(lb, mb, rb, mx, my))
        {
        }


        if (mx >= px1 && mx <= px1 + 163 && my >= py1 - 75 && my <= py1)
        {
            h1 = 1;
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                menu.LoadingGame();
                int score = run();
                if(score == -1)
                    score = 0;
                Score newScore = {score, "gomoku"};
                updateScores(scores, newScore);
                writeScoresToFile(scores);
                appState = IN_GAME;
                h1 = 0;
            }
        }
        else if (mx >= px2 && mx <= px2 + 163 && my >= py2 - 75 && my <= py2)
        {
            h2 = 1;
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                menu.LoadingGame();
//                runFishing();
//                appState = IN_GAME;
//                h2 = 0;
                int score = runFishing();
                
                if(score == -1)
                    score = 0;
                Score newScore = {score, "fishing"};
                updateScores(scores, newScore);
                writeScoresToFile(scores);
                appState = IN_GAME;
                h2 = 0;
                
            }
        }
        else if (mx >= px3 && mx <= px3 + 163 && my >= py3 - 75 && my <= py3)
        {
            h3 = 1;
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                menu.LoadingGame();
                int score =  runGoldMiner();
                Score newScore = {score, "goldMiner"};
                updateScores(scores, newScore);
                writeScoresToFile(scores);
                appState = IN_GAME;
                h3 = 0;
            }
        }
        else if (mx >= px4 && mx <= px4 + 163 && my >= py4 - 75 && my <= py4)
        {
            h4 = 1;
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                menu.LoadingGame();
                int score = runBreakBricks();
                Score newScore = {score, "breakbricks"};
                updateScores(scores, newScore);
                writeScoresToFile(scores);
                appState = IN_GAME;
                h4 = 0;
            }
        }
        else if (mx >= px5 && mx <= px5 + 163 && my >= py5 - 75 && my <= py5)
        {
            h5 = 1;
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                menu.LoadingGame();
                int score = runStarWar();
                Score newScore = {score, "starWar"};
                updateScores(scores, newScore);
                writeScoresToFile(scores);
                appState = IN_GAME;
                h5 = 0;
            }
        }
        else if (mx >= px7 && mx <= px7 + 150 && my >= py7 - 40 && my <= py7)
        {
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                menu.drawTutorial();
                appState = IN_GAME;
                printf("Tutorial");
            }
        }
        else if (mx >= px8 && mx <= px8 + 150 && my >= py8 - 60 && my <= py8)
        {
            if (FSMOUSEEVENT_LBUTTONDOWN == evt)
            {
                totalScore = menu.drawScores(scores, 50, 50);
                appState = IN_GAME;
                printf("rank");
            }
        }
        else
        {
            h1 = 0, h2 = 0, h3 = 0, h4 = 0, h5 = 0;
        }
        
       
        if (FSKEY_ESC == key) {
            if (appState == IN_GAME) {
                appState = IN_MENU;
            } else if (appState == IN_MENU) {
                break;
            }
        }
    
        menu.drawBackground(15);
        menu.drawTitle();
        menu.drawGame1(px1, py1, h1);
        menu.drawGame2(px2, py2, h2);
        menu.drawGame3(px3, py3, h3);
        menu.drawGame4(px4, py4, h4);
        menu.drawGame5(px5, py5, h5);
        menu.drawTutorialButton(px7, py7);
        menu.drawRank(px8, py8);
        menu.drawRover();
        menu.DrawPointCount(totalScore);
        FsSwapBuffers();
        FsSleep(10);
    }

    return 0;
}

