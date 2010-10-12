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


int main(int argc, char* argv[])
{

  vector<int, char, std::string> w1(make_vector(1, 'x', "howdy"));
  // save data to archive
  {
    std::ofstream ofs("archive.bin");
    boost::archive::text_oarchive oa(ofs);
    oa << w1;
    for_each(w1, print_xml());

  }

  vector<int, char, std::string> w2;
  // load data from archive
  {
    std::ifstream ifs("archive.bin");
    boost::archive::text_iarchive ia(ifs);
    ia >> w2;
    for_each(w2, print_xml());

  }

}
