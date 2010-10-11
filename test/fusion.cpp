#include <iostream>
#include <typeinfo>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/functional.hpp>


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

void testfunction(int a, char b, std::string c)
{
  std::cout << a << " " << b << " " << c << std::endl;
}

template <typename T>
class testclass
{
 public:
  testclass(T data_) : data(data_) {}
  void execute()
  {
    for_each(data, print_xml());
    std::cout << std::endl;
    invoke(testfunction, data);
  }
 private:
  T data;
};




int main(int argc, char* argv[])
{
  std::cout << "hi!" << std::endl;
  testclass< vector<int, char, std::string> > hi(make_vector(1, 'x', "howdy"));
  hi.execute();


}
