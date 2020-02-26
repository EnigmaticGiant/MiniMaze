
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "cda.h"

typedef struct queue{
	CDA *cda;
	void (*display)(void *, FILE *);
	int debug;
	void (*free)(void *);
}QUEUE;

QUEUE *newQUEUE(void){
	QUEUE *q = (void *)malloc(sizeof(QUEUE));
	q->cda = newCDA();
	q->debug = 0;
	q->free = 0;
	q->display = 0;
	return q;
}

int   sizeQUEUE(QUEUE *items){
	return sizeCDA(items->cda);
}

void  setQUEUEdisplay(QUEUE *items,void (*d)(void *,FILE *)){
	items->display = d;
	return;
}

void  setQUEUEfree(QUEUE *items,void (*f)(void *)){
	items->free = f;
	return;
}

void  enqueue(QUEUE *items,void *value){
	insertCDAback(items->cda, value);
	//printf("insert sucessful\n");
	return;
}

void *dequeue(QUEUE *items){
	void *temp = removeCDAfront(items->cda);
	return temp;
}

void *peekQUEUE(QUEUE *items){
	void *result = getCDA(items->cda, 0);
	return result;
}

void  displayQUEUE(QUEUE *items,FILE *fp){
	int i;
	setCDAdisplay(items->cda, items->display);
	if(items->debug == 1){
		displayCDA(items->cda, fp);
	}
	else if(items->debug == 2){
		int old;
		old = debugCDA(items->cda, 1);
		displayCDA(items->cda, fp);
		old = debugCDA(items->cda, old);
	}
	else{
		//printf("printing queue now \n");
		fprintf(fp, "<");
		for(i=0; i < sizeQUEUE(items); i++){
			//printf("getting the value at the spot %d\n", i);
			items->display(getCDA(items->cda, i), fp);
			if(i+2 <= sizeQUEUE(items)){
				fprintf(fp, ",");
			}
		}
		fprintf(fp, ">");
	}
	return;
}

int   debugQUEUE(QUEUE *items,int level){
	int old = items->debug;
	items->debug = level;
	return old;
}

void  freeQUEUE(QUEUE *items){
	setCDAfree(items->cda, items->free);
	//if(items->free){
		freeCDA(items->cda);
	//}
	free(items);
	return;
}
