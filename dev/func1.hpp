#ifndef DEV_FUNC1_HPP
#define DEV_FUNC1_HPP

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <ext/fusion/serialization/include.hpp>

#include "masterslave.hpp"



struct func1
{
  void operator()(int, char, double);
};


template <typename T>
class func1_wrapper : public base
{
 public:
  func1_wrapper() {}
  func1_wrapper(T t_) : t(t_) {}

  void execute()
  {
    func1 f;
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

typedef func1_wrapper<boost::fusion::vector<int, char, double> > func1_type;


BOOST_CLASS_EXPORT_IMPLEMENT(func1_type);
BOOST_CLASS_EXPORT_KEY(func1_type);


#endif // DEV_FUNC1_HPP

