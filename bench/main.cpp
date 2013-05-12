/* Copyright (c) 2013 Kyle Verrier */

#include <getopt.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include "../src/UniformConvolver.h"
#include "CycleTimer.h"

// Impulse size
int hSize = 0;

// Output buffer size
int framesToOutput = 0;

// Number of times to run the simulation
int nRuns = 0;

// Number of threads to use
int nThreads = 0;


void usage(char *program) {
  printf("Usage: %s [options]\n"
         "  Solves the Wandering Salesman Problem for the distances provided \n"
         "  on stdin.\n"
         "\n"
         "Program Options:\n"
         "  -i <file>          Use <file> instead of standard in\n"
         "  -?  --help         This message\n", program);
}


void parse_args(int argc, char **argv) {
  int opt;

  static struct option long_opts[] = {
    {"input", 1, 0, 'i'},
    {"help", 0, 0, '?'},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "o:n:r:t:?h", long_opts, NULL)) != EOF) {
    switch (opt) {
    case 'o':
      framesToOutput = atoi(optarg);
      break;
    case 'n':
      nRuns = atoi(optarg);
      break;
    case 'r':
      hSize = atoi(optarg);
      break;
    case 't':
      nThreads = atoi(optarg);
      break;
    case 'h':                  /* Explicit fall through */
    case '?':
      if (optopt == 'r' || optopt == 't')
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      else {
        usage(argv[0]);
      }
      exit(EXIT_SUCCESS);
    default:
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


double rand_float( double low, double high ) {
  return ( ( double )rand() * ( high - low ) ) / ( double )RAND_MAX + low;
}

int main(int argc, char** argv) {
  parse_args(argc, argv);
  /*
  printf("Running simulation for IR size(%d), outputbuffer size (%d): %d times using %d threads...\n", 
      hSize, framesToOutput, nRuns, nThreads );
  */

  srand(1234);

  // Create Random impulse response
  double *h = new double[hSize];
  for (int i = 0; i < hSize; i++)
    h[i] = rand_float(0, 1);

  // Create input stream
  int inputSize = framesToOutput * nRuns;
  double *input = new double[inputSize];
  for (int i = 0; i < inputSize / 2; i++)
    input[i] = rand_float(0, 1);

  // Allocate output buffer
  double *output = new double[framesToOutput];

  // Create convolver
  UniformConvolver uc(h, hSize, framesToOutput, nThreads);

  // Run simulation for all of the input and measure computation time
  double total = 0.0;
  for (int i = 0; i < inputSize / framesToOutput; i++) {
    double startTime = CycleTimer::currentSeconds();
    uc.process(input+(i*framesToOutput), output);
    double endTime = CycleTimer::currentSeconds();
    total += endTime - startTime;
  }

  double avg = 1000 * total / (inputSize / framesToOutput);
  /*
  printf("Avg Processing per period: %.4fms\n", avg);
  printf("Rate: %.4fms\n", avg / framesToOutput);
  */
  printf("%.4f", avg / framesToOutput);

  return 0;
}
