#include <stdio.h>
#include <stdint.h>

int main()
{
  double data = 123.456;
  long long value = *(long long *)&data;
  printf("%f - %llx - %llx\n", data, *(long long *)&data, value);
  return (0);
}
