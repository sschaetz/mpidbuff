#include "func2.hpp"


void func2::operator()(int a, char b, int c, std::string d)
{
  std::cout << "func2 " << comm.rank() << " " << a << b << c << d << std::endl;
}

