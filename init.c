#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "queue.h"
#include <inttypes.h>
#include <sys/shm.h>
#include <stddef.h>
#include <stdlib.h>

int main() {
    void* shmem = shmat(524338, NULL, 0);
    struct queue_t* queue = (struct queue_t*)shmem;
    queue_init(queue);
}
