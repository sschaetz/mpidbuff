#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "../lib/window.hpp"

#define DIM 100

namespace mpi = boost::mpi;

int main(int argc, char* argv[])
{
  int data[DIM];

  for(int i=0; i<DIM; i++)
  {
    data[i] = 0;
  }

  mpi::environment env(argc, argv);
  mpi::communicator world;

  mpi::window win((int*)&data, DIM, world);

  // test window properties
  BOOST_TEST_EQ((int*)&data, win.base<int>());
  BOOST_TEST_EQ(DIM*sizeof(int), win.size());
  BOOST_TEST_EQ(sizeof(int), win.dispunit());


  if(world.size() != 2)
  {
    BOOST_ERROR("this test should be executed with 2 processors "
                "(mpirun -n 2 <executable>)\n");
    exit(1);
  }

  if(world.rank() == 0) {
    for(int i=0; i<DIM; i++)                            // fill window on rank 0
    {
      data[i] = i;
    }
    win.fence();
    win.put(data, 100, 1, 0);                                        // put data
    win.fence();

    // _________________________________________________________________________

    world.send(1, 10);

    for(int i=0; i<DIM; i++)                                       // reset data
    {
      data[i] = 0;
    }

    world.recv(1, 10);
    for(int i=0; i<DIM; i++)                       // check if data was received
    {
      if(data[i] != i)
      {
        BOOST_TEST_EQ(data[i], i);
        break;
      }
    }

  }
  else
  {
    win.fence();
    win.fence();
    for(int i=0; i<DIM; i++)                       // check if data was received
    {
      if(data[i] != i)
      {
        BOOST_TEST_EQ(data[i], i);
        break;
      }
    }

    // _________________________________________________________________________

    world.recv(0, 10);
    win.lock(mpi::lock_exclusive, 0);
    win.put(data, 100, 0, 0);
    win.unlock(0);
    world.send(0, 10);
  }

  return 0;
}
