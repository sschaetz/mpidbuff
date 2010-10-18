#include <iostream>
#include <string>
#include <boost/function_types/function_type.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/function.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/functional.hpp>
#include <boost/fusion/functional/invocation/invoke_function_object.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/mpl.hpp>

#include <ext/fusion/serialization/include.hpp>

struct myftor
{
  double operator()(int i, int j)
  {
    return (double)i+j;
  }
};

template <typename Retval, typename Params>
class slave
{
  typedef typename boost::mpl::push_front< Params, Retval >::type Func_params;
  typedef typename boost::function_types::function_type< Func_params >::type Func_type;
  typedef typename boost::fusion::result_of::as_vector< Params >::type Fusion_type;

  struct params
  {
    Fusion_type p;
   private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & p;
    }
  };

 public:
  Retval execute(boost::function< Func_type > f, Fusion_type s)
  {
    std::cout << typeid(*this).name() << std::endl;
    return boost::fusion::invoke(f, s);
  }

 private:
  boost::function< Func_type > f;
};

int main(int argc, char* argv[])
{
  slave<double, boost::mpl::vector< int, int > > s;
  std::cout << s.execute(myftor(), boost::fusion::make_vector(1, 2)) << std::endl;
}
