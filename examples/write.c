#define BBB_IMPL
#include "../bbb.h"

int main() {
  bbb_t stream = {
    0,0,
    fopen("data.bin", "wb")
  };

  // [10110]
  unsigned int b1 = 0b10110; // 5 bits
  // [00110111, 1101]
  unsigned int b2 = 0b110100110111; // 12 bits
  // [11011011, 111111]
  unsigned int b3 = 0b11111111011011; // 14 bits
  // [1001011]
  unsigned int b4 = 0b1001011; // 7 bits

  // Raw bits: 
  // 10110 00110111 1101 11011011 111111 1001011

  // The bits packaged in 8-bits:
  // 10110001 10111110 11101101 11111111 00101100
  // Decimal values:
  // 177      190      237      255      44

  fwriteb(&b1, 5,  &stream);
  fwriteb(&b2, 12, &stream);
  fwriteb(&b3, 14, &stream);
  fwriteb(&b4, 7,  &stream);

  fflushb(&stream);
  fclose(stream.fp);

  return 0;
}
