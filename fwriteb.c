size_t fwriteb(const void *restrict ptr, size_t nbits, FILE *restrict stream) {
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
    fputc(temp, stream);
  }

  int remaining = nbits % 8;
  if (remaining > 0) {
    unsigned char byte = bytes[length] // <- last byte
    unsigned char diff = 8 - count;
      if (remaining <= diff) {
      buf <<= remaining;
      buf |= byte;
      count += remaining;
      if (count == 8) {
        fputc(buf, stream);
        buf = 0;
        count = 0;
      }
    } else {
      unsigned char overflow = remaining - count; 
      buf <<= diff;
      buf |= byte >> overflow;
      fputc(buf, stream); 
      buf = byte;
      count = overflow;
    }
  }

  return 0;
}
