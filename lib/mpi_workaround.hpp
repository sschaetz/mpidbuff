#ifndef MPI_WORKAROUND_HPP
#define MPI_WORKAROUND_HPP

#include <string>
#include <iostream>
#include <boost/mpi.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


template <typename T>
void mpi_send_workaround(int dest, int tag, const T& value,
                         boost::mpi::communicator & comm)
{
  // serialize T into a string
  std::ostringstream oss;
  boost::archive::text_oarchive oa(oss);
  oa << value;
  // send the string
  comm.send(dest, tag, oss.str());
}

template <typename T>
boost::mpi::status mpi_recv_workaround(int source, int tag, T& value,
                                       boost::mpi::communicator & comm)
{
  // receive a string
  std::string s;
  boost::mpi::status st = comm.recv(source, tag, s);
  // serialize into T
  std::istringstream iss(s);
  boost::archive::text_iarchive ia(iss);
  ia >> value;
  return st;
}

template<typename T>
void mpi_broadcast_workaround(const boost::mpi::communicator& comm,
                              T& value, int root)
{
  if(comm.rank() == root)
  {
    // serialize T into a string
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << value;
    std::string s = oss.str();
    boost::mpi::broadcast(comm, s, root);
  }
  else
  {
    std::string s;
    boost::mpi::broadcast(comm, s, root);
    // serialize into T
    std::istringstream iss(s);
    boost::archive::text_iarchive ia(iss);
    ia >> value;
  }
}

#endif // MPI_WORKAROUND_HPP

