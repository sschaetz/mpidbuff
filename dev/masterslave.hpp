#ifndef DEV_MASTERSLAVE_HPP
#define DEV_MASTERSLAVE_HPP

#define MASTERSLAVE_TAG 1000

#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/mpi/communicator.hpp>

class base
{
 public:
  base() {}
  virtual void execute(boost::mpi::communicator & comm) {}

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version){}
};

class wrapper
{
 private:
  bool quitflag;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ptr;
    ar & quitflag;
  }
  boost::shared_ptr<base> ptr;

 public:
  wrapper(boost::shared_ptr<base> ptr_) : ptr(ptr_) { quitflag = false; }
  wrapper() : quitflag(true) {  }
  void execute(boost::mpi::communicator & comm) { ptr->execute(comm); }
  bool quit() { return quitflag; }

};


#endif // DEV_MASTERSLAVE_HPP

