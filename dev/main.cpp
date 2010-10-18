#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <lib/masterworker/worker_function.hpp>
#include <lib/masterworker/worker.hpp>
#include <lib/masterworker/master.hpp>

using namespace masterworker;

WORKER_FUNCTION(func3, (int, i) (int, j))
{
  std::cout << "OH MY GOD IT'S FUNC3!!" << i << j << std::endl;
}

WORKER_FUNCTION(func4, (int, i) (double, j))
{
  std::cout << "OH MY GOD IT'S FUNC3!!" << i << j << std::endl;
}


namespace mpi = boost::mpi;
using namespace boost::fusion;


int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.rank() == 0)
  {
    run<worker::func3>(make_vector(42, 110), world);
    run<worker::func4>(make_vector(42, 110.999), world);
    quit(world);
  }
  else if(world.rank() != 0)
  {
    workerloop(world, 0);
  }

}

