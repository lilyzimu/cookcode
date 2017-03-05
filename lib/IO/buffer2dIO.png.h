///////////////////////////////
// PNG importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_PNG_H_
#define _BUFFER2DIO_PNG_H_

#include "exceptions.h"
#include "tempArray.h"

#ifdef INCLUDE_PNG

#include <png.h>
#include <zlib.h>

#endif /* INCLUDE_PNG */

using namespace std;

namespace io {
  namespace png {

#ifndef INCLUDE_PNG

    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, float pad=0.0f) { throw unsupportedFormat(); }

    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf, float pad=0.0f) { throw unsupportedFormat(); }

#else /* INCLUDE_PNG */

    ////////////////
    // Export PNG //
    ////////////////
    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, float pad=0.0f)
    {
      // open file
      FILE *fp = fopen(filename.c_str(), "wb");
      if(!fp) throw fileNotFound(filename, "writing");

      // allocate support memory structures
      png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
      if(!png_ptr) throw customException("Failed to allocated PNG-write structures");

      png_infop info_ptr = png_create_info_struct(png_ptr);
      if(!info_ptr)
      {
	png_destroy_write_struct(&png_ptr, NULL);
	throw customException("Failed to allocate PNG-info structures");
      }

      // setup write buffer
      unsigned long channels = ((C(*buf.begin())).size() == 1) ? 1 : 3;

      tempArray(png_bytep, row_ptr, buf.height());
      buffer2d<unsigned char> temp(buf.width()*channels, buf.height());
      
      for(size_t i=0; i < buf.height(); i++)
	row_ptr[i] = &temp(0,i);

      typename buffer2d<unsigned char>::iterator tempItr = temp.begin();
      for(typename Buffer::const_iterator itr = buf.begin(); itr != buf.end(); itr++)
      {
	// convert
	const C wrapper(*itr);
	typename C::const_iterator typeItr = wrapper.begin();
	unsigned int count = 0;
	while(count < channels && typeItr != wrapper.end())
	{
	  *tempItr = *typeItr * 255.0f;
	  count++;
	  typeItr++;
	  tempItr++;
	}

	// pad
	while(count < channels)
	{
	  *tempItr = pad;
	  count++;
	  tempItr++;
	}
      }

      // setup IO
      png_init_io(png_ptr, fp);

      // setup header
      png_set_IHDR(png_ptr, info_ptr, 
		   buf.width(),           // width
		   buf.height(),          // height
		   8,                     // bitdepth
		   (channels == 3) ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_GRAY,
		   PNG_INTERLACE_NONE,    // interlacing
		   PNG_COMPRESSION_TYPE_DEFAULT,
		   PNG_FILTER_TYPE_DEFAULT);

      // write image data
      png_write_info(png_ptr, info_ptr);
      png_write_image(png_ptr, row_ptr);
      png_write_end(png_ptr, info_ptr);

      // clean up
      png_destroy_write_struct(&png_ptr, &info_ptr);

      // Done
      fclose(fp);
    }

    ////////////////
    // Import PNG //
    ////////////////
    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& result, float pad=0.0f)
    {
      // read temp buffer
      FILE *fp = fopen(filename.c_str(), "rb");
      if(!fp) throw fileNotFound(filename, "reading");

      // read header & validate if a PNG
      png_byte header[8];
      fread(header, 1, 8, fp);
      if(png_sig_cmp(header, 0, 8)) throw unsupportedFormat();

      // allocate support memory structures
      png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
      if(!png_ptr) throw customException("Failed to allocate PNG-read structures");

      png_infop info_ptr = png_create_info_struct(png_ptr);
      if(!info_ptr)
      {
	png_destroy_read_struct(&png_ptr, NULL, NULL);
	throw customException("Failed to allocate PNG-info structures");
      }

      // setup IO
      png_init_io(png_ptr, fp);
      png_set_sig_bytes(png_ptr, 8);

      png_read_info(png_ptr, info_ptr);
      png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
      png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
      png_uint_32 channels = png_get_channels(png_ptr, info_ptr);
      png_uint_32 bitdepth = png_get_bit_depth(png_ptr, info_ptr);

      if(bitdepth < 8) png_set_expand(png_ptr);

      // read buffer
      tempArray(png_bytep, row_ptr, height);
      buffer2d<unsigned char> temp(width*channels, height);
      
      for(size_t i=0; i < height; i++)
	row_ptr[i] = &temp(0,i);
      
      png_read_image(png_ptr, row_ptr);

      // clean up
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      fclose(fp);

      // convert into a floating point buffer
      result.resize(width, height);

      typename buffer2d<unsigned char>::const_iterator tempItr = temp.begin();
      for(typename Buffer::iterator itr = result.begin(); itr != result.end(); itr++)
      {
	// convert
	C wrapper(*itr);
	typename C::iterator typeItr = wrapper.begin();
	unsigned int count = 0;
	while(count < channels && typeItr != wrapper.end())
	{
	  *typeItr = *tempItr / 255.0f;
	  count++;
	  typeItr++;
	  tempItr++;
	}

	// skip unread channels
	if(count < channels) tempItr += channels - count;

	// pad
	while(typeItr != wrapper.end())
	{
	  *typeItr = pad;
	  typeItr++;
	}
      }

      // Done.
    }

#endif /* INCLUDE_PNG */

  } // end png namespace
} // end png namespace

#endif /* _BUFFER2DIO_PNG_H_ */
