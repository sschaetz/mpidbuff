#include <exception>
#include <boost/throw_exception.hpp>
#include "../lib/window.hpp"

namespace boost {
  namespace mpi {

// _____________________________________________________________________________

window::window(communicator & comm, bool no_locks)
{
  create_window_impl(NULL, 0, 1, comm, no_locks);
}

// _____________________________________________________________________________

window::window(const void * base, int size, int disp_unit,
               communicator & comm, bool no_locks)
{
  create_window_impl(base, size, disp_unit, comm, no_locks);
}

// _____________________________________________________________________________

void window::create_window_impl(const void * base, int size, int disp_unit,
                                const communicator & comm, bool no_locks)
{
  MPI_Info info;
  BOOST_MPI_CHECK_RESULT(MPI_Info_create, (&info));
  if(no_locks)
  {
    BOOST_MPI_CHECK_RESULT(MPI_Info_set, (info, (char *)"no_locks",
                                          (char *)"true"));
  }
  else
  {
    BOOST_MPI_CHECK_RESULT(MPI_Info_set, (info, (char *)"no_locks",
                                          (char *)"false"));
  }

  BOOST_MPI_CHECK_RESULT(MPI_Win_create, (const_cast<void *>(base), size,
    disp_unit, info, MPI_Comm(comm), &win));
}

// _____________________________________________________________________________

int window::size() const
{
  unsigned int size_ = 0;
  void * sp = 0;
  int flag;
  BOOST_MPI_CHECK_RESULT(MPI_Win_get_attr, (win, MPI_WIN_SIZE,
                                            (void*)&sp, &flag));
  if(flag)
  {
    size_ = *(int*)sp;
  }
  return size_;
}

// _____________________________________________________________________________

int window::dispunit() const
{
  unsigned int dispunit_ = 0;
  void * dp = 0;
  int flag;
  BOOST_MPI_CHECK_RESULT(MPI_Win_get_attr,
                         (win, MPI_WIN_DISP_UNIT, (void*)&dp, &flag));
  if(flag)
  {
    dispunit_ = *(int*)dp;
  }
  return dispunit_;
}

// _____________________________________________________________________________

void window::put_impl(const void * origin_addr, int origin_count,
                      MPI_Datatype origin_datatype, int target_rank,
                      int target_disp, int target_count,
                      MPI_Datatype target_datatype, mpl::true_) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Put, (const_cast<void *>(origin_addr),
                                   origin_count, origin_datatype,
                                   target_rank, target_disp,
                                   target_count, target_datatype,
                                   win));
}

// _____________________________________________________________________________

void window::put_impl(const void * origin_addr, int origin_count,
                      MPI_Datatype origin_datatype, int target_rank,
                      int target_disp, int target_count,
                      MPI_Datatype target_datatype, mpl::false_) const
{
  boost::throw_exception(boost::mpi::exception(
    "one sided communication currently supports only MPI datatypes", 0));
}

// _____________________________________________________________________________

void window::get_impl(const void * origin_addr, int origin_count,
                      MPI_Datatype origin_datatype, int target_rank,
                      int target_disp, int target_count,
                      MPI_Datatype target_datatype, mpl::true_) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Get, (const_cast<void *>(origin_addr),
                                   origin_count, origin_datatype,
                                   target_rank, target_disp,
                                   target_count, target_datatype,
                                   win));
}

// _____________________________________________________________________________

void window::get_impl(const void * origin_addr, int origin_count,
                      MPI_Datatype origin_datatype, int target_rank,
                      int target_disp, int target_count,
                      MPI_Datatype target_datatype, mpl::false_) const
{
  boost::throw_exception(boost::mpi::exception(
    "one sided communication currently supports only MPI datatypes", 0));
}

// _____________________________________________________________________________

void window::fence() const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_fence, (0, win));
}

// _____________________________________________________________________________

void window::start(group g) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_start, (MPI_Group(g), 0, win));
}

// _____________________________________________________________________________

void window::complete() const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_complete, (win));
}

// _____________________________________________________________________________

void window::post(group g) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_post, (MPI_Group(g), 0, win));
}

// _____________________________________________________________________________

void window::wait() const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_wait, (win));
}

// _____________________________________________________________________________

bool window::test() const
{
  int flag;
  BOOST_MPI_CHECK_RESULT(MPI_Win_test, (win, &flag));
  if(flag)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// _____________________________________________________________________________

void window::lock(lock_type lock_type_, int rank) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_lock, (lock_type_, rank, 0, win));
}

// _____________________________________________________________________________

void window::unlock(int rank) const
{
  BOOST_MPI_CHECK_RESULT(MPI_Win_unlock, (rank, win));
}

// _____________________________________________________________________________

window::~window()
{
  MPI_Win_free(&win);
}

// _____________________________________________________________________________

  }
}

