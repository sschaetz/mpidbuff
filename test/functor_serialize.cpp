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
