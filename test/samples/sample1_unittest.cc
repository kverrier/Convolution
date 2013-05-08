// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "../../src/UniformConvolver.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>


// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>
//

using namespace testing;

TEST(ConvolutionTest, Simple1) {
    double h[] = {0,2,2,2};
    int hSize  = 4;

    double inputStream[] = {1,2,1,1,0,0,0,0};
    int inputLength = 8;

    int framesToOutput = 2;
    double output[2] = {0};

    UniformConvolver uc(h, hSize, framesToOutput);

    std::vector<double> outputStream;
    for (int i = 0; i < inputLength / framesToOutput; i++) {
        uc.process(inputStream+(i*framesToOutput), output);
        std::cout << std::setprecision(5) << output[0] << std::endl;
        for (int i = 0; i < framesToOutput; i++) {
          outputStream.push_back(output[i]);
          // outputStream.push_back(i);
        }
    }

    double expected[] = {0, 2, 6, 8, 8, 4, 2, 0};

    ASSERT_DOUBLE_EQ(outputStream.at(7), 0.0f);
    // ASSERT_THAT(outputStream, ElementsAre(0, 2, 6, 8, 8, 4, 2, 0));

    for (int i = 0; i < inputLength; i++) {
      // std::cout << std::setprecision(5) << outputStream.at(i) << std::endl;
    }
}

/*
TEST(ConvolutionTest, Simple) {
    double h[] = { 4, 10, 6, 0, 2, 8, 6, 1, 1, 5, 0, 5, 0, 2, 7, 3};
    int hSize = 16;

    double inputStream[] = { 10, 0, 10, 7, 8, 0, 3, 6, 2, 0, 10, 5, 6, 6, 6, 5,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int inputLength = 32;

    int framesToOutput = 16;
    double *output = new double[framesToOutput];


    UniformConvolver uc(h, hSize, framesToOutput);

    std::vector<double> outputStream;
    for (int i = 0; i < inputLength / framesToOutput; i++) {
        uc.process(inputStream+(i*framesToOutput), output);
        for (int i = 0; i < framesToOutput; i++)
            outputStream.push_back(output[i]);
    }

    double expected[] = { 40, 100, 100, 128, 182, 202, 140, 158,
                          228, 222, 123, 271, 247, 279, 266, 319,
                          314, 250, 218, 195, 146, 177,  98,  91,
                          135, 107,  94,  72,  70,  53,  15 };

    for (int i = 0; i < inputLength; i++) {
      EXPECT_DOUBLE_EQ(outputStream[i], expected[i]);
    }


}
*/


// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
