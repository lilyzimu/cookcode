//////////////////////////////////////////////////////
// EXR Importer and exporter implementation details //
//////////////////////////////////////////////////////

#if !defined(_BUFFER2DIO_EXR_DETAIL_H_) && defined(_BUFFER2DIO_EXR_H_)
#define _BUFFER2D_EXR_DETAIL_H_

#include "ImfPixelType.h"
#include "half.h"

#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/int.hpp>

#include <cassert>

namespace io {
  namespace exr {
    namespace detail {

      /************************************************************************/
      /*                       Determine optimal EXR data type                */
      /*                              at compile time.                        */
      /************************************************************************/
      template<typename T>
	class typeConvertor : public std::unary_function<boost::uint32_t, T> {

	//////////////////
	// Sanity check //
	//////////////////
	BOOST_MPL_ASSERT_MSG(  sizeof(T) <= sizeof(boost::uint32_t), EXR_GENERIC_TYPE_TOO_LARGE, (T) );

      public:
	typedef boost::uint32_t   type;
	boost::uint32_t operator()(T& type) const { return uint32_t(type); }
	T operator()(uint32_t value) const { return (T)(value); }
	Imf::PixelType pixelType(void) const { return Imf::UINT; }
	bool sameType(void) const { return boost::is_same<boost::uint32_t, T>::value; }
	unsigned int size(void) const { return sizeof(type); }
      };
      
      
      template<>
	class typeConvertor<float> : public std::unary_function<float, float> {
      public:
	typedef float   type;
	float operator()(float type) const { return type; }
	Imf::PixelType pixelType(void) const { return Imf::FLOAT; }
	bool sameType(void) const { return true; }
	unsigned int size(void) const { return sizeof(type); }
      };
      
      template<>
	class typeConvertor<double> : public std::unary_function<float, double> {
      public:
	typedef double   type;
	float operator()(double type) const { return (float)(type); }
	double operator()(float value) const { return (double)(value); }
	Imf::PixelType pixelType(void) const { return Imf::FLOAT; }
	bool sameType(void) const { return false; }
	unsigned int size(void) const { return sizeof(type); }
      };
      
      template<>
	class typeConvertor<half> : public std::unary_function<half, half> {
      public:
	typedef half   type;
	half operator()(half type) const { return type; }
	Imf::PixelType pixelType(void) const { return Imf::HALF; }
	bool sameType(void) const { return true; }
	unsigned int size(void) const { return sizeof(type); }
      };


      /************************************************************************/
      /*                      frameBuffer for input/output of                 */
      /*                      buffer channels.  Automatically                 */
      /*                      converts (if req.) the data to                  */
      /*                           an EXR data type.                          */
      /************************************************************************/
      template<typename PtrType>
      class frameBuffer_base {
	typedef typename boost::remove_const<PtrType>::type    BufferType;
	
      public:

	frameBuffer_base(void) : _buffer(NULL), _ptr(NULL), _stride(0), _allocSize(0), _pixelType(Imf::HALF), _inPlace(true) {}

	template<typename Iterator>
	  frameBuffer_base(unsigned long size, Iterator itr) 
	  {
	    typedef typeConvertor<typename boost::remove_const<typename std::iterator_traits<Iterator>::value_type>::type > convertor_type;
	    convertor_type convertor;

	    // init
	    _ptr = (PtrType*)(&(*itr));
	    PtrType* next = (PtrType*)(&(*(itr+1)));
	    _buffer = NULL;
	    _inPlace = convertor.sameType();
	    _stride = (_inPlace) ? std::distance(_ptr, next) : convertor.size();
	    _pixelType = convertor.pixelType();	 
	    _allocSize = size * convertor.size();

	    // allocate
	    if(!_inPlace) _buffer = new BufferType[_allocSize];
	  }


	frameBuffer_base(const frameBuffer_base& fb)
	{
	  // trivial copy
	  _inPlace = fb._inPlace;
	  _allocSize = fb._allocSize;
	  _stride = fb._stride;
	  _pixelType = fb._pixelType;
	  _ptr = fb._ptr;
	  _buffer = NULL;

	  // check if we need to copy the buffer
	  if(!fb._inPlace)
	  {
	    _buffer = new BufferType[fb._allocSize];
	    std::copy(_buffer, _buffer + _allocSize, fb._buffer);
	  }

	  // Done.
	}

	////////////////
	// Destructor //
	////////////////
	virtual ~frameBuffer_base(void) { if(_buffer) delete[] _buffer; }

	///////////////
	// Operators //
	///////////////
	frameBuffer_base& operator=(const frameBuffer_base& src) { _assign(src); return *this; }

	////////////////
	// Inspectors //
	////////////////
	unsigned int stride(void) const             { return _stride; }
	PtrType* buffer(void) const                 { return (_inPlace) ? _ptr : _buffer; }
	const Imf::PixelType& pixelType(void) const { return _pixelType; }
	bool convertedBuffer(void) const            { return _inPlace; }

