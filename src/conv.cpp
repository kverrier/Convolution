/* Copyright (c) 2013 Kyle Verrier */

#include <iostream>
#include <vector>
#include <fftw3.h>

std::vector<float> conv(std::vector<float> x, std::vector<float> h) {
    int conv_size = x.size() + h.size() - 1;
    std::vector<float> conv_data(conv_size);

    for (int i = 0; i < conv_size; i++) {
        float tmp = 0.0;
        int i1 = i;
        for (int j = 0; j < h.size(); j++) {
            if (i1 >= 0 && i1 < x.size())
                tmp += x.at(i1) * h.at(j);
            i1--;
            conv_data.at(i) = tmp;
        }
    }

    return conv_data;
}

/*
 * Convolves 2 signals a and b on a per sample basis on the time domain.
 * Complexity O(L) multiply-add per sample where L is size of b.
 */
void conv1(double* a, int size_a, double* b, int size_b, double* output, int n) {
    for (int i = 0; i < n; i++) {
        double tmp = 0.0;
        int i1 = i;
        for (int j = 0; j < size_b; j++) {
            if (i1 >= 0 && i1 < size_a)
                tmp += a[i1] * b[j];
            i1--;
            output[i] = tmp;
        }
    }
}

/*
 * Convolves 2 signals a and b by performing FFTs, multiplication and IFFT.
 * Assumes a and b are zero padded upto n = a_size + b_size - 1.
 */
void conv2(double* a, double* b, double* output, int n) {

    fftw_complex* aHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
    fftw_complex* bHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );

    // Perform FFT on input a and b
    fftw_plan pa = fftw_plan_dft_r2c_1d(n, a, aHat, FFTW_ESTIMATE);
    fftw_execute(pa);
    fftw_destroy_plan(pa);

    fftw_plan pb = fftw_plan_dft_r2c_1d(n, b, bHat, FFTW_ESTIMATE);
    fftw_execute(pb);
    fftw_destroy_plan(pb);

    // Multiply FFTs of a and b in freq domain to perform convolution
    fftw_complex* abHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
    for(int i = 0; i < n; i++) {
        abHat[i][0] = (aHat[i][0]*bHat[i][0] - aHat[i][1]*bHat[i][1]);
        abHat[i][1] = (aHat[i][1]*bHat[i][0] + aHat[i][0]*bHat[i][1]);
    }

    // IFFT the convolution back into the time domain
    fftw_plan ipab = fftw_plan_dft_c2r_1d(n, abHat, output, FFTW_ESTIMATE);
    fftw_execute(ipab);
    fftw_destroy_plan(ipab);

    // Normalize output
    for (int i = 0; i < n; i++) {
        output[i] = output[i] / n;
    }

    // Free the malloc'd resources
    fftw_free(aHat);
    fftw_free(bHat);
    fftw_free(abHat);
}
