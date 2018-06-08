#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../src/z_circular_buffer/z_circular_buffer.h"

#define LOOP_COUNT ( 4096 << 10 )
#define PRODUCER_THREAD_NUM 1
#define CONSUMER_THREAD_NUM 1
#define PERF_TRHEAD_NUM 1
#define TOTAL_THREAD_NUM ( PRODUCER_THREAD_NUM + \
                           CONSUMER_THREAD_NUM + \
                           PERF_TRHEAD_NUM)

char *produce_data = "Produce,dataextrabytes";
char consume_buffer[20];

struct thread_arg {
    int i;
    struct z_cirbuf *cb;
};

void *
produce_thread_fn(void *arg)
{
    printf("Produce thread created\n");
    int i = ((struct thread_arg *)arg)->i;
    uint32_t size = strlen(produce_data);
    struct z_cirbuf *cb = ((struct thread_arg *)arg)->cb;
    while(1) {
        z_cirbuf_produce(cb, (void *)produce_data, size);
        //i--;
    }
    return NULL;
}

void *
consume_thread_fn(void *arg)
{
    printf("Consume thread created\n");
    int i = ((struct thread_arg *)arg)->i;
    uint32_t size = strlen(produce_data);
    struct z_cirbuf *cb = ((struct thread_arg *)arg)->cb;
    while(1) {
        z_cirbuf_consume(cb, (void *)consume_buffer, size);
        //i--;
    }
    return NULL;
}

void *
perf_thread_fn(void *arg)
{
    printf("Perf thread created\n");
    uint64_t old_write = 0;
    uint64_t new_write = 0;
    uint64_t old_read = 0;
    uint64_t new_read = 0;

    struct z_cirbuf *cb = (struct z_cirbuf *)arg;
    while(1) {
        pthread_spin_lock(&cb->lock);
        new_write = cb->stat.w_bytes;
        new_read = cb->stat.r_bytes;
        pthread_spin_unlock(&cb->lock);
        printf("write rate: %d bytes/sec\n", (new_write - old_write));
        printf("read rate: %d bytes/sec\n", (new_read - old_read));
        old_write = new_write;
        old_read = new_read;
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t pids[TOTAL_THREAD_NUM];

    struct z_cirbuf *cb = z_cirbuf_create(4096);

    struct thread_arg prod_arg = {.i = LOOP_COUNT, .cb = cb};
    struct thread_arg cons_arg = {.i = LOOP_COUNT, .cb = cb};

    if (cb == NULL) {
        fprintf(stderr, "circular buffer create failed\n");
        exit(0);
    }
    int i;
    for(i = 0; i < PRODUCER_THREAD_NUM; i++) {
        pthread_create(&pids[i], NULL, produce_thread_fn, (void *)&prod_arg);
    }

    for(i = 0; i < CONSUMER_THREAD_NUM; i++) {
        pthread_create(&pids[PRODUCER_THREAD_NUM + i], NULL, consume_thread_fn, (void *)&cons_arg);
    }

    pthread_create(&pids[PRODUCER_THREAD_NUM + CONSUMER_THREAD_NUM], NULL, perf_thread_fn, (void *)cb);

    for(i = 0; i < TOTAL_THREAD_NUM; i++) {
        pthread_join(pids[i], NULL);
    }

    printf("Text: %s\n", consume_buffer);
    printf("Total write bytes: %ld\n", cb->stat.w_bytes);
    printf("Total read bytes: %ld\n", cb->stat.r_bytes);
    printf("Total times of full: %ld\n", cb->stat.nr_full);
    printf("Total times of empty: %ld\n", cb->stat.nr_empty);

    z_cirbuf_destroy(cb);

    return 0;
}
