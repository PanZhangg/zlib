#include <stdlib.h>
#include <stdio.h>
#include "../src/z_circular_buffer/z_circular_buffer.h"

int main() {
    struct z_cirbuf *cb = z_cirbuf_create(4096);
    if (cb == NULL) {
        fprintf(stderr, "circular buffer create failed\n");
        exit(0);
    }
    char *produce_data = "Produce data";
    char consume_buffer[20];
    int ret = z_cirbuf_produce(cb,(void *)produce_data, sizeof(*produce_data));
    ret = z_cirbuf_consume(cb, (void*)consume_buffer, sizeof(*produce_data));

    printf("Text: %s\n", consume_buffer);
    z_cirbuf_destroy(cb);
    return 0;
}
