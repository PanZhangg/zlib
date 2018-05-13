#include <stdlib.h>
#include <stdio.h>
#include "../z_circular_buffer/z_circular_buffer.h"

int main() {
    struct z_cirbuf *cb = z_cirbuf_create(4096);
    if (cb == NULL) {
    }
    return 0;
}
