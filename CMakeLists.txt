cmake_minimum_required(VERSION 2.6)
project(calc_pi)

#SET(CMAKE_VERBOSE_MAKEFILE TRUE)
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")

# Enable Unicode
ADD_DEFINITIONS(-D_UNICODE -DUNICODE)

# GMP/MPIR
IF(UNIX AND NOT APPLE)
	# Use GMP instead of MPIR
	#SET(MPIR_LIBRARIES /usr/lib/x86_64-linux-gnu/libgmp.a /usr/lib/x86_64-linux-gnu/libgmpxx.a)
	FIND_PACKAGE(GMP REQUIRED)
	INCLUDE_DIRECTORIES(${GMP_INCLUDE_DIR})
	message(${GMP_LIBRARIES})
ELSE(UNIX AND NOT APPLE)
	FIND_PACKAGE(MPIR REQUIRED)
	INCLUDE_DIRECTORIES(${MPIR_INCLUDE_DIR})
	SET(CP_HAVE_MPIR 1)
ENDIF(UNIX AND NOT APPLE)

# MPFR
FIND_PACKAGE(MPFR REQUIRED)
INCLUDE_DIRECTORIES(${MPFR_INCLUDE_DIR})
message(${MPFR_LIBRARIES})

IF(NOT DEFINED ${IS_64BITS})
	SET(IS_64BITS 0)
ENDIF(NOT DEFINED ${IS_64BITS})

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET(IS_64BITS 1)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

# wxWidgets
IF(WIN32)
	IF(CMAKE_COMPILER_IS_GNUCC)
		IF(${IS_64BITS} EQUAL 1)
			SET(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/gcc_lib64)
		ELSE(${IS_64BITS} EQUAL 1)
			SET(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/gcc_lib)
		ENDIF(${IS_64BITS} EQUAL 1)
	ELSE(CMAKE_COMPILER_IS_GNUCC)
		IF(${IS_64BITS} EQUAL 1)
			SET(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_lib64)
		ELSE(${IS_64BITS} EQUAL 1)
			SET(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_lib)
		ENDIF(${IS_64BITS} EQUAL 1)
	ENDIF(CMAKE_COMPILER_IS_GNUCC)
	IF(CMAKE_BUILD_TYPE="Debug")
		SET(wxWidgets_CONFIGURATION mswud)
	ELSE()
		SET(wxWidgets_CONFIGURATION mswu)
	ENDIF()
ENDIF(WIN32)
message("wxWidgets_ROOT_DIR = ${wxWidgets_ROOT_DIR}")
#message("wxWidgets_LIB_DIR = ${wxWidgets_LIB_DIR}")
#message("wxWidgets_CONFIGURATION = ${wxWidgets_CONFIGURATION}")

FIND_PACKAGE(wxWidgets REQUIRED net gl adv core base)
INCLUDE(${wxWidgets_USE_FILE})
message("wxWidgets_INCLUDE_DIRS = ${wxWidgets_INCLUDE_DIRS}")
#message("wxWidgets_LIBRARIES = ${wxWidgets_LIBRARIES}")
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
IF(MSVC)
	SET(Boost_USE_STATIC_RUNTIME     OFF)
ELSE(MSVC)
	SET(Boost_USE_STATIC_RUNTIME     ON)
