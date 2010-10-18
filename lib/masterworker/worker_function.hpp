#ifndef LIB_MASTERWORKER_WORKER_FUNCTION_HPP
#define LIB_MASTERWORKER_WORKER_FUNCTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/parameter/aux_/preprocessor/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <ext/fusion/serialization/include.hpp>

#include <lib/masterworker/base.hpp>

// helper macros to allow a sequence of tuples without double brackets _________
// by articifially adding brackets so boost pp can handle it
// idea taken from
// http://www.drdobbs.com/article/printableArticle.jhtml?articleId=224700827

#define MASTER_WORKER_ADD_PARENTHESIS(r, n, elem, data)                        \
  (elem)                                                                       \
  /**/

#define MASTER_WORKER_PARENTHESIZE_ELEMENTS_AUX(r, _, seq)                     \
  BOOST_PARAMETER_FOR_EACH_R(                                                  \
    r, 2, seq, _, MASTER_WORKER_ADD_PARENTHESIS                                \
  )                                                                            \
  /**/

#define MASTER_WORKER_PE(seq)                                                  \
  BOOST_PP_SEQ_FOR_EACH(                                                       \
    MASTER_WORKER_PARENTHESIZE_ELEMENTS_AUX, _, (seq)                          \
  )                                                                            \
  /**/


// helper macro to generate a list of types (, T) from the parameters __________

#define MASTER_WORKER_TYPELIST(r, data, elem)                                  \
  , BOOST_PP_TUPLE_ELEM(2, 0, elem)                                            \
  /**/


// helper macro to generate a list of arguments (, T arg) from the parameters __

#define MASTER_WORKER_ARGLIST(r, data, elem)                                   \
  , BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem)            \
  /**/


// macro to generate worker function

#define WORKER_FUNCTION(functionname, parameters)                              \
namespace masterworker                                                         \
{                                                                              \
struct functionname                                                            \
{                                                                              \
  functionname (boost::mpi::communicator & comm_) : comm(comm_){}              \
  void operator()(                                                             \
    BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_HEAD(MASTER_WORKER_PE(parameters))) \
    BOOST_PP_SEQ_FOR_EACH(                                                     \
      MASTER_WORKER_TYPELIST, _,                                               \
      BOOST_PP_SEQ_TAIL(MASTER_WORKER_PE(parameters))                          \
    )                                                                          \
  );                                                                           \
  boost::mpi::communicator comm;                                               \
};                                                                             \
template <typename T>                                                          \
class BOOST_PP_CAT(functionname, _wrapper) : public masterworker::base         \
{                                                                              \
 public:                                                                       \
  BOOST_PP_CAT(functionname, _wrapper) () {}                                   \
  BOOST_PP_CAT(functionname, _wrapper) (T t_) : t(t_) {}                       \
  void execute(boost::mpi::communicator & comm)                                \
  {                                                                            \
    functionname f(comm);                                                      \
    invoke_procedure(f, t);                                                    \
  }                                                                            \
  T t;                                                                         \
                                                                               \
 private:                                                                      \
  friend class boost::serialization::access;                                   \
  template<class Archive>                                                      \
  void serialize(Archive & ar, const unsigned int version)                     \
  {                                                                            \
    ar & t;                                                                    \
    ar & boost::serialization::base_object<base>( *this );                     \
  }                                                                            \
};                                                                             \
}                                                                              \
                                                                               \
namespace worker                                                               \
{                                                                              \
  struct functionname                                                          \
  {                                                                            \
    typedef boost::fusion::vector<                                             \
    BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_HEAD(MASTER_WORKER_PE(parameters))) \
    BOOST_PP_SEQ_FOR_EACH(                                                     \
      MASTER_WORKER_TYPELIST, _,                                               \
      BOOST_PP_SEQ_TAIL(MASTER_WORKER_PE(parameters))                          \
    )                                                                          \
    > vector_type;                                                             \
                                                                               \
    typedef BOOST_PP_CAT(functionname, _wrapper) <boost::fusion::vector<       \
    BOOST_PP_TUPLE_ELEM(2, 0,                                                  \
                        BOOST_PP_SEQ_HEAD(MASTER_WORKER_PE(parameters))        \
                       )                                                       \
    BOOST_PP_SEQ_FOR_EACH(MASTER_WORKER_TYPELIST, _,                           \
                          BOOST_PP_SEQ_TAIL(MASTER_WORKER_PE(parameters))      \
                         )                                                     \
                                                                       >       \
                                                > wrapper_type;                \
  };                                                                           \
}                                                                              \
                                                                               \
BOOST_CLASS_EXPORT_IMPLEMENT(worker:: functionname ::wrapper_type);            \
BOOST_CLASS_EXPORT_KEY(worker:: functionname ::wrapper_type);                  \
                                                                               \
                                                                               \
void functionname ::operator()(                                                \
    BOOST_PP_TUPLE_ELEM(                                                       \
      2, 0, BOOST_PP_SEQ_HEAD(MASTER_WORKER_PE(parameters))                    \
    )                                                                          \
    BOOST_PP_TUPLE_ELEM(                                                       \
      2, 1, BOOST_PP_SEQ_HEAD(MASTER_WORKER_PE(parameters))                    \
    )                                                                          \
    BOOST_PP_SEQ_FOR_EACH(                                                     \
      MASTER_WORKER_ARGLIST, _, BOOST_PP_SEQ_TAIL(MASTER_WORKER_PE(parameters))\
    )                                                                          \
  )


#endif // LIB_MASTERWORKER_WORKER_FUNCTION_HPP

