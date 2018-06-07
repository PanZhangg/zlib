#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "z_circular_buffer.h"

#define PAGE_SIZE 4096

#ifndef __NR_memfd_create
#define __NR_memfd_create 319
#endif

static int
memfd_create(const char *name, unsigned int flags) {
    return syscall(__NR_memfd_create, name, flags);
}

struct z_cirbuf *
z_cirbuf_create(uint32_t buffer_size)
{
    struct z_cirbuf *cb = (struct z_cirbuf *)memalign(CACHELINE_SIZE, sizeof(struct z_cirbuf));
    if (cb == NULL) {
        perror("malloc failed");
        exit(0);
    }
    if ((uint64_t)cb % 64 != 0) {
        fprintf(stderr, "z_cirbuf addess is not 64 byte aligned\n");
        exit(0);
    }

    if (buffer_size % PAGE_SIZE != 0) {
        return NULL;
    }
    cb->buffer_size = buffer_size;
    cb->w = 0;
    cb->r = 0;
    cb->buffer = NULL;
    cb->fd = memfd_create("z_cirbuf", 0);
    if (cb->fd == -1) {
       perror("memfd_create failed");
       exit(0);
    }
    ftruncate(cb->fd, buffer_size);

    cb->buffer = mmap(NULL, buffer_size << 1,
            PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (cb->buffer == (void *) -1) {
        perror("mmap failed");
    }
    mmap(cb->buffer, buffer_size,
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, cb->fd, 0);
    mmap((char *)cb->buffer + buffer_size, buffer_size,
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, cb->fd, 0);

    return cb;
}

void
z_cirbuf_destroy(struct z_cirbuf *cb) {
    int ret = munmap(cb->buffer, cb->buffer_size);
    if (ret != 0) {
        perror("munmap failed");
    }

    ret = munmap((char *)cb->buffer + cb->buffer_size, cb->buffer_size);
    if (ret != 0) {
        perror("munmap failed");
    }
    free(cb);
}

int
z_cirbuf_produce(struct z_cirbuf *cb, void *data, uint32_t size) {
    if(cb->buffer_size - (cb->w - cb->r) < size) {
        return -1;
    }
    memcpy((char *)cb->buffer + cb->w, data, size);
    cb->w += size;
    return 0;
}

int
z_cirbuf_consume(struct z_cirbuf *cb, void *ret_buf, uint32_t size) {
    if (cb->w - cb->r < size) {
        return -1;
    }

    memcpy(ret_buf, (char *)cb->buffer + cb->r, size);
    cb->r += size;
    if (cb->r > cb->buffer_size) {
        cb->r -= cb->buffer_size;
        cb->w -= cb->buffer_size;
    }
    return 0;
}
