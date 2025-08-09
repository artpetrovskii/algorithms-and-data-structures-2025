#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

// ==================== Стек на векторе ====================
typedef struct {
    int* data;
    int capacity;
    int top;
} VectorStack;

VectorStack* vector_stack_create(int capacity) {
    VectorStack* stack = (VectorStack*)malloc(sizeof(VectorStack));
    stack->data = (int*)malloc(capacity * sizeof(int));
    stack->capacity = capacity;
    stack->top = -1;
    return stack;
}

void vector_stack_free(VectorStack* stack) {
    free(stack->data);
    free(stack);
}

bool vector_stack_is_empty(VectorStack* stack) {
    return stack->top == -1;
}

bool vector_stack_is_full(VectorStack* stack) {
    return stack->top == stack->capacity - 1;
}

void vector_stack_push(VectorStack* stack, int value) {
    if (vector_stack_is_full(stack)) {
        printf("Stack overflow!\n");
        return;
    }
    stack->data[++stack->top] = value;
}

int vector_stack_pop(VectorStack* stack) {
    if (vector_stack_is_empty(stack)) {
        printf("Stack underflow!\n");
        return -1;
    }
    return stack->data[stack->top--];
}

int vector_stack_peek(VectorStack* stack) {
    if (vector_stack_is_empty(stack)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return stack->data[stack->top];
}

// ==================== Стек на списке ====================
typedef struct ListNode {
    int data;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* top;
} ListStack;

ListStack* list_stack_create() {
    ListStack* stack = (ListStack*)malloc(sizeof(ListStack));
    stack->top = NULL;
    return stack;
}

void list_stack_free(ListStack* stack) {
    ListNode* current = stack->top;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
}

bool list_stack_is_empty(ListStack* stack) {
    return stack->top == NULL;
}

void list_stack_push(ListStack* stack, int value) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->data = value;
    node->next = stack->top;
    stack->top = node;
}

int list_stack_pop(ListStack* stack) {
    if (list_stack_is_empty(stack)) {
        printf("Stack underflow!\n");
        return -1;
    }
    ListNode* temp = stack->top;
    int value = temp->data;
    stack->top = temp->next;
    free(temp);
    return value;
}

int list_stack_peek(ListStack* stack) {
    if (list_stack_is_empty(stack)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return stack->top->data;
}

// ==================== Тестирование ====================
void test_vector_stack() {
    printf("Testing Vector Stack...\n");
    
    VectorStack* stack = vector_stack_create(5);
    
    // Тест пустого стека
    assert(vector_stack_is_empty(stack));
    assert(!vector_stack_is_full(stack));
    
    // Тест добавления элементов
    vector_stack_push(stack, 10);
    assert(vector_stack_peek(stack) == 10);
    assert(!vector_stack_is_empty(stack));
    
    vector_stack_push(stack, 20);
    vector_stack_push(stack, 30);
    assert(vector_stack_peek(stack) == 30);
    
    // Тест извлечения элементов
    assert(vector_stack_pop(stack) == 30);
    assert(vector_stack_peek(stack) == 20);
    
    assert(vector_stack_pop(stack) == 20);
    assert(vector_stack_pop(stack) == 10);
    assert(vector_stack_is_empty(stack));
    
    // Тест переполнения
    for (int i = 0; i < 5; i++) {
        vector_stack_push(stack, i);
    }
    assert(vector_stack_is_full(stack));
    vector_stack_push(stack, 100); // Должно вывести "Stack overflow!"
    
    vector_stack_free(stack);
    printf("Vector Stack tests passed!\n\n");
}

void test_list_stack() {
    printf("Testing List Stack...\n");
    
    ListStack* stack = list_stack_create();
    
    // Тест пустого стека
    assert(list_stack_is_empty(stack));
    
    // Тест добавления элементов
    list_stack_push(stack, 10);
    assert(list_stack_peek(stack) == 10);
    assert(!list_stack_is_empty(stack));
    
    list_stack_push(stack, 20);
    list_stack_push(stack, 30);
    assert(list_stack_peek(stack) == 30);
    
    // Тест извлечения элементов
    assert(list_stack_pop(stack) == 30);
    assert(list_stack_peek(stack) == 20);
    
    assert(list_stack_pop(stack) == 20);
    assert(list_stack_pop(stack) == 10);
    assert(list_stack_is_empty(stack));
    
    // Тест извлечения из пустого стека
    assert(list_stack_pop(stack) == -1); // Должно вывести "Stack underflow!"
    
    // Тест большого количества элементов
    for (int i = 0; i < 1000; i++) {
        list_stack_push(stack, i);
    }
    assert(!list_stack_is_empty(stack));
    
    list_stack_free(stack);
    printf("List Stack tests passed!\n\n");
}

void performance_test() {
    printf("Performance Testing...\n");
    const int N = 100000;
    clock_t start, end;
    
    // Тест производительности стека на векторе
    VectorStack* vstack = vector_stack_create(N);
    start = clock();
    for (int i = 0; i < N; i++) {
        vector_stack_push(vstack, i);
    }
    for (int i = 0; i < N; i++) {
        vector_stack_pop(vstack);
    }
    end = clock();
    printf("Vector Stack time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    vector_stack_free(vstack);
    
    // Тест производительности стека на списке
    ListStack* lstack = list_stack_create();
    start = clock();
    for (int i = 0; i < N; i++) {
        list_stack_push(lstack, i);
    }
    for (int i = 0; i < N; i++) {
        list_stack_pop(lstack);
    }
    end = clock();
    printf("List Stack time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    list_stack_free(lstack);
    
    printf("Performance tests completed!\n\n");
}

int main() {
    test_vector_stack();
    test_list_stack();
    performance_test();
    return 0;
}