#ifndef LIB_MASTERWORKER_WRAPPER_HPP
#define LIB_MASTERWORKER_WRAPPER_HPP

namespace masterworker
{
  class base;

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
}

#endif // LIB_MASTERWORKER_WRAPPER_HPP

