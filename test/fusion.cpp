#include <iostream>
#include <typeinfo>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/functional.hpp>
#include <boost/mpi.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/bind.hpp>
#include "../lib/mpi_workaround.hpp"


using namespace boost::fusion;

struct saver
{
  template <typename Archive, typename T>
  void operator()(Archive &ar, T const& x) const
  {
    ar << x;
  }
};

class base
{
 public:
  base() {}
  virtual void execute() = 0;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version){}
};

template <typename T1, typename T2, typename R>
class slavefunction : public base
{
 public:
  typedef vector<T1, T2> params;

  slavefunction() {}
  slavefunction(T1 p1, T2 p2)
  {
    data = make_vector(p1, p2);
  }

  virtual R operator()(T1, T2) = 0;

  void execute()
  {
    std::cout << (*this)(at_c<0>(data), at_c<1>(data)) << std::endl;
  }

 private:
  params data;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    //saver s;
    //for_each(data, boost::bind(s, _2));
  }
};

class myfunction : public slavefunction<int, int, int>
{
 public:
  myfunction() {}
  myfunction(int a, int b) : slavefunction<int, int, int>(a, b) {}

  int operator()(int a, int b)
  {
    return a + b;
  }

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)  { }
};

class wrapper
{
 public:
  boost::shared_ptr<base> ptr;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ptr;
  }
};

BOOST_CLASS_EXPORT_IMPLEMENT( slavefunction );
BOOST_CLASS_EXPORT_KEY( slavefunction );

BOOST_CLASS_EXPORT_IMPLEMENT(myfunction);
BOOST_CLASS_EXPORT_KEY(myfunction);

namespace mpi = boost::mpi;

int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  std::cout << "hi!" << std::endl;
  myfunction f(4, 2);
  myfunction(4, 2).execute();

  if(world.rank() == 0) // test with wrapper
  {
    wrapper w;
    w.ptr = boost::shared_ptr<myfunction>(new myfunction(4, 8));
    mpi_send_workaround(1, 0, w, world);
  }
  else if(world.rank() == 1)
  {
    wrapper w;
    mpi_recv_workaround(0, 0, w, world);
    w.ptr->execute();
  }

}
