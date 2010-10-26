#ifndef BOOST_MPI_WINDOW_HPP
#define BOOST_MPI_WINDOW_HPP

#include <exception>
#include <boost/throw_exception.hpp>
#include <boost/mpi/config.hpp>
#include <boost/mpi/exception.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/group.hpp>
#include <boost/mpi/operations.hpp>

namespace boost {
  namespace mpi {

/**
 * @brief A constant representing a lock type
 */
enum lock_type
{
  lock_exclusive = MPI_LOCK_EXCLUSIVE,
  lock_shared = MPI_LOCK_SHARED
};


class BOOST_MPI_DECL window
{

 public:

  /**
   * Create an empty window
   */
  window(communicator & comm, bool no_locks = false);

  /**
   * Create window on one value
   */
  template<typename T>
  window(const T & base, communicator & comm, bool no_locks = false);

  /**
   * Create window on array of values
   */
  template<typename T>
  window(const T * base, int n, const communicator & comm, bool no_locks = false);

  /**
   * Create window on block of memory
   */
  window(const void * base, int size, int disp_unit,
         communicator & comm, bool no_locks = false);

  /**
   * Get the base address of the window
   */
  template<typename T>
  T * base() const;

  /**
   * Get the size of the window
   */
  int size() const;

  /**
   * Get the displacement unit of the window
   */
  int dispunit() const;

  /**
   * Put one value to a destination
   * Data is copied from the origin to the target
   */
  template<typename T>
  void put(const T & origin, int target_rank, int target_disp) const;

  /**
   * Put an array of values to a destination
   * Data is copied from the origin to the target
   */
  template<typename T>
  void put(const T * origin, int count, int target_rank, int target_disp) const;

  /**
   * Get one value from a destination
   * Data is copied from the target to the origin
   */
  template<typename T>
  void get(const T & origin, int target_rank, int target_disp) const;

  /**
   * Get an array of values from a destination
   * Data is copied from the target to the origin
   */
  template<typename T>
  void get(const T * origin, int count, int target_rank, int target_disp) const;

  /**
   * Put one value to a destination
   * Data is copied from the origin to the target
   */
  template<typename T, typename Op>
  void accumulate(const T & origin, int target_rank,
                  int target_disp, Op op) const;

  /**
   * Put an array of values to a destination
   * Data is copied from the origin to the target
   */
  template<typename T, typename Op>
  void accumulate(const T * origin, int count, int target_rank, int target_disp,
                  Op op) const;

  /**
   * Synchornize remote memory access calls
   */
  void fence() const;

  /**
   * Start remote memory access epoch
   */
  void start(group g) const;

  /**
   * Complete remote memory access epoch
   */
  void complete() const;

  /**
   * Start remote memory access exposure epoch
   */
  void post(group g) const;

  /**
   * Complete remote memory access exposure epoch
   */
  void wait() const;

  /**
   * Test if remote memory access exposure epoch is completed
   */
  bool test() const;

  /**
   * Start remote memory access epoch
   */
  void lock(lock_type lock_type_, int rank) const;

  /**
   * Complete remote memory access epoch
   */
  void unlock(int rank) const;

  /**
   * Destory window
   */
  ~window();

 private:

  void create_window_impl(const void * base, int size, int disp_unit,
                          const communicator & comm, bool no_locks = false);

  void put_impl(const void * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype,
                mpl::true_) const;

  void put_impl(const void * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype,
                mpl::false_) const;

  void get_impl(const void * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype,
                mpl::true_) const;


  void get_impl(const void * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype,
                mpl::false_) const;

  template<typename T, typename Op>
  void accumulate_impl(const T * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype, Op op,
                mpl::true_ /*is_mpi_datatype*/, mpl::true_ /*is_mpi_op*/) const;

  template<typename T, typename Op>
  void accumulate_impl(const T * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype, Op op,
                mpl::true_ /*is_mpi_datatype*/,
                mpl::false_ /*is_mpi_op*/) const;

  template<typename T, typename Op>
  void accumulate_impl(const T * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype, Op op,
                mpl::false_ /*is_mpi_datatype*/,
                mpl::true_ /*is_mpi_op*/) const;

  template<typename T, typename Op>
  void accumulate_impl(const T * origin_addr, int origin_count,
                MPI_Datatype origin_datatype, int target_rank, int target_disp,
                int target_count, MPI_Datatype target_datatype, Op op,
                mpl::false_ /*is_mpi_datatype*/,
                mpl::false_ /*is_mpi_op*/) const;

