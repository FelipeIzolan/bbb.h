#ifndef
#define BBB_H

#include <stdio.h>

typedef struct {
  unsigned char buf;
  unsigned char count;
} bbb_t;

size_t freadb(const void *restrict ptr, size_t nbits, bbb_t *restrict ctx, FILE *restrict stream)
size_t fwriteb(const void *restrict ptr, size_t nbits, bbb_t *restrict ctx, FILE *restrict stream) 

#endif // BBB_H

#ifdef BBB_IMPL
size_t fwriteb(const void *restrict ptr, size_t nbits, bbb_t *restrict ctx, FILE *restrict stream) {
  if (ptr == NULL && stream == NULL)
    return 0;

  static unsigned char buf = 0;
  static unsigned char count = 0; 

  unsigned char * bytes = (char *)ptr;
  size_t length = nbits / 8;
  for (size_t i = 0; i < length; i++) {
    unsigned char byte = bytes[i];
    unsigned char temp = byte;
    if (count > 0) {
      buf <<= 8 - count;
      buf |= byte >> count;
      temp = buf;
      buf = byte;
    }
    if (fputc(temp, stream) == EOF)
      return EOF;
  }

  int remaining = nbits % 8;
  if (remaining > 0) {
    unsigned char byte = bytes[length];  
    unsigned char diff = 8 - count;
    if (remaining <= diff) {
      buf <<= remaining;
      buf |= byte;
      count += remaining;
      if (count == 8) {
        if (fputc(buf, stream) == EOF)
          return EOF;
        buf = 0;
        count = 0;
      }
    } else {
//       unsigned char overflow = remaining - count; 
//       buf <<= diff;
//       buf |= byte >> overflow;
//       if (fputc(buf, stream) == EOF)
//         return EOF;
//       buf = byte;
//       count = overflow;
    }
  }

  return 0;
}
#endif // BBB_IMPL

