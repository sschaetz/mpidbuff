//#include <string>
//#include <iostream>
//
//#include <boost/mpi.hpp>
//#include <boost/detail/lightweight_test.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/serialization/shared_ptr.hpp>
//#include <boost/serialization/string.hpp>
//#include <boost/serialization/base_object.hpp>
//#include <boost/serialization/export.hpp>
//#include "../lib/mpi_workaround.hpp"
//
//class base
//{
// public:
//  base() {}
//  virtual void execute() {}
//
// private:
//  friend class boost::serialization::access;
//  template<class Archive>
//  void serialize(Archive & ar, const unsigned int version){}
//};
//
//class wrapper
//{
// private:
//  friend class boost::serialization::access;
//  template<class Archive>
//  void serialize(Archive & ar, const unsigned int version)
//  {
//    ar & ptr;
//  }
// public:
//  boost::shared_ptr<base> ptr;
//};
//
//
//void f1(int a, double b, std::string s)
//{
//  std::cout << "f1 " << a << " " << b << " " << s << std::endl;
//}
//
//void f2(float a, std::string s)
//{
//  std::cout << "f2 " << a << " " << s << std::endl;
//}
//
//class exec_f1 : public base
//{
// public:
//  exec_f1() {}
//  exec_f1(int a_, double b_, std::string s_) : a(a_), b(b_), s(s_) {}
//  void execute()
//  {
//    f1(a, b, s);
//  }
//
// private:
//  int a;
//  double b;
//  std::string s;
//
//  friend class boost::serialization::access;
//  template<class Archive>
//  void serialize(Archive & ar, const unsigned int version)
//  {
//    ar & a;
//    ar & b;
//    ar & s;
//    ar & boost::serialization::base_object<base>( *this );
//  }
//};
//
//BOOST_CLASS_EXPORT_IMPLEMENT(exec_f1);
//BOOST_CLASS_EXPORT_KEY(exec_f1);
//
//namespace mpi = boost::mpi;
//
//int main(int argc, char* argv[])
//{
//
//
//    // save data to archive
//    {
//      wrapper w;
//      w.ptr = boost::shared_ptr<exec_f1>
//        (new exec_f1(10, 11, std::string("hello!")));
//
//      std::ofstream ofs("archive.bin");
//      boost::archive::text_oarchive oa(ofs);
//      oa << w;
//    }
//
//
//    // load data from archive
//    {
//      wrapper w2;
//      std::ifstream ifs("archive.bin");
//      boost::archive::text_iarchive ia(ifs);
//      ia >> w2;
//      w2.ptr->execute(); // ok
//    }
//
//
//  return 0;
//}

#include <string>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


template <typename R, typename P>
struct wrapper
{

  struct functor_interface
  {
    functor_interface() { }
    virtual R operator()(P x) = 0;

   private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) { }
  };

 public:
  boost::shared_ptr<functor_interface> ptr;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ptr;
  }
};

struct f1 : public wrapper<int, int>::functor_interface {
  virtual int operator()(int x)
  {
    std::cout << x << std::endl;
    return x;
  }

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & boost::serialization::base_object<wrapper<int, int>::functor_interface >( *this );
  }
};

BOOST_CLASS_EXPORT_IMPLEMENT(f1);
BOOST_CLASS_EXPORT_KEY(f1);

int main(int argc, char* argv[])
{

  // save data to archive
  {
    wrapper<int, int> w;
    w.ptr = boost::shared_ptr<f1>(new f1());
    std::ofstream ofs("archive.bin");
    boost::archive::text_oarchive oa(ofs);
    oa << w;
  }


  // load data from archive
  {
    wrapper<int, int> w;
    std::ifstream ifs("archive.bin");
    boost::archive::text_iarchive ia(ifs);
    ia >> w;
    (*w.ptr)(15);
  }

  return 0;
}
