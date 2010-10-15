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
  void operator()(int, char, int, std::string);
};


template <typename T>
class func2_wrapper : public base
{
 public:
  func2_wrapper() {}
  func2_wrapper(T t_) : t(t_) {}

  void execute()
  {
    func2 f;
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

typedef func2_wrapper<boost::fusion::vector<int, char, int, std::string> > func2_type;


BOOST_CLASS_EXPORT_IMPLEMENT(func2_type);
BOOST_CLASS_EXPORT_KEY(func2_type);


#endif // DEV_FUNC2_HPP

