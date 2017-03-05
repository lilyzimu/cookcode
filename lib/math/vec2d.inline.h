///////////////////////////////
// Inline methods of vec2d.h //
///////////////////////////////

#include "zeroLengthVector.h"

using namespace std;


///////////////////////////////
// Constructor               //
///////////////////////////////
template<typename T>
inline vec2d<T>::vec2d(const T& value)
{
  this->x = value;
  this->y = value;
}


///////////////////////////////
// Constructor               //
///////////////////////////////
template<typename T>
inline vec2d<T>::vec2d(const T& x, const T& y)
{
  this->x = x;
  this->y = y;
}


///////////////////////////////
// Constructor               //
///////////////////////////////
template<typename T>
 template<typename Itr>
inline vec2d<T>::vec2d(const Itr& begin, const Itr& end, const T& pad, typename boost::disable_if<boost::is_fundamental<Itr> >::type* dummy)
{
  Itr itr = begin;
  this->x = (itr != end) ? *itr++ : pad;
  this->y = (itr != end) ? *itr : pad;
}

///////////////////////////////
// Copy Constructor          //
///////////////////////////////
template<typename T>
inline vec2d<T>::vec2d(const vec2d<T>& v)
{
  x = v.x;
  y = v.y;
}


//////////////////////////////
// Operator+=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator+=(const vec2d<T>& v)
{
  x += v.x;
  y += v.y;
  return *this;
}


//////////////////////////////
// Operator+=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator+=(const T& s)
{
  x += s;
  y += s;
  return *this;
}


//////////////////////////////
// Operator-=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator-=(const vec2d<T>& v)
{
  x -= v.x;
  y -= v.y;
  return *this;
}


//////////////////////////////
// Operator-=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator-=(const T& s)
{
  x -= s;
  y -= s;
  return *this;
}


//////////////////////////////
// Operator*=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator*=(const T& s)
{
  x *= s;
  y *= s;
  return *this;
}


//////////////////////////////
// Operator*=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator*=(const vec2d<T>& v)
{
  x *= v.x;
  y *= v.y;
  return *this;
}


//////////////////////////////
// Operator/=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator/=(const T& s)
{
  x /= s;
  y /= s;
  return *this;
}


//////////////////////////////
// Operator/=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator/=(const vec2d<T>& v)
{
  x /= v.x;
  y /= v.y;
  return *this;
}


//////////////////////////////
// Operator^=               //
//////////////////////////////
template<typename T>
inline vec2d<T>& vec2d<T>::operator^=(const T& s)
{
  x = pow(x, s);
  y = pow(y, s);
  return *this;
}


//////////////////////////////
// normalize                //
//////////////////////////////
template<typename T>
inline vec2d<T> vec2d<T>::normalize(void) const
{ 
  T l = length();
  if(l == (T)(0)) throw zeroLengthVector();
  else return *this / l; 
}

//////////////////////////////
// _swap                    //
//////////////////////////////
template<typename T>
inline void vec2d<T>::_swap(vec2d<T>& v)
{
  swap(v.x, x);
  swap(v.y, y);
}


//////////////////////////////
// _assign                  //
//////////////////////////////
template<typename T>
inline void vec2d<T>::_assign(const vec2d<T>& v)
{
  x = v.x;
  y = v.y;
}
