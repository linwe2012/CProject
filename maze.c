#include<stdio.h>
#include <time.h>
#include<stdlib.h>
#include<windows.h>
#include<tchar.h>
#include <commdlg.h>
#include<conio.h>

#define CLEAR 1
#define BLOCK 0
#define WAY 2
#define DESTINATION 3
#define START 4

#define MAX 100
#define MIN 2
#define DEBUG 0
#define DEMO 0

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
}solution[MAX*MAX][MAX*MAX],start,end,move[4];
int DFS_Flag;

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
//void 
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



/*display the maze*/
void drawMap(int  maxLine, HANDLE handle, int Enter){
	int i,j;
	
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
#if DEBUG
printf("%d ",maze[i][j]);
#endif
                        /*display varied figures as the value of maze change*/
			if (maze[i][j] == DESTINATION || maze[i][j] == WAY || maze[i][j] == START){
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
	move[1].x = 0;  move[1].y = -1; //down
	move[2].x = -1; move[2].y = 0;  //left
	move[3].x = 0;  move[3].y = 1;  //up
	
	iniSolution();
}

/**
* @brief: initialize the visit[][], set the boundries visited
* @param [in] edge: the size of the maze
* @param [in] edgeClosed: whether the boundries should be set visited
* @exception 
* @note
*/
void iniVisit(int edge, int edgeClosed){
	int i,j;
	
	for(i = 1;i<MAX;i++){
		for(j=1;j<MAX;j++){
			visit[i][j] = 0;
		}
	}
	if(edgeClosed == 1){
		for(i = 0;i<edge;i++){
			visit[0][i] = 1;
			visit[i][0] = 1;
			visit[edge-1][i] = 1;
			visit[i][edge-1] = 1;
		}
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
	iniVisit(edge, 0);
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
	DFS_Flag = 0;
}


/**
* @brief: initialize the visit[][], set the boundries visited
* @param [in] edge: the size of the maze
* @param [in] x: current position
* @param [in] y: current position
* return 0: find a solution
* return -1:fails to find one
* @exception 
* @note: DFS_Flag = 1 indicates one solution if found;
*        when there is no solution ,it will crash
*/

int dfs(int x,int y,int edge)
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

		DFS_Flag = 1;
		return 0;
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
		/*trying each diretions*/
		if (x1>=0&&y1>=0&&x1<edge&&y1<edge && visit[x1][y1] == 0 && maze[x1][y1] != BLOCK){
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
			if(DFS_Flag == 1){
				return 0;
			}
			visit[x1][y1] = 0;
			if (sol_col>0){
				sol_col--;
			}
		}
		
	}
#if DEBUG >=2
printf("fail,withdraw\n");
#endif
	if(DFS_Flag == 1){
		return 0;
	}
	else{
		return -1;
	}
}



///////Maze Generator////////////////////////////////////////////////////////////////////////////////
void evenNumberFixer(int edge){
	int i;
	int seed;
	for(i = 1;i<edge-1;i++){
		
		if (rand() % 5 == 0){
			maze[i][edge-2] = CLEAR;
		}
		if (rand() % 5 == 0){
			maze[edge-2][i] = CLEAR;
		}
	}
	for(i=-2;i<3;i++){
		if (edge-2 != end.y){
			if (end.x+i < edge && end.x+i > 0)
				maze[end.x+i][edge-2] = CLEAR;
			if (end.y+i < edge && end.y+i > 0)
				maze[edge-2][end.y+i] = CLEAR;
			if (start.x+i < edge && start.x+i > 0)
				maze[start.x+i][edge-2] = CLEAR;
			if (start.y+i < edge && start.y+i > 0)
			maze[edge-2][start.y+i] = CLEAR;
		}
	} 
	maze[end.x][end.y] = DESTINATION;
	maze[start.x][start.y] = START;
}

