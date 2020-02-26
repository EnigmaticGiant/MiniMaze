#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct da {
    void **array; //an array of void pointers
    int capacity; //total number of slots
    int size; //number of filled slots
    void (*display)(void *, FILE *);
    int debug;
    void (*free)(void *);
}DA;

DA *newDA(void){
	DA *new = malloc(sizeof(DA));
	assert(new != 0);
	new->array = (void *)malloc(sizeof(void *));
	new->capacity =1;
	new->size = 0;
	new->debug = 0;
    new->free = 0;
    new->display = 0;
	return new;
};

void  setDAdisplay(DA *items,void (*d)(void *, FILE *)){
	items->display = d;
	return;
}

void  setDAfree(DA *items,void (*f)(void *)){
	items->free = f;
	return;
}

int   sizeDA(DA *items){
	return items->size;
}

void  insertDA(DA *items,int index,void *value){
    int slot;
    void *temp1;
    void *temp2;
	if (items->size == items->capacity) {
        items->capacity *= 2;
        items->array = realloc(items->array,sizeof(void *)*items->capacity);
    }
    if(items->size == index){
    	items->array[items->size] = value;
    }
    else{
    	slot = index;
    	temp1 = items->array[slot];
    	items->array[slot] = value;
    	while(slot < items->size){
    		temp2 = items->array[slot+1];
       		items->array[slot+1] = temp1;
    		temp1 = temp2;
    		slot++;
    	}
    }
    slot = 0;
    items->size++;
	return;
}

void *removeDA(DA *items,int index){
	if (items->size == 0)
        return NULL;
    void *v = items->array[index];
    if(items->size==1 && items->capacity == 2){
        items->array[0] = NULL;
        items->capacity /= 2;
        items->array = realloc(items->array, sizeof(void *)*items->capacity);
        return v;
    }
    int slot = index;
    void *temp;
    while(slot < items->size - 1){
        temp = items->array[slot + 1];
    	items->array[slot] = temp;
    	slot++;
    	//temp = items->array[slot + 1];
    }
    items->size--;
    if(items->size == 0){
    	items->capacity = 1;
    }
    else if (items->capacity != 1 && (items->size < (items->capacity/4))) {
        items->capacity /= 2;
        items->array = realloc(items->array, sizeof(void *)*items->capacity);
    }
    return v;
}

void *getDA(DA *items,int index){
	return items->array[index];
}

void  unionDA(DA *recipient,DA *donor){
	int size = sizeDA(donor);
    int i = 0;
	while(i < size){
        void *temp = getDA(donor, i);

		insertDA(recipient, recipient->size, temp);
        i++;
	}
//    void **empty = (void *)malloc(sizeof(void));
    free(donor->array);
    donor->array = (void *)malloc(sizeof(void *));
    donor->size = 0;
    donor->capacity = 1;
    //freeDA()
    return;
}



void *setDA(DA *items,int index,void *value){
	void *temp = NULL;
	if(items->size == index){
		insertDA(items, items->size, value);
	}
	else{
		temp = items->array[index];
		items->array[index] = value;
	}
	return temp;
}


void  displayDA(DA *items,FILE *fp){
	int i;
	fprintf(fp,"[");
    for(i = 0; i < items->size; i++) {
    	if(items->display == NULL){
    		void *test = &items->array[i];
    		fprintf(fp, "@%p", test);
    	}
    	else{
        	items->display(getDA(items, i), fp);
    	}
    	if (i < sizeDA(items) - 1)
            fprintf(fp,",");
    }
    if(items->debug > 0){
    	if(items->size > 0 && items->capacity >= 1) fprintf(fp, ",");
    	fprintf(fp, "[%d]", items->capacity - items->size);
    }
    fprintf(fp,"]");
	return;
}

int   debugDA(DA *items,int level){
	int old = items->debug;
	items->debug = level;
	return old;
}

void  freeDA(DA *items){
	int i;
    if(items->free){
    //printf("freeing da\n");
		for(i = 0; i < items->size; i++){
            //printf("freeing item at slot %d\n",i);
			items->free(items->array[i]);
		}
	}
    items->size = 0;
    items->capacity = 0;
    items->debug = 0;
	free(items->array);
	free(items);
	return;
}