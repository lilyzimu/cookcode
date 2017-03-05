///////////////////////////////
// PPM importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_PPM_H_
#define _BUFFER2DIO_PPM_H_

#include "exceptions.h"
#include <cstdio>

using namespace std;

namespace io {
  namespace ppm {
    
    namespace detail {
      
      /////////////////////////////////
      // Skip Comments in the Header //
      /////////////////////////////////
      void skipComment(FILE *fp, char token='#')
      {
	int b = fgetc(fp);

	while(b != EOF && (b == token))
	{
	  while (b != EOF && b != '\n' && b != '\r')
	    b = fgetc(fp);
	  
	  b = fgetc(fp);
	}
	ungetc(b, fp);
      }

    } // namespace detail


  ////////////////
  // Export PPM //
  ////////////////
  template<typename Buffer, typename C>
    void _export(const string& filename, const Buffer& buf, float pad=0.0f)
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // scale pad
      pad *= 255.0f;

      // create temp buffer
      buffer2d<unsigned char> temp(buf.width() * 3, buf.height());
      typename buffer2d<unsigned char>::iterator tempItr = temp.begin();
      for(typename Buffer::const_iterator itr = buf.begin(); itr != buf.end(); itr++)
      {
	// convert
	const C wrapper(*itr);
	typename C::const_iterator typeItr = wrapper.begin();
	unsigned int count = 0;
	while(count < 3 && typeItr != wrapper.end())
	{
	  *tempItr = *typeItr * 255.0f;
	  count++;
	  typeItr++;
	  tempItr++;
	}

	// pad
	while(count < 3)
	{
	  *tempItr = pad;
	  count++;
	  tempItr++;
	}
      }

      // write temp buffer to file
      FILE *fp = fopen(filename.c_str(), "wb");
      char dummy='\n';
      fprintf(fp,"P6%c%lu %lu%c255%c", dummy, (unsigned long)(buf.width()), (unsigned long)(buf.height()), dummy, dummy);
      fwrite(temp.begin(), sizeof(unsigned char), temp.size(), fp);
      fclose(fp);      

      // done.
    }


  ////////////////
  // Import PPM //
  ////////////////
  template<typename Buffer, typename C>
    void _import(const string& filename, Buffer& result, float pad=0.0f)
    {
      // read temp buffer
      char dummy;
      int width, height, colors;
      FILE *fp = fopen(filename.c_str(), "rb");

      if(!fp) throw fileNotFound(filename);

      // scan header
      fscanf(fp, "P6\n");
      detail::skipComment(fp);
      int res = 0;
      res += fscanf(fp, "%d%c%d%c", &width, &dummy, &height, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%d%c", &colors, &dummy);

      // Check if PPM file format
      if (res != 6) throw unsupportedFormat(); 
      if (colors != 255) throw unsupportedFormat();

      // read buffer
      buffer2d<unsigned char> temp(width*3, height);
      fread(temp.begin(), sizeof(unsigned char),  temp.size(), fp);
      fclose(fp);

      // allocate
      result.resize(width, height);

      typename buffer2d<unsigned char>::const_iterator tempItr = temp.begin();
      for(typename Buffer::iterator itr = result.begin(); itr != result.end(); itr++)
      {
	// convert
	C wrapper(*itr);
	typename C::iterator typeItr = wrapper.begin();
	unsigned int count = 0;
	while(count < 3 && typeItr != wrapper.end())
	{
	  *typeItr = *tempItr / 255.0f;
	  count++;
	  typeItr++;
	  tempItr++;
	}

	// pad
	tempItr += (3 - count);
	while(typeItr != wrapper.end())
	{
	  *typeItr = pad;
	  typeItr++;
	}
      }

      // done.
    }

  } // namespace ppm
}   // namespace io

#endif /* _BUFFER2DIO_PPM_H_ */

