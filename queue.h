#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>

struct queue_t {
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_pop;
};

void queue_init(struct queue_t* queue) {
    queue->head = 0;
    queue->tail = 0;

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&queue->mutex, &mutex_attr);

    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);

    pthread_cond_init(&queue->can_pop, &cond_attr);
}

void queue_destroy(struct queue_t* queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->can_pop);
}

void queue_push(struct queue_t* queue, uint64_t val) {
    pthread_mutex_lock(&queue->mutex);
    ((uint64_t*)(queue+22))[queue->tail] = val;
    queue->tail++;
    //printf("~%d\n", queue->tail);
    pthread_cond_broadcast(&queue->can_pop);
    //printf("unlock\n");
    pthread_mutex_unlock(&queue->mutex);
    //printf("exit\n");
}

uint64_t queue_pop(struct queue_t* queue) {
    pthread_mutex_lock(&queue->mutex);
    //printf("pop in %d %d\n", queue->head, queue->tail);
    while (queue->head >= queue->tail) {
    //    printf("cycle\n");
        pthread_cond_wait(&queue->can_pop, &queue->mutex);
    }
    //printf("pop out%d %d\n", queue->head, queue->tail);
    uint64_t val = ((uint64_t*)(queue+22))[queue->head];
    queue->head++;
    pthread_mutex_unlock(&queue->mutex);
    return val;
}
