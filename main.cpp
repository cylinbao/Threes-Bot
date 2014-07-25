#include "Threes.h"
#include <ctime>
#ifdef _WIN32
#include <conio.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <math.h>
#include <queue>
#define NONE 0
#define UP_DOWN 1
#define RIGHT_LEFT 2
#define PASS 1
#define NOT_PASS 2




using namespace std;

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

class tile{
	public: 
		int x;
		int y;
		int val;
};

class step{
	public:
		int x; 
		int y;
		int data;
		int tot_move;
		vector<dir_e> dir;
};

struct cmp
{
	bool operator()(const step* l, const step* r)
	{
		return l->tot_move > r->tot_move;
	}
};

tile Tile[4][4];
map<int, vector<tile*> > dis_table;
map<int, vector<tile*> >::iterator it_source, it_target, it_tmp;
int up_down, right_left, move, Round;
Game myGame;
bool PATH_FOUND;
step *Step;


bool FIND_PATH(tile &S, tile &T, Grid &pre_myGrid, int dir_t, int status, step &Step)
{
	Grid myGrid(pre_myGrid);
	up_down = T.x - S.x;
	right_left = T.y - S.y;
	dir_e dir;
//	if(S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3 && myGrid(0, 0) == 3){
//	if(Round == 3){
		gotoXY(40, 14);
		//scanf("%d", &dir);
		gotoXY(40, 15);
		printf("Source: (%d, %d) VAL: %d \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
		gotoXY(40, 16);
		printf("Target: (%d, %d) VAL: %d \n", T.x, T.y, pre_myGrid(S.x, S.y));
		gotoXY(40, 17);
		printf("Pre dir: %d", dir_t);
		pre_myGrid.print(40, 20);
		myGrid.print(40, 30);
	//}
	if((abs(up_down) > 0 && (dir_t == UP_DOWN || dir_t == NONE))){
		if(up_down < 0)	{myGrid.shift(UP); move = -1; dir = UP;}
		else {myGrid.shift(DOWN); move = 1; dir = DOWN;}
		if(pre_myGrid(T.x, T.y) + pre_myGrid(S.x, S.y) == myGrid(T.x, T.y) && (abs(up_down) == 1 && right_left == 0)){
			if(S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3){
				gotoXY(0, 20);
				printf("A\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x + move, S.y, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x + move, T.y, myGrid(T.x + move, T.y));
			}
			Step.tot_move++;
			Step.dir.push_back(dir);
			PATH_FOUND = true;
			return true;
		}
		if(pre_myGrid(S.x, S.y) == myGrid(S.x, S.y) && pre_myGrid(T.x, T.y) == myGrid(T.x, T.y)){
//			printf("B");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 20);
				printf("B\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x + move, S.y, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x + move, T.y, myGrid(T.x + move, T.y));
			}
			if(status == NOT_PASS || right_left == 0){
				PATH_FOUND = false;
				return false;
			}
			else 
				FIND_PATH(S, T, pre_myGrid, RIGHT_LEFT, NOT_PASS, Step);
		}
		else if(pre_myGrid(S.x, S.y) == myGrid(S.x, S.y) && pre_myGrid(T.x, T.y) == myGrid(T.x + move, T.y)){
//			printf("C");
			if(S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3){
				gotoXY(0, 20);
				printf("C\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x + move, S.y, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x + move, T.y, myGrid(T.x + move, T.y));
			}
			tile T_next;
			T_next.x = T.x + move;
			T_next.y = T.y;
			T_next.val = T.val;
			Step.tot_move++;
			Step.dir.push_back(dir);
			FIND_PATH(S, T_next, myGrid, UP_DOWN, PASS, Step);	
		}
		else if(pre_myGrid(S.x, S.y) == myGrid(S.x + move, S.y) && pre_myGrid(T.x, T.y) == myGrid(T.x, T.y)){
			if((S.x == 1 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 20);
				printf("D\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x + move, S.y, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x + move, T.y, myGrid(T.x + move, T.y));
		//		pre_myGrid.print(40, 20);
		//		myGrid.print(40, 30);
			}
//			printf("D");
			tile S_next;
			S_next.x = S.x + move;
			S_next.y = S.y;
			S_next.val = S.val;
			Step.tot_move++;
			Step.dir.push_back(dir);
			if(S_next.x - T.x == 0)
				FIND_PATH(S_next, T, myGrid, RIGHT_LEFT, PASS, Step);	
			else
				FIND_PATH(S_next, T, myGrid, UP_DOWN, PASS, Step);	
		}
		else if(pre_myGrid(S.x, S.y) == myGrid(S.x + move, S.y) && pre_myGrid(T.x, T.y) == myGrid(T.x + move, T.y)){
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 20);
				printf("E\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x + move, S.y, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x + move, T.y, myGrid(T.x + move, T.y));
			}
//			printf("E");
			tile S_next;
			tile T_next;
			S_next.x = S.x + move;
			S_next.y = S.y;
			S_next.val = S.val;
			T_next.x = T.x + move;
			T_next.y = T.y;
			T_next.val = T.val;
			Step.tot_move++;
			Step.dir.push_back(dir);
			FIND_PATH(S_next, T_next, myGrid, UP_DOWN, PASS, Step);	
		}
		else{	
//			printf("F");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 20);
				printf("F\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x + move, S.y, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x + move, T.y, myGrid(T.x + move, T.y));
			}
			if(status == NOT_PASS || right_left == 0){
				PATH_FOUND = false;
				return false;
			}
			else FIND_PATH(S, T, pre_myGrid, RIGHT_LEFT, NOT_PASS, Step);
		}
	}
	else if((abs(right_left > 0) && (dir_t == RIGHT_LEFT || dir_t == NONE))){
		if(right_left > 0) {myGrid.shift(RIGHT); move = 1; dir = RIGHT;}
		else {myGrid.shift(LEFT); move = -1; dir = LEFT;}
		if(pre_myGrid(T.x, T.y) + pre_myGrid(S.x, S.y) == myGrid(T.x, T.y) && (abs(right_left) == 1 && up_down == 0)){
	//		printf("G");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 29);
				printf("G\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y + move, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y + move, myGrid(T.x + move, T.y));
			}
			Step.tot_move++;
			Step.dir.push_back(dir);
			PATH_FOUND = true;
			return true;
		}
		if(pre_myGrid(S.x, S.y) == myGrid(S.x, S.y) && pre_myGrid(T.x, T.y) == myGrid(T.x, T.y)){
	//		printf("H");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 29);
				printf("H\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y + move, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y + move, myGrid(T.x + move, T.y));
			}
			if(status == NOT_PASS || up_down == 0){
				PATH_FOUND = false;
				return false;
			}
			else FIND_PATH(S, T, pre_myGrid, UP_DOWN, NOT_PASS, Step);
		}
		else if(pre_myGrid(S.x, S.y) == myGrid(S.x, S.y) && pre_myGrid(T.x, T.y) == myGrid(T.x, T.y + move)){
	//		printf("I");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 29);
				printf("I\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y + move, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y + move, myGrid(T.x + move, T.y));
			}
			tile T_next;
			T_next.x = T.x;
			T_next.y = T.y + move;
			T_next.val = T.val;
			Step.tot_move++;
			Step.dir.push_back(dir);
			FIND_PATH(S, T_next, myGrid, RIGHT_LEFT, PASS, Step);	
		}
		else if(pre_myGrid(S.x, S.y) == myGrid(S.x, S.y + move) && pre_myGrid(T.x, T.y) == myGrid(T.x, T.y)){
	//		printf("J");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 29);
				printf("J\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y + move, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y + move, myGrid(T.x + move, T.y));
			}
			tile S_next;
			S_next.x = S.x;
			S_next.y = S.y + move;
			S_next.val = S.val;
			Step.tot_move++;
			Step.dir.push_back(dir);
			if(S_next.y - T.y == 0)
				FIND_PATH(S_next, T, myGrid, UP_DOWN, PASS, Step);	
			else
				FIND_PATH(S_next, T, myGrid, RIGHT_LEFT, PASS, Step);	
		}
		else if(pre_myGrid(S.x, S.y) == myGrid(S.x, S.y + move) && pre_myGrid(T.x, T.y) == myGrid(T.x, T.y + move)){
	//		printf("K");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 29);
				printf("K\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y + move, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y + move, myGrid(T.x + move, T.y));
			}
			tile S_next;
			tile T_next;
			S_next.x = S.x;
			S_next.y = S.y + move;
			S_next.val = S.val;
			T_next.x = T.x;
			T_next.y = T.y + move;
			T_next.val = T.val;
			Step.tot_move++;
			Step.dir.push_back(dir);
			FIND_PATH(S_next, T_next, myGrid, RIGHT_LEFT, PASS, Step);	
		}
		else{
	//		printf("L");
			if((S.x == 0 && S.y ==2 && T.x == 0 && T.y ==3)){
				gotoXY(0, 29);
				printf("L\n");
				printf("Source: (%d, %d) VAL: %d DIR: %d  \n", S.x, S.y, pre_myGrid(S.x, S.y), dir);
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, pre_myGrid(S.x, S.y));
				printf("After: \n");
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y, myGrid(S.x, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y, myGrid(T.x, T.y));
				printf("Source: (%d, %d) VAL: %d  \n", S.x, S.y + move, myGrid(S.x + move, S.y));
				printf("Target: (%d, %d) VAL: %d  \n", T.x, T.y + move, myGrid(T.x + move, T.y));
			}
			if(status == NOT_PASS || up_down == 0){
				PATH_FOUND = false;
				return  false;
			}
			else FIND_PATH(S, T, pre_myGrid, UP_DOWN, NOT_PASS, Step);
		}
	}
	if(PATH_FOUND == true)
		return true;
	else
		return false;
}


void PlayNRounds(int n){
#ifdef _WIN32
	system("cls");
#elif defined(__linux__)
	system("clear");
#endif
	int score;

	bool isGameOver;
	for(int i = 0; i <= 16; i++){
		vector<tile*> tmp;
		if(i < 3){
			dis_table[i] = tmp;
			continue;
		}
		dis_table[3*(int)pow(2, i - 3)] = tmp;
	}

	gotoXY(5,0);
	std::cout<<"Previous";
	gotoXY(35,0);
	std::cout<<"Current (Hint: "<<myGame.getHint()<<")";
	myGame.printGrid(35,2);

	if(myGame.isGameOver(score))  myGame.reset();

	Grid myGrid;
	for(Round = 0;Round < n;Round++){    
		isGameOver = false;
		while(!isGameOver){

			//while((dir = getDirFromKeyboard()) == INVALID);
			//while((dir = getRandDir()) == INVALID);
			/*gotoXY(5,10);
			std::cout<<dirToStr(dir);
			myGame.printGrid(5,2);

			myGame.insertDirection(dir);
			gotoXY(50,0);
			std::cout<<myGame.getHint();
			isGameOver = myGame.isGameOver(score);
			myGame.printGrid(35,2);*/
			myGame.getCurrentGrid(myGrid);

			/*  Initialize Each Run   */
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					Tile[i][j].x = i;
					Tile[i][j].y = j;
					Tile[i][j].val = myGrid(i, j);
					if(Tile[i][j].val != 0)
						dis_table[Tile[i][j].val].push_back(&Tile[i][j]);
				}
			}
			it_source = dis_table.end();
			priority_queue<step*, vector<step*>, cmp> pq_step;
			for(it_source--;it_source->first != 2;it_source--){
				if(it_source->second.size()>=2){
					it_target = it_source;
					for(int i = 0; i < (int)it_source->second.size(); i++){
						for(int j = 0; j < (int)it_target->second.size(); j++){
							if(i != j){
								gotoXY(0, 13);
					/*			if(it_target->second[j]->x == 1 && it_target->second[j]->y == 3 && it_source->second[i]->x == 3 && it_source->second[i]->y == 2){
									printf("Source: (%d, %d): VAL %d\n", it_source->second[i]->x, it_source->second[i]->y, it_source->second[i]->val);
									printf("Target: (%d, %d): VAL %d\n", it_target->second[j]->x, it_target->second[j]->y, it_target->second[j]->val);
								}*/
								PATH_FOUND = false;
								step *Step = new step;
								Step->tot_move = 0;
								if(FIND_PATH(*it_source->second[i], *it_target->second[j], myGrid, NONE, PASS,* Step)){
									pq_step.push(Step);
								}
							}
						}
					}
					if(pq_step.size() > 0){
						gotoXY(0, 40);
						printf("PQ_SIZE: %d\n", pq_step.size());
						printf("Step Size in PQ: %d\n", pq_step.top()->dir.size());
						break;
					}
				}
				else if(it_source->first == 2 && it_source->second.size() > 0 && dis_table[1].size() > 0){
					it_target = dis_table.begin();
					it_target++;
					it_tmp = it_source;
					for(int i = 0; i < (int)it_source->second.size(); i++){
						for(int j = 0; j < (int)it_target->second.size(); j++){
							PATH_FOUND = false;
							step Step;
							Step.tot_move = 0;
							if(FIND_PATH(Tile[it_source->second[i]->x][it_source->second[i]->y], Tile[it_target->second[j]->x][it_target->second[j]->y], myGrid, NONE, PASS, Step))
								pq_step.push(&Step);

						}
					}
					it_target++;
					it_source--;
					for(int i = 0; i < (int)it_source->second.size(); i++){
						for(int j = 0; j < (int)it_target->second.size(); j++){
							PATH_FOUND = false;
							step Step;
							Step.tot_move = 0;
							if(FIND_PATH(Tile[it_source->second[i]->x][it_source->second[i]->y], Tile[it_target->second[j]->x][it_target->second[j]->y], myGrid, NONE, PASS, Step))
								pq_step.push(&Step);

						}
					}
					if(pq_step.size() > 0){
						it_source = it_tmp;
						gotoXY(0, 40);
						printf("PQ_SIZE: %d\n", pq_step.size());
						printf("Step Size in PQ: %d\n", pq_step.top()->dir.size());
						break;
					}
					it_source = it_tmp;
				}
			}
		//	printf("FINISH\n");
			if(pq_step.size() > 0){
				for(int i = 0; i < (int)pq_step.top()->dir.size(); i++){
					gotoXY(5,10);
					std::cout<<dirToStr(pq_step.top()->dir[i]);
					myGame.printGrid(5,2);

					myGame.insertDirection(pq_step.top()->dir[i]);
					gotoXY(50,0);
					std::cout<<myGame.getHint();
					isGameOver = myGame.isGameOver(score);
					myGame.printGrid(35,2);
				}
				while(!pq_step.empty()) pq_step.pop();
			}else{
				gotoXY(5, 30);
				printf("b");
				gotoXY(5,10);
				std::cout<<dirToStr(getRandDir());
				myGame.printGrid(5,2);

				myGame.insertDirection(getRandDir());
				gotoXY(50,0);
				std::cout<<myGame.getHint();
				isGameOver = myGame.isGameOver(score);
				myGame.printGrid(35,2);
				
			}
			for(it_source = dis_table.begin(); it_source != dis_table.end(); it_source++)
				it_source->second.clear();
		}
		myGame.printGrid(35,2);
		if(Round < n - 1)  myGame.reset();
		gotoXY(40,11); 
		printf("  Round:    %d      \n", Round+1);
		gotoXY(40,12); 
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

