/* Copyright (c) 2013 Kyle Verrier */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "./conv.h"
#include <fftw3.h>


bool test(std::vector<float> actual, std::vector<float> expected){
  if (actual != expected) {
    std::cout << "!FAILED TEST!" << std::endl;
    std::cout << "Expected:\t";
    for (std::vector<float>::const_iterator i = expected.begin(); i != expected.end(); ++i)
      std::cout << *i << '\t';
    std::cout << std::endl;

    std::cout << "Actual:\t\t";
    for (std::vector<float>::const_iterator i = actual.begin(); i != actual.end(); ++i)
      std::cout << *i << '\t';
    std::cout << std::endl;
    return false;
  }
  return true;
}

bool test_conv(std::vector<float> input, std::vector<float> impulse, std::vector<float> expected) {
  /*
  std::cout << "AUDIO DATA" << std::endl;
  std::cout << "==========" << std::endl;
  for (std::vector<float>::const_iterator i = input.begin(); i != input.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;

  std::cout << "IR DATA" << std::endl;
  std::cout << "==========" << std::endl;
  for (std::vector<float>::const_iterator i = impulse.begin(); i != impulse.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;
  */

  // TODO Print out fancy debugging jazz

  /* Perform convolution */
  std::vector<float> conv_data = conv(input, impulse);
  test(conv_data, expected);
}

int main(int argc, const char *argv[]) {
  /*
  std::string line;
  std::ifstream au_file("au.txt");
  std::vector<float> audio_data;
  while (std::getline(au_file, line)) {
    std::istringstream iss(line);
    float f;
    while (iss >> f) {
      audio_data.push_back(f);
    }
  }
  std::ifstream ir_file("ir.txt");
  std::vector<float> ir_data;
  while (std::getline(ir_file, line)) {
    std::istringstream iss(line);
    float f;
    while (iss >> f) {
      ir_data.push_back(f);
    }
  }

  static const float arr[] = {0,2,9,8,8,4,2};
  std::vector<float> ans (arr, arr + sizeof(arr) / sizeof(arr[0]) );
  test_conv(audio_data, ir_data, ans);
  */

  float a[10];
  float b[10];
  conv2();

  return 0;
}
