#include "lib/mpi_workaround.hpp"
#include "dev/func1.hpp"

namespace mpi = boost::mpi;

int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.rank() == 0) // test with wrapper
  {
    wrapper w;
    w.ptr = boost::shared_ptr< func1_type >
      (new func1_type ("hi", boost::fusion::make_vector(1, 'x', "howdy")));
    w.ptr->execute();
    mpi_send_workaround(1, 0, w, world);
  }
  else if(world.rank() == 1)
  {
    wrapper w;
    mpi_recv_workaround(0, 0, w, world);
    w.ptr->execute();
  }

}
