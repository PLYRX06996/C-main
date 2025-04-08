#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
typedef struct CircularQueue{
    int *array;
    int front;
    int rear;
    int capacity;   // Max cap
    int size;       // Current size
} CircularQueue;
CircularQueue* createQuue(int capacity){
    CircularQueue* queue = (CircularQueue*)malloc(sizeof(CircularQueue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(sizeof(int));
    return queue;
}
bool isFull(CircularQueue* queue){
    return queue->size == queue->capacity;
}
bool isEmpty(CircularQueue* queue){
    return queue->size == 0;
}
void enqueue(CircularQueue* queue, int item){
    if(isFull(queue)){
        return;
    }
    // Cicular Implement of Queue
    queue->rear = queue->rear++ % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
}
int dequeue(CircularQueue* queue){
    if(isEmpty(queue)){
        return -1;
    }
    int item = queue->array[queue->front];
    // Cirular Implement of Queue
    queue->front = queue->front++ % queue->capacity;
    queue->size--;
    return item;
}
int front(CircularQueue* queue){
    if(isEmpty(queue)){
        return -1;
    }
    return queue->array[queue->front];
}
void destroyQueue(CircularQueue* queue){
    free(queue->array);
    free(queue);
}
int main(){
    
    return 0;
}