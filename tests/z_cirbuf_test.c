#include <stdlib.h>
#include <stdio.h>
#include "../src/z_circular_buffer/z_circular_buffer.h"

int main() {
    struct z_cirbuf *cb = z_cirbuf_create(4096);
    printf("size of int: %d\n", sizeof(int));
    printf("size of z_cirbuf: %d\n", sizeof(struct z_cirbuf));
    printf("size of void *: %d\n", sizeof(void *));
    if (cb == NULL) {
    }
    return 0;
}
