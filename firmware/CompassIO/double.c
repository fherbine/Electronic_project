#include <stdio.h>
#include <stdint.h>

int main()
{
  /* Double to long long */
  double data = 123.999;
  long long value = *(long long *)&data;
  printf("%f - %llx - %llx\n", data, *(long long *)&data, value);
  /* Merge unsigned char to int */
  unsigned char array[4] = {0x15, 0xCD, 0x5B, 0x7};
  unsigned int output = 0;
  for (int i = 0; i < 4; i++)
  {
    output = (array[i] << (i * 8)) | output;
  }
  /* Store float * 1000 used as flat by divinding by 1000 */
  printf("%f\n", output / 1000.0);

  short h = 0b11000001;
  unsigned char l = 0b10000000;
  short nb = ((h << 8) | l);
  printf("%d\n", nb);
  return (0);
}
