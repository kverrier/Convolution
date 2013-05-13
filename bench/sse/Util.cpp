#include <stdio.h>
#include <pmmintrin.h>
#include "Util.h"


extern "C"
{
#include <emmintrin.h>
#include <mmintrin.h>
}


// Multiplying complex numbers in C
void multiply_C(complex_num x, complex_num y, complex_num *z)
{
  z->real = (x.real*y.real) - (x.img*y.img);
  z->img  = (x.img*y.real)  + (y.img*x.real);
}

// Multiplying complex numbers using SSE2 intrinsics
void multiply_SSE2(complex_num x, complex_num y, complex_num *z)
{
  __m128d num1, num2, num3, num4;
  // Copies a single element into the vector
  //   num1:  [x.real, x.real]
  num1 = _mm_load1_pd(&x.real);
  // Move y elements into a vector
  //   num2: [y.img, y.real]
  num2 = _mm_set_pd(y.img, y.real);
  // Multiplies vector elements
  //   num3: [(x.real*y.img), (x.real*y.real)]
  num3 = _mm_mul_pd(num2, num1);
  //   num1: [x.img, x.img]
  num1 = _mm_load1_pd(&x.img);
  // Swaps the vector elements.
  //   num2: [y.real, y.img]
  num2 = _mm_shuffle_pd(num2, num2, 1);
  //   num2: [(x.img*y.real), (x.img*y.img)]
  num2 = _mm_mul_pd(num2, num1);
  num4 = _mm_add_pd(num3, num2);
  num3 = _mm_sub_pd(num3, num2);
  num4 = _mm_shuffle_pd(num3, num4, 2);
  // Stores the elements of num4 into z
  _mm_storeu_pd((double *)z, num4);
}

// Multiplying complex numbers using SSE3 intrinsics
void multiply_SSE3(complex_num x, complex_num y, complex_num *z)
{
  __m128d num1, num2, num3;
  // Duplicates lower vector element into upper vector element.
  //   num1: [x.real, x.real]
  num1 = _mm_loaddup_pd(&x.real);
  // Move y elements into a vector
  //   num2: [y.img, y.real]
  num2 = _mm_set_pd(y.img, y.real);
  // Multiplies vector elements
  //   num3: [(x.real*y.img), (x.real*y.real)]
  num3 = _mm_mul_pd(num2, num1);
  //   num1: [x.img, x.img]
  num1 = _mm_loaddup_pd(&x.img);
  // Swaps the vector elements
  //   num2: [y.real, y.img]
  num2 = _mm_shuffle_pd(num2, num2, 1);
  //   num2: [(x.img*y.real), (x.img*y.img)]
  num2 = _mm_mul_pd(num2, num1);
  // Adds upper vector element while subtracting lower vector element
  //   num3: [((x.real *y.img)+(x.img*y.real)),
  //          ((x.real*y.real)-(x.img*y.img))]
  num3 = _mm_addsub_pd(num3, num2);
  // Stores the elements of num3 into z
  _mm_storeu_pd((double *)z, num3);
}