ENDIF(MSVC)
#SET(Boost_DEBUG TRUE)
find_package( Boost 1.53.0 COMPONENTS date_time thread system chrono)
SET(CP_HAVE_BOOST ${Boost_FOUND})
IF(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	IF(UNIX AND NOT APPLE)
		# Workaround for apparent bug in FindBoost (librt is not added at the end):
		message("Boost_LIBRARIES = ${Boost_LIBRARIES}")
		SET(Boost_LIBRARIES "${Boost_LIBRARIES};rt")
	ENDIF(UNIX AND NOT APPLE)
ENDIF(Boost_FOUND)

# Eigen
SET( CMAKE_MODULE_PATH "${EIGEN3_INCLUDE_DIR}/cmake" )
FIND_PACKAGE(Eigen3 3.0.0)
IF(EIGEN3_FOUND)
	message("Found Eigen ${EIGEN3_VERSION}")
	SET(CP_HAVE_EIGEN 1)
	include_directories( ${EIGEN3_INCLUDE_DIR} )
ENDIF(EIGEN3_FOUND)

# OpenMP
IF(NOT CP_DISABLE_OPENMP)
	find_package( OpenMP )
	IF(OPENMP_FOUND)
		SET( CP_HAVE_OPENMP 1 )
		SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
		SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
	ENDIF(OPENMP_FOUND)
ENDIF(NOT CP_DISABLE_OPENMP)

# Determine whether 32 or 64 bits
IF(${IS_64BITS} EQUAL 1)
	SET( CP_CMAKE_ARCHITECTURE "x64" )
	SET( CP_COMPILER_ARCH "" )
	IF(MINGW)
		SET( CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -D WX_CPU_AMD64" )
	ENDIF(MINGW)
ELSE(${IS_64BITS} EQUAL 1)
	SET( CP_CMAKE_ARCHITECTURE "x86")
	SET( CP_COMPILER_ARCH "/arch:SSE2")
	IF(MINGW)
		SET( CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -D WX_CPU_X86" )
	ENDIF(MINGW)
ENDIF(${IS_64BITS} EQUAL 1)

# Prepare config.h with several defines and make sure it's in our include path
IF(WIN32)
	SET( CP_WIN32 1 )
ENDIF(WIN32)
IF(APPLE)
	SET( CP_MACOSX 1 )
ENDIF(APPLE)
configure_file(config.h.in cp_config.h)
include_directories(${CMAKE_BINARY_DIR})

if(MSVC)
	SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CP_COMPILER_ARCH} /MP" )
	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CP_COMPILER_ARCH} /MP" )
	SET( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /Ob2 /Oi /Ot /GL /GS- ${CP_COMPILER_ARCH} /fp:fast /MP" )
	SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Oi /Ot /GL /GS- ${CP_COMPILER_ARCH} /fp:fast /MP" )
ENDIF(MSVC)

# -flto
IF(CMAKE_COMPILER_IS_GNUCC)
	SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2 -mfpmath=sse" )
	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -mfpmath=sse -fpermissive" )
	SET( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=core2 -ffast-math -funsafe-loop-optimizations" )
	SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=core2 -ffast-math -funsafe-loop-optimizations" )

	IF(CP_USE_STATIC_LIBGCC)
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
	ENDIF(CP_USE_STATIC_LIBGCC)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -msse2 -ffast-math")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

SET(ALL_SRC ${ALL_SRC} CalcPiApp.cpp CalcPiMainFrame.cpp CalcPiMainFrameBase.cpp
	CalculatePIChudnovsky.cpp CalculatePIMPFR.cpp
	CalcPiWorkerThread.cpp CalcPiVirtualListControl.cpp CalculateEMPFR.cpp
	CalculatorFactory.cpp CalculatePIChudnovskyOMP.cpp CalcPiBenchmarkThread.cpp
	CalcPiFitBenchmarkResults.cpp getRSS.c getMemorySize.c)
IF(Boost_FOUND)
	SET(ALL_SRC ${ALL_SRC} CalculatePIBoost.cpp)
ENDIF(Boost_FOUND)

SET(ALL_HEADERS ${ALL_HEADERS} CalcPiApp.h CalcPiMainFrame.h CalcPiMainFrameBase.h CommonIncludes.h
	CalculationInterface.h ProgressIndicatorInterface.h CalculatePIChudnovsky.h
	CalculatePIMPFR.h CalcPiWorkerThread.h CalcPiVirtualListControl.h
	CalculateEMPFR.h version.h CalculatorFactory.h CalculatePIChudnovskyOMP.h
	CalcPiBenchmarkThread.h CalcPiFitBenchmarkResults.h)
IF(Boost_FOUND)
	SET(ALL_HEADERS ${ALL_HEADERS} CalculatePIBoost.h)
ENDIF(Boost_FOUND)

# Add resource definitions for Windows
IF(WIN32)
	SET(ALL_SRC ${ALL_SRC} res/win/calc_pi.rc)
ENDIF(WIN32)

# For Apple set the icns file containing icons
if(APPLE)
	# icon files to copy in the bundle
	SET( OSX_ICON_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/osx/calc_pi.icns )
	# set where in the bundle to put the icns files
	SET_SOURCE_FILES_PROPERTIES( ${OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	# include the icns files in the target
	SET( ALL_SRC ${ALL_SRC} ${OSX_ICON_FILES} )
ENDIF(APPLE)

ADD_EXECUTABLE(calc_pi WIN32 MACOSX_BUNDLE ${ALL_SRC} ${ALL_HEADERS})

#TARGET_LINK_LIBRARIES(calc_pi ${wxWidgets_LIBRARIES} ${MPFR_LIBRARIES} ${MPIR_LIBRARIES} ${MPIRXX_LIBRARIES} ${Boost_LIBRARIES})

IF(GMP_FOUND)
	IF(UNIX)
	TARGET_LINK_LIBRARIES(calc_pi ${wxWidgets_LIBRARIES} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${MPIRXX_LIBRARIES} ${Boost_LIBRARIES})
	ENDIF(UNIX)
ENDIF(GMP_FOUND)

IF(Boost_FOUND)
	TARGET_LINK_LIBRARIES(calc_pi ${Boost_LIBRARIES})
ENDIF(Boost_FOUND)
IF(WIN32)
	TARGET_LINK_LIBRARIES(calc_pi psapi.lib)
ENDIF(WIN32)

if(APPLE)
	# configure CMake to use a custom Info.plist
	SET_TARGET_PROPERTIES( calc_pi PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/osx/calc_pi-Info.plist )
ENDIF(APPLE)
