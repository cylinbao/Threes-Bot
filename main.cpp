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
	color tileSta[16] = {WHITE};
	int i, nowIdx;
	bool canMove;

	//run dfs
	pathIdx.push(source);
	tileSta[source] = GRAY;
	existPath = false;
	while(pathIdx.size() > 0){
		nowIdx = pathIdx.top();
		if(nowIdx == target){
			existPath = true;
			break;
		}
		tileSta[nowIdx] = GRAY;

		nowGrid = myGrid;
		for(i=0; i<steps.size(); i++)
			nowGrid.shift(steps[i]);	
		
		for(i=0; i<4; i++)
			testGrid[i] = nowGrid;

		canMove = false;
		if((tileSta[nowIdx - 1] == WHITE) && (testGrid[1].shift(LEFT))){
			steps.push_back(LEFT);
			pathIdx.push(nowIdx-1);
			tileSta[nowIdx-1] = GRAY;
			canMove = true;
		}
		else if((tileSta[nowIdx + 4] == WHITE) && (testGrid[2].shift(DOWN))){
			steps.push_back(DOWN);
			pathIdx.push(nowIdx+4);
			tileSta[nowIdx+4] = GRAY;
			canMove = true;
		}
		else if((tileSta[nowIdx + 1] == WHITE) && (testGrid[3].shift(RIGHT))){
			steps.push_back(RIGHT);
			pathIdx.push(nowIdx+1);
			tileSta[nowIdx+1] = GRAY;
			canMove = true;
		}
		else if((tileSta[nowIdx - 4] == WHITE) && (testGrid[4].shift(UP))){
			steps.push_back(UP);
			pathIdx.push(nowIdx-4);
			tileSta[nowIdx-4] = GRAY;
			canMove = true;
		}

		if(!canMove){
			tileSta[nowIdx] = BLACK;
			pathIdx.pop();
			steps.pop_back();
		}
	}

	return existPath;
}

struct myComparision{
  bool operator() (const tile lhs, const tile rhs) {
    return lhs.value < rhs.value;
  }
};

std::vector<dir_e> getSteps(Game &myGame){
	Grid myGrid;
  std::vector<dir_e> steps;
  std::priority_queue<tile, std::vector<tile>, myComparision> p_q;
	#ifdef debug1
  std::priority_queue<tile, std::vector<tile>, myComparision> p_q2;
	#endif
  tile tile1, tile2, *p_tile;
	dir_e dir;
  int i;
	bool existPath;

	myGame.getCurrentGrid(myGrid);

  for(i=0; i < 16; i++){
    p_tile = new tile (myGrid[i], i);
    p_q.push(*p_tile);
  }
	#ifdef debug1
	p_q2 = p_q;
	gotoXY(0,16);
	while(!p_q2.empty()){
		printf("index: %d, value: %d\n", p_q2.top().index, p_q2.top().value);
		p_q2.pop();
	}
	#endif

	tile1 = p_q.top();	
	existPath = false;
	while((tile1.value) > 0){
		p_q.pop();
		tile2 = p_q.top();

		if((tile1.value) == (tile2.value)){
			#ifdef debug1
			gotoXY(0,13);
			std::cout << "Try to combine index: " << tile1.index << " and " 
				<< tile2.index;
			#endif
			existPath = findPath(tile1.index, tile2.index, steps, myGrid);
		}

		if(existPath)
			break;

		tile1 = tile2;
	}
	if(!existPath){
		#ifdef debug1
		gotoXY(0,14);
		std::cout << "No path, use random" << std::endl; 
		#endif
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
