#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "da.h"
#include "stack.h"
#include "queue.h"
#include "cda.h"

long int random(void);
void srandom(unsigned int seed);



typedef struct cell{
	int xPos;
	int yPos;
	int rightWall;
	int bottomWall;
	char value;
	int visited;
}CELL;


typedef struct maze{
	int rows;
	int columns;
	CELL ***array;
}MAZE;

void freeMAZE(MAZE *maze);

CELL *newCELL(int x, int y){
	CELL *c = malloc(sizeof(CELL));
	c->xPos = x;
	c->yPos = y;
	c->rightWall = 0;
	c->bottomWall = 0;
	c->value = '\0';
	c->visited = 0;
	return c;
}

MAZE *newMAZE(int rows, int columns){
	MAZE *x = malloc(sizeof(MAZE));
	x->rows = rows;
	x->columns = columns;
	x->array = malloc(sizeof (CELL **) * x->rows);
	for (int i = 0; i < rows; ++i){
   		x->array[i] = malloc(sizeof(CELL *) * x->columns);
   		for (int j = 0; j < columns; ++j)
       		x->array[i][j] = newCELL(i,j);
   	}
	return x;
}

void buildMAZE(int rows, int columns, int rand, FILE *fp){
///for my buildMAZE
	MAZE *maze = newMAZE(rows, columns);
	if(rand != 1) srandom(rand);
	STACK *stack = newSTACK();
	push(stack, maze->array[0][0]);
	while(sizeSTACK(stack) != 0){
		CELL *cc = peekSTACK(stack);
		cc->visited = 1;
		DA *temp = newDA();


		if(cc->xPos-1 >= 0 && (maze->array[cc->xPos-1][cc->yPos])->visited == 0){
			insertDAback(temp, maze->array[cc->xPos-1][cc->yPos]);
		}		//left
		if(cc->yPos-1 >= 0 && maze->array[cc->xPos][cc->yPos-1]->visited == 0){
			insertDAback(temp, maze->array[cc->xPos][cc->yPos-1]);
		}		//above
		if(cc->yPos+1 < maze->columns && (maze->array[cc->xPos][cc->yPos+1])->visited == 0){
			insertDAback(temp, maze->array[cc->xPos][cc->yPos+1]);
		}		//below
		if(cc->xPos+1 < maze->rows && (maze->array[cc->xPos+1][cc->yPos])->visited == 0){
			insertDAback(temp, maze->array[cc->xPos+1][cc->yPos]);
		}		//right
		if(sizeDA(temp) == 0){
		freeDA(temp);
		pop(stack);
		}
		else{
			CELL *next = removeDA(temp, random() % sizeDA(temp));
			//remove the wall
			if(cc->xPos < next->xPos) //the next cell is to the bottome
				cc->bottomWall = 1;
			else if(cc->xPos > next-> xPos) //the next cell is to the top
				next->bottomWall = 1;
			else if(cc->yPos < next->yPos) //the next cell is to the right
				cc->rightWall = 1;
			else if(cc->yPos > next->yPos) // the next cell is to the left
				next->rightWall = 1;  
			push(stack, next);
			freeDA(temp);
		}
	}
	freeSTACK(stack);
	//printing relevant info to the file
	//number of rows
	fprintf(fp, "%d\n",maze->rows);
	//number of columns
	fprintf(fp, "%d\n", maze->columns);
	//noting that the maze is NOT solved
	fprintf(fp, "0\n");
	//printing the status of each cell's right and bottom wall, respectively
	for(int i = 0; i < maze->rows; i++){
		for(int j = 0; j < maze->columns; j++){
			fprintf(fp, "%d\n%d\n%d\n%d\n", i, j, maze->array[i][j]->rightWall, maze->array[i][j]->bottomWall);
		}
	}
	freeMAZE(maze);
	return;
}

