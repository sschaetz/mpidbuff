#ifndef LIB_MASTERWORKER_BASE_HPP
#define LIB_MASTERWORKER_BASE_HPP

#include <boost/mpi/communicator.hpp>

namespace masterworker
{
  class base
  {
   public:
    base() {}
    virtual void execute(boost::mpi::communicator & comm, int root = 0) {}

   private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){}
  };
}

#endif // LIB_MASTERWORKER_BASE_HPP
