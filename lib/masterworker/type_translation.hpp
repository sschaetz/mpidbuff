#ifndef LIB_MASTERWORKER_TYPE_TRANSLATION_HPP
#define LIB_MASTERWORKER_TYPE_TRANSLATION_HPP


namespace masterworker
{
  /**
   * The worker_function macro in some cases requires the argumenttypes without
   * const or ref qualifier. These structures help to remove those qualifiers.
   */

  /**
   * Generic type translators
   */
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

  /**
   * Type translators for function arguments
   */
  template <typename T>
  struct translate_argument_type
  {
    typedef typename translate_type<T>::type type;
  };

  /**
   * Type translators for return types
   */
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

