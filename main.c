#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "maze.h"

long int random(void);
void srandom(unsigned int seed);


int main(int argc, char **argv){

	assert(argc > 1);

	int argIndex = 1;
	char *value, *mazeFile, *mazeRead, *mazePrev, *mazeAfter;
	int cMark = 0, dMark = 0, sMark = 0;
	int rows, columns, rand = 1;
	FILE *fpWrite, *fpRead;
	while(argIndex < argc){
		value = argv[argIndex];
		if(strcmp(value, "-v")== 0){
			printf("Author: Titus Davis\n");
			exit(-1);
		}
		else if(strcmp(value, "-s")==0){
			sMark = 1;
			mazePrev = argv[argIndex+1];
			mazeAfter = argv[argIndex+2];
			argIndex += 2;
		}
		else if(strcmp(value, "-c")==0){
			cMark = 1;
			rows = atoi(argv[argIndex+1]);
			columns = atoi(argv[argIndex+2]);
			mazeFile = argv[argIndex+3];
			argIndex += 3;
		}
		else if(strcmp(value, "-r")==0){
			rand = (atoi(argv[argIndex+1]));
			argIndex++;
		}
		else if(strcmp(value, "-d")==0){
			dMark = 1;
			mazeRead = argv[argIndex+1];
			argIndex++;
		}
		else{
			printf("that's not a valid input\n");
		}
		argIndex++;
	}
	if(cMark == 1){
		fpWrite = fopen(mazeFile, "w");
		buildMAZE(rows, columns, rand, fpWrite);
		fclose(fpWrite);
	}
	if(sMark == 1){
		solveMAZE(mazePrev, mazeAfter);
	}
	if(dMark == 1){
		fpRead = fopen(mazeRead, "r");
		drawMAZE(fpRead);
		fclose(fpRead);
	}

	return 0;
}