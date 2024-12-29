#include "helper.h"

/*
 * io_readn - Read n bytes (unbuffered)
 */
 
ssize_t io_readn(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno != EINTR) {
                return -1; /* errno set by read() */
            }

            /* Interrupted by sig handler return, call read() again */
            nread = 0;
        } else if (nread == 0) {
            break; /* EOF */
        }
        nleft -= (size_t)nread;
        bufp += nread;
    }
    return (ssize_t)(n - nleft); /* Return >= 0 */
}

/*
 * io_writen - Write n bytes (unbuffered)
 */
 
ssize_t io_writen(int fd, const void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    const char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno != EINTR) {
                return -1; /* errno set by write() */
            }

            /* Interrupted by sig handler return, call write() again */
            nwritten = 0;
        }
        nleft -= (size_t)nwritten;
        bufp += nwritten;
    }
    return (ssize_t)n;
}

/*
 * io_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, io_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    io_cnt is the number of unread bytes in the internal buffer. On
 *    entry, io_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
 
static ssize_t io_read(io_t *rp, char *usrbuf, size_t n) {
    size_t cnt;

    while (rp->io_cnt <= 0) { /* Refill if buf is empty */
        rp->io_cnt = read(rp->io_fd, rp->io_buf, sizeof(rp->io_buf));
        if (rp->io_cnt < 0) {
            if (errno != EINTR) {
                return -1; /* errno set by read() */
            }

            /* Interrupted by sig handler return, nothing to do */
        } else if (rp->io_cnt == 0) {
            return 0; /* EOF */
        } else {
            rp->io_bufptr = rp->io_buf; /* Reset buffer ptr */
        }
    }

    /* Copy min(n, rp->io_cnt) bytes from internal buf to user buf */
    cnt = n;
    if ((size_t)rp->io_cnt < n) {
        cnt = (size_t)rp->io_cnt;
    }
    memcpy(usrbuf, rp->io_bufptr, cnt);
    rp->io_bufptr += cnt;
    rp->io_cnt -= cnt;
    return (ssize_t)cnt;
}

/*
 * io_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
 
void io_readinitb(io_t *rp, int fd) {
    rp->io_fd = fd;
    rp->io_cnt = 0;
    rp->io_bufptr = rp->io_buf;
}

/*
 * io_readnb - Read n bytes (buffered)
 */
 
ssize_t io_readnb(io_t *rp, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = io_read(rp, bufp, nleft)) < 0) {
            return -1; /* errno set by read() */
        } else if (nread == 0) {
            break; /* EOF */
        }
        nleft -= (size_t)nread;
        bufp += nread;
    }
    return (ssize_t)(n - nleft); /* return >= 0 */
}

/*
 * io_readlineb - Read a text line (buffered)
 */
 
ssize_t io_readlineb(io_t *rp, void *usrbuf, size_t maxlen) {
    size_t n;
    ssize_t rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {
        if ((rc = io_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return 0; /* EOF, no data read */
            } else {
                break; /* EOF, some data was read */
            }
        } else {
            return -1; /* Error */
        }
    }
    *bufp = 0;
    return (ssize_t)(n - 1);
}
