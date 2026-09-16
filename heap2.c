#include "codexion.h"

int extract_root(Heap *heap)
{
    int root;

    if (heap->size <= 0)
        return INT_MIN;
    root = heap->coder[0]->id;
    heap->coder[0] = heap->coder[heap->size - 1];
    heap->size--;
    if (heap->size > 0)
		heapify(heap, 0);
    return root;
}

void deleteKey(Heap *heap, t_coder *coder)
{
    int i;

    i = 0;
    while (i < heap->size && heap->coder[i] != coder)
        i++;
    if (i == heap->size)
        return;
    heap->coder[i] = heap->coder[heap->size - 1];
    heap->size--;
    if (i < heap->size)
        heapify(heap, i);
}

int already_in(Heap *heap, t_coder *coder)
{
    int i;

    i = 0;
    while (i < heap->size)
    {
        if (heap->coder[i]->id == coder->id)
            return 1;
        i++;
    }
    return 0;
}

int higher_priority(t_coder *a, t_coder *b)
{
    long time_a = burnout_calculator(a);
    long time_b = burnout_calculator(b);

    if (a->game->heap->scheduler == 0 || time_a == time_b)
        return (a->order < b->order);
    return (time_a < time_b);
}
