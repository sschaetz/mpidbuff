//////////////////////////////////////////////////////////////////////////////
// detail::fusion::serialization::vector.hpp                             	//
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_SERIALIZATION_VECTOR_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_SERIALIZATION_VECTOR_HPP_ER_2009
#include <boost/fusion/include/make_vector.hpp>
#include <ext/fusion/serialization/save.hpp>
#include <ext/fusion/serialization/load.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>


namespace boost{ namespace serialization{


	// A generic function restricted by enable_if<is_sequence<S>,void>
    // causes ambiguous compile error. So, apparently, each type of sequence and
    // each length within requires an overoad.



  #define GENERATE_FUSION_VECTOR_SERIALIZATION_CODE(z, n, data)                \
    template<class Archive, BOOST_PP_ENUM_PARAMS(n, typename T) >              \
    void serialize(                                                            \
        Archive & ar,                                                          \
        boost::fusion::vector< BOOST_PP_ENUM_PARAMS(n, T) >& t,                \
        const unsigned int file_version                                        \
    )                                                                          \
    {                                                                          \
        split_free(ar, t, file_version);                                       \
    }                                                                          \
    template<class Archive, BOOST_PP_ENUM_PARAMS(n, typename T) >              \
    void serialize(                                                            \
        Archive & ar,                                                          \
        boost::fusion::                                                        \
        BOOST_PP_CAT(vector, n) < BOOST_PP_ENUM_PARAMS(n, T) >& t,             \
        const unsigned int file_version                                        \
    )                                                                          \
    {                                                                          \
        split_free(ar, t, file_version);                                       \
    }                                                                          \
    /**/

  BOOST_PP_REPEAT_FROM_TO(1, 10, GENERATE_FUSION_VECTOR_SERIALIZATION_CODE, T)

  #undef GENERATE_FUSION_VECTOR_SERIALIZATION_CODE


	// n = 1

//    template<class Archive,typename T1>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector<T1>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//    template<class Archive,typename T1>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector1<T1>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//	// n = 2
//
//    template<class Archive,typename T1,typename T2>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector<T1,T2>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//    template<class Archive,typename T1,typename T2>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector2<T1,T2>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//	// n = 3
//
//    template<class Archive,typename T1,typename T2,typename T3>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector<T1,T2,T3>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//    template<class Archive,typename T1,typename T2,typename T3>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector3<T1,T2,T3>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//	// n = 4
//
//    template<class Archive,typename T1,typename T2,typename T3, typename T4>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector<T1,T2,T3,T4>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }
//
//    template<class Archive,typename T1,typename T2,typename T3,typename T4>
//    void serialize(
//        Archive & ar,
//        boost::fusion::vector4<T1,T2,T3,T4>& t,
//        const unsigned int file_version
//    )
//    {
//        split_free(ar, t, file_version);
//    }

}}

#endif
