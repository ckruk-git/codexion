#include "codexion.h"

Heap *createHeap(int capacity, int scheduler)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->coder = malloc(capacity * sizeof(t_coder *));
    heap->scheduler = scheduler;
    heap->order = 1;
    return heap;
}

void swap(t_coder **a, t_coder **b)
{
    t_coder *temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Heap *heap, int i)
{
    int best;
    int left;
    int right;

    best = i;
    left = 2 * i + 1;
    right = 2 * i + 2;

    if (left < heap->size
        && higher_priority(heap->coder[left], heap->coder[best]))
        best = left;
    if (right < heap->size
        && higher_priority(heap->coder[right], heap->coder[best]))
        best = right;
    if (best != i)
    {
        swap(&heap->coder[i], &heap->coder[best]);
        heapify(heap, best);
    }
}

void buildHeap(Heap *heap)
{
    int i;

    if (heap->size <= 1)
		return ;
	i = (heap->size - 2) / 2;
    while (i >= 0)
    {
        heapify(heap, i);
        i--;
    }
}

void insertHeap(Heap *heap, t_coder *coder)
{
    int i;

    heap->size++;
    i = heap->size - 1;
    heap->coder[i] = coder;
    coder->order = heap->order;
    heap->order++;
    while (i != 0 && higher_priority(heap->coder[i], heap->coder[(i - 1) / 2]))
    {
        swap(&heap->coder[i], &heap->coder[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}