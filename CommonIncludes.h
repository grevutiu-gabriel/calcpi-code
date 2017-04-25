/**
 * Copyright (C) 2013 Roman Hiestand
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef COMMONINCLUDES_H
#define COMMONINCLUDES_H

// Define the windows API level we'd like to use: Windows XP
#define _WIN32_WINNT 0x0501

#include "cp_config.h"

// Workaround for missing symbols like _wxTheAssertHandler in MinGW
#define wxDEBUG_LEVEL 0

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/datetime.h"
#include "wx/image.h"
#include "wx/bookctrl.h"
#include "wx/artprov.h"
#include "wx/imaglist.h"
#include "wx/sysopt.h"
#include <wx/aboutdlg.h>
#include <wx/timer.h>
#include <wx/listctrl.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/mstream.h>

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "gmp.h"
// Disable warning  C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning( disable : 4800 )
#include "gmpxx.h"
//#pragma warning( default : 4800 )
#include "mpfr.h"

#if defined(CP_HAVE_BOOST)
// boost
#include <boost/version.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#endif

// Declaration of the functions in getMemorySize.c and getRSS.c
extern "C" size_t getCurrentRSS();
extern "C" size_t getPeakRSS();
extern "C" size_t getMemorySize();

#endif
