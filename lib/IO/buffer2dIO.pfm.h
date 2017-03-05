///////////////////////////////
// PFM importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_PFM_H_
#define _BUFFER2DIO_PFM_H_

#include "Endian.h"
#include "exceptions.h"
#include <cstdio>

using namespace std;

namespace io {
  namespace pfm {

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
  // Export PFM //
  ////////////////
  template<typename Buffer, typename C>
    void _export(const string& filename, const Buffer& buf, float pad=0.0f)
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // init
      float littlePad = endian::little(pad);

      // create temp buffer
      buffer2d<float> temp(buf.width() * 3, buf.height());
      typename buffer2d<float>::iterator tempItr = temp.begin();
      for(typename Buffer::const_iterator itr = buf.begin(); itr != buf.end(); itr++)
      {
	// convert
	const C wrapper(*itr);
	typename C::const_iterator typeItr = wrapper.begin();
	unsigned int count = 0;
	while(count < 3 && typeItr != wrapper.end())
	{
	  *tempItr = endian::little(*typeItr);
	  count++;
	  typeItr++;
	  tempItr++;
	}

	// pad
	while(count < 3)
	{
	  *tempItr = littlePad;
	  count++;
	  tempItr++;
	}
      }

      // write temp buffer to file
      FILE *fp = fopen(filename.c_str(), "wb");
      char dummy='\n';
      fprintf(fp,"PF%c%lu %lu%c-1.000000%c", dummy, (unsigned long)(buf.width()), (unsigned long)(buf.height()), dummy, dummy);
      fwrite(temp.begin(), sizeof(float), temp.size(), fp);
      fclose(fp);      

      // done.
    }


  ////////////////
  // Import PFM //
  ////////////////
  template<typename Buffer, typename C>
    void _import(const string& filename, Buffer& result, float pad=0.0f)
    {
      // read temp buffer
      char dummy, type;         // type is 'f' or 'F'
      float endianess;          // endianess is -1 (LE) or 1 (BE)
      int width, height;
      FILE *fp = fopen(filename.c_str(), "rb");

      if(!fp) throw fileNotFound(filename);

      // scan header
      int res = 0;
      res += fscanf(fp, "P%c%c", &type, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%d%c%d%c", &width, &dummy, &height, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%g%c", &endianess, &dummy);

      // Check if PFM file format
      if (res != 8) throw unsupportedFormat(); 
      if (type != 'f' && type != 'F') throw unsupportedFormat();

      // Determine number of channels
      unsigned int channels = (type == 'f') ? 1 : 3;
      bool bigEndian = (endianess > 0);

      // read buffer
      buffer2d<float> temp(width*channels, height);
      fread(temp.begin(), sizeof(float),  temp.size(), fp);
      fclose(fp);

      // allocate
      result.resize(width, height);

      typename buffer2d<float>::const_iterator tempItr = temp.begin();
      for(typename Buffer::iterator itr = result.begin(); itr != result.end(); itr++)
      {
	// convert
	C wrapper(*itr);
	typename C::iterator typeItr = wrapper.begin();
	unsigned int count = 0;
	while(count < channels && typeItr != wrapper.end())
	{
	  *typeItr = (bigEndian) ? endian::big(*tempItr) : endian::little(*tempItr);
	  count++;
	  typeItr++;
	  tempItr++;
	}

	// skip
	tempItr += channels - count;

	// pad
	while(typeItr != wrapper.end())
	{
	  *typeItr = pad;
	  typeItr++;
	}
      }

      // done.
    }

  } // namespace pfm
}   // namespace io

#endif /* _BUFFER2DIO_PFM_H_ */
