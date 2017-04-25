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

#include "CommonIncludes.h"
#include "CalcPiWorkerThread.h"
#include "CalculationInterface.h"
#include "cp_config.h"

CalcPiWorkerThread::CalcPiWorkerThread(CalculationInterface *pCalculationInterface)
	: wxThread(wxTHREAD_JOINABLE),
	pCalculationInterface_(pCalculationInterface), mutex_(), progress_(0),
	duration_(0), cpuUserTime_(0)
{
}

CalcPiWorkerThread::~CalcPiWorkerThread()
{
}

int CalcPiWorkerThread::getProgress()
{
	wxMutexLocker lock(mutex_);
	return progress_;
}

double CalcPiWorkerThread::getDuration()
{
	wxMutexLocker lock(mutex_);
	return duration_;
}

double CalcPiWorkerThread::getCPUUserTime()
{
	wxMutexLocker lock(mutex_);
	return cpuUserTime_;
}

// Gets called from the Worker thread!
void CalcPiWorkerThread::updateProgress(int progress)
{
	wxMutexLocker lock(mutex_);
	progress_ = progress;
}

wxThread::ExitCode CalcPiWorkerThread::Entry()
{
	{
		wxMutexLocker lock(mutex_);
		progress_ = 0;
		duration_ = 0;
	}
#if defined(CP_HAVE_BOOST)
	boost::chrono::high_resolution_clock::time_point t1 = boost::chrono::high_resolution_clock::now();
#	if defined(BOOST_CHRONO_HAS_PROCESS_CLOCKS)
	boost::chrono::process_cpu_clock::time_point cpu_t1 = boost::chrono::process_cpu_clock::now();
#	endif
#else
	wxDateTime t1 = wxDateTime::UNow();
#endif
	pCalculationInterface_->calculate(this);
#if defined(CP_HAVE_BOOST)
	boost::chrono::duration<double> diff = boost::chrono::high_resolution_clock::now() - t1;
#	if defined(BOOST_CHRONO_HAS_PROCESS_CLOCKS)
	boost::chrono::process_cpu_clock::duration dur_cpu = boost::chrono::process_cpu_clock::now() - cpu_t1;
	boost::chrono::process_cpu_clock_times diff_cpu = dur_cpu.count();
#	endif
#else
	wxTimeSpan diff = wxDateTime::UNow() - t1;
#endif
	{
		wxMutexLocker lock(mutex_);
		progress_ = 100;
		cpuUserTime_ = 0;
#if defined(CP_HAVE_BOOST)
		duration_ = diff.count();
#	if defined(BOOST_CHRONO_HAS_PROCESS_CLOCKS)
		cpuUserTime_ = static_cast<double>(diff_cpu.user)/1000000000.0;
#	endif
#else
		duration_ = std::abs(static_cast<double>(diff.GetMilliseconds().GetValue())) / 1000.0;
#endif
	}
	return 0;
}
