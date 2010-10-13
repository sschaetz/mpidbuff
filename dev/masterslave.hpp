#ifndef DEV_MASTERSLAVE_HPP
#define DEV_MASTERSLAVE_HPP


class base
{
 public:
  base() {}
  virtual void execute() {}

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version){}
};

class wrapper
{
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & ptr;
  }
 public:
  boost::shared_ptr<base> ptr;
};


#endif // DEV_MASTERSLAVE_HPP

