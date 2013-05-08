#ifndef _CIRBUF_H_
#define _CIRBUF_H_

#define BUFF_SIZE 6
#define BUFF_SIZE_MASK (BUFF_SIZE-1)


typedef struct buffer {
    float buff[BUFF_SIZE];
    int writeIndex;
} buffer;

void write(buffer* buffer, double value);

float readn(buffer* buffer, int Xn);

#endif
