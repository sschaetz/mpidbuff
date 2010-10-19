#ifndef LIB_MASTERWORKER_TYPE_TRANSLATION_HPP
#define LIB_MASTERWORKER_TYPE_TRANSLATION_HPP


namespace masterworker
{
  // generic type translation __________________________________________________
  template <typename T>
  struct translate_type
  {
    typedef T type;
  };

  template <typename T>
  struct translate_type<T const>
  {
    typedef T type;
  };

  template <typename T>
  struct translate_type<T&>
  {
    typedef T type;
  };

  template <typename T>
  struct translate_type<T const&>
  {
    typedef T type;
  };

  // argument type translation _________________________________________________
  template <typename T>
  struct translate_argument_type
  {
    typedef typename translate_type<T>::type type;
  };

  // parameter type translation ________________________________________________
  template <typename T>
  struct translate_return_type
  {
    typedef typename translate_type<T>::type type;
  };

  template<>
  struct translate_return_type<void>
  {
    typedef int type;
  };
}

#endif // LIB_MASTERWORKER_TYPE_TRANSLATION_HPP

