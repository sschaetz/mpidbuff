#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include "dev/func1.hpp"
#include "dev/func2.hpp"
#include "lib/mpi_workaround.hpp"
#include "dev/slave.hpp"
#include "dev/master.hpp"

namespace mpi = boost::mpi;
using namespace boost::fusion;

int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.rank() == 0)
  {
    run<slave::func1>(make_vector(1, 'x', 1.0), world);

    for(int i=0; i<2; i++)
    {
      run<slave::func1>(make_vector(1, 'x', 1.0), world);
    }
    run<slave::func2>(make_vector(1, 'x', 1.0, "yay buddy"), world);
    quit(world);
  }
  else if(world.rank() != 0)
  {
    slaveloop(world, 0);
  }

}

