#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"

struct mydata
{
	int index; 
	int value; 
}; 

int cmp(void *a, void *b)
{
	struct mydata *p1 = (struct mydata *)a, *p2 = (struct mydata *)b; 
	if(p1->value > p2->value)
		return 1; 
	else
		return 0; 
}

int main(int argc, char *argv[])
{
	int arr[] = {11,4,7,3,8,1,9,5,2}; 
	int N = sizeof(arr)/sizeof(arr[0]); 
	struct mydata *e = malloc(N * sizeof(struct mydata)); 
	int i; 
	struct mydata *p; 
	min_heap_t *s = min_heap_ctor(cmp); 

	for(i = 0; i < N; i++)
	{
		e[i].value = arr[i]; 
		e[i].index = -1; 
	}

	for(i = 0; i < N; i++)
		min_heap_push(s, e + i); 

	for(i = 0; i < N; i++)
		printf("%d ", ((struct mydata *)s->p[i])->index); 
	printf("\n"); 

	while(min_heap_size(s) > 0)
	{
		p = min_heap_pop(s); 
		printf("%d ", p->value); 
	}
	printf("\n"); 

	min_heap_dtor(s); 

	return 0; 
}

