#include <stdlib.h>
#include "stack_queue.h"

void stack_init(Stack* s) {
    s->top = NULL;
    s->size = 0;
}

bool stack_is_empty(Stack* s) {
    return s->top == NULL;
}

void stack_push(Stack* s, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    
    newNode->data = data;
    newNode->next = s->top;
    s->top = newNode;
    s->size++;
}

int stack_pop(Stack* s, bool* success) {
    if (stack_is_empty(s)) {
        *success = false;
        return 0;
    }
    
    Node* temp = s->top;
    int data = temp->data;
    s->top = s->top->next;
    free(temp);
    s->size--;
    
    *success = true;
    return data;
}

int stack_peek(Stack* s, bool* success) {
    if (stack_is_empty(s)) {
        *success = false;
        return 0;
    }
    
    *success = true;
    return s->top->data;
}

void stack_clear(Stack* s) {
    bool success;
    while (!stack_is_empty(s)) {
        stack_pop(s, &success);
    }
}

void queue_init(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

bool queue_is_empty(Queue* q) {
    return q->front == NULL;
}

void queue_enqueue(Queue* q, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    
    newNode->data = data;
    newNode->next = NULL;
    
    if (queue_is_empty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

int queue_dequeue(Queue* q, bool* success) {
    if (queue_is_empty(q)) {
        *success = false;
        return 0;
    }
    
    Node* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    *success = true;
    return data;
}

int queue_peek(Queue* q, bool* success) {
    if (queue_is_empty(q)) {
        *success = false;
        return 0;
    }
    
    *success = true;
    return q->front->data;
}

void queue_clear(Queue* q) {
    bool success;
    while (!queue_is_empty(q)) {
        queue_dequeue(q, &success);
    }
}
