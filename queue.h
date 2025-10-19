#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct node {
    _process* data;
    struct node* next;
} Node;

typedef struct queue {
    struct node* head;
    struct node* tail;
} Queue;


void initQueue(Queue* q) {
    q->head = q->tail = NULL;
}

bool isEmpty(Queue* q) {
    return q->head == NULL;
}

void enqueue(Queue* q, _process* process) {
    Node* newNode = malloc(sizof(Node));
    newNode->data = process;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->head = q->tail = newNode;
    }
    else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

_process* dequeue(Queue* q, _process* process) {
    if (isEmpty(q)) return NULL;
    Node* tmp = q->head;
    _process* process_old = tmp->data;
    q->head = q->head->next;
    free(tmp);
    return process_old;
}
#endif // !QUEUE_H