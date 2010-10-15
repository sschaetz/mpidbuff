#include "func1.hpp"


void func1::operator()(int a, char b, double)
{
  std::cout << "func1 " << comm.rank() << " " << a << b << std::endl;
}

