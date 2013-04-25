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

void conv2() {
  double a[] = {1,2,0,0,0,0,0};
  double b[] = {0,2,2,2,0,0,0};
  int n = 7;

  double output[8] = {0};

  fftw_complex* aHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
  fftw_complex* bHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );

  // Perform FFT on input a and b
  fftw_plan pa = fftw_plan_dft_r2c_1d(n, a, aHat, FFTW_ESTIMATE);
  fftw_execute(pa);
  fftw_destroy_plan(pa);

  fftw_plan pb = fftw_plan_dft_r2c_1d(n, b, bHat, FFTW_ESTIMATE);
  fftw_execute(pb);
  fftw_destroy_plan(pb);

  // Multiply FFTs of a and b to perform convolution in the frequency domain
  fftw_complex* abHat = (fftw_complex*) fftw_malloc( sizeof(fftw_complex)*n );
  for(int i = 0; i < n; i++) {
    abHat[i][0] = (aHat[i][0]*bHat[i][0] - aHat[i][1]*bHat[i][1]);
    abHat[i][1] = (aHat[i][1]*bHat[i][0] + aHat[i][0]*bHat[i][1]);
  }

  // IFFT the convolution back into the time domain
  fftw_plan ipab = fftw_plan_dft_c2r_1d(n, abHat, output, FFTW_ESTIMATE);
  fftw_execute(ipab);
  fftw_destroy_plan(ipab);

  // Free the malloc'd resources
  fftw_free(aHat);
  fftw_free(bHat);
  fftw_free(abHat);

  for (int i = 0; i < n; i++) {
    output[i] = output[i] / n;
    std::cout.precision(4);
    std::cout << std::fixed << output[i] << " ";
  }
  std::cout << std::endl;
}
