#include<stdio.h>
#include <time.h>
#include<stdlib.h>
#include<windows.h>

#define CLEAR 1
#define BLOCK 0
#define WAY 2
#define DESTINATION 3
#define MAX 100

#define DEBUG 0

/*
* DEBUG MODES:
* 0:DEBUG disabled
* 1:Just print the maps for comparison
* 2:more details concerning dfs
* 3:include details which operations with lists are concerned with, and the console may be loaded with info
* 4:full details, may have prepostrous lines of info
*/


int maze[MAX][MAX];
int visit[MAX][MAX];/*visited 1, unvisited 2*/
struct vector{
	int x;
	int y;
}solution[MAX][MAX*MAX],start,end,move[4];

/*@note: operations with lists are none of others' business but functions executing prim algorithm
*/
struct lists{
	int x;
	int y;
	struct lists *next;
};
struct lists *creatNewList();
int listLen;
void deleteList(int n);
void addList(int x, int y);
struct lists *phead,*ptail;

int sol_row,sol_col;

#if  DEBUG
void testdrawMap(int  maxLine,int Enter){
	int i,j;
	int map[MAX][MAX];
	i= 0;
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
			map[i][j] = maze[i][j];
		}
	}
	while ((solution[0][i].x != end.x  || solution[0][i].y != end.y )&& (solution[0][i].x != -1  || solution[0][i].y != -1)){ 
		map[solution[0][i].x][solution[0][i].y] = WAY;
		i++;
	}
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
			if (map[i][j] >= 2){
				printf("%c",'o');
			}
			if (map[i][j] == CLEAR){
				printf(" ");
			}
			if (map[i][j] == BLOCK){
				printf("%c",'#');
			}
		}
		printf("\n");
	}
	for(i=1;i<=Enter;i++){
		printf("\n");
	}
}

