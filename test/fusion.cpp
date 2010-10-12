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
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
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

template <typename T>
class testclass
{
  public:
   testclass() {}
   testclass(std::string s_, T t_) : s(s_), t(t_) {}

   std::string s;
   T t;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & s;
    ar & t;
  }
};


int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.rank() == 0) // test with wrapper
  {
    testclass<vector<int, char, std::string> >
      w("hi", make_vector(1, 'x', "howdy"));
    for_each(w.t, print_xml());
    mpi_send_workaround(1, 0, w, world);
  }
  else if(world.rank() == 1)
  {
    testclass<vector<int, char, std::string> > w;
    mpi_recv_workaround(0, 0, w, world);
    for_each(w.t, print_xml());
  }

}
