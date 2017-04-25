# Try to find the MPIR librairies
#  MPIR_FOUND - system has MPIR lib
#  MPIR_INCLUDE_DIR - the MPIR include directory
#  MPIR_LIBRARIES - Libraries needed to use MPIR
#  MPIRXX_LIBRARIES - Libraries needed to use MPIR C++ interface

# Copyright (c) 2012, Roman Hiestand
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (MPIR_INCLUDE_DIR AND MPIR_LIBRARIES)
	# Already in cache, be silent
	set(MPIR_FIND_QUIETLY TRUE)
endif (MPIR_INCLUDE_DIR AND MPIR_LIBRARIES)

find_path(MPIR_INCLUDE_DIR NAMES mpir.h PATHS ${MPIR_PATH} ${MPIR_PATH}/inc ${MPIR_PATH}/include)
find_library(MPIR_LIBRARIES NAMES mpir libmpir PATHS ${MPIR_PATH} ${MPIR_PATH}/lib)
find_library(MPIRXX_LIBRARIES NAMES mpirxx libmpirxx PATHS ${MPIR_PATH} ${MPIR_PATH}/lib)
MESSAGE(STATUS "MPIR libs: " ${MPIR_LIBRARIES} " " ${MPIRXX_LIBRARIES} )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MPIR DEFAULT_MSG MPIR_INCLUDE_DIR MPIR_LIBRARIES)

mark_as_advanced(MPIR_INCLUDE_DIR MPIR_LIBRARIES)
