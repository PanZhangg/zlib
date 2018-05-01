#ifndef Z_CIRCULAR_BUFFER
#define Z_CIRCULAR_BUFFER

#include <stdint.h>

struct buffer_slot {
    uint16_t size;
    uint16_t data[0];
};

struct z_circular_buffer {
    uint32_t write_seq;
    uint32_t buffer_size;
    uint32_t n_buffer_slot;
    struct buffer_slot *buffer_head;
    uint32_t read_seq;
};

#endif
