# Try to find the GMP librairies
#  GMP_FOUND - system has GMP lib
#  GMP_INCLUDE_DIR - the GMP include directory
#  GMP_LIBRARIES - Libraries needed to use GMP
#  GMPXX_LIBRARIES - Libraries needed to use GMP C++ interface

# Copyright (c) 2012, Roman Hiestand
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (GMP_INCLUDE_DIR AND GMP_LIBRARIES)
	# Already in cache, be silent
	set(GMP_FIND_QUIETLY TRUE)
endif (GMP_INCLUDE_DIR AND GMP_LIBRARIES)

find_path(GMP_INCLUDE_DIR NAMES gmp.h PATHS ${GMP_PATH} ${GMP_PATH}/inc ${GMP_PATH}/include)
find_library(GMP_LIBRARIES NAMES gmp libgmp PATHS ${GMP_PATH} ${GMP_PATH}/lib)
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx PATHS ${GMP_PATH} ${GMP_PATH}/lib)
MESSAGE(STATUS "GMP libs: " ${GMP_LIBRARIES} " " ${GMPXX_LIBRARIES} )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMP DEFAULT_MSG GMP_INCLUDE_DIR GMP_LIBRARIES)

mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARIES)
