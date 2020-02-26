#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cda.h"
#include <assert.h>



typedef struct cda{
	void **array;
	int capacity;
	int size;
	int front;
	int back;
	void (*display)(void *, FILE *);
	int debug;
	void (*free)(void *);
}CDA;

static void growArray(CDA *items);
static void shrinkArray(CDA *items);
static int frontOrBack(int size, int index);


CDA *newCDA(void){
	CDA *x = malloc(sizeof(CDA));
	assert(x !=0);
	x->array = (void *)malloc(sizeof(void *));
	x->size = 0;
	x->capacity = 1;
	x->front = 0;
	x->back = 0;
	x->debug = 0;
	x->display = 0;
	x->free = 0;
	return x;
}

void setCDAdisplay(CDA *items, void (*d)(void *,FILE *)){
	items->display = d;
	return;
}

void setCDAfree(CDA *items, void (*f)(void *)){
	items->free = f;
	return;
}

void *getCDA(CDA *items,int index){
	int spot = (index + items->front) % items->capacity;
	return items->array[spot];
}

void insertCDA(CDA *items,int index,void *value){
	if(items->size == items->capacity){
		growArray(items);
	}
	if(items->size == 0){
    	items->array[items->size] = value;
    	items->size++;
    	return;
    }
   int adjusted = (index + items->front)%items->capacity;
    int result = frontOrBack(items->size, index);
    items->size++;
    if(result == 0){
    	//printf("inserting to the front \n");
    	items->front--;
    	if(items->front < 0){
    		items->front = (items->front + items->capacity)%items->capacity;
    	}
    	int prev = adjusted - 1;
    	if(prev < 0){
    		prev = items->capacity-1;
    	}
    	if(index == 0){
    		items->array[items->front] = value;
    	}
    	else{
    		int wall = items->front;
    		int start = prev; 
    		void *temp1;
    		void *temp2;
    		temp1 = items->array[start];
    		while(start != wall){
    			if(start == 0){
    				start = items->capacity -1;
    				temp2 = items->array[start];
    				items->array[start] = temp1;
    				temp1 = temp2;
    			}
    			else{
    				start--;
    				temp2 = items->array[start];
    				items->array[start] = temp1;
    				temp1 = temp2;
    			}
    		}
    		items->array[prev] = value;
    	}
    }
    else{ //inserting into the back
    	items->back++;
    	if(items->back > items->capacity-1){
    		items->back = 0;	
    	}
    	if(index >= items->size-1){
    		items->array[items->back] = value;
    	}
    	else{
    		int wall = items->back;
    		int start = adjusted; 
    		void *temp1;
    		void *temp2;
   		 	temp1 = items->array[start];
	    	while(start != wall){
   		 		if(start == items->capacity){
    				start = 0;
  		  			temp2 = items->array[start];
    				items->array[start] = temp1;
    				temp1 = temp2;
    			}
    			else{
    				start++;
    				temp2 = items->array[start];
    				items->array[start] = temp1;
    				temp1 = temp2;
    			}
    		}
    		items->array[adjusted] = value;
    	}
    }
	return;
}

void *removeCDA(CDA *items,int index){
	void *v = NULL;
	if(items->size == 1){
		v = getCDA(items, 0);
		items->array[0] = NULL;
		items->size--;
		items->capacity = 1;
		return v;
	}
	int adjusted = (index + items->front)%items->capacity;
    v = items->array[adjusted];
		int result = frontOrBack(items->size, index);
		if(result == 0){  //remove from the front
			if(index == 0){
				items->array[items->front] = NULL;
			}
			else{
				int i;
				for(i=adjusted; i != items->front; i--){
					if(i==0){
						void *temp = items->array[items->capacity-1];
						items->array[0] = temp;
						i = items->capacity;
					}
					else items->array[i] = items->array[i-1];
				}
			}
			items->front = (items->front+1)%items->capacity;
		}
		else{      //remove from the back
			if(index >= items->size-1){
				items->array[items->back] = NULL;
			}
			else{
				int i;
				int wall = items->back;
				for(i = adjusted; i != wall; i++){
					if(i >= items->capacity -1){
						void *temp = items->array[0];
						items->array[items->capacity-1] = temp;
						i = -1;
					}
					else{
						items->array[i] = items->array[i+1];
					}
				}
			}
			items->back = (items->back-1 +items->capacity) % items->capacity;
		}
		items->size--;
		//printf("the item size is %d \n", items->size);
		if(items->capacity !=1 && (items->size < (items->capacity/4))){
    		shrinkArray(items);
    	}
	return v;
}

void unionCDA(CDA *recipient,CDA *donor){
	int i;
	for(i = 0; i < donor->size; i++){
		insertCDAback(recipient, getCDA(donor, i));
	}
	//void **empty = (void*)malloc(sizeof(void));
	free(donor->array);
	donor->array = (void *)malloc(sizeof(void *));
	donor->size = 0;
	donor->capacity = 1;
	return;
}


void *setCDA(CDA *items,int index,void *value){
	void *replaced = NULL;
	if(index == -1){
		insertCDAfront(items, value);
	}
	else if(index == sizeCDA(items)){
		insertCDAback(items, value);
	}
	else{
		int spot = (index + items->front) % items->capacity;
		replaced = items->array[spot];
		items->array[spot] = value;
	}
	return replaced;
}

int  sizeCDA(CDA *items){
	return items->size;
}

void displayCDA(CDA *items,FILE *fp){
	int i = 0;
	int countdown = sizeCDA(items);
	int spot;
	fprintf(fp,"(");
	for(i=0;i<items->size;i++){
		if(items->display == NULL){
			spot = (i + items->front) % items->capacity;
			void *address = &items->array[spot];
			fprintf(fp, "@%p", address);
		}
		else{
			spot = (i + items->front) % items->capacity;
			items->display(items->array[spot], fp);
		}
		countdown--;
		if(countdown != 0){
			fprintf(fp, ",");
		}
	}
	if(items->debug > 0){
		if(items->size > 0 && items->capacity >= 1) fprintf(fp, ",");
		fprintf(fp, "(%d)", items->capacity - items->size);
	}
    fprintf(fp,")");
	return;
}

int debugCDA(CDA *items,int level){
	int old;
	old = items->debug;
	items->debug = level;
	return old;
}

void freeCDA(CDA *items){
	if(items->free){
		int i;
		for(i = 0; i < items->size; i++){
			items->free(getCDA(items, i));
		}
	}
	free(items->array);
	free(items);
	return;
}

static void growArray(CDA *items){
	void** temp;
	temp = (void *)malloc(sizeof(void*) * items->capacity*2);
	int i;
	for(i=0; i<items->size; i++){
		temp[i] = getCDA(items,i);
	}
	free(items->array);
	items->array = temp;
	items->front = 0;
	items->back = sizeCDA(items) -1;
	items->capacity *=2;
}

static void shrinkArray(CDA *items){
	void** temp;
	temp = (void *)malloc(sizeof(void *) *items->capacity/2);
	int i;
	for(i=0; i<items->size; i++){
		temp[i] = getCDA(items,i);
	}
	free(items->array);
	items->array = temp;
	items->front = 0;
	items->back = sizeCDA(items) -1;
	items->capacity /=2;	
}

static int frontOrBack(int size, int index){
	int middle = size/2;
	if(middle > index) return 0;
	else return 1;
}