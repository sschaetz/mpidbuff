#ifndef DEV_SLAVE_HPP
#define DEV_SLAVE_HPP

#include <boost/mpi/communicator.hpp>
#include "dev/masterslave.hpp"
#include "lib/mpi_workaround.hpp"

inline void slaveloop(boost::mpi::communicator & comm, int root)
{
  while(true)
  {
    wrapper w;
    mpi_broadcast_workaround(comm, w, root);
    if(w.quit())
    {
      comm.barrier();
      return;
    }
    else
    {
      w.execute(comm);
    }
  }
}


#endif // DEV_SLAVE_HPP

