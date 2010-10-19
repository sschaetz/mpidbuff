#ifndef LIB_MASTERWORKER_MASTER_HPP
#define LIB_MASTERWORKER_MASTER_HPP

#include <vector>

#include <boost/fusion/include/vector.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <lib/masterworker/wrapper.hpp>
#include <lib/mpi_workaround.hpp>

namespace masterworker
{
  template <typename T>
  std::vector<typename T::return_type>
  run(typename T::vector_type myvec, boost::mpi::communicator & comm)
  {
    typedef typename T::wrapper_type wrapper_type;
    typedef typename T::return_type return_type;
    masterworker::wrapper
      w(boost::shared_ptr< wrapper_type >(new wrapper_type(myvec)));
                                                              // start execution
    mpi_broadcast_workaround(comm, w, comm.rank());
    return_type r;
    std::vector<return_type> ret;
    boost::mpi::gather(comm, r, ret, comm.rank());
    ret.erase(ret.begin());
    return ret;
  }

  inline void quit(boost::mpi::communicator & comm)
  {
    masterworker::wrapper q;
    mpi_broadcast_workaround(comm, q, comm.rank());
    comm.barrier();
  }
}

#endif // LIB_MASTERWORKER_MASTER_HPP
