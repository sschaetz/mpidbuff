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
  /**
   * @brief execute worker function
   *
   * execute a worker function on worker nodes (all ranks in communicater except
   * comm.rank()), wait for completion and return the results in a vector
   *
   * @param myvec parameters of the worker function in boost::fusion format
   *
   * @param comm the communicator the worker function should be distributed to
   *
   * @return an std::vector containing the return type of the worker function
   * enumerated like a communicator in which the master does not exist
   */
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
    ret.erase(ret.begin()+comm.rank());
    return ret;
  }

  /**
   * @brief start worker function asynchronously
   *
   * start a worker function on worker nodes (all ranks in communicater except
   * comm.rank()) and return
   *
   * @param myvec parameters of the worker function in boost::fusion format
   *
   * @param comm the communicator the worker function should be distributed to
   */
  template <typename T>
  void
  run_async(typename T::vector_type myvec, boost::mpi::communicator & comm)
  {
    typedef typename T::wrapper_type wrapper_type;

    masterworker::wrapper
      w(boost::shared_ptr< wrapper_type >(new wrapper_type(myvec)));
                                                              // start execution
    mpi_broadcast_workaround(comm, w, comm.rank());
  }

  /**
   * @brief wait for asynchronously started worker function
   *
   * wait for the completion of the worker function on all worker nodes (all
   * ranks in communicater except comm.rank()), and return the results in a
   * vector
   *
   * @param comm the communicator the worker function was distributed to
   *
   * @return an std::vector containing the return type of the worker function
   * enumerated like a communicator in which the master does not exist
   */
  template <typename T>
  std::vector<typename T::return_type>
  sync(boost::mpi::communicator & comm)
  {
    typedef typename T::return_type return_type;
    return_type r;
    std::vector<return_type> ret;
    boost::mpi::gather(comm, r, ret, comm.rank());
    ret.erase(ret.begin()+comm.rank());
    return ret;
  }

  /**
   * @brief quit all worker nodes
   *
   * signals all worker nodes to shut down (all ranks in communicater except
   * comm.rank()), returns as soon as all workers shut down
   * caution: can not be used to stop worker function execution
   *
   * @param comm the communicator containing all ranks that should be shut down
   */
  inline void quit(boost::mpi::communicator & comm)
  {
    masterworker::wrapper q;
    mpi_broadcast_workaround(comm, q, comm.rank());
    comm.barrier();
  }
}

#endif // LIB_MASTERWORKER_MASTER_HPP
