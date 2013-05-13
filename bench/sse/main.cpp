#include <stdio.h>
#include "Util.h"
#include "CycleTimer.h"

double rand_float( double low, double high ) {
  return ( ( double )rand() * ( high - low ) ) / ( double )RAND_MAX + low;
}

#define NUM 65536

inline static __m128 intrin_sse3_cmul(__m128 ab, __m128 cd)
{
  __m128 aa, bb, dc, x0, x1;
  aa = _mm_moveldup_ps(ab);
  bb = _mm_movehdup_ps(ab);
  x0 = _mm_mul_ps(aa, cd);    //ac ad
  dc = _mm_shuffle_ps(cd, cd, _MM_SHUFFLE(2,3,0,1));
  x1 = _mm_mul_ps(bb, dc);    //bd bc
  return _mm_addsub_ps(x0, x1);

}

inline static intrin_sse2_cmul(__m128 *ab, __m128 *cd, __m128 *ef)
{
  __m128d aa, bb, cc, dd, ac, ad, bd, bc, ee, ff;
  aa = _mm_shuffle_ps(ab[0], ab[1], _MM_SHUFFLE(2,0,2,0));
  bb = _mm_shuffle_ps(ab[0], ab[1], _MM_SHUFFLE(3,1,3,1));
  cc = _mm_shuffle_ps(cd[0], cd[1], _MM_SHUFFLE(2,0,2,0));
  dd = _mm_shuffle_ps(cd[0], cd[1], _MM_SHUFFLE(3,1,3,1));
  ac = _mm_mul_ps(aa, cc);
  ad = _mm_mul_ps(aa, dd);
  bd = _mm_mul_ps(bb, dd);
  bc = _mm_mul_ps(bb, cc);
  ee = _mm_sub_ps(ac, bd);
  ff = _mm_add_ps(bc, ad);
  ef[0] = _mm_unpacklo_ps(ee, ff);
  ef[1] = _mm_unpackhi_ps(ee, ff);
}


int main(int argc, const char *argv[])
{
  srand(1234);

  complex_num* a = new complex_num[NUM];
  complex_num* b = new complex_num[NUM];
  complex_num* c = new complex_num[NUM];

  for (int i = 0; i < NUM; i++) {
    a[i].real = rand_float(0, 1);
    a[i].img = rand_float(0, 1);
    b[i].real = rand_float(0, 1);
    b[i].img = rand_float(0, 1);
  }


  double c_total = 0;
  for (int i = 0; i < NUM; i++) {
    double startTime = CycleTimer::currentSeconds();
    multiply_C(a[i], b[i], &c[i]);
    double endTime = CycleTimer::currentSeconds();
    c_total += endTime - startTime;
  }

  double sse2_total = 0;
  for (int i = 0; i < NUM; i++) {
    double startTime = CycleTimer::currentSeconds();
    multiply_SSE2(a[i], b[i], &c[i]);
    double endTime = CycleTimer::currentSeconds();
    sse2_total += endTime - startTime;
  }

  double sse3_total = 0;
  for (int i = 0; i < NUM; i++) {
    double startTime = CycleTimer::currentSeconds();
    multiply_SSE3(a[i], b[i], &c[i]);
    double endTime = CycleTimer::currentSeconds();
    sse3_total += endTime - startTime;
  }



  printf("C:\t%f\n", c_total / NUM * 1000000);
  printf("SSE2:\t%f\n", sse2_total / NUM * 1000000);
  printf("SSE3:\t%f\n", sse3_total / NUM * 1000000);

  return 0;
}
