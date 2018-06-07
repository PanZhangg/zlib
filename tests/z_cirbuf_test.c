#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/z_circular_buffer/z_circular_buffer.h"

#define LOOP_COUNT ( 4096 << 10 )

int main() {
    struct z_cirbuf *cb = z_cirbuf_create(4096);
    if (cb == NULL) {
        fprintf(stderr, "circular buffer create failed\n");
        exit(0);
    }
    char *produce_data = "Produce,data";
    char consume_buffer[20];
    int i = LOOP_COUNT;

    while(i) {
        int ret = z_cirbuf_produce(cb, (void *)produce_data, strlen(produce_data));
        ret = z_cirbuf_consume(cb, (void*)consume_buffer, strlen(produce_data));
        i--;
    }

    printf("Text: %s\n", consume_buffer);
    printf("Total write bytes: %ld\n", cb->stat.w_bytes);
    printf("Total read bytes: %ld\n", cb->stat.r_bytes);
    printf("Total times of full: %ld\n", cb->stat.nr_full);
    printf("Total times of empty: %ld\n", cb->stat.nr_empty);

    z_cirbuf_destroy(cb);

    return 0;
}
