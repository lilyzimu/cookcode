# Find glabLib
###############
# => GLABLIB_DEFINITIONS
# => GLABLIB_INCLUDE_DIRS
# => GLABLIB_LIBRARIES
# <= GLABLIB_BASE_DIR
###############

# include boost
find_package(Boost REQUIRED)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


# check for CLAPACK	
find_package(CLapack)

if(CLAPACK_FOUND)
	set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CLAPACK_INCLUDE_DIRS})
	set(GLABLIB_LIBRARIES ${GLABLIB_LIBRARIES} ${CLAPACK_LIBRARIES} )
	set(GLABLIB_DEFINITIONS "${GLABLIB_DEFINITIONS} -DINCLUDE_LAPACK")
endif(CLAPACK_FOUND)


# check for OpenEXR
find_package(OpenEXR)

if(OPENEXR_FOUND)
	set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${OPENEXR_INCLUDE_DIRS})
	set(GLABLIB_LIBRARIES ${GLABLIB_LIBRARIES} ${OPENEXR_LIBRARIES})
	set(GLABLIB_DEFINITIONS "${GLABLIB_DEFINITIONS} -DINCLUDE_OPENEXR")
endif(OPENEXR_FOUND)


# check for PNG
find_package(PNG)

if(PNG_FOUND)
	set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
	set(GLABLIB_LIBRARIES ${GLABLIB_LIBRARIES} ${PNG_LIBRARIES})
	set(GLABLIB_DEFINITIONS "${GLABLIB_DEFINITIONS} -DINCLUDE_PNG")
	set(GLABLIB_DEFINITIONS ${GLABLIB_DEFINITIONS} ${PNG_DEFINITIONS})
endif(PNG_FOUND)


# check for JPEG
find_package(JPEG)

if(JPEG_FOUND)
	set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${JPEG_INCLUDE_DIRS})
	set(GLABLIB_LIBRARIES ${GLABLIB_LIBRARIES} ${JPEG_LIBRARIES})
	set(GLABLIB_DEFINITIONS "${GLABLIB_DEFINITIONS} -DINCLUDE_JPEG")
endif(JPEG_FOUND)


# add subdirs to include
find_path(CURRENT_DIR cmake/Modules/FindGlabLib.cmake ${CMAKE_MODULE_PATH}/../.. NO_DEFAULT_PATH)
if(DEFINED GLABLIB_BASE_DIR)
	set(CURRENT_DIR ${GLABLIB_BASE_DIR})
endif(DEFINED GLABLIB_BASE_DIR)

message("-- GlabLib base dir: ${CURRENT_DIR}") 


set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/IO)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/exceptions)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/camera)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/image)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/geometry)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/container)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/util)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/math)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/brdf)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/bssrdf)
set(GLABLIB_INCLUDE_DIRS ${GLABLIB_INCLUDE_DIRS} ${CURRENT_DIR}/algorithms)

# Done.
mark_as_advanced(GLABLIB_INCLUDE_DIRS GLABLIB_LIBRARIES GLABLIB_DEFINITIONS)

