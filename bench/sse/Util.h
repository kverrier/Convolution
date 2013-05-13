#include <math.h>
#include <stdlib.h>

typedef struct {
  double real;
  double img;
} complex_num;


void multiply_C(complex_num x, complex_num y, complex_num *z);

void multiply_SSE2(complex_num x, complex_num y, complex_num *z);

void multiply_SSE3(complex_num x, complex_num y, complex_num *z);

