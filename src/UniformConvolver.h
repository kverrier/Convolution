#ifndef UniformConvolver_h
#define UniformConvolver_h

#include <vector>
#include "FFTFrame.h"
#include "ReverbAccumulationBuffer.h"

class UniformConvolver {
  public:
    UniformConvolver(double* impulseResponse, int impulseSize, int outputSize, int nThreads);
    ~UniformConvolver();
    void process(double* input, double* output);
    void print();

  private:
    double* m_impulseResponse;
    int m_impulseSize;
    int m_outputSize;
    // std::vector<FFTFrame> m_fftFrames;
    std::vector<FFTFrame*> m_fftFrames;
    ReverbAccumulationBuffer m_accumulationBuffer;
    int m_nThreads;

};
#endif