	/////////////
	// Friends //
	/////////////
	friend void swap(frameBuffer_base& a, frameBuffer_base& b) { return a._swap(b); }

      protected:
	/////////////////////
	// Private Methods //
	/////////////////////
	template<typename Iterator>
	  void copyToBuffer(Iterator itr)
	  {
	    typedef typeConvertor<typename boost::remove_const<typename std::iterator_traits<Iterator>::value_type>::type > convertor_type;
	    convertor_type convertor;

	    // sanity check
	    assert( (PtrType*)(&(*itr)) == _ptr );

	    // do we need a temp buffer?
	    if(!_inPlace)
	    {
	      typename convertor_type::type* typeBuffer = (typename convertor_type::type *)(_buffer);

	      // convert
	      for(unsigned long i=0; i < _allocSize / convertor.size(); i++, itr++, typeBuffer++)
		*typeBuffer = convertor(*itr);
	    }

	    // Done.
	  }

	template<typename Iterator>
	  void copyFromBuffer(Iterator itr) const
	  {
	    typedef typeConvertor<typename boost::remove_const<typename std::iterator_traits<Iterator>::value_type>::type > convertor_type;
	    convertor_type convertor;

	    // sanity check
	    assert( (PtrType*)(&(*itr)) == _ptr );

	    // do we need to do anything?
	    if(!_inPlace)
	    {
	      typename convertor_type::type* typeBuffer = (typename convertor_type::type *)(_buffer);

	      // convert
	      for(unsigned long i=0; i < _allocSize / convertor.size(); i++, itr++, typeBuffer++)
		*itr = convertor(*typeBuffer);
	    }

	    // Done.
	  }

	void _assign(const frameBuffer_base& src)
	{
	  // sanity check
	  if(this == &src) return;

	  // create new buffer
	  frameBuffer_base temp(src);
	  _swap(temp);

	  // Done.
	}

	void _swap(frameBuffer_base& swp)
	{
	  std::swap(_inPlace, swp._inPlace);
	  std::swap(_buffer, swp._buffer);
	  std::swap(_ptr, swp._ptr);
	  std::swap(_stride, swp._stride);
	  std::swap(_allocSize, swp._allocSize);
	  std::swap(_pixelType, swp._pixelType);
	}

	/////////////////////
	// Private Members //
	/////////////////////
	bool _inPlace;
	BufferType* _buffer;
	PtrType* _ptr;
	unsigned int _stride;
	unsigned int _allocSize;
	Imf::PixelType _pixelType;
      };


      class outputFrameBuffer : public frameBuffer_base<const boost::uint8_t> 
      {
      public:
	//////////////////
	// Constructors //
	//////////////////
	outputFrameBuffer(void) : frameBuffer_base() {}
        outputFrameBuffer(const outputFrameBuffer& src) : frameBuffer_base(src) {}

	template<typename Iterator>
	  outputFrameBuffer(unsigned long size, const Iterator& itr) : frameBuffer_base(size, itr)
	  {
	    copyToBuffer(itr);
	  }

	////////////////
	// Destructor //
	////////////////
	virtual ~outputFrameBuffer(void) {}
	
	///////////////
	// Operators //
	///////////////
	outputFrameBuffer& operator=(const outputFrameBuffer& fb) { _assign(fb); return *this; }
	
	/////////////
	// Friends //
	/////////////
	friend void swap(outputFrameBuffer& a, outputFrameBuffer& b) { a._swap(b); }
      };

      
      class inputFrameBuffer : public frameBuffer_base<boost::uint8_t> 
      {
      public:
	//////////////////
	// Constructors //
	//////////////////
	inputFrameBuffer(void) : frameBuffer_base() {}
	inputFrameBuffer(const inputFrameBuffer& src) : frameBuffer_base(src) {}

	template<typename Iterator>
	  inputFrameBuffer(unsigned long size, Iterator itr) : frameBuffer_base(size, itr) {}


	/////////////////
	// Destructors //
	/////////////////
	virtual ~inputFrameBuffer(void) {}

	///////////////
	// Operators //
	///////////////
        inputFrameBuffer& operator=(const inputFrameBuffer& src) { _assign(src); return *this; }

	/////////////
	// Methods //
	/////////////
	template<typename Iterator>
	  void convertBuffer(Iterator dst) const
	  {
	    copyFromBuffer(dst);
	  }

	/////////////
	// Friends //
	/////////////
	friend void swap(inputFrameBuffer& a, inputFrameBuffer& b) { a._swap(b); }
      };

    }  // io     namespace end
  }    // exr    namespace end
}      // detail namespace end 

#endif /* _BUFFER2D_EXR_DETAIL_H_ */

