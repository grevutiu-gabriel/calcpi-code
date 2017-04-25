#ifndef VERSION_H
#define VERSION_H

#define CP_STRINGIFY2(x) #x
#define CP_STRINGIFY(x) CP_STRINGIFY2(x)

#define CALC_PI_VERSION_MAJOR 1
#define CALC_PI_VERSION_MINOR 0
#define CALC_PI_VERSION_BUILD 0

#define CALC_PI_VERSION_STRING CP_STRINGIFY(CALC_PI_VERSION_MAJOR) \
	"." CP_STRINGIFY(CALC_PI_VERSION_MINOR) \
	"." CP_STRINGIFY(CALC_PI_VERSION_BUILD)

#define CALC_PI_NAME "Calc PI"
#define CALC_PI_COPYRIGHT_NAME "Roman Hiestand"
#define CALC_PI_COPYRIGHT_YEAR "2013"

// The check for __INTEL_COMPILER needs to be before _MSC_VER and __GNUG__, as
// the Intel compiler defines _MSC_VER and __GNUG__ as well
#if defined(__INTEL_COMPILER)
#	define CALC_PI_COMPILER "Intel C++"
#	define CALC_PI_COMPILER_VERSION CP_STRINGIFY(__INTEL_COMPILER)
#elif defined(_MSC_VER)
#	define CALC_PI_COMPILER "Visual C++"
#	define CALC_PI_COMPILER_VERSION CP_STRINGIFY(_MSC_FULL_VER)
#elif defined(__GNUG__)
#	define CALC_PI_COMPILER "GCC"
#	if defined(__GNUC_PATCHLEVEL__)
#		define CALC_PI_COMPILER_VERSION CP_STRINGIFY(__GNUC__) "." CP_STRINGIFY(__GNUC_MINOR__) "." CP_STRINGIFY(__GNUC_PATCHLEVEL__)
#	else
#		define CALC_PI_COMPILER_VERSION CP_STRINGIFY(__GNUC__) "." CP_STRINGIFY(__GNUC_MINOR__)
#	endif
#else
#	define CALC_PI_COMPILER "Unknown compiler"
#	define CALC_PI_COMPILER_VERSION "Unknown version"
#endif

// Architecture
#if defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define CALC_PI_ARCHITECTURE_X64
#define CALC_PI_ARCHITECTURE_STRING "x64"
#elif defined(_M_ARM) || defined(__arm__) || defined(_ARM)
#define CALC_PI_ARCHITECTURE_ARM
#define CALC_PI_ARCHITECTURE_STRING "ARM"
#elif defined(_M_IX86) || defined(__i386) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_X86_)
#define CALC_PI_ARCHITECTURE_X86
#define CALC_PI_ARCHITECTURE_STRING "x86"
#elif defined(_M_IA64) || defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64)
#define CALC_PI_ARCHITECTURE_ITANIUM
#define CALC_PI_ARCHITECTURE_STRING "Itanium"
#elif defined(_M_PPC) || defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__)
#define CALC_PI_ARCHITECTURE_PPC
#define CALC_PI_ARCHITECTURE_STRING "PPC"
#elif defined(__sparc__) || defined(__sparc)
#define CALC_PI_ARCHITECTURE_SPARC
#define CALC_PI_ARCHITECTURE_STRING "SPARC"
#endif

#endif
