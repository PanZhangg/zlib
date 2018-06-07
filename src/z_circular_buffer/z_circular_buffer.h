#ifndef Z_CIRCULAR_BUFFER
#define Z_CIRCULAR_BUFFER

#include <stdint.h>
#include "../utilities/utilities.h"

struct z_cirbuf_stat {
    uint64_t w_bytes;
    uint64_t r_bytes;
    uint64_t nr_full;
    uint64_t nr_empty;
};
/* Locate the most frequently written value
 * align with cache line
 */
struct z_cirbuf {
    /* cacheline0 */
    uint32_t w;
    void *buffer;
    uint32_t buffer_size;
    int fd;
    struct z_cirbuf_stat stat;
    uint32_t padding1[2];
    /* cacheline1 */
    uint32_t r;
    uint32_t padding2;
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
