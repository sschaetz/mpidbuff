#ifndef DEV_FUNC1_HPP
#define DEV_FUNC1_HPP

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
#include <boost/fusion/functional/invocation/invoke_function_object.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <ext/fusion/serialization/include.hpp>

#include "masterslave.hpp"


struct func1
{
  void operator()(int, char, std::string);
};


template <typename T>
class func1_class : public base
{
 public:
  func1_class() {}
  func1_class(std::string s_, T t_) : s(s_), t(t_) {}

  void execute()
  {
    func1 f;
    invoke_procedure(f, t);
  }

  std::string s;
  T t;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & s;
    ar & t;
    ar & boost::serialization::base_object<base>( *this );
  }
};

typedef func1_class<boost::fusion::vector<int, char, std::string> > func1_type;


BOOST_CLASS_EXPORT_IMPLEMENT(func1_type);
BOOST_CLASS_EXPORT_KEY(func1_type);


#endif // DEV_FUNC1_HPP

