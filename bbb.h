#ifndef BBB_H
#define BBB_H

#include <stdio.h>

typedef struct {
  unsigned char buf;
  unsigned char count;
  FILE * fp;
} bbb_t;

// int freadb(const void *restrict ptr, size_t nbits, bbb_t *restrict stream)
int fwriteb(const void *restrict ptr, size_t nbits, bbb_t *restrict stream);
int fflushb(bbb_t *restrict stream);

#endif // BBB_H

#ifdef BBB_IMPL
int fwriteb(const void *restrict ptr, size_t nbits, bbb_t *restrict stream) {
  if (ptr == NULL || stream == NULL)
    return 0;

  unsigned char * bytes = (char *)ptr;
  size_t length = nbits / 8;
  
  for (size_t i = 0; i < length; i++) {
    unsigned char byte = bytes[i];
    unsigned char temp = byte;
    if (stream->count > 0) {
      stream->buf <<= 8 - stream->count;
      stream->buf |= byte >> stream->count;
      temp = stream->buf;
      stream->buf = byte;
    }
    if (fputc(temp, stream->fp) == EOF)
      return EOF;
  }

  int remaining = nbits % 8;
  if (remaining > 0) {
    unsigned char byte = bytes[length];  
    unsigned char diff = 8 - stream->count;
    if (remaining <= diff) {
      stream->buf <<= remaining;
      stream->buf |= byte;
      stream->count += remaining;
      if (stream->count == 8) {
        if (fputc(stream->buf, stream->fp) == EOF)
          return EOF;
        stream->buf = 0;
        stream->count = 0;
      }
    } else {
      unsigned char overflow = remaining - diff; 
      stream->buf <<= diff;
      stream->buf |= byte >> overflow;
      if (fputc(stream->buf, stream->fp) == EOF)
        return EOF;
      stream->buf = byte;
      stream->count = overflow;
    }
  }

  return nbits - stream->count;
}

int fflushb(bbb_t *restrict stream) {
  stream->buf <<= 8 - stream->count;
  if (fputc(stream->buf, stream->fp) == EOF)
    return EOF;
  stream->buf = 0;
  stream->count = 0;
  return fflush(stream->fp);
}

#endif // BBB_IMPL

