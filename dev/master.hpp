#ifndef DEV_MASTER_HPP
#define DEV_MASTER_HPP

#include <boost/fusion/include/vector.hpp>
#include <boost/mpi/communicator.hpp>
#include "dev/masterslave.hpp"
#include "lib/mpi_workaround.hpp"

template <typename T>
void run(typename T::vector_type myvec, boost::mpi::communicator & comm)
{
  typedef typename T::wrapper_type wrapper_type;

  wrapper w(boost::shared_ptr< wrapper_type >(new wrapper_type(myvec)));
  mpi_broadcast_workaround(comm, w, comm.rank());
}


inline void quit(boost::mpi::communicator & comm)
{
  wrapper q;
  mpi_broadcast_workaround(comm, q, comm.rank());
  comm.barrier();
}


#endif // DEV_MASTER_HPP
