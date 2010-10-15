#ifndef DEV_FUNC1_HPP
#define DEV_FUNC1_HPP

#include <boost/mpi/communicator.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <ext/fusion/serialization/include.hpp>

#include "masterslave.hpp"



struct func1
{
  func1(boost::mpi::communicator & comm_) : comm(comm_){}
  void operator()(int, char, double);
  boost::mpi::communicator comm;
};


template <typename T>
class func1_wrapper : public base
{
 public:
  func1_wrapper() {}
  func1_wrapper(T t_) : t(t_) {}

  void execute(boost::mpi::communicator & comm)
  {
    func1 f(comm);
    invoke_procedure(f, t);
  }

  T t;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & t;
    ar & boost::serialization::base_object<base>( *this );
  }
};

namespace slave
{
  struct func1
  {
    typedef boost::fusion::vector<int, char, double> vector_type;
    typedef func1_wrapper<boost::fusion::vector<int, char, double> >
      wrapper_type;
  };
}


BOOST_CLASS_EXPORT_IMPLEMENT(slave::func1::wrapper_type);
BOOST_CLASS_EXPORT_KEY(slave::func1::wrapper_type);


#endif // DEV_FUNC1_HPP

