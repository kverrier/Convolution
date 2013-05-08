#include <iostream>
#include <cstring>
#include "./FFTFrame.h"
#include "./conv.h"
#include "./ReverbAccumulationBuffer.h"

FFTFrame::FFTFrame(double* impulseResponseChunk, int framesToProcess, ReverbAccumulationBuffer* accumulationBuffer, int index) {
  m_framesToProcess = framesToProcess;
  m_size = 2 * framesToProcess; // Size needed for the convolution
  m_readIndex = index;
  m_impulseResponse = new double[m_size]();
  memcpy(m_impulseResponse, impulseResponseChunk + m_readIndex, sizeof(double)*framesToProcess);
  m_processBuffer   = new double[m_size]();
  m_overflowBuffer  = new double[m_size]();
  m_resultBuffer    = new double[m_size]();
  m_accumulationBuffer = accumulationBuffer;
}

int FFTFrame::getReadIndex() {
  return m_readIndex;
}

void FFTFrame::process(double* input) {
  int offset = m_framesToProcess;
  int overlapAmount = m_size / 2; // FIXME make sure this is right

  memcpy(m_processBuffer, input, sizeof(double)*m_framesToProcess);
  conv2(m_processBuffer, m_impulseResponse, m_resultBuffer, m_size-1);

  // Add previous overflow to results
  for (int i = 0; i < m_framesToProcess; i++) {
    m_resultBuffer[i] += m_overflowBuffer[i];
  }

  // Write results to accumulationBuffer
  m_accumulationBuffer->writeTo(m_resultBuffer, m_readIndex, m_framesToProcess);

  // Copy overflow into buffer from results
  memcpy(m_overflowBuffer, m_resultBuffer + offset, sizeof(double)*overlapAmount );
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


