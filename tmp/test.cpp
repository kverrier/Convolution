
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
