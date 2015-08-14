#include <stdlib.h>
#include "minheap.h"

static int min_heap_reserve(min_heap_t *s, int n); 
static void min_heap_shift_up(min_heap_t *s, int hole_index, void *e); 
static void min_heap_shift_down(min_heap_t *s, int hole_index, void *e); 

min_heap_t *min_heap_ctor(int (*f)(void *, void *))
{
	min_heap_t *s = malloc(sizeof(min_heap_t)); 
	if(!s)
		return 0; 
	s->p = 0; 
	s->n = 0; 
	s->a = 0; 
	s->f = f; 
	return s; 
}

void min_heap_dtor(min_heap_t *s)
{
	if (s->p)
		free(s->p); 
	free(s); 
}

int min_heap_empty(min_heap_t *s) { return 0 == s->n; }

int min_heap_size(min_heap_t *s) { return s->n; }

void *min_heap_top(min_heap_t *s) { return s->n ? *s->p : 0; }

int min_heap_push(min_heap_t *s, void *e)
{
	if (min_heap_reserve(s, s->n + 1))
		return -1; 
	min_heap_shift_up(s, s->n++, e); 
	return 0; 
}

void *min_heap_pop(min_heap_t *s)
{
	if (s->n)
	{
		void *e = *s->p; 
		min_heap_shift_down(s, 0, s->p[--s->n]); 
		*(int *)e = -1; 
		return e; 
	}
	return 0; 
}

int min_heap_elt_is_top(const void *e)
{
	return *(int *)e == 0; 
}

int min_heap_erase(min_heap_t *s, void *e)
{
	if (-1 != *(int *)e)
	{
		void *last = s->p[--s->n]; 
		int parent = (*(int *)e - 1) / 2; 
		/* we replace e with the last element in the heap.  We might need to
		shift it upward if it is less than its parent, or downward if it is
		greater than one or both its children. Since the children are known
		to be less than the parent, it can't need to shift both up and
		down. */
		if (*(int *)e > 0 && s->f(s->p[parent], last) > 0)
			min_heap_shift_up(s, *(int *)e, last); 
		else
			min_heap_shift_down(s, *(int *)e, last); 
		*(int *)e = -1; 
		return 0; 
	}
	return -1; 
}

int min_heap_reserve(min_heap_t *s, int n)
{
	if (s->a < n)
	{
		void **p; 
		int a = s->a ? s->a * 2 : 8; 
		if (a < n)
			a = n; 
		if (!(p = realloc(s->p, a * sizeof *p)))
			return -1; 
		s->p = p; 
		s->a = a; 
	}
	return 0; 
}

void min_heap_shift_up(min_heap_t *s, int hole_index, void *e)
{
	int parent = (hole_index - 1) / 2; 
	while (hole_index && s->f(s->p[parent], e) > 0)
	{
		*(int *)(s->p[hole_index] = s->p[parent]) = hole_index; 
		hole_index = parent; 
		parent = (hole_index - 1) / 2; 
	}
	*(int *)(s->p[hole_index] = e) = hole_index; 
}

void min_heap_shift_down(min_heap_t *s, int hole_index, void *e)
{
	int min_child = 2 * (hole_index + 1); 
	while (min_child <= s->n)
	{
		min_child -= min_child == s->n || s->f(s->p[min_child], s->p[min_child - 1]) > 0; 
		if (!(s->f(e, s->p[min_child]) > 0))
			break; 
		*(int *)(s->p[hole_index] = s->p[min_child]) = hole_index; 
		hole_index = min_child; 
		min_child = 2 * (hole_index + 1); 
	}
	*(int *)(s->p[hole_index] = e) = hole_index; 
}
