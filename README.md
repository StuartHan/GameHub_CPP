# GameHub_CPP
This is the Final Project of CMU 24780 created by Team Water Army.
## User Guide
### Menu
When you open the game, you will see this menu. Press ESC to exit the program. The menu page includes a Tutorial, a Rank Board, and 5 Games.

<img width="562" alt="Screenshot 2023-12-22 at 08 23 16" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/c57a3a1e-59cd-49bf-bbd1-bf4222bb40f7">

### Tutorial
When you click the Tutorial on the Menu, you can see more detailed introductions of each game.

<img width="544" alt="Screenshot 2023-12-22 at 08 24 17" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/0cb09c92-122d-4d09-839f-1fedc11d4896">

### Games
Just click games to enter, press ESC to exit from games to the menu.

### Game - Gomoku (5 In A Row)
The play black in the game and the computer will be the white opponent. Click the position to place pieces. The one who places five pieces of chess in a continuous line.

<img width="321" alt="Screenshot 2023-12-22 at 08 26 10" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/fbd71e89-8f6c-47de-8b12-258b12719b4a">

### Game - Fishing
Users can press left and right keys to control the movement of the boat. Press space to release the hook. Try to get as many fish as possible and avoid black bombs in 60 seconds.

<img width="323" alt="Screenshot 2023-12-22 at 08 26 41" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/93f115b0-e9d2-429f-a8e8-79dc3ae6cbfe">

### Game - Gold Miner
The player is located at the top of the screen as a dark blue square. Your hook is swinging at a certain speed. Press space to release the hook to mine diamonds, gold, and rocks. Try to get as many points as possible in 60 seconds.

<img width="327" alt="Screenshot 2023-12-22 at 08 26 57" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/879cc236-61bf-472b-abe5-edf61574b39b">

### Game - Bricks Breaker
The user uses the ball to break the bricks. Press left and key to move the When the ball hits a brick, a wall, or the stick, it rebound based on the law of reflection. If the balls fall down.

<img width="324" alt="Screenshot 2023-12-22 at 08 28 20" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/916b3092-56fd-4e89-996d-746805e4ec25">

### Game - Star War
A shooting game in which the player controls the Millennium Falcon in space to defend enemies by destroying their warships. Press up/down/left/right keys to move. Press space to shoot. Try to avoid enemies’ attack and crashing. Watch out your health level.

<img width="322" alt="Screenshot 2023-12-22 at 08 28 36" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/757686d7-45d0-46cf-9f57-ed8c83dd3297">

### Score Board
After each game, the score is passed back to Menu. The menu records and orders them and rank them on the Score Board. You may press ESC to exit from board to menu.

<img width="324" alt="Screenshot 2023-12-22 at 08 28 53" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/d90e658d-5651-4648-9ce8-744e76eea660">

## List of external toolkits:
- Cocoa framework
- openGLframework
- GLUT framework
- AVFoundation framework
  
## Compile instruction using X-Code (Recommended):
- Create an X-Code project and remove AppDelegate.h, AppDelegate.c, main.c, and Sand Box.
- Copy source files into the project. If X-Code asks “Would you like to configure an Objective-C bridging header?” Select No.
- Go to Build Phase of project settings.

<img width="602" alt="Screenshot 2023-12-22 at 08 31 14" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/ec39f2b1-9476-4ea7-abc6-44fbecffab3e">

- Replace items in Link Binary With Libraries with these 3 items.

<img width="580" alt="Screenshot 2023-12-22 at 08 31 33" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/7a773d8c-1f89-4ac2-b4ea-aa17ddb34b1c">

- Make sure Compile Sources includes these 14 items.

<img width="575" alt="Screenshot 2023-12-22 at 08 31 52" src="https://github.com/StuartHan/GameHub_CPP/assets/56141315/9c7442ff-de0a-4d5b-a970-ba7ccaf18aae">

- Enjoy our games!

## Compile instruction using Terminal (Not Recommended):
PS: This method does not guarantee successful keyboard interaction.
- Running the compilation command requires the use of the external libraries listed above. If any of the libraries has not been installed, then the compilation process will not proceed.
- G++ Command for compiling C++ source files ( with C++ 11), which also specifies the use of external frameworks that are use for graphic and user interface:
  ```
  g++ -std=c++11 -c breakbricks.cpp fishing.cpp fssimplewindowcpp.cpp GoldMiner.cpp gomoku.cpp Menu.cpp starwar.cpp textstring.cpp ysglfontdata.c yspng.cpp yssimplesound.cpp -framework Cocoa -framework OpenGL -framework GLUT
  ```
- Clang Command for compiling Objective-C source Files:
  ```
  clang -framework Cocoa -framework OpenGL -framework GLUT -c fssimplewindowobjc.m yssimplesound_macosx_objc.m
  ```
- G++ command for linking all the object files together and creating a executable file named “final_project_exec”:
  ```
  g++ -o final_project_exec -framework Cocoa -framework OpenGL -framework GLUT -framework AVFoundation breakbricks.o fishing.o fssimplewindowcpp.o GoldMiner.o gomoku.o Menu.o starwar.o textstring.o ysglfontdata.o yspng.o yssimplesound.o fssimplewindowobjc.o yssimplesound_macosx_objc.o
  ```
- Execute the created file “final_project_exec”
  ```
  ./final_project_exec
  ```
- Enjoy our games!
