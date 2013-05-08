#include "./cirbuf.h"

void write(buffer* buffer, double value) {
    buffer->buff[(buffer->writeIndex++) & BUFF_SIZE_MASK] = value;
}

float readn(buffer* buffer, int Xn) {
    return buffer->buff[(buffer->writeIndex + (~Xn)) & BUFF_SIZE_MASK];
}
