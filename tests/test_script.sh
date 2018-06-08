#!/bin/sh

CIRBUF_TEST=cirbuf_test.out

gcc -g -o $CIRBUF_TEST z_cirbuf_test.c ../src/z_circular_buffer/z_circular_buffer.c -lpthread

time ./$CIRBUF_TEST

perf stat ./$CIRBUF_TEST

rm $CIRBUF_TEST
