#include "Threes.h"
#include <ctime>
#ifdef _WIN32
#include <conio.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class tile{
public:
	tile(int arg1, int arg2) {value = arg1; index = arg2;}
	int value, index;
};

int getch(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
#endif

const char* dirToStr(dir_e dir){
    if(dir == LEFT)   return "LEFT   ";
    if(dir == DOWN)   return "DOWN   ";
    if(dir == RIGHT)  return "RIGHT  ";
    if(dir == UP)     return "UP     ";
    return "INVALID";
}

// Exmaple1. Original game played with AWSD keys
dir_e getDirFromKeyboard(){
    int dir;
    dir = getch();
    
    if(dir == 'A' || dir == 'a')  return LEFT;
    if(dir == 'S' || dir == 's')  return DOWN;
    if(dir == 'D' || dir == 'd')  return RIGHT;
    if(dir == 'W' || dir == 'w')  return UP;
    return INVALID;
}

// Exmaple2. Random direction inserting bot
dir_e getRandDir(){
    int dir = rand()%4;
    
    if(dir == 0)  return LEFT;
    if(dir == 1)  return DOWN;
    if(dir == 2)  return RIGHT;
    if(dir == 3)  return UP;
    return INVALID;
}

std::vector<dir_e> getSteps(Grid myGrid);
void PlayNRounds(int n){
#ifdef _WIN32
    system("cls");
#elif defined(__linux__)
    system("clear");
#endif
    int score;
    Game myGame;
    bool isGameOver;
    dir_e dir;
		std::vector<dir_e> steps;

    gotoXY(5,0);
    std::cout<<"Previous";
    gotoXY(35,0);
    std::cout<<"Current (Hint: "<<myGame.getHint()<<")";
    myGame.printGrid(35,2);

    if(myGame.isGameOver(score))  myGame.reset();
    Grid myGrid;
    for(int i = 0;i < n;i++){    
        isGameOver = false;
        while(!isGameOver){
            //while((dir = getDirFromKeyboard()) == INVALID);
            //while((dir = getRandDir()) == INVALID);
					steps = getSteps(myGrid);
					for(int j=0; j < steps.size(); j++){
						dir = steps[j];
						gotoXY(5,10);
						std::cout<<dirToStr(dir);
						myGame.printGrid(5,2);

						myGame.insertDirection(dir);
						gotoXY(50,0);
						std::cout<<myGame.getHint();
						isGameOver = myGame.isGameOver(score);
						myGame.printGrid(35,2);
						//int temp;
						//std::cin >> temp;
					}
       }
        myGame.printGrid(35,2);
        myGame.reset();
        gotoXY(0,15); 
        printf("  Round:    %d      \n", i+1);
        printf("  Score:    %d      \n", score);
 
    }
}

int main(int argc, char* argv[]){
    // Note: API function calls performed by any 'Game' object effects the same set of static class members,
    // so even through the 2 following function calls use different 'Game' objects, the same game continues
    PlayNRounds(50);
    PlayNRounds(50);
    return 0;
}

struct myComparision{
  bool operator() (const tile lhs, const tile rhs) {
    return lhs.value < rhs.value;
  }
};

std::vector<dir_e> getSteps(Grid myGrid){
  std::vector<dir_e> steps;
  std::priority_queue<tile, std::vector<tile>, myComparision> p_q;
  tile *tempTile;
  int i;

  for(i=0; i < 16; i++){
    tempTile = new tile (myGrid[i], i);
    p_q.push(*tempTile);
  }
	/*
  gotoXY(0,25);
  for(i=0; i < 16; i++){
    printf("value: %d, index: %d\n", p_q.top().value, p_q.top().index);
    p_q.pop();
  }
	*/
  steps.push_back(LEFT);
  steps.push_back(DOWN);
  steps.push_back(RIGHT);
  steps.push_back(UP);

  return steps;
}
