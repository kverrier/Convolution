/* Copyright (c) 2013 Kyle Verrier */

ReverbAccumulationBuffer::ReverbAccumulationBuffer(size_t length)
  : m_buffer(length)
  , m_readIndex(0)
{
}

