#include <algorithm>
#include <iostream>
#include <fftw3.h>
#include "./FFTFrame.h"
#include "./ReverbAccumulationBuffer.h"
#include <omp.h>
// #include <pmmintrin.h>

extern "C"
{
#include <emmintrin.h>
#include <mmintrin.h>
}



FFTFrame::FFTFrame(double* impulseResponseChunk, int framesToProcess, ReverbAccumulationBuffer* accumulationBuffer, int index) {
  m_framesToProcess = framesToProcess;
  m_size = 2 * framesToProcess; // Size needed for the convolution
  m_readIndex = index;
  m_impulseResponse = new double[m_size]();

  std::copy( &impulseResponseChunk[m_readIndex], &impulseResponseChunk[m_readIndex + m_framesToProcess], m_impulseResponse );
  m_processBuffer   = new double[m_size]();
  m_overflowBuffer  = new double[m_size]();
  m_resultBuffer    = new double[m_size]();
  m_accumulationBuffer = accumulationBuffer;

  // Create proper structures for FFTs
  int n = m_size - 1;
  m_aHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
  m_bHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
  m_abHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );

  fftw_plan pb = fftw_plan_dft_r2c_1d(n, m_impulseResponse, m_bHat, FFTW_PATIENT);
  fftw_execute(pb);
  fftw_destroy_plan(pb);

  m_pa = fftw_plan_dft_r2c_1d(n, m_processBuffer, m_aHat, FFTW_PATIENT);
  m_ipab = fftw_plan_dft_c2r_1d(n, m_abHat, m_resultBuffer, FFTW_PATIENT);
}

FFTFrame::~FFTFrame() {
  fftw_destroy_plan(m_pa);
  fftw_destroy_plan(m_ipab);

  fftw_free(m_aHat);
  fftw_free(m_bHat);
  fftw_free(m_abHat);

  delete[] m_impulseResponse;
  delete[] m_processBuffer;
  delete[] m_overflowBuffer;
  delete[] m_resultBuffer;
}

int FFTFrame::getReadIndex() {
  return m_readIndex;
}

void FFTFrame::process(double* input) {
  int offset = m_framesToProcess;
  int overlapAmount = m_size / 2;

  // Copy input into processing buffer and convolve with precomputed FFT(impulse)
  std::copy( &input[0], &input[m_framesToProcess], m_processBuffer );
  performConvolution();

  // Add previous overflow to results
  for (int i = 0; i < m_framesToProcess; i++)
    m_resultBuffer[i] += m_overflowBuffer[i];

  // Write results to accumulationBuffer
  m_accumulationBuffer->writeTo(m_resultBuffer, m_readIndex, m_framesToProcess);

  // Copy overflow into buffer from results
  std::copy( &m_resultBuffer[offset], &m_resultBuffer[offset+overlapAmount], m_overflowBuffer );
}

inline void FFTFrame::performConvolution() {
    int n = m_size - 1;

    // Perform FFT on current contents of m_processBuffer
    fftw_execute(m_pa);

    // Multiply FFTs of input and impulse response in freq domain to perform convolution
    /*
    */
    for(int i = 0; i < n; i++) {
        m_abHat[i][0] = (m_aHat[i][0]*m_bHat[i][0] - m_aHat[i][1]*m_bHat[i][1]);
        m_abHat[i][1] = (m_aHat[i][1]*m_bHat[i][0] + m_aHat[i][0]*m_bHat[i][1]);
    }

    /*
    for(int i = 0; i < n; i++) {
      __m128d num1, num2, num3, num4;
      // Copies a single element into the vector
      //   num1:  [x.real, x.real]
      num1 = _mm_load1_pd(&m_aHat[i][0]);
      // Move y elements into a vector
      //   num2: [y.img, y.real]
      num2 = _mm_set_pd(m_bHat[i][1], m_bHat[i][0]);
      // Multiplies vector elements
      //   num3: [(x.real*y.img), (x.real*y.real)]
      num3 = _mm_mul_pd(num2, num1);
      //   num1: [x.img, x.img]
      num1 = _mm_load1_pd(&m_aHat[i][0]);
      // Swaps the vector elements.
      //   num2: [y.real, y.img]
      num2 = _mm_shuffle_pd(num2, num2, 1);
      //   num2: [(x.img*y.real), (x.img*y.img)]
      num2 = _mm_mul_pd(num2, num1);
      num4 = _mm_add_pd(num3, num2);
      num3 = _mm_sub_pd(num3, num2);
      num4 = _mm_shuffle_pd(num3, num4, 2);
      // Stores the elements of num4 into z
      _mm_storeu_pd((double *)m_abHat, num4);
    }
    */

    // Perform IFFT and store into m_resultBuffer
    fftw_execute(m_ipab);

    // Normalize output
    for (int i = 0; i < n; i++)
        m_resultBuffer[i] = m_resultBuffer[i] / n;
}

void FFTFrame::print() {
  using namespace std;
  cout << "RESULT:\t\t";
  for (int i = 0; i < m_size; i++) {
    cout << fixed << m_resultBuffer[i] << "\t";
  }
  cout << endl;

  cout << "OVERFLOW:\t";
  for (int i = 0; i < m_size; i++) { cout << m_overflowBuffer[i] << "\t";
  }
  cout << endl;

}