void solveMAZE(char *prev, char *after){
	int info, x, y, rW, bW, rows, cols;
	FILE *fpRead = fopen(prev, "r");
	fscanf(fpRead, "%d", &info);
	rows = info;
	fscanf(fpRead, "%d", &info);
	cols = info;
	MAZE *temp = newMAZE(rows, cols);
	fscanf(fpRead, "%d", &info);
	fscanf(fpRead, "%d", &x);
	while(!feof(fpRead)){
		fscanf(fpRead, "%d", &y);
		fscanf(fpRead, "%d", &rW);
		fscanf(fpRead, "%d", &bW);
		temp->array[x][y]->rightWall = rW;
		temp->array[x][y]->bottomWall = bW;
		fscanf(fpRead, "%d", &x);
	}
	fclose(fpRead);
	QUEUE *bfs = newQUEUE();
	CELL *end = temp->array[temp->rows-1][temp->columns-1];
	enqueue(bfs, temp->array[0][0]);
	CELL *front = peekQUEUE(bfs);
	front->value = '0';
	int step = 0;
	while(front != end){
		step = front->value - '0' + 1;
		if((front->xPos-1 >= 0 && temp->array[front->xPos-1][front->yPos]->bottomWall == 1) && temp->array[front->xPos-1][front->yPos]->value =='\0'){
			CELL *above = temp->array[front->xPos-1][front->yPos];
			above->value = '0' + step % 10;
			enqueue(bfs, above);
		}
		if((front->yPos-1 >= 0 && temp->array[front->xPos][front->yPos-1]->rightWall == 1)&& temp->array[front->xPos][front->yPos-1]->value =='\0'){
			CELL *left = temp->array[front->xPos][front->yPos-1];
			left->value = '0' + step % 10;
			enqueue(bfs, left);
		}
		if((front->xPos+1 < temp->rows && temp->array[front->xPos][front->yPos]->bottomWall == 1)&& temp->array[front->xPos+1][front->yPos]->value =='\0'){
			CELL *below = temp->array[front->xPos+1][front->yPos];
			below->value = '0' + step % 10;
			enqueue(bfs, below);
		}
		if((front->yPos+1 < temp->columns && temp->array[front->xPos][front->yPos]->rightWall == 1)&& temp->array[front->xPos][front->yPos+1]->value =='\0'){
			CELL *right = temp->array[front->xPos][front->yPos+1];
			right->value = '0' + step % 10;
			enqueue(bfs, right);
		}
		dequeue(bfs);
		front = peekQUEUE(bfs);
	}
	front = dequeue(bfs);
	while(sizeQUEUE(bfs) != 0){
		front = peekQUEUE(bfs);
		front->value = '\0';
		dequeue(bfs);
	}
	freeQUEUE(bfs);
	FILE *fpWrite = fopen(after, "w");
	//number of rows
	fprintf(fpWrite, "%d\n", temp->rows);
	//number of columns
	fprintf(fpWrite, "%d\n", temp->columns);
	//change unsolved to solved
	fprintf(fpWrite, "1\n");
	//the matrix of every cell that has a value and its value
	for(int i = 0; i < temp->rows; i++){
		for(int j = 0; j < temp->columns; j++){
			fprintf(fpWrite, "%d\n%d\n%d\n%d\n", i, j, temp->array[i][j]->rightWall, temp->array[i][j]->bottomWall);
			if(temp->array[i][j]->value != '\0')
				fprintf(fpWrite, "%c\n", temp->array[i][j]->value);
			else
				fprintf(fpWrite,";\n");
		}
	}
	fclose(fpWrite);
	freeMAZE(temp);
}

void drawMAZE(FILE *fp){

	int info, rW, bW, x, y, rows, cols;
	char semi;
	fscanf(fp, "%d", &info);
	rows = info;
	fscanf(fp, "%d", &info);
	cols = info;
	MAZE *temp = newMAZE(rows, cols);
	fscanf(fp, "%d", &info);
	if(info == 0){ //maze is unsolved
		fscanf(fp, "%d", &x);
		while(!feof(fp)){
			fscanf(fp, "%d", &y);
			fscanf(fp, "%d", &rW);
			fscanf(fp, "%d", &bW);
			temp->array[x][y]->bottomWall = bW;
			temp->array[x][y]->rightWall = rW;
			fscanf(fp, "%d", &x);
		}
	}
	else{ //maze is solved
		fscanf(fp, "%d", &x);
		while(!feof(fp)){
			fscanf(fp, "%d", &y);
			fscanf(fp, "%d", &rW);
			fscanf(fp, "%d", &bW);
			temp->array[x][y]->bottomWall = bW;
			temp->array[x][y]->rightWall = rW;
			fscanf(fp, "%c", &semi);
			fscanf(fp, "%c", &semi);
			if(semi != ';'){
				temp->array[x][y]->value = semi;
			}
			fscanf(fp, "%d", &x);
		}
	}
	int m, n;
	int totalRows = (temp->rows*2)+1;
	int totalColumns = (temp->columns*2)+1;
	for(n=0; n < totalRows; n++){
		for(m=0; m < totalColumns; m++){
			if(n % 2 == 0){ //horizontal bars
				if(n == 0 || n == totalRows-1){
					if(m % 2 == 0)	fprintf(stdout,"-");
					else	fprintf(stdout,"---");
				}
				else{
					if(m % 2 == 0)fprintf(stdout, "-");
					else{
						if(temp->array[(n-1)/2][(m-1)/2]->bottomWall == 1) fprintf(stdout, "   ");
						else fprintf(stdout,"---");
					}
				}
			}
			else if(m % 2 == 0){ //vertical bars
				if(m == 0 || m == totalColumns-1){
					if(m==0 && n==1) fprintf(stdout, " ");
					else if(m == totalColumns-1 && n == totalRows-2) fprintf(stdout, " ");
					else fprintf(stdout,"|");
				}
				else{
					if(temp->array[(n-1)/2][(m-1)/2]->rightWall == 1){
						fprintf(stdout, " ");
					}
					else fprintf(stdout, "|");
				}
			}
			else{ //spaces where the cells are
				if(temp->array[(n-1)/2][(m-1)/2]->value == '\0') fprintf(stdout,"   ");
				else fprintf(stdout," %c ", temp->array[(n-1)/2][(m-1)/2]->value);
			}
		}
		fprintf(stdout,"\n");
	}
	freeMAZE(temp);
}

void freeCELL(CELL *c){
	c->yPos = 0;
	c->xPos = 0;
	c->rightWall = 0;
	c->bottomWall = 0;
	c->value = '\0';
	c->visited = 0;
	free(c);
}

void freeMAZE(MAZE *maze){
	for(int i = 0; i < maze->rows; i++){
		for(int j = 0; j < maze->columns; j++){
			freeCELL(maze->array[i][j]);
		}
		free(maze->array[i]);
	}
	maze->rows = 0;
	maze->columns = 0;
	free(maze->array);
	free(maze);
	return;
}