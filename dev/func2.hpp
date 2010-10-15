#ifndef DEV_FUNC2_HPP
#define DEV_FUNC2_HPP

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <ext/fusion/serialization/include.hpp>

#include "masterslave.hpp"


struct func2
{
  func2(boost::mpi::communicator & comm_) : comm(comm_){}
  void operator()(int, char, int, std::string);
  boost::mpi::communicator comm;
};


template <typename T>
class func2_wrapper : public base
{
 public:
  func2_wrapper() {}
  func2_wrapper(T t_) : t(t_) {}

  void execute(boost::mpi::communicator & comm)
  {
    func2 f(comm);
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
  struct func2
  {
    typedef boost::fusion::vector<int, char, double, std::string> vector_type;
    typedef func2_wrapper<boost::fusion::vector<int, char, double, std::string>
                         > wrapper_type;
  };
}


BOOST_CLASS_EXPORT_IMPLEMENT(slave::func2::wrapper_type);
BOOST_CLASS_EXPORT_KEY(slave::func2::wrapper_type);


#endif // DEV_FUNC2_HPP

