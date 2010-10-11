#include <boost/mpi.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include "../lib/mpi_workaround.hpp"

class test
{
 public:
  test(std::string i_, std::string j_) : i(i_), j(j_) { }
  test() {}
  std::string i;
  std::string j;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & i;
    ar & j;
  }
};

class wrapper
{
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ptr;
  }
 public:
  boost::shared_ptr<test> ptr;
};

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

  std::string s1("fooooo");
  std::string s2("baaaaar");

  if(world.rank() == 0) // test with wrapper
  {
    wrapper w1;
    w1.ptr = boost::shared_ptr<test>(new test(s1, s2));
    mpi_send_workaround(1, 0, w1, world);
  }
  else if(world.rank() == 1)
  {
    wrapper w2;
    mpi_recv_workaround(0, 0, w2, world);
    BOOST_TEST_EQ(s1, w2.ptr->i);
    BOOST_TEST_EQ(s2, w2.ptr->j);
  }

  return 0;
}
