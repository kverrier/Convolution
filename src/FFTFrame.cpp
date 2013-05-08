#include <algorithm>
#include <iostream>
#include <fftw3.h>
#include "./FFTFrame.h"
#include "./ReverbAccumulationBuffer.h"

FFTFrame::FFTFrame(double* impulseResponseChunk, int framesToProcess, ReverbAccumulationBuffer* accumulationBuffer, int index) {
  m_framesToProcess = framesToProcess;
  m_size = 2 * framesToProcess; // Size needed for the convolution
  m_readIndex = index;
  m_impulseResponse = new double[m_size]();
  // memcpy(m_impulseResponse, impulseResponseChunk + m_readIndex, sizeof(double)*framesToProcess);
  std::copy( &impulseResponseChunk[m_readIndex], &impulseResponseChunk[m_readIndex + m_framesToProcess], m_impulseResponse );
  m_processBuffer   = new double[m_size]();
  m_overflowBuffer  = new double[m_size]();
  m_resultBuffer    = new double[m_size]();
  m_accumulationBuffer = accumulationBuffer;

  int n = m_size - 1;
  m_aHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
  m_bHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
  // m_abHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );

  fftw_plan pb = fftw_plan_dft_r2c_1d(n, m_impulseResponse, m_bHat, FFTW_ESTIMATE);
  fftw_execute(pb);
  fftw_destroy_plan(pb);

  m_pa = fftw_plan_dft_r2c_1d(n, m_processBuffer, m_aHat, FFTW_ESTIMATE);
}

FFTFrame::~FFTFrame() {
  fftw_destroy_plan(m_pa);
  // Free the malloc'd resources
  /*
  fftw_free(m_aHat);
  fftw_free(m_bHat);
  fftw_free(m_abHat);
  */
}

int FFTFrame::getReadIndex() {
  return m_readIndex;
}

void FFTFrame::process(double* input) {

  int offset = m_framesToProcess;
  int overlapAmount = m_size / 2;

  // Copy input into processing buffer and convolve with precomputed FFT(impulse)
  std::copy( &input[0], &input[m_framesToProcess], m_processBuffer );
  convolve(m_processBuffer, m_impulseResponse, m_resultBuffer, m_size-1);

  // Add previous overflow to results
  for (int i = 0; i < m_framesToProcess; i++)
    m_resultBuffer[i] += m_overflowBuffer[i];

  // Write results to accumulationBuffer
  m_accumulationBuffer->writeTo(m_resultBuffer, m_readIndex, m_framesToProcess);

  // Copy overflow into buffer from results
  std::copy( &m_resultBuffer[offset], &m_resultBuffer[offset+overlapAmount], m_overflowBuffer );
}

void FFTFrame::convolve(double* a, double* b, double *output, int n) {
    // Perform FFT on input a and b
    fftw_plan pa, ipab;
    #pragma omp critical
    {
      pa = m_pa;
    }
    fftw_execute(pa);
    #pragma omp critical
    {
      fftw_destroy_plan(pa);
    }

    // Multiply FFTs of a and b in freq domain to perform convolution
    fftw_complex* abHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );

    for(int i = 0; i < n; i++) {
        abHat[i][0] = (m_aHat[i][0]*m_bHat[i][0] - m_aHat[i][1]*m_bHat[i][1]);
        abHat[i][1] = (m_aHat[i][1]*m_bHat[i][0] + m_aHat[i][0]*m_bHat[i][1]);
    }

    #pragma omp critical
    {
      // IFFT the convolution back into the time domain
      ipab = fftw_plan_dft_c2r_1d(n, abHat, output, FFTW_ESTIMATE);
    }

    fftw_execute(ipab);

    #pragma omp critical
    {
      fftw_destroy_plan(ipab);
    }

    // Free the malloc'd resources
    fftw_free(abHat);

    // Normalize output
    for (int i = 0; i < n; i++)
        output[i] = output[i] / n;
}

void FFTFrame::print() {
  using namespace std;
  cout << "RESULT:\t\t";
  for (int i = 0; i < m_size; i++) {
    cout << fixed << m_resultBuffer[i] << "\t";
  }
  cout << endl;

  cout << "OVERFLOW:\t";
  for (int i = 0; i < m_size; i++) {
    cout << m_overflowBuffer[i] << "\t";
  }
  cout << endl;

}


