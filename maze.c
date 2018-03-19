#include<stdio.h>
#include <time.h>
#include<stdlib.h>

#define CLEAR 1
#define BLOCK 0
#define WAY 2
#define DESTINATION 3
#define MAX 100

#define DEBUG 0



int maze[MAX][MAX];
int visit[MAX][MAX];
struct vector{
	int x;
	int y;
}solution[MAX][MAX*MAX],start,end,move[4];
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

#if DEBUG
void testdrawMap(int  maxLine,int Enter){
	int i,j;
	int map[MAX][MAX];
	i= 0;
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
			map[i][j] = maze[i][j];
		}
	}
	while((solution[0][i].x != end.x  || solution[0][i].y != end.y )&& (solution[0][i].x != -1  || solution[0][i].y != -1)){ 
		map[solution[0][i].x][solution[0][i].y] = WAY;
		i++;
	}
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
			if(map[i][j] >= 2){
				printf("%c",'o');
			}
			if(map[i][j] == CLEAR){
				printf(" ");
			}
			if(map[i][j] == BLOCK){
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
	while(p1 != NULL){
		p2 = p1;
		p1 = p1->next;
		i++;
	}
	printf("Listchecked: %d\n",i);
	if(p2 != ptail){
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

int testIsolateCell(int x,int y,int edge,int step ){
	int i;
	if(step == 0){
		return 0;
	}
	for(i=0;i<4;i++){
		if(maze[x+move[i].x][y+move[i].y] == CLEAR && x+move[i].x<edge-1 && x+move[i].x>0 && y+move[i].y<edge-1 && y+move[i].y>0){
			if(testIsolateCell(x+move[i].x,y+move[i].y,edge,step-1) == 0){
				return 0;
			}
		}
	}
	return 1;//its blocked in several steps;
}
#endif


void drawMap(int  maxLine,int Enter){
	int i,j;
	
	for(i=0;i<maxLine;i++){
		for(j=0;j<maxLine;j++){
			//printf("%d ",maze[i][j]);
			if(maze[i][j] >= 2){
				printf("%c",'o');
			}
			if(maze[i][j] == CLEAR){
				printf(" ");
			}
			if(maze[i][j] == BLOCK){
				printf("%c",'#');
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
		if(x>=0 && y>=0 && x<edge && y<edge && maze[x][y] != BLOCK){
			visit[x][y] = 0;
		}
		if(x1>=0 && y1>=0 && x1<edge && y1<edge && maze[x1][y1] != BLOCK){
			visit[x1][y1] = 0;
		}
	}
}

/////////find the way out///////////////////////////////////////////////////////////////////////////
void dfs(int x,int y,int edge)
{
	int i,x1,y1;
	
	//printf("In it\n");
	if(maze[x][y] == DESTINATION){
		solution[sol_row][sol_col].x  = x;
		solution[sol_row][sol_col].y  = y;
		sol_row ++;
		//sol_col= 0;
		#if DEBUG
		printf("find one!\n");
		#endif
		//drawMap(edge,2);
		return;
	}
	
	
	
	//printf("start for\n");
	for(i=0;i<4;i++){
		x1 = x + move[i].x ;
		y1 = y + move[i].y ;
		//printf("In for %d \n",i);
		if(x1>=0&&y1>=0&&x1<edge&&y1<edge &&visit[x1][y1] == 0 && maze[x1][y1] != BLOCK){
			solution[sol_row][sol_col].x  = x1;
			solution[sol_row][sol_col].y  = y1;
			sol_col++;
			visit[x1][y1] = 1;
			//printf("row = %d,col = %d\n pos.x = %d",sol_row,sol_col);
			//printf("nextpath found\n");
			//testdrawMap(edge,2);
			dfs(x1,y1,edge);
			visit[x1][y1] = 0;
			if(sol_col>0){
				sol_col--;
			}
		}
		
	}
	/*
	if(sol_col>0){
		sol_col--;
	}
	printf("fail,withdraw\n");*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////



///////Maze Generator////////////////////////////////////////////////////////////////////////////////
void oddNumberFixer(int edge){
	int i;
	int visitmap[7][7];
	/*
	for(i = 1;i<edge-1;i++){
		srand(time(0));
		if(rand() % 2 == 0){
			maze[i][edge-2] = CLEAR;
		}
		srand(time(0));
		if(rand() % 2 == 0){
			maze[edge-2][i] = CLEAR;
		}
	}*/
	for(i=-2;i<3;i++){
		if(edge-2 != end.y){
		
			maze[end.x+i][edge-2] = CLEAR;
			maze[edge-2][end.y+i] = CLEAR;
			maze[start.x+i][edge-2] = CLEAR;
			maze[edge-2][start.y] = CLEAR;
		}
	} 
	maze[end.x][end.y] = DESTINATION;
}

void prePrim(int edge){
	int i,j;
	for(i = 1;i<edge-1;i+=2){
		for(j=1;j<edge-1;j+=2){
			maze[i][j] = CLEAR;
		}
		
	}
	
	maze[start.x][start.y] = CLEAR;
	maze[end.x][end.y] = DESTINATION;
	//make sure there is a way
	//if(start.x %2 ==0 && start.y%2==0){
		if(start.x == 0){
			maze[start.x+1][start.y] = CLEAR;
		}
		else if(start.x == edge-1){
			maze[start.x-1][start.y] = CLEAR;
		}
		else if(start.y == edge-1){
			maze[start.x][start.y-1] = CLEAR;
		}
		else if(start.y == 0){
			maze[start.x][start.y+1] = CLEAR;
		}
	//}
	
	//if(end.x %2 ==0 && end.y%2==0){
		if(end.x == 0){
			maze[end.x+1][end.y] = CLEAR;
		}
		else if(end.x == edge-1){
			maze[end.x-1][end.y] = CLEAR;
		}
		else if(end.y == edge-1){
			maze[end.x][end.y-1] = CLEAR;
		}
		else if(end.y == 0){
			maze[end.x][end.y+1] = CLEAR;
		}
		//fix bugs when odd number may cause no path
		
	//}
	//ptmap(edge);
	
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
	if(visit[x][y] == 1){
		return;
	}
	if(phead == NULL){
		newList(x,y);
		//printf("new list!\n");
	}
	else{
		ptail->next=p1 = (struct lists *) malloc (sizeof(struct lists));
		p1->next = NULL;
		p1->x = x;
		p1->y = y;
		ptail = p1;
		listLen++;
		//printf("list added x = %d, y = %d\n",x,y);
	}
}

void deleteList(int position){
	struct lists *p1 = phead,*p2;
	int i=0;
	if(listLen == 1){
		free(phead);
		listLen --;
		//printf("delList, delall,position = %d\n",position);
		return;
	}
	else if(position == 0){
		phead = phead->next;
		free(p1);
		listLen --;
		//printf("delList, delphead,position = %d\n",position);
		return;
	}
	
	while(i<position-1){
		p1 = p1->next;
		i++;
	}
	if(position == listLen-1){
		ptail = p1;
		ptail->next = NULL;
		listLen --;
		//printf("delList, delPtail,position = %d\n",position);
		return;
	}
	p2 = p1->next;
	p1->next = p2->next;
	free(p2);
	
	listLen --;	
	//printf("delList, delnormal,position = %d\n",position);
}

struct lists *getlistinfo(int position){
	struct lists *p1 = phead;
	int i=0;
	//printf("getlistinfo : position = %d\n",position);
	while(i<position){
		p1 = p1->next;
		//printf("in listiinfo func: i = %d\n",i);
		i++;
		
	}
	return p1;
}



int generateMaze_prim(int edge){
	struct vector nstart;
	struct lists *visiting;
	int x,y;
	int clear_x,clear_y;//Path
	int tempVisit;
	int i,j;
	prePrim(edge);
	
	//the first random cell
	
	srand(time(0));
	clear_x=nstart.x = rand() % (edge-1) / 2 * 2 + 1;
	srand(time(0));
	clear_y=nstart.y = rand() % (edge-1) / 2 * 2 + 1;
	if(nstart.x == edge){
		nstart.x-=2;
	}
	if(nstart.y == edge){
		nstart.y-=2;
	}
	visit[nstart.x][nstart.y] = 1;
 	for(i =0;i<4;i++){
 		if(nstart.x+move[i].x != 0 && nstart.x+move[i].x != edge && nstart.y+move[i].y != 0 && nstart.y+move[i].y != 1)
			addList(nstart.x+move[i].x,nstart.y + move[i].y);
	}
	
	
	while(listLen>0){
		//printf("new while preparing, current listLen= %d, tempVisit = %d\n",listLen,tempVisit);
		srand(time(0));
		tempVisit = rand() % listLen;
		//printf("new while preparing, current listLen= %d, tempVisit = %d,ready to getinfo\n",listLen,tempVisit);
		visiting = getlistinfo(tempVisit);
		//printf("new while preparing, current listLen= %d, tempVisit = %d, getinfo is done\n",listLen,tempVisit);
		x = visiting->x;
		y = visiting->y;
		visit[x][y] = 1;
		//printf("new while prepared , current listLen= %d, tempVisit = %d\n",listLen,tempVisit);
		//if((visit[x-1][y]==1 || visit[x+1][y] == 1) || (maze[x][y-1]==1 && maze[x][y-1]==1)){
				for(i=0;i<4;i++){
					x = visiting->x + move[i].x;
					y = visiting->y + move[i].y;
					
					if(maze[x][y] == CLEAR && visit[x][y] == 0){
						maze[visiting->x][visiting->y] = CLEAR;
						clear_x = x;
						clear_y = y;
						visit[clear_x][clear_y] = 1;
						for(i=0;i<4;i++){
							x = clear_x + move[i].x;
							y = clear_y + move[i].y;
							if(maze[x][y] == BLOCK && visit[x][y] == 0){
								//printf("ready to add node, current listLen = %d\n",listLen);
								addList(x,y);
								//printf("node added, current listLen = %d\n",listLen);
								//checkList();
							}
						};
						break;
					};//if no pair of oppposite & adjacent cells is both 1	
				
				}
				
		//}
		/*else{
			clear_x = x;
			clear_y = y;
			
		
			for(i=0;i<4;i++){
				x = clear_x + move[i].x;
				y = clear_x + move[i].y;
				if(maze[x][y] == BLOCK && visit[x][y] == 0){
					addList(x,y);
				}
				
			}
		}*/
		//printf("tempVisit = %d\n",tempVisit);
		//printf("beforedel,liseLen = %d\n",listLen);
		deleteList(tempVisit);
		//printf("afterdel,liseLen = %d\n",listLen);
		//ptmap(edge);
		//checkList();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



void getmaze(int n) 
{
	int i,j,a,b;	
	i=n;j=n;a=1;b=1;
	maze[a][b]=WAY;/*入口*/ 
	start.x = a; start.y = b;
	
	srand(time(NULL));
    while(i>3||j>3){
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
//////////////////////////////////////////////////////////////////////////////////////////////
void solutionToMaze(){
	int i = 0;
	while(solution[0][i].x != end.x  || solution[0][i].y != end.y){
		maze[solution[0][i].x][solution[0][i].y] = WAY;
		i++;
	}
}
int main()
{
	int i;
	int maxLine;
	int genMethod;
	while(1){
		printf("Input the size of the Input any non-positive number to end.\n");
		scanf("%d",&maxLine);
		if(maxLine<=0){
			return 0;
		}
		iniData();
		
		start.x = start.y = 0;
		end.x = end.y = maxLine-1;
		
		while(1){
		printf("Choose maze generation method: 0.change the size    1.random     2.prim.\n");
		scanf("%d",&genMethod);
			if(genMethod == 1 || genMethod == 2){
				
				if(genMethod == 2){
					generateMaze_prim(maxLine);
					if(maxLine%2 == 0){
						oddNumberFixer(maxLine);
					}
				}
				else{
					getmaze(maxLine);
				}
				
				
				#if DEBUG
				ptmap(maxLine);
				#endif
				drawMap(maxLine,2);
				iniDfs(maxLine);
				
				//printf("iniVisit Finished\n");
				dfs(start.x,start.y,maxLine);
				//printf("dfs Finished\n");
				solutionToMaze();
				
				getchar();
				getchar();
				drawMap(maxLine,3);
			}
			else if(genMethod == 0){
				break;
			}
			else {
				printf("Input 0, 1 or 2.\n");
			}
		}
		
	}
	return 0; 
} 


