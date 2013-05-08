#ifndef _CONV_H_
#define _CONV_H_

#include <vector>

std::vector<float> conv(std::vector<float> x, std::vector<float> h);
void conv1(double* a, int size_a, double* b, int size_b, double* output, int n);
void conv2(double *a, double *b, double *output, int n);

#endif