  MPI_Win win;

};


// Implementation
// _____________________________________________________________________________


template<typename T>
window::window(const T& base, communicator & comm, bool no_locks)
{
  create_window_impl((void *) &base, sizeof(T), sizeof(T), comm, no_locks);
}

// _____________________________________________________________________________

template<typename T>
window::window(const T * base, int n, const communicator & comm, bool no_locks)
{
  create_window_impl((void *) base, sizeof(T)*n, sizeof(T), comm, no_locks);
}

// _____________________________________________________________________________

template<typename T>
T * window::base() const
{
  T * base;
  int flag;
  BOOST_MPI_CHECK_RESULT(MPI_Win_get_attr, (win, MPI_WIN_BASE, &base, &flag));
  return base;
}

// _____________________________________________________________________________

template<typename T>
void window::put(const T & origin, int target_rank, int target_disp) const
{
  put_impl((void *)&origin, 1, get_mpi_datatype<T>(origin), target_rank,
           target_disp, 1, get_mpi_datatype<T>(*origin),
           is_mpi_datatype<T>());
}

// _____________________________________________________________________________

template<typename T>
void window::put(const T * origin, int count,
                 int target_rank, int target_disp) const
{
  put_impl((void *)origin, count, get_mpi_datatype<T>(*origin), target_rank,
           target_disp, count, get_mpi_datatype<T>(*origin),
           is_mpi_datatype<T>());
}

// _____________________________________________________________________________

template<typename T>
void window::get(const T & origin,
                 int target_rank, int target_disp) const
{
  get_impl((void *)&origin, 1, get_mpi_datatype<T>(origin), target_rank,
           target_disp, 1, get_mpi_datatype<T>(*origin),
           is_mpi_datatype<T>());
}

// _____________________________________________________________________________

template<typename T>
void window::get(const T * origin, int count,
                 int target_rank, int target_disp) const
{
  get_impl((void *)origin, count, get_mpi_datatype<T>(*origin), target_rank,
           target_disp, count, get_mpi_datatype<T>(*origin),
           is_mpi_datatype<T>());
}

// _____________________________________________________________________________

template<typename T, typename Op>
void window::accumulate(const T & origin, int target_rank,
                        int target_disp, Op op) const
{
  accumulate_impl((T *)&origin, 1, get_mpi_datatype<T>(*origin),
                  target_rank, target_disp, 1, get_mpi_datatype<T>(*origin),
                  op, is_mpi_datatype<T>(), is_mpi_op<Op, T>());
}

// _____________________________________________________________________________

template<typename T, typename Op>
void window::accumulate(const T * origin, int count, int target_rank,
                        int target_disp, Op op) const
{
  accumulate_impl(origin, count, get_mpi_datatype<T>(*origin),
                  target_rank, target_disp, count, get_mpi_datatype<T>(*origin),
                  op, is_mpi_datatype<T>(), is_mpi_op<Op, T>());
}

// _____________________________________________________________________________

template<typename T, typename Op>
void window::accumulate_impl(const T * origin_addr, int origin_count,
              MPI_Datatype origin_datatype, int target_rank, int target_disp,
              int target_count, MPI_Datatype target_datatype, Op op,
              mpl::true_ /*is_mpi_datatype*/, mpl::true_ /*is_mpi_op*/) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Accumulate,
                                  (const_cast<void *>((void*)origin_addr),
                                   origin_count, origin_datatype,
                                   target_rank, target_disp,
                                   target_count, target_datatype,
                                   (is_mpi_op<Op, T>::op()), win));
}

// _____________________________________________________________________________

template<typename T, typename Op>
void window::accumulate_impl(const T * origin_addr, int origin_count,
              MPI_Datatype origin_datatype, int target_rank, int target_disp,
              int target_count, MPI_Datatype target_datatype, Op op,
              mpl::true_ /*is_mpi_datatype*/, mpl::false_ /*is_mpi_op*/) const
{
  detail::user_op<Op, T> mpi_op(op);
  BOOST_MPI_CHECK_RESULT(MPI_Accumulate,
                                  (const_cast<void *>((void*)origin_addr),
                                   origin_count, origin_datatype,
                                   target_rank, target_disp,
                                   target_count, target_datatype,
                                   mpi_op.get_mpi_op(), win));
}

// _____________________________________________________________________________

template<typename T, typename Op>
void window::accumulate_impl(const T * origin_addr, int origin_count,
              MPI_Datatype origin_datatype, int target_rank, int target_disp,
              int target_count, MPI_Datatype target_datatype, Op op,
              mpl::false_ /*is_mpi_datatype*/, mpl::true_ /*is_mpi_op*/) const
{
  boost::throw_exception(boost::mpi::exception(
    "one sided communication currently supports only MPI datatypes", 0));
}

// _____________________________________________________________________________

template<typename T, typename Op>
void window::accumulate_impl(const T * origin_addr, int origin_count,
              MPI_Datatype origin_datatype, int target_rank, int target_disp,
              int target_count, MPI_Datatype target_datatype, Op op,
              mpl::false_ /*is_mpi_datatype*/, mpl::false_ /*is_mpi_op*/) const
{
  boost::throw_exception(boost::mpi::exception(
    "one sided communication currently supports only MPI datatypes", 0));
}

// _____________________________________________________________________________


  }
}

#endif // BOOST_MPI_WINDOW_HPP
