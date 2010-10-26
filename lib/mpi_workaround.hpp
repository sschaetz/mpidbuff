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


 /**
 * @brief mpi send workaround
 *
 * This is a workaround for Boost MPI <= v1.44, these versions do not handle
 * through-base-pointer serialization properly
 * It serializes the data to be sent into a string and sends the string
 *
 * @param dest The rank of the remote process to which the data
 * will be sent.
 *
 * @param tag The tag that will be associated with this message. Tags
 * may be any integer between zero and an implementation-defined
 * upper limit. This limit is accessible via @c environment::max_tag().
 *
 * @param value The value that will be transmitted to the
 * receiver. The type @c T of this value must meet the aforementioned
 * criteria for transmission.
 *
 * @param comm the communicator that the send should be performed on
 *
 */
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

 /**
 * @brief mpi recv workaround
 *
 * This is a workaround for Boost MPI <= v1.44, these versions do not handle
 * through-base-pointer serialization properly
 * It receives a string and deserializes the string
 *
 * @param source The process that will be sending data. This will
 * either be a process rank within the communicator or the
 * constant any_source, indicating that we can receive the
 * message from any process.
 *
 * @param tag The tag that matches a particular kind of message sent
 * by the source process. This may be any tag value permitted by
 * send. Alternatively, the argument may be the constant any_tag,
 * indicating that this receive matches a message with any tag.
 *
 * @param values Will contain the values in the message after a
 * successful receive. The type of these elements must match the
 * type of the elements transmitted by the sender.
 *
 * @param n The number of values that can be stored into the
 * values array. This shall not be smaller than the number of
 * elements transmitted by the sender.
 *
 * @param comm the communicator that the send should be performed on
 *
 * @throws std::range_error if the message to be received contains
 * more than n values.
 *
 * @returns Information about the received message.
 *
 */
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

 /**
 * @brief mpi broadcast workaround
 *
 * This is a workaround for Boost MPI <= v1.44, these versions do not handle
 * through-base-pointer serialization properly
 * On the root the data is serialized into a string, on all other nodes a string
 * is received and deserialized
 *
 * @param comm The communicator over which the broadcast will
 * occur.
 *
 * @param value The value to be transmitted (if the rank of comm is equal to
 * root) or received (if the rank of comm is not equal to root).
 *
 * @param root The rank/process ID of the process that will be
 * transmitting the value.
 *
 */
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

