#include <iostream>
#include <typeinfo>
#include <string>
#include <fstream>
#include <memory>
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


using namespace boost::fusion;

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

  testclass<vector<int, char, std::string> >
    w1("hi", make_vector(1, 'x', "howdy"));
  // save data to archive
  {
    std::ofstream ofs("archive.bin");
    boost::archive::text_oarchive oa(ofs);
    oa << w1;
    for_each(w1.t, print_xml());

  }

  testclass<vector<int, char, std::string> > w2;
  // load data from archive
  {
    std::ifstream ifs("archive.bin");
    boost::archive::text_iarchive ia(ifs);
    ia >> w2;
    for_each(w2.t, print_xml());

  }

}
