#include "./ReverbAccumulationBuffer.h"
#include <iostream>
#include <cstring>

ReverbAccumulationBuffer::ReverbAccumulationBuffer(int size, int outputFrames) {
  m_size = size;
  m_dataBuffer = new double[m_size]();
  m_outputFrames = outputFrames;
  m_index = 0;
}

ReverbAccumulationBuffer::~ReverbAccumulationBuffer() {
  delete[] m_dataBuffer;
}

void ReverbAccumulationBuffer::loadOutput(double* audioOut) {
  memcpy(audioOut, m_dataBuffer + m_index, sizeof(double) * m_outputFrames);
  memset(m_dataBuffer + m_index, 0, sizeof(double) * m_outputFrames);
  m_index = (m_index + m_outputFrames) % m_size ;
}

void ReverbAccumulationBuffer::writeTo(double* data, int index, int size) {
  for (int i = 0; i < size; i++) {
    m_dataBuffer[(m_index + index + i) % m_size] += data[i];
  }
}

void ReverbAccumulationBuffer::print() {
  using namespace std;
  cout << "REVERB_ACC_BUFF: ";
  for (int i = 0; i < m_size; i++) {
    cout << fixed << m_dataBuffer[(m_index + i) % m_size] << "\t";
  }
  cout << endl;

}
