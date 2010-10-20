#ifndef LIB_MASTERWORKER_WORKER_HPP
#define LIB_MASTERWORKER_WORKER_HPP

#include <boost/mpi/communicator.hpp>
#include <lib/masterworker/wrapper.hpp>
#include <lib/mpi_workaround.hpp>

namespace masterworker
{
  /**
   * workerloop runs on all workers and executes worker functions
   *
   * the worker loop waits for wrapper objects to be broadcastet to all
   * workers, if the wrapper is a quit objects it shuts down, if the wrapper is
   * a work object it executes it
   *
   * @params comm the communicator object the workers and the master live in
   *
   * @params root the id of the root or master node that distribute the jobs
   */
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
        w.execute(comm, root);
      }
    }
  }
}

#endif // LIB_MASTERWORKER_WORKER_HPP

