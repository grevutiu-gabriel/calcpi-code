# Try to find the MPFR librairies
#  MPFR_FOUND - system has MPFR lib
#  MPFR_INCLUDE_DIR - the MPFR include directory
#  MPFR_LIBRARIES - Libraries needed to use MPFR

# Copyright (c) 2013, Roman Hiestand
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (MPFR_INCLUDE_DIR AND MPFR_LIBRARIES)
	# Already in cache, be silent
	set(MPFR_FIND_QUIETLY TRUE)
endif (MPFR_INCLUDE_DIR AND MPFR_LIBRARIES)

find_path(MPFR_INCLUDE_DIR NAMES mpfr.h PATHS ${MPFR_PATH} ${MPFR_PATH}/inc ${MPFR_PATH}/include)
find_library(MPFR_LIBRARIES NAMES mpfr libmpfr PATHS ${MPFR_PATH} ${MPFR_PATH}/lib ${MPFR_PATH}/mingw32 ${MPFR_PATH}/lib_mingw64tdm)
MESSAGE(STATUS "MPFR libs: " ${MPFR_LIBRARIES} )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MPFR DEFAULT_MSG MPFR_INCLUDE_DIR MPFR_LIBRARIES)

mark_as_advanced(MPFR_INCLUDE_DIR MPFR_LIBRARIES)
