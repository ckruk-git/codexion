#include "codexion.h"

// Function to create a heap
Heap *createHeap(int capacity, int scheduler)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->coder = (int *)malloc(capacity * sizeof(int));
    heap->scheduler = scheduler;
    heap->order = 1;
    return heap;
}

// Function to swap two integers
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify the node at index i
void heapify(Heap *heap, int i)
{
    int best;
    int left;
    int right;

    best = i;
    left = 2 * i + 1;
    right = 2 * i + 2;

    if (left < heap->size
        && higher_priority(heap, heap->coder[left], heap->coder[best]))
        best = left;
    if (right < heap->size
        && higher_priority(heap, heap->coder[right], heap->coder[best]))
        best = right;
    if (best != i)
    {
        swap(&heap->coder[i], &heap->coder[best]);
        heapify(heap, best);
    }
}

// Function to build a max heap from an existing array
void buildHeap(Heap *heap)
{
    int i;
    int n;

    n = heap->size;
    i = (n - 1) / 2;
    while (i >= 0)
    {
        heapify(heap, i);
        i--;
    }
}

// Function to insert a new value into the heap
void insertHeap(Heap *heap, t_coder *coder)
{
    int i;

    heap->size++;
    i = heap->size - 1;
    heap->coder[i] = coder;
    coder->order = heap->coder;
    heap->coder++;

    // Fix the heap property if it is violated
    while (i != 0 && heap->coder[(i - 1) / 2] < heap->coder[i])
    {
        swap(&heap->coder[i], &heap->coder[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Function to extract the root
int extract_root(Heap *heap)
{
    int root;

    if (heap->size <= 0)
        return INT_MIN;
    if (heap->size == 1)
    {
        heap->size--;
        return heap->coder[0];
    }

    root = heap->coder[0];
    heap->coder[0] = heap->coder[heap->size - 1];
    heap->size--;
    heapify(heap, 0);

    return root;
}

// Function to delete an element at a given index
void deleteKey(Heap *heap, t_coder *coder)
{
    int i;

    i = 0;
    while (i < heap->size && heap->coder[i] != coder)
        i++;
    if (i == heap->size)
        return;
    heap->size--;
    if (i == heap->size)
        return;
    heap->coder[i] = heap->coder[heap->size];
    heapify(heap, i);
}

int already_in(Heap *heap, t_coder *coder)
{
    int i;

    i = 0;
    while (i < heap->size)
    {
        if (heap->coder[i].id == coder.id)
            return 1;
        i++;
    }
    return 0;
}

int higher_priority(t_coder *a, t_coder *b)
{
    if (heap->scheduler == 0)
        return (a->queue_order < b->queue_order);
    return (burnout_calculator(a) < burnout_calculator(b));
}