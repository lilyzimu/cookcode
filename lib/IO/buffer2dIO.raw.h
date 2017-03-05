//////////////////////////////////////////////////////
// RAW/NEF Importer. Requires DCRAW to be installed //
//////////////////////////////////////////////////////

#ifndef _BUFFER2DIO_RAW_H_
#define _BUFFER2DIO_RAW_H_

#include "exceptions.h"
#include <cstdio>

#include <iostream>

using namespace std;

namespace io {
  namespace raw {

    namespace detail {
      struct uint16 {
	unsigned char high, low;
	operator unsigned int() const { return (unsigned int)(high) << 8 | (unsigned int)(low); }
      };
    }

    #ifdef _WIN32
      #define RAW_EXEC_STRING "dcraw.exe -c -4"
      #define POPEN _popen
      #define PCLOSE _pclose
    #else
      #define RAW_EXEC_STRING "dcraw -c -4"
      #define POPEN popen
      #define PCLOSE pclose
    #endif

    ////////////////
    // Import RAW //
    ////////////////
    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& result, float pad=0.0f)
      {
	// setup dcraw execution environment
	std::string cmd = std::string(RAW_EXEC_STRING) + " " + filename;

	// exec dcraw: pipe RAW as a 16 bit PPM
	FILE *epo = POPEN(cmd.c_str(), "r");

	// failed: did not find dcraw
	if(!epo) throw fileNotFound(cmd, "execution");

	// read temp buffer
	char dummy;
	int width, height, colors;

	// scan header
	fscanf(epo, "P6\n");
	int res = fscanf(epo, "%d%c%d%c%d%c", &width, &dummy, &height, &dummy, &colors, &dummy);

	// Sanity check
	if (res != 6) throw unsupportedFormat(); 
	if (colors != 65535) throw unsupportedFormat();

	// read buffer
	buffer2d<detail::uint16> temp(width*3, height);
	fread(temp.begin(), sizeof(detail::uint16),  temp.size(), epo);
	PCLOSE(epo);

	// allocate
	result.resize(width, height);

	// convert from 16bit PPM to 
	typename buffer2d<detail::uint16>::const_iterator tempItr = temp.begin();
	for(typename Buffer::iterator itr = result.begin(); itr != result.end(); itr++)
	{
	  // convert
	  C wrapper(*itr);
	  typename C::iterator typeItr = wrapper.begin();
	  unsigned int count = 0;
	  while(count < 3 && typeItr != wrapper.end())
	  {
	    *typeItr = (unsigned int)(*tempItr) / 65535.0f;
	    count++;
	    typeItr++;
	    tempItr++;
	  }

	  // pad
	  while(typeItr != wrapper.end())
	  {
	    *typeItr = pad;
	    typeItr++;
	  }
	}

      // done.
    }



  } // namespace raw
}   // namespace io

#endif /* _BUFFER2D_RAW_H_ */