/**
* @brief: make sure the starting point & destination and their neighbors are accessible
* @param [in] edge: the size of the maze
* @exception 
* @note
*/
void makeWayforStartAndDestination(int edge){
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
	
	makeWayforStartAndDestination(edge);

#if DEBUG >= 2
ptmap(edge);
#endif
	
	iniVisit(edge, 1);

	listLen = 0;
	phead = ptail = NULL;
#if DEMO
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
system ("color 0b");
drawMap(edge,handle,5);
#endif
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
*  using algorithms illustrated at: https://en.wikipedia.org/wiki/Maze_generation_algorithm
* it will have problems with even numbers(last but one row & column won't have any accessible cell , so evenNumberFixer is used to address the problem
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
	clear_x=nstart.x = rand() % (edge-1) / 2 * 2 + 1;
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

#if DEMO >= 2
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
system ("color 0b");
drawMap(edge,handle,5);
#endif
	}

	maze[start.x][start.y] = START;
#if DEMO >= 2
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
printf("Prim Algorithm is Finished. Start = (%d, %d)\n\n",start.x,start.y);
drawMap(edge,handle,5);
#endif
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
	i=n;j=n;a=0;b=0;
	maze[a][b]=CLEAR;/*Èë¿Ú*/ 
	start.x = a; start.y = b;
	end.x = n-1; end.y= n-1;
	maze[end.x][end.y] = CLEAR;
	
	srand(time(NULL));
    while (i>1||j>1){
    	if(i==1)
    	{
    		maze[a][++b]=CLEAR;j--;
		}
		else if(j==1)
		{
			maze[++a][b]=CLEAR;i--;
		}
		else if(rand()%2==1)
    	{
    		maze[a][++b]=CLEAR;j--;
		}
		else
		{
			maze[++a][b]=CLEAR;i--;
		}
	}/*Randomly generate a path to the end*/
	for(i=1;i<n-1;i++)
	{
		for(j=1;j<n-1;j++)
		{
			if(maze[i][j]!=CLEAR)
			{
				maze[i][j]=rand()%2;
			}
		}
	}/*The former value of the maze: blocks is 0, the path is 1.
	Now the rest part except the outer ring wall is randomly set to 0 or 1.*/
	makeWayforStartAndDestination(n);
	evenNumberFixer(n);
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

int Move(char ch, int maxline, struct vector *pos)
{
	if(pos->x != start.x && pos->y != start.y && pos->x != end.x && pos->y != end.y)
		maze[pos->x][pos->y] = CLEAR;
	switch(ch)
	{
		case 'w': case 'W': {
			if(maze[pos->x-1][pos->y] != BLOCK && pos->x > 0) (pos->x)--;
			break;
		}
		case 's': case 'S': {
			if(maze[pos->x+1][pos->y] != BLOCK && pos->x < maxline-1) (pos->x)++;
			break;
		}
		case 'a': case 'A': {
			if(maze[pos->x][pos->y-1] != BLOCK && pos->y > 0) (pos->y)--;
			break;
		}
		case 'd': case 'D': {
			if(maze[pos->x][pos->y+1] != BLOCK && pos->y < maxline-1) (pos->y)++;
			break;
		}
	}
	if(pos->x != start.x && pos->y != start.y && pos->x != end.x && pos->y != end.y)
		maze[pos->x][pos->y] = WAY;
	if(pos->x == end.x && pos->y == end.y)return 1;
	else return 0;
}

int solveMaze(int edge){
	char ch;
	struct vector pos;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.x = start.x;
	pos.y = start.y;
	fflush(stdin);
	while((ch = getch()) != '\n' && (ch) != '\r'){
		if(Move(ch, edge, &pos) == 1){
			printf("Congratulations!\n");
			return 1;
		}
		else{
			system("cls");
			printf("U R working on a %d * %d maze.\n",edge, edge);
			printf("Press Enter to see the answer.\n");
			drawMap(edge, handle, 0);
		}
	}
	maze[pos.x][pos.y] = CLEAR;
	printf("\n\n\n");
	return 0;
}


int main()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hwnd = GetForegroundWindow();
    system ("color 0b");
	int i;
	int maxLine;
	int genMethod;
	if(SetWindowTextA(hwnd, "Maze Generate & Solve") == 0){
    	printf("Fails to set Windows Title.\n");
	}
	srand(time(NULL));
	while (1){
		printf("Input the size of the maze. Input any negative number to end.\n");
		printf("Input 0 to View the Code at Github.com.\n");
		scanf("%d",&maxLine);
		if(maxLine < 0){
			return 0;
		}
		else if(maxLine == 0){
			ShellExecute(NULL,"open","https://github.com/linwe2012/CProject",NULL,NULL,SW_SHOWNORMAL);
			MessageBox(NULL,"If You Find any Bugs, DON'T be shy to Label Issues.","Thanks For Visiting.",MB_OK);
			printf("\n\n");
			continue;
		}

		else if(maxLine <= MIN){
			i = rand() % 4;
			if(i == 0)
				printf("Come on! Choose a larger number!\n");
			else if(i == 1)
				printf("Any number larger than 2!\n");
			else if(i == 2)
				printf("Have some Guts! Challenge yourself! Number bigger than 2!\n");
			else{
				printf("What's the point in having a maze of %d*%d? Number bigger than 2!\n",maxLine,maxLine);
			}
			continue;
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
						evenNumberFixer(maxLine);
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
				if(dfs(start.x,start.y,maxLine) == -1){
					printf("Ooops, it seems there is no solution to the maze... Try again?\n");
					getchar();
					continue;
				}
				
				
#if DEBUG >= 2
printf("dfs Finished\n");
#endif
				getchar();
				printf("Press Enter to see the answer.\n");
				solveMaze(maxLine);
				solutionToMaze();

				drawMap(maxLine, handle, 3);
			}
			else if (genMethod == 0){
				break;
			}
			else {
				i = rand() % 5;
				if (i == 0)
					printf("Mmmm..., just... Input 0, 1 or 2.\n");
				else if (i == 1)
					printf("+ Knock! Knock!\n- Who's There?\n+ Number %d\n- Wrong!\nNow just input 0, 1 or 2.\n",genMethod);
				else if (i == 2)
					printf("Sorry! The number you have dialed is wrong, Input 0, 1 or 2.\n");
				else if (i == 3){
					printf("Ahah! U R Wrong! Input 0, 1 or 2.\n");
				}
				else{
					printf("\aBuzzzzzz. Wrong! Input 0, 1 or 2.\n");
				}
			}
		}
		
	}
	return 0; 
} 

