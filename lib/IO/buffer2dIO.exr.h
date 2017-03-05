///////////////////////////////
// EXR importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_EXR_H_
#define _BUFFER2DIO_EXR_H_

#include "exceptions.h"

#ifdef INCLUDE_OPENEXR

#include "offset_iterator.h"
#include "buffer2dIO.exr.detail.h"

#include <vector>
#include <string>

#include "ImfHeader.h"
#include "ImfPixelType.h"
#include "half.h"
#include "ImfCompression.h"
#include "ImfInputFile.h"
#include "ImfOutputFile.h"
#include "ImfFrameBuffer.h"
#include "ImfChannelList.h"

#endif /* INCLUDE_OPENEXR */

using namespace std;

namespace io {
  namespace exr {

#ifndef INCLUDE_OPENEXR

    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf) { throw unsupportedFormat(); }

    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf) { throw unsupportedFormat(); }

#else /* INCLUDE_OPENEXR */

    /////////////
    // options //
    /////////////
    struct options_struct {

      public:
      options_struct(void) : _ignoreNaming(false),
			     _forceChannels(false),
			     _numberOfChannels(3),
			     _compressionType(Imf::ZIP_COMPRESSION),
			     _padding(0.0f)
      {
	_channelNames.push_back("R");
	_channelNames.push_back("G");
	_channelNames.push_back("B");
      }

      bool forceChannels(void) const { return _forceChannels; }
      bool ignoreNaming(void) const { return _ignoreNaming; }
      unsigned int numberOfChannels(void) const { return _numberOfChannels; }
      Imf::Compression compressionType(void) const { return _compressionType; }
      float padding(void) const { return _padding; }
      std::string channelNames(unsigned int index) const
      {
	if(!_ignoreNaming && index < _channelNames.size()) return _channelNames[index].c_str();
	else
	{
	  std::string name("C");
	  name += index;
	  return name;
	}
      }

      private:
      /////////////////////
      // Private Members //
      /////////////////////
      float _padding;
      bool _forceChannels;
      bool _ignoreNaming;
      unsigned int _numberOfChannels;
      Imf::Compression _compressionType;
      std::vector<std::string> _channelNames;
    };


    options_struct options;
    
    void defaultOptions(void) { options = options_struct(); }


    ////////////////
    // Export EXR //
    ////////////////
    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf)
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // determine number of channels
      const C wrapper(*(buf.begin()));
      unsigned int storedChannels = std::distance( wrapper.begin(), wrapper.end() );
      unsigned int numChannels = (options.forceChannels()) ? options.numberOfChannels() : storedChannels;

      // check if we need to allocate an padding buffer
      buffer2d<half> padd;
      if(storedChannels < numChannels)
      {
	buffer2d<half> temp(buf.width(), buf.height());
	temp.clear( options.padding() );
	swap(temp, padd);
      }

      // setup EXR file
      Imf::Header header(buf.width(), buf.height(), 1, Imath::V2f(0,0), 1, Imf::INCREASING_Y, options.compressionType());
      Imf::FrameBuffer frameBuffer;

      // create frameBuffers
      std::vector<detail::outputFrameBuffer> tempBuffer(numChannels);
      for(unsigned int c=0; c < numChannels; c++)
      {
	// non padding channel
	if(c < storedChannels)
	{
	  const_offset_iterator<typename Buffer::const_iterator> itr(buf.begin(), c);
	  detail::outputFrameBuffer fb(buf.width() * buf.height(), itr);

	  // add to frameBuffer list
	  swap(tempBuffer[c], fb);
	}

	// padding channel
	else
	{
	  detail::outputFrameBuffer fb(buf.width() * buf.height(), padd.begin());
	  swap(tempBuffer[c], fb);
	}

	// setup header
	std::string name = options.channelNames(c).c_str();
	header.channels().insert( name.c_str(), tempBuffer[c].pixelType() );

	// insert in EXR frame buffer
	frameBuffer.insert( name.c_str(),
			    Imf::Slice(tempBuffer[c].pixelType(),
				       (char *)((void *)(tempBuffer[c].buffer())),      // Double-cast-void work-around for stupid EXR non-const pointer requirement.  
				       tempBuffer[c].stride(),
				       buf.width() * tempBuffer[c].stride(),
				       1, 1,
				       options.padding()) );
      }

      // write file
      Imf::OutputFile file(filename.c_str(), header);
      file.setFrameBuffer(frameBuffer);
      file.writePixels(buf.height());

      // done.
    }


    ////////////////
    // Import EXR //
    ////////////////
    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf)
    {
      // check number of requested channels
      const C wrapper(*(buf.begin()));
      unsigned int availableChannels = std::distance( wrapper.begin(), wrapper.end() );
      unsigned int numChannels = availableChannels;
      if(options.forceChannels())
      {
	if(options.numberOfChannels() > availableChannels) throw customException("import EXR: more channels requested than available in target buffer.");
	numChannels = options.numberOfChannels();
      }

      // open exr and get relevant data
      Imf::InputFile file(filename.c_str());

      const Imath::Box2i& dataWindow = file.header().dataWindow();
      const Imf::ChannelList& chanList = file.header().channels();

      unsigned long width = dataWindow.max.x - dataWindow.min.x + 1;
      unsigned long height = dataWindow.max.y - dataWindow.min.y + 1;
      unsigned int storedChannels = 0;
      for(Imf::ChannelList::ConstIterator i = chanList.begin(); i != chanList.end(); i++) storedChannels++;

      // allocate memory
      buf.resize(width, height);
      if(numChannels < storedChannels) buf.clear();

      // setup frame buffers
      Imf::FrameBuffer frameBuffer;
      std::vector<detail::inputFrameBuffer> tempBuffer(numChannels);
      
      std::string chanName;
      Imf::ChannelList::ConstIterator chanItr = chanList.begin();
      for(unsigned int c=0; c < numChannels; c++)
      {
	// get name of channel
	if(options.ignoreNaming())
	{
	  chanName = chanItr.name();
	  chanItr++;
	}
	else chanName = options.channelNames(c);

	// allocate memory for channel's frame buffer (if needed)
	offset_iterator<typename Buffer::iterator> itr(buf.begin(), c);
	detail::inputFrameBuffer fb(buf.width() * buf.height(), itr);
	swap(tempBuffer[c], fb);
	
	// setup frame buffer
	frameBuffer.insert(chanName.c_str(),
			   Imf::Slice( tempBuffer[c].pixelType(),
				       (char *)(tempBuffer[c].buffer()),
				       tempBuffer[c].stride(),
				       buf.width() * tempBuffer[c].stride(),
				       1, 1, 
				       0.0));
      }

      // read file
      file.setFrameBuffer(frameBuffer);
      file.readPixels(dataWindow.min.y, dataWindow.max.y);

      // convert data if required to native data type of buf
      for(unsigned int c=0; c < numChannels; c++)
      {
	offset_iterator<typename Buffer::iterator> itr(buf.begin(), c);
	tempBuffer[c].convertBuffer(itr);
      }

      // Done.
    }
#endif /* INCLUDE_OPENEXR */


  } // end exr namespace
}   // end io  namespace

#endif /* _BUFFER2DIO_EXR_H_ */
