#ifndef LIB_ASYNC_REMOTE_RANGE_HPP
#define LIB_ASYNC_REMOTE_RANGE_HPP

#include <vector>
#include <cstdlib>

#include <boost/mpi/communicator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/function.hpp>
#include <lib/window.hpp>

template <typename T>
class async_remote_range
{
 public:
  async_remote_range(int num_windows_, std::size_t windowsize_,
                     boost::function<int (int rank, int iteration)>
                     nextwindow_func_,
                     const boost::mpi::communicator & comm_) :
                     num_windows(num_windows_),
                     windowsize(windowsize_),
                     nextwindow_func(nextwindow_func_),
                     comm(comm_)
  {
  }

  void init()
  {
    // generate windows on remote node
    for(int i=0; i<num_windows; i++)
    {
      windows.push_back(boost::mpi::window(comm));
    }
  }

  template <typename X>
  void init(boost::iterator_range<X> r)
  {
    // generate windows on local node
    typedef typename boost::iterator_range<X>::iterator it_type;
    it_type it = r.begin();
    for(int i=0; i<num_windows; i++)
    {
      boost::mpi::window w(&*it, (int)windowsize, comm);
      windows.push_back(w);
      it += windowsize;
    }
  }

  boost::mpi::window & get_window(int n) const
  {
    return windows[nextwindow_func(comm.rank(), n)];
  }

 private:
  const boost::mpi::communicator & comm;
  int num_windows;
  std::size_t windowsize;
  boost::function<int (int rank, int iteration)> nextwindow_func;
  std::vector<boost::mpi::window> windows;

};

template <typename T>
class async_remote_iterator
{
 public:
  async_remote_iterator(const async_remote_range<T> & range_, int i_) :
    range(range_), i(i_)
  {
  }

  async_remote_iterator<T> operator ++(int)
  {
    return async_remote_iterator(range, (i++));
  }

  boost::mpi::window & operator *()
  {
    return range.get_window(i);
  }

 private:
  const async_remote_range<T> & range;
  int i;
};

template <typename T>
void fetch(async_remote_iterator<T> it, T* dest, int root)
{
  (*it).lock(boost::mpi::lock_shared, root);
  (*it).get(dest, (*it).size()/sizeof(T), root, 0);
  (*it).unlock(root);
}

#endif // LIB_ASYNC_REMOTE_RANGE_HPP
