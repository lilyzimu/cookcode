/////////////////////////////////////
// Generic buffer2d IO functions.  //
//                                 //
// Works for all fundamental types //
// and types with iterator support //
// Specialization required in other//
// cases.                          //
/////////////////////////////////////

#ifndef _BUFFER2DIO_H_
#define _BUFFER2DIO_H_

#include <string>

#include "buffer2d.h"
#include "iteratorWrapper.h"

//////////////////
// File formats //
//////////////////
#include "buffer2dIO.ppm.h"
#include "buffer2dIO.pfm.h"
#include "buffer2dIO.exr.h"
#include "buffer2dIO.raw.h"
#include "buffer2dIO.png.h"
#include "buffer2dIO.jpg.h"

using namespace std;

namespace io {

  ///////////////
  // Exporters //
  ///////////////
  template<typename T> void exportPFM(const string& filename, const buffer2d<T>& buf, float pad=0.0f)                                 { pfm::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad); }
  template<typename T> void exportPPM(const string& filename, const buffer2d<T>& buf, float pad=0.0f)                                 { ppm::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad); }
  template<typename T> void exportEXR(const string& filename, const buffer2d<T>& buf, float pad=0.0f)                                 { exr::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf); }
  template<typename T> void exportPNG(const string& filename, const buffer2d<T>& buf, float pad=0.0f)                                 { png::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad); }
  template<typename T> void exportJPG(const string& filename, const buffer2d<T>& buf, float pad=0.0f, float compressionQuality=0.95f) { jpg::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad, compressionQuality); }


  ///////////////
  // Importers //
  ///////////////
  template<typename T> void importPFM(const string& filename, buffer2d<T>& result, float pad=0.0f) { pfm::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importPPM(const string& filename, buffer2d<T>& result, float pad=0.0f) { ppm::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importEXR(const string& filename, buffer2d<T>& result, float pad=0.0f) { exr::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result); }
  template<typename T> void importRAW(const string& filename, buffer2d<T>& result, float pad=0.0f) { raw::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importPNG(const string& filename, buffer2d<T>& result, float pad=0.0f) { png::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importJPG(const string& filename, buffer2d<T>& result, float pad=0.0f) { jpg::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }

} // io namespace

#endif /* _BUFFER2DIO_H_ */
