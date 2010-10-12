#include <string>
#include <iostream>
#include <fstream>
#include <boost/mpi.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include "../lib/mpi_workaround.hpp"

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


void f1(int a, double b, std::string s)
{
  std::cout << "f1 " << a << " " << b << " " << s << std::endl;
}

void f2(float a, std::string s)
{
  std::cout << "f2 " << a << " " << s << std::endl;
}

class exec_f1 : public base
{
 public:
  exec_f1() {}
  exec_f1(int a_, double b_, std::string s_) : a(a_), b(b_), s(s_) {}
  void execute()
  {
    f1(a, b, s);
  }

 private:
  int a;
  double b;
  std::string s;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & a;
    ar & b;
    ar & s;
    ar & boost::serialization::base_object<base>( *this );
  }
};

BOOST_CLASS_EXPORT_IMPLEMENT(exec_f1);
BOOST_CLASS_EXPORT_KEY(exec_f1);

namespace mpi = boost::mpi;

int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.size() != 2)
  {
    BOOST_ERROR("this test should be executed with 2 processors "
                "(mpirun -n 2 <executable>)\n");
    exit(1);
  }

  if(world.rank() == 0) // test with wrapper
  {
    wrapper w;
    w.ptr =
      boost::shared_ptr<exec_f1>(new exec_f1(10, 11, std::string("hello!")));
    mpi_send_workaround(1, 0, w, world);
  }
  else if(world.rank() == 1)
  {
    wrapper w;
    mpi_recv_workaround(0, 0, w, world);
    w.ptr->execute();
  }

  return 0;
}
