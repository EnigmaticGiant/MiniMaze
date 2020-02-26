#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

typedef struct maze MAZE;
typedef struct cell CELL;

extern MAZE *newMAZE(int, int);
extern CELL *newCELL(int, int);
extern void buildMAZE(int, int, int, FILE *);
extern void drawMAZE(FILE *);
extern void solveMAZE(char *, char *);
extern void freeMAZE(MAZE *);


#endif