#include <limits.h>
#include "gtest/gtest.h"

#include <iostream>
using namespace std;

int Factorial(int n) {
  int result = 1;
  for (int i = 1; i <= n; i++) {
    result *= i;
  }

  return result;
}

TEST(FactorialTest, Zero) {
  EXPECT_EQ(1, Factorial(0));
}
