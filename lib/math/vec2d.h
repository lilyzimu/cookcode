#ifndef _VEC2D_H_
#define _VEC2D_H_

#include <cmath>
#include <ostream>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

using namespace std;

template<typename T>
class vec2d {
public:
  ///////////////
  // type defs //
  ///////////////
  typedef T             value_type;
  typedef T*            iterator;
  typedef const T*      const_iterator;
  typedef T&            reference;
  typedef const T&      const_reference;
  typedef std::size_t   size_type;

  /////////////////////////
  // Public Data Members //
  /////////////////////////
  union {
    struct { T u, v; };
    struct { T x, y; };
  };

  //////////////////
  // Constructors //
  //////////////////
  vec2d(const T& value=(T)(0));
  vec2d(const T& x, const T& y);
  vec2d(const vec2d<T>& v);

  template<typename Itr>
    vec2d(const Itr& begin, const Itr& end, const T& pad=(T)(0), typename boost::disable_if<boost::is_fundamental<Itr> >::type* dummy=0);

  ////////////////
  // Destructor //
  ////////////////
  ~vec2d(void) {}

  ///////////////////
  // Cast Operator //
  ///////////////////
  template<typename S>
    operator vec2d<S>() const { return vec2d<S>( (S)(x), (S)(y) ); }

  /////////////
  // Methods //
  /////////////
  const_iterator    begin(void) const     { return (iterator)(this); }
  iterator          begin(void)           { return (iterator)(this); }
  const_iterator    end(void) const       { return begin() + size(); }
  iterator          end(void)             { return begin() + size(); }

  size_type         size(void) const      { return 2; }

  const T& operator[](int index) const    { return begin()[index]; }
  T&       operator[](int index)          { return begin()[index]; }

  vec2d<T>& operator=(const vec2d<T>& v)  { _assign(v); return *this; }

  bool     operator==(const vec2d<T>& v) const { return (v.x == x) && (v.y == y); }
  bool     operator!=(const vec2d<T>& v) const { return (v.x != x) || (v.y != y); }

  bool     operator<(const T& val) const       { return squared_length() <  val*val; }
  bool     operator<=(const T& val) const      { return squared_length() <= val*val; }
  bool     operator>(const T& val) const       { return squared_length() >  val*val; }
  bool     operator>=(const T& val) const      { return squared_length() >= val*val; }

  vec2d<T> operator+(const T& s) const         { return vec2d<T>(x + s, y + s); }
  vec2d<T> operator+(const vec2d<T>& v) const  { return vec2d<T>(x + v.x, y + v.y); }
  vec2d<T> operator-(void) const               { return vec2d<T>(-x, -y); }
  vec2d<T> operator-(const T& s) const         { return vec2d<T>(x - s, y - s); }
  vec2d<T> operator-(const vec2d<T>& v) const  { return vec2d<T>(x - v.x, y - v.y); }
  vec2d<T> operator*(const T& s) const         { return vec2d<T>(x*s, y*s); }
  vec2d<T> operator*(const vec2d<T>& v) const  { return vec2d<T>(x*v.x, y*v.y); }
  vec2d<T> operator/(const T& s) const         { return vec2d<T>(x/s, y/s); }
  vec2d<T> operator/(const vec2d<T>& v) const  { return vec2d<T>(x/v.x, y/v.y); }
  vec2d<T> operator^(const T& s) const         { return vec2d<T>(pow(x,s), pow(y, s)); }

  vec2d<T>& operator+=(const T& s);
  vec2d<T>& operator+=(const vec2d<T>& v);
  vec2d<T>& operator-=(const T& s);
  vec2d<T>& operator-=(const vec2d<T>& v);
  vec2d<T>& operator*=(const T& s);
  vec2d<T>& operator*=(const vec2d<T>& v);
  vec2d<T>& operator/=(const T& s);
  vec2d<T>& operator/=(const vec2d<T>& v);
  vec2d<T>& operator^=(const T& s);

  T dot(const vec2d<T>& v) const               { return x*v.x + y*v.y; }
  T squared_length(void) const                 { return dot(*this); }
  T length(void) const                         { return sqrt(squared_length()); }
  T squared_distance(const vec2d<T>& v) const  { return (*this - v).squared_length(); }
  T distance(const vec2d<T>& v) const          { return sqrt(squared_distance(v)); }

  T kross(const vec2d<T>& v) const             { return x*v.y - y*v.x; }

  vec2d<T> normalize(void) const; 

  vec2d<T>  min(const vec2d<T> &v) { return vec2d<T>(std::min(x, v.x), std::min(y, v.y)); }
  vec2d<T>  max(const vec2d<T> &v) { return vec2d<T>(std::max(x, v.x), std::max(y, v.y)); }

  /////////////
  // Friends //
  /////////////
  friend void swap(vec2d<T>& a, vec2d<T>& b)                { a._swap(b) ; }
  friend vec2d<T> pow(const vec2d<T>& v, const T& s)        { return (v ^ s); }
  friend vec2d<T> abs(const vec2d<T>& v)                    { return vec2d<T>( abs(v.x), abs(v.y) ); }
  friend vec2d<T> max(const vec2d<T>& a, const vec2d<T>& b) { return vec2d<T>(std::max(a.x,b.x), std::max(a.y,b.y)); }
  friend vec2d<T> min(const vec2d<T>& a, const vec2d<T>& b) { return vec2d<T>(std::min(a.x,b.x), std::min(a.y,b.y)); }
  friend void normalize(vec2d<T>& v)                        { v = v.normalize(); }

  friend T squared_distance(const vec2d<T>& a, const vec2d<T>& b) { return a.squared_distance(b); }
  friend T Distance(const vec2d<T>& a, const vec2d<T>& b)         { return a.distance(b); }

  friend vec2d<T> operator*(const T& s, const vec2d<T>& v)        { return (v*s); }

  friend ostream& operator<<(ostream& s, const vec2d<T>& v)
  {
    s << "[" << v.x << ", " << v.y << "]";
    return s;
  }

 protected:
  ///////////////////////
  // Protected Methods //
  ///////////////////////
  void _swap(vec2d<T>& v);
  void _assign(const vec2d<T>& src);
};


////////////////////
// Inline Methods //
////////////////////
#include "vec2d.inline.h"

#endif /* _VEC2D_H_ */
