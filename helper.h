// helper.h ``

#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define IO_BUFSIZE 9999

// Struct 
typedef struct {
    int io_fd;                /* Descriptor for this internal buf */
    ssize_t io_cnt;           /* Unread bytes in internal buf */
    char *io_bufptr;          /* Next unread byte in internal buf */
    char io_buf[IO_BUFSIZE];  /* Internal buffer */
} io_t;

// Function prototypes
ssize_t io_readn(int fd, void *usrbuf, size_t n);
ssize_t io_writen(int fd, const void *usrbuf, size_t n);
void io_readinitb(io_t *rp, int fd);
ssize_t io_readnb(io_t *rp, void *usrbuf, size_t n);
ssize_t io_readlineb(io_t *rp, void *usrbuf, size_t maxlen);

#endif // HELPER_H
