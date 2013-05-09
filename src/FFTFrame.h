#ifndef FFTFrame_h
#define FFTFrame_h

#include "ReverbAccumulationBuffer.h"
#include <fftw3.h>

class FFTFrame {
  public:
    FFTFrame(double* impulseResponseChunk, int framesToProcess, ReverbAccumulationBuffer* accumulationBuffer, int index);
    ~FFTFrame();
    int getReadIndex();
    void process(double* input);
    void print();

  private:
    void performConvolution();
    double* m_impulseResponse;
    double* m_processBuffer;
    double* m_overflowBuffer;
    double* m_resultBuffer;
    int m_framesToProcess;
    int m_size;
    int m_readIndex;
    ReverbAccumulationBuffer* m_accumulationBuffer;

    // stuff for FFTW
    fftw_complex* m_aHat;
    fftw_complex* m_bHat;
    fftw_complex* m_abHat;
    fftw_plan m_pa;
    fftw_plan m_ipab;
};
#endif
