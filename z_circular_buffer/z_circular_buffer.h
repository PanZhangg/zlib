#ifndef Z_CIRCULAR_BUFFER
#define Z_CIRCULAR_BUFFER

#include <stdint.h>

struct z_cirbuf{
    uint32_t w;
    uint32_t buffer_size;
    int fd;
    void *buffer;
    uint32_t padding[4];
    uint32_t r;
};

struct z_cirbuf *
z_cirbuf_create(uint32_t buffer_size);

void
z_cirbuf_destroy(struct z_cirbuf *cb);

int
z_cirbuf_produce(struct z_cirbuf *cb, void *data, uint32_t size);

int
z_cirbuf_consume(struct z_cirbuf *cb, void *ret_buf, uint32_t size);

#endif
