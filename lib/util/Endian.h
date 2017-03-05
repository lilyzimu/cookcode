#ifndef _eNDIAN_H_
#define _eNDIAN_H_

#include "Endian.detail.h"

namespace endian {

 /////////////////////
 // Check Endianess //
 /////////////////////
 bool isLittleEndian(void) { return detail::_isLittleEndian;  }
 bool isBigEndian(void)    { return !detail::_isLittleEndian; }

 /////////////////////////////////////////////////////////
 // Convert to Little Endian <=> Machine Representation //
 /////////////////////////////////////////////////////////
 template<typename T>
   T little(const T& value) 
 { 
   if(isLittleEndian()) return value;
   else return detail::swapOrder(value);
 }

 ///////////////////////////////////////////////////
 // Convert Big Endian <=> Machine Representation //
 ///////////////////////////////////////////////////
 template<typename T>
   T big(const T& value) 
 { 
   if(isBigEndian()) return value;
   else return detail::swapOrder(value);
 }
 
} // end endian namespace

#endif /* _eNDIAN_H_ */
