#ifndef ReverbAccumulationBuffer_h
#define ReverbAccumulationBuffer_h

class ReverbAccumulationBuffer {
  public:
    ReverbAccumulationBuffer(int size, int outputFrames);
    ~ReverbAccumulationBuffer();
    void loadOutput(double* audioOut);
    void writeTo(double* data, int index, int size);
    int getIndex();
    void print();

  private:
    double* m_dataBuffer;
    int m_index;
    int m_size;
    int m_outputFrames;
};

#endif
