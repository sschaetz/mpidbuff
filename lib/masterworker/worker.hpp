#ifndef LIB_MASTERWORKER_WORKER_HPP
#define LIB_MASTERWORKER_WORKER_HPP

#include <boost/mpi/communicator.hpp>
#include <lib/masterworker/wrapper.hpp>
#include <lib/mpi_workaround.hpp>

namespace masterworker
{
  inline void workerloop(boost::mpi::communicator & comm, int root)
  {
    while(true)
    {
      masterworker::wrapper w;
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
}

#endif // LIB_MASTERWORKER_WORKER_HPP

