#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
    int size;
} Stack;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

void stack_init(Stack* s);
bool stack_is_empty(Stack* s);
void stack_push(Stack* s, int data);
int stack_pop(Stack* s, bool* success);
int stack_peek(Stack* s, bool* success);
void stack_clear(Stack* s);
void queue_init(Queue* q);
bool queue_is_empty(Queue* q);
void queue_enqueue(Queue* q, int data);
int queue_dequeue(Queue* q, bool* success);
int queue_peek(Queue* q, bool* success);
void queue_clear(Queue* q);

#endif
