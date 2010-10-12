#include <iostream>
#include <typeinfo>
#include <string>
#include <fstream>
#include <memory>
#include <boost/mpi.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/functional.hpp>
#include <boost/fusion/functional/invocation/invoke_function_object.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


#include <ext/fusion/serialization/include.hpp>

#include "lib/mpi_workaround.hpp"

using namespace boost::fusion;
namespace mpi = boost::mpi;

struct print_xml
{
    template <typename T>
    void operator()(T const& x) const
    {
        std::cout
            << '<' << typeid(x).name() << '>'
            << x
            << "</" << typeid(x).name() << '>'
            ;
    }
};

class base
{
 public:
  base() {}
  virtual void execute() {}

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version){}
};

class wrapper
{
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ptr;
  }
 public:
  boost::shared_ptr<base> ptr;
};

void func1(int a, char b, std::string c)
{
  std::cout << a << b << c << std::endl;
}



template <typename T>
class testclass : public base
{
 public:
  testclass() {}
  testclass(std::string s_, T t_) : s(s_), t(t_) {}

  void execute()
  {

    invoke(func1, t);
  }

  std::string s;
  T t;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & s;
    ar & t;
    ar & boost::serialization::base_object<base>( *this );
  }
};

typedef testclass<vector<int, char, std::string> > func1_type;

namespace boost {
namespace archive {
namespace detail {
namespace {
template<>
struct init_guid< func1_type > {
    static guid_initializer< func1_type > const & g;
};
guid_initializer< func1_type > const & init_guid< func1_type >::g =
    ::boost::serialization::singleton<
        guid_initializer< func1_type >
    >::get_mutable_instance().export_guid();
}}}}


namespace boost {
namespace serialization {
template<>
struct guid_defined< func1_type > : boost::mpl::true_ {};
template<>
inline const char * guid< func1_type >(){
    return "func1_type";
}
} /* serialization */
} /* boost */




int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.rank() == 0) // test with wrapper
  {
    wrapper w;
    w.ptr = boost::shared_ptr<func1_type >
      (new func1_type
        ("hi", make_vector(1, 'x', "howdy")));
    w.ptr->execute();
    mpi_send_workaround(1, 0, w, world);
  }
  else if(world.rank() == 1)
  {
    wrapper w;
    mpi_recv_workaround(0, 0, w, world);
    w.ptr->execute();
  }

}
