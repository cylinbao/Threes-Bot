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

#define debug1

typedef enum{
	WHITE = 0,
	GRAY,
	BLACK
} color;

class tile{
public:
	tile() {};
	tile(int arg1, int arg2) {
		value = arg1; index = arg2; 
	}
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

bool findPath(int source, int target, std::vector<dir_e> &steps, Grid &myGrid){
	bool existPath;
	std::stack<int> pathIdx;
	Grid nowGrid, testGrid[4];
	color tileSta[4][4];
	int i, j, nowIdx, posX, posY;
	bool canMove;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			tileSta[i][j] = WHITE;

	//run dfs
	pathIdx.push(source);
	tileSta[source%4][source/4] = GRAY;
	existPath = false;
	while(!pathIdx.empty()){
		nowIdx = pathIdx.top();
		posY = nowIdx / 4; 
		posX = nowIdx % 4;
		if(nowIdx == target){
			existPath = true;
			break;
		}

		nowGrid = myGrid;
		for(i=0; i<steps.size(); i++)
			nowGrid.shift(steps[i]);	
		
		for(i=0; i<4; i++)
			testGrid[i] = nowGrid;

		canMove = false;
		if(((posX - 1) > -1) && (tileSta[posX - 1][posY] == WHITE) 
			 && (testGrid[1].shift(LEFT))){
			steps.push_back(LEFT);
			pathIdx.push(nowIdx-1);
			canMove = true;
			tileSta[posX - 1][posY] = GRAY;
		}
		else if(((posY + 1) < 4) && (tileSta[posX][posY + 1] == WHITE) 
						&& (testGrid[2].shift(DOWN))){
			steps.push_back(DOWN);
			pathIdx.push(nowIdx+4);
			canMove = true;
			tileSta[posX][posY + 1] = GRAY;
		}
		else if(((posX + 1) < 4) && (tileSta[posX + 1][posY] == WHITE) 
						&& (testGrid[3].shift(RIGHT))){
			steps.push_back(RIGHT);
			pathIdx.push(nowIdx+1);
			canMove = true;
			tileSta[posX + 1][posY] = GRAY;
		}
		else if(((posY - 1) > -1) && (tileSta[posX][posY - 1] == WHITE) 
						&& (testGrid[4].shift(UP))){
			steps.push_back(UP);
			pathIdx.push(nowIdx-4);
			canMove = true;
			tileSta[posX][posY - 1] = GRAY;
		}

		if(!canMove){
			tileSta[posX][posY] = BLACK;
			pathIdx.pop();
			if(!steps.empty())
				steps.pop_back();
		}
	}

	return existPath;
}

struct myComparision{
  bool operator() (const tile& lhs, const tile& rhs) {
    return lhs.value < rhs.value;
  }
};

std::vector<dir_e> getSteps(Game &myGame){
	Grid myGrid;
  std::vector<dir_e> steps;
  std::priority_queue<tile, std::vector<tile>, myComparision> p_q;
	dir_e dir;
  int i, value1, index1, value2, index2;
	bool existPath;

	myGame.getCurrentGrid(myGrid);

  for(i=0; i < 16; i++){
		tile tempTile;
		tempTile.value = myGrid[i];
		tempTile.index = i;
    p_q.push(tempTile);
  }

	value1 = p_q.top().value;	
	index1 = p_q.top().index;	
	existPath = false;
	gotoXY(0,40);
	printf("p_q.size():%d\n ", p_q.size());
	while((value1 > 0) && (!p_q.empty())){
		p_q.pop();

		value2 = p_q.top().value;	
		index2 = p_q.top().index;	

		if(value1 == value2)
			existPath = findPath(index1, index2, steps, myGrid);

		if(existPath)
			break;
		
		value1 = value2;
		index1 = index2;
	}
	if(!existPath){
		while((dir = getRandDir()) == INVALID);
		steps.clear();
		steps.push_back(dir);
		//gotoXY(0,15);
		//printf("Random direction\n");
	}
	
  return steps;
}

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
    for(int i = 0;i < n;i++){    
        isGameOver = false;
        while(!isGameOver){
            //while((dir = getDirFromKeyboard()) == INVALID);
            //while((dir = getRandDir()) == INVALID);
					steps = getSteps(myGame);
					#ifdef debug1
					gotoXY(0,15);
					std::cout << "Get steps:";
					for(unsigned int j=0; j < steps.size(); j++){
						std::cout << ' ' << steps[j];
					}
					std::cout << std::endl;
					#endif
					// insert those steps
					for(unsigned int j=0; j < steps.size(); j++){
						dir = steps[j];
						gotoXY(5,10);
						std::cout<<dirToStr(dir);
						myGame.printGrid(5,2);

						// return false when fail to insert
						myGame.insertDirection(dir);

						gotoXY(50,0);
						std::cout<<myGame.getHint();
						isGameOver = myGame.isGameOver(score);
						myGame.printGrid(35,2);
						if(isGameOver)
							break;

						int temp;
						std::cin >> temp;
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
