#include <vector>
#include <algorithm>

#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <lib/masterworker/masterworker.hpp>

using namespace masterworker;

WORKER_FUNCTION(int, func3, (int, i) (int, j))
{
  std::cout << "OH MY GOD IT'S FUNC3!!" << i << j << std::endl;
  return 2;
}

WORKER_FUNCTION(int, func4, (int, i) (const double &, j))
{
  std::cout << "OH MY GOD IT'S FUNC4!!!!" << i << j << std::endl;
  return 1;
}

WORKER_FUNCTION(void, func5, (const int, i) (double &, j))
{
  std::cout << "OH MY GOD IT'S FUNC5!!!!" << i << j << std::endl;
}

WORKER_VOID_FUNCTION(func6, (const int, i) (double &, j))
{
  std::cout << "OH MY GOD IT'S FUNC5!!!!" << i << j << std::endl;
}

namespace mpi = boost::mpi;
using namespace boost::fusion;

template <typename T>
void printvector(T i)
{
  std::cout << " " << i;
}


int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.rank() == 0)
  {
    std::vector<int> f3 = run<worker::func3>(make_vector(42, 110), world);
    std::cout << "f3 returns: ";
    for_each (f3.begin(), f3.end(), printvector<int>);
    std::cout << std::endl << "f4 returns: ";

    std::vector<worker::func4::return_type> f4 =
      run<worker::func4>(make_vector(42, 110.999), world);
    for_each (f4.begin(), f4.end(), printvector<int>);
    std::cout << std::endl;

    run<worker::func5>(make_vector(42, 110.999), world);

    std::vector<worker::func6::return_type> f6 =
      run<worker::func6>(make_vector(42, 110.999), world);
    for_each (f6.begin(), f6.end(), printvector<int>);
    std::cout << std::endl;

    quit(world);
  }
  else if(world.rank() != 0)
  {
    workerloop(world, 0);
  }

}
