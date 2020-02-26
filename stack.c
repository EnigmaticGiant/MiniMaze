#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "da.h"

typedef struct stack{
	DA *da;
	void (*display)(void *, FILE *);
    int debug;
    void (*free)(void *);
}STACK;

STACK *newSTACK(void){
	STACK *s = (void *)malloc(sizeof(STACK));
	s->da = newDA();
	s->debug = 0;
	s->display = 0;
	s->free = 0;
	return s;
};

void  setSTACKdisplay(STACK *items,void (*d)(void *,FILE *)){
	items->display = d;
	return;
}

void  setSTACKfree(STACK *items,void (*f)(void *)){
	items->free = f;
	return;
}

void  push(STACK *items,void *value){
	insertDA(items->da, sizeDA(items->da), value);
	return;
}

void *pop(STACK *items){
	void * v = removeDA(items->da, sizeDA(items->da)-1);
	return v;
}

void *peekSTACK(STACK *items){
	int size = sizeDA(items->da);
	return getDA(items->da, size-1);
}

void  displaySTACK(STACK *items,FILE *fp){
	int size = sizeDA(items->da);
	int i = 1;
	setDAdisplay(items->da, items->display);
	if(items->debug == 1){
		displayDA(items->da, fp);
	}
	else if(items->debug == 2){
		int old; 
		old = debugDA(items->da, 1);
		displayDA(items->da, fp);
		old = debugDA(items->da, old);
	}
	else{
		fprintf(fp,"|");
		for(i=size; i>0; i--){
			items->display(getDA(items->da, i-1), fp);
			if(i-2 >= 0){
				fprintf(fp, ",");
			}
		}
		fprintf(fp,"|");
	}
	return;
}

int debugSTACK(STACK *items,int level){
	int old = items->debug;
	items->debug = level;
	return old;
}

void freeSTACK(STACK *items){
	setDAfree(items->da, items->free);
	//if(items->free){
		freeDA(items->da);
	//}
	free(items);
	return;
}

int sizeSTACK(STACK *items){
	return sizeDA(items->da);
}