void checkList(){
	int i=0;
	struct lists *p1 = phead,*p2;
	while (p1 != NULL){
		p2 = p1;
		p1 = p1->next;
		i++;
	}
	printf("Listchecked: %d\n",i);
	if (p2 != ptail){
		printf("Ptail is wrong!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
}

void ptmap(int maxLine){
	int i,j;
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
			printf("%d ",maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

/**
* @brief: testing whether the cell is blocked within steps;
* @param [in] x,y: position of the cell
* @param [in] edge: the size of the maze

* @return 1: it is block within the steps;
* @exception 
* @note
*/
void 
int testIsolateCell(int x,int y,int edge,int step ){
	int i;
	if (step == 0){
		return 0;
	}
	for(i=0;i<4;i++){
		if (maze[x+move[i].x][y+move[i].y] == CLEAR && x+move[i].x<edge-1 && x+move[i].x>0 && y+move[i].y<edge-1 && y+move[i].y>0){
			if (testIsolateCell(x+move[i].x,y+move[i].y,edge,step-1) == 0){
				return 0;
			}
		}
	}
	return 1; /*its blocked in several steps;*/
}
#endif 


void drawMap(int  maxLine, HANDLE handle, int Enter){
	int i,j;
	
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
#if DEBUG
printf("%d ",maze[i][j]);
#endif
			if (maze[i][j] == DESTINATION || maze[i][j] == WAY){
				SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				printf("%c%c",161,239);
			}
			if (maze[i][j] == CLEAR){
				printf("%c%c",165,254);
			}
			if (maze[i][j] == BLOCK){
				SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
				printf("%c%c",161,246);
			}
		}
		printf("\n");
	}
	for(i=1;i<=Enter;i++){
		printf("\n");
	}
}


void iniSolution(){
	int i,j,max = MAX*MAX;
	for(i = 0;i<MAX;i++){
		for(j=0;j<max;j++){
			solution[i][j].x = -1; // no solution
			solution[i][j].y = -1; // no solution
		}
	}		
	sol_row=sol_col=0;
}

/**
* @brief: initialize the data including: Block all cell, set move value, set Solutions to -1;

* @exception 
* @note
*/

void iniData(){
	int i,j;
	for(i = 0;i<MAX;i++){
		for(j=0;j<MAX;j++){
			maze[i][j] = BLOCK;
		}
	}
	move[0].x = 1;  move[0].y = 0;  //right
	move[1].x = -1; move[1].y = 0;  //left
	move[2].x = 0;  move[2].y = 1;  //up
	move[3].x = 0;  move[3].y = -1; //down
	
	iniSolution();
}

/**
* @brief: initialize the visit[][], set the boundries visited
* @param [in] edge: the size of the maze

* @exception 
* @note
*/
void iniVisit(int edge){
	int i,j;
	
	for(i = 1;i<MAX;i++){
		for(j=1;j<MAX;j++){
			visit[i][j] = 0;

		}
	}
	for(i = 0;i<edge;i++){
		visit[0][i] = 1;
		visit[i][0] = 1;
		visit[edge-1][i] = 1;
		visit[i][edge-1] = 1;
	}
}

/**
* @brief: initialize necessary data for dfs: initialize visit[][] and 
          making starting point & destination accesible by setting its & its neighbor's visit value to 0;
          
* @param [in] edge: the size of the maze

* @exception 
* @note
*/
void iniDfs(int edge){
	int x,y,x1,y1;
	int i;
	iniVisit(edge);
	visit[start.x][start.y] = 0;
	visit[end.x][end.y] = 0;
	for(i=0;i<4;i++){
		x = end.x + move[i].x ;
		y = end.y + move[i].y ;
		x1 = start.x + move[i].x ;
		y1 = start.y + move[i].y ;
		if (x>=0 && y>=0 && x<edge && y<edge && maze[x][y] != BLOCK){
			visit[x][y] = 0;
		}
		if (x1>=0 && y1>=0 && x1<edge && y1<edge && maze[x1][y1] != BLOCK){
			visit[x1][y1] = 0;
		}
	}
}

/////////find the way out///////////////////////////////////////////////////////////////////////////
void dfs(int x,int y,int edge)
{
	int i,x1,y1;
#if DEBUG >= 2
printf("In it\n");
#endif
	if (maze[x][y] == DESTINATION){
		solution[sol_row][sol_col].x  = x;
		solution[sol_row][sol_col].y  = y;
		sol_row ++;
#if  DEBUG
printf("find one!\n");
#endif 
		//drawMap(edge,2);
		return;
	}
	
	
#if DEBUG >= 3
printf("start for\n");
#endif
	for(i=0;i<4;i++){
		x1 = x + move[i].x ;
		y1 = y + move[i].y ;
#if DEBUG >= 3
printf("In for %d \n",i);
#endif
		if (x1>=0&&y1>=0&&x1<edge&&y1<edge &&visit[x1][y1] == 0 && maze[x1][y1] != BLOCK){
			solution[sol_row][sol_col].x  = x1;
			solution[sol_row][sol_col].y  = y1;
			sol_col++;
			visit[x1][y1] = 1;
#if DEBUG >= 3
printf("row = %d,col = %d\n pos.x = %d",sol_row,sol_col);
printf("nextpath found\n");
ptMap(edge,2);
#endif
			dfs(x1,y1,edge);
			visit[x1][y1] = 0;
			if (sol_col>0){
				sol_col--;
			}
		}
		
	}
#if DEBUG >=2
printf("fail,withdraw\n");
#endif
}



///////Maze Generator////////////////////////////////////////////////////////////////////////////////
void oddNumberFixer(int edge){
	int i;
	int visitmap[7][7];
	/*
	for(i = 1;i<edge-1;i++){
		srand(time(0));
		if (rand() % 2 == 0){
			maze[i][edge-2] = CLEAR;
		}
		srand(time(0));
		if (rand() % 2 == 0){
			maze[edge-2][i] = CLEAR;
		}
	}*/
	for(i=-2;i<3;i++){
		if (edge-2 != end.y){
		
			maze[end.x+i][edge-2] = CLEAR;
			maze[edge-2][end.y+i] = CLEAR;
			maze[start.x+i][edge-2] = CLEAR;
			maze[edge-2][start.y] = CLEAR;
		}
	} 
	maze[end.x][end.y] = DESTINATION;
}

/**
* @brief: prepare for prim: setting cells as clear and surrounded by walls
							initialize visit[][] & phead & ptail
* @param [in] edge: the size of the maze

* @exception 
* @note
*/
void prePrim(int edge){
	int i,j;
	for(i = 1;i<edge-1;i+=2){
		for(j=1;j<edge-1;j+=2){
			maze[i][j] = CLEAR;
		}
		
	}
	
	maze[start.x][start.y] = CLEAR;
	maze[end.x][end.y] = DESTINATION;
	/*make sure there is a way*/
		if (start.x == 0){
			maze[start.x+1][start.y] = CLEAR;
		}
		else if (start.x == edge-1){
			maze[start.x-1][start.y] = CLEAR;
		}
		else if (start.y == edge-1){
			maze[start.x][start.y-1] = CLEAR;
		}
		else if (start.y == 0){
			maze[start.x][start.y+1] = CLEAR;
		}
		else{
			;
		}

		if (end.x == 0){
			maze[end.x+1][end.y] = CLEAR;
		}
		else if (end.x == edge-1){
			maze[end.x-1][end.y] = CLEAR;
		}
		else if (end.y == edge-1){
			maze[end.x][end.y-1] = CLEAR;
		}
		else if (end.y == 0){
			maze[end.x][end.y+1] = CLEAR;
		}
#if DEBUG >= 2
ptmap(edge);
#endif
	
	iniVisit(edge);

	listLen = 0;
	phead = ptail = NULL;
}
void newList(int x,int y){
	ptail = phead = (struct lists *) malloc (sizeof(struct lists));
	phead->next = NULL;
	phead->x = x;
	phead->y = y;
	listLen = 1;
}

void addList(int x,int y){
	struct lists *p1;
	if (visit[x][y] == 1){
		return;
	}
	if (phead == NULL){
		newList(x,y);
#if DEBUG >=3
printf("new list!\n");
#endif
	}
	else{
		ptail->next=p1 = (struct lists *) malloc (sizeof(struct lists));
		p1->next = NULL;
		p1->x = x;
		p1->y = y;
		ptail = p1;
		listLen++;
#if DEBUG >= 3
printf("list added x = %d, y = %d\n",x,y);
#endif
	}
}

void deleteList(int position){
	struct lists *p1 = phead,*p2;
	int i=0;
	if (listLen == 1){
		free(phead);
		listLen --;
#if DEBUG >=3
printf("delList, delall,position = %d\n",position);
#endif
		return;
	}
	else if (position == 0){
		phead = phead->next;
		free(p1);
		listLen --;
#if DEBUG >=3
printf("delList, delphead,position = %d\n",position);
#endif
		return;
	}
	
	while (i<position-1){
		p1 = p1->next;
		i++;
	}
	if (position == listLen-1){
		ptail = p1;
		ptail->next = NULL;
		listLen --;
#if DEBUG >= 3
printf("delList, delPtail,position = %d\n",position);
#endif
		return;
	}
	p2 = p1->next;
	p1->next = p2->next;
	free(p2);
	
	listLen --;	
#if DEBUG >= 3
printf("delList, delnormal,position = %d\n",position);
#endif
}

struct lists *getlistinfo(int position){
	struct lists *p1 = phead;
	int i=0;
	//printf("getlistinfo : position = %d\n",position);
	while (i<position){
		p1 = p1->next;
		//printf("in listiinfo func: i = %d\n",i);
		i++;
		
	}
	return p1;
}


/**
* @brief: generate maze
* @param [in] edge: the size of the maze


* @exception 
* @note
*1.Start with a grid full of walls.
*2.Pick a cell, mark it as part of the maze. Add the walls of the cell to the wall list.
*3.While there are walls in the list:
*	1.Pick a random wall from the list. If only one of the two cells that the wall divides is visited, then:
*		1.Make the wall a passage and mark the unvisited cell as part of the maze.
*		2.Add the neighboring walls of the cell to the wall list.
*	2.Remove the wall from the list.
*
*  using algorithms illustrated in: https://en.wikipedia.org/wiki/Maze_generation_algorithm
*/
int generateMaze_prim(int edge){
	struct vector nstart;
	struct lists *visiting;
	int x,y;
	int clear_x,clear_y;//Path
	int tempVisit;
	int i,j;
	prePrim(edge);
	
	/*the first random cell*/
	
	srand(time(0));
	clear_x=nstart.x = rand() % (edge-1) / 2 * 2 + 1;
	srand(time(0));
	clear_y=nstart.y = rand() % (edge-1) / 2 * 2 + 1;
	if (nstart.x == edge){
		nstart.x-=2;
	}
	if (nstart.y == edge){
		nstart.y-=2;
	}
	visit[nstart.x][nstart.y] = 1;
 	for(i =0;i<4;i++){
 		if (nstart.x+move[i].x != 0 && nstart.x+move[i].x != edge && nstart.y+move[i].y != 0 && nstart.y+move[i].y != 1)
			addList(nstart.x+move[i].x,nstart.y + move[i].y);
	}
	
	
	while (listLen>0){
#if DEBUG >= 4
printf("new while preparing, current listLen= %d, tempVisit = %d\n",listLen,tempVisit);
#endif
		srand(time(0));
		tempVisit = rand() % listLen;
#if DEBUG >= 4
printf("new while preparing, current listLen= %d, tempVisit = %d,ready to getinfo\n",listLen,tempVisit);
#endif
		visiting = getlistinfo(tempVisit);
#if DEBUG >= 4
printf("new while  preparing, current listLen= %d, tempVisit = %d, getinfo is done\n",listLen,tempVisit);
#endif
		x = visiting->x;
		y = visiting->y;
		visit[x][y] = 1;
#if DEBUG >= 4
printf("new while prepared , current listLen= %d, tempVisit = %d\n",listLen,tempVisit);
#endif
			for (i=0;i<4;i++){
				x = visiting->x + move[i].x;
				y = visiting->y + move[i].y;
				
				if (maze[x][y] == CLEAR && visit[x][y] == 0){
					maze[visiting->x][visiting->y] = CLEAR;
					clear_x = x;
					clear_y = y;
					visit[clear_x][clear_y] = 1;
					for (i=0;i<4;i++){
						x = clear_x + move[i].x;
						y = clear_y + move[i].y;
						if (maze[x][y] == BLOCK && visit[x][y] == 0){
#if DEBUG >= 4
printf("ready to add node, current listLen = %d\n",listLen);
#endif
							addList(x,y);
#if DEBUG >= 4
printf("node added, current listLen = %d\n",listLen);
#endif
#if DEUBUG >=2
checkList();
#endif
						}
						else{
							;
						}
					};
					break;
				};
			
			}
				
#if DEBUG >= 2
printf("tempVisit = %d\n",tempVisit);
printf("beforedel,liseLen = %d\n",listLen);
#endif
		deleteList(tempVisit);
#if DEBUG >= 2
printf("afterdel,liseLen = %d\n",listLen);
ptmap(edge);
checkList();
#endif
	}
}


/**
* @brief: 
* @param [in] n: 

* @exception 
* @note
*/
void getmaze(int n) 
{
	int i,j,a,b;	
	i=n;j=n;a=1;b=1;
	maze[a][b]=WAY;/*入口*/ 
	start.x = a; start.y = b;
	
	srand(time(NULL));
    while (i>3||j>3){
    	if(i==3)
    	{
    		maze[++a][b]=WAY;i--;
		}
		else if(j==3)
		{
			maze[a][++b]=WAY;j--;
		}
		else if(rand()%2==1)
    	{
    		maze[a][++b]=WAY;j--;
		}
		else
		{
			maze[++a][b]=WAY;i--;
		}
	}/*随机生成一条通向终点的路径*/
	for(i=1;i<n-1;i++)
	{
		for(j=1;j<n-1;j++)
		{
			if(maze[i][j]!=1)
			{
				maze[i][j]=rand()%2;
			}
		}
	}/*本来其他地方都是0，路径是1，现在把除了最外圈围墙以外的部分随机设为0或1*/
}

/**
* @brief: put solutions into the maze array;

* @exception 
* @note
*/
void solutionToMaze(){
	int i = 0;
	while (solution[0][i].x != end.x  || solution[0][i].y != end.y){
		maze[solution[0][i].x][solution[0][i].y] = WAY;
		i++;
	}
}
int main()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    system ("color 0b");
	int i;
	int maxLine;
	int genMethod;
	while (1){
		printf("Input the size of the Input any non-positive number to end.\n");
		scanf("%d",&maxLine);
		if(maxLine<=0){
			return 0;
		}
	
		while (1){
		printf("Choose maze generation method: 0.change the size    1.random     2.prim.\n");
		scanf("%d",&genMethod);
			if (genMethod == 1 || genMethod == 2){
				iniData();
		
				start.x = start.y = 0;
				end.x = end.y = maxLine-1;
				if (genMethod == 2){
					generateMaze_prim(maxLine);
					if (maxLine%2 == 0){
						oddNumberFixer(maxLine);
					}
				}
				else{
					getmaze(maxLine);
				}	
#if  DEBUG
ptmap(maxLine);
#endif 
				drawMap(maxLine, handle, 2);
				iniDfs(maxLine);
#if DEBUG >= 2				
printf("iniVisit Finished\n");
#endif
				dfs(start.x,start.y,maxLine);
#if DEBUG >= 2
printf("dfs Finished\n");
#endif
				solutionToMaze();
				
				getchar();
				getchar();
				drawMap(maxLine, handle, 3);
			}
			else if (genMethod == 0){
				break;
			}
			else {
				printf("Input 0, 1 or 2.\n");
			}
		}
		
	}
	return 0; 
} 


