#ifndef _ITERATOR_WRAPPER_H_
#define _ITERATOR_WRAPPER_H_

#include <boost/utility.hpp>
#include <boost/type_traits.hpp>

//////////////////////////////////////////////////
// The IteratorWrapper provides a uniform way   //
// of creating an iterator of objects that have //
// iterator support and of fundamental types.   //
// This can be usefull for template classes and //
// functions that should work on both objects   //
// as well as fundamental types.                //
//////////////////////////////////////////////////

using namespace boost;

//////////////////
// Default case //
//////////////////
template<typename T, class Enabled=void>
class iteratorWrapper {
  public:
  //////////////
  // Typedefs //
  //////////////
  typedef T                                      value_type;
  typedef typename value_type::iterator          iterator;
  typedef typename value_type::const_iterator    const_iterator;
  typedef typename value_type::reference         reference;
  typedef typename value_type::const_reference   const_reference;

  /////////////////
  // Constructor //
  /////////////////
  iteratorWrapper(T& object) : _object(object) {};
  iteratorWrapper(const iteratorWrapper& src) : _object(src._object) {}

  /////////////
  // Methods //
  /////////////
  iterator       begin(void)       { return _object.begin(); }
  const_iterator begin(void) const { return _object.begin(); }

  iterator       end(void)         { return _object.end(); }
  const_iterator end(void) const   { return _object.end(); }

  std::size_t size(void) const     { return std::distance(begin(), end()); }

  private:
  //////////////////////////
  // Private Data Members //
  //////////////////////////
  T& _object;
};


////////////////////////
// Default Const case //
////////////////////////
template<typename T>
class iteratorWrapper<const T, typename disable_if< is_fundamental<T> >::type> {
  public:
  //////////////
  // Typedefs //
  //////////////
  typedef const T                                value_type;
  typedef typename value_type::const_iterator    iterator;
  typedef typename value_type::const_iterator    const_iterator;
  typedef typename value_type::const_reference   reference;
  typedef typename value_type::const_reference   const_reference;

  /////////////////
  // Constructor //
  /////////////////
  iteratorWrapper(const T& object) : _object(object) {};
  iteratorWrapper(const iteratorWrapper& src) : _object(src._object) {}

  /////////////
  // Methods //
  /////////////
  const_iterator begin(void) const { return _object.begin(); }
  const_iterator end(void) const   { return _object.end(); }

  std::size_t size(void) const     { return std::distance(begin(), end()); }

  private:
  //////////////////////////
  // Private Data Members //
  //////////////////////////
  const T& _object;
};


///////////////////////////
// Fundamental type case //
///////////////////////////
template<typename T>
struct iteratorWrapper<T, typename enable_if< is_fundamental<T> >::type > {
  public:
  //////////////
  // Typedefs //
  //////////////
  typedef T                                        value_type;
  typedef value_type*                              iterator;
  typedef typename add_const<value_type>::type*    const_iterator;
  typedef value_type&                              reference;
  typedef typename add_const<value_type>::type&    const_reference;

  /////////////////
  // Constructor //
  /////////////////
  iteratorWrapper(T& object) : _object(object) {}
  iteratorWrapper(const iteratorWrapper& src) : _object(src._object) {}

  /////////////
  // Methods //
  /////////////
  iterator       begin(void)       { return &_object; }
  const_iterator begin(void) const { return &_object; }

  iterator end(void)               { return begin() + 1; }
  const_iterator end(void) const   { return begin() + 1; }

  std::size_t size(void) const     { return 1; }

  private:
  //////////////////////////
  // Private Data Members //
  //////////////////////////
  T& _object;
};

#endif /* _ITERATOR_WRAPPER_H_ */
