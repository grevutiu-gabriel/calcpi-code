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
#include "CalcPiBenchmarkThread.h"
#include "CalculationInterface.h"
#include "cp_config.h"
#include "CalcPiFitBenchmarkResults.h"
#include "CalcPiMainFrame.h"

// The benchmark runs at least this many milliseconds, then stops after the next experiment
const int CalcPiBenchmarkThread::runtimeMS_ = 5000;
// The first experiment runs with this amount of digits
const int CalcPiBenchmarkThread::startDigits_ = 1000;
// The number of digits is incremented by this factor for each experiment
const int CalcPiBenchmarkThread::increaseDigits_ = 2;

CalcPiBenchmarkThread::CalcPiBenchmarkThread(CalculationInterface *pCalculationInterface)
	: wxThread(wxTHREAD_JOINABLE),
	pCalculationInterface_(pCalculationInterface), pCalcPiMainFrame_(NULL)
{
}

CalcPiBenchmarkThread::~CalcPiBenchmarkThread()
{
}

void CalcPiBenchmarkThread::transferBenchmarkResults(CalcPiFitBenchmarkResults *pCalcPiFitBenchmarkResults)
{
	pCalcPiFitBenchmarkResults->setBenchmarkResults(experimentDigits_, experimentTimes_, experimentMemory_);
}

void CalcPiBenchmarkThread::setMainFrame(CalcPiMainFrame *pCalcPiMainFrame)
{
	pCalcPiMainFrame_ = pCalcPiMainFrame;
}

// Gets called from the Worker thread!
void CalcPiBenchmarkThread::updateProgress(int progress)
{
}

wxThread::ExitCode CalcPiBenchmarkThread::Entry()
{
	wxDateTime startTime = wxDateTime::UNow();
	long long int digits = static_cast<long long int>(startDigits_);
	int elapsedMS = 0;

	while(elapsedMS < runtimeMS_)
	{
		if(pCalcPiMainFrame_ != NULL)
			pCalcPiMainFrame_->clearMaxMemoryUsed();

		runExperiment(digits);

		size_t memoryUsed = 0;
		if(pCalcPiMainFrame_ != NULL)
			memoryUsed = pCalcPiMainFrame_->getMaxMemoryUsed();
		if(memoryUsed == 0)
			memoryUsed = getCurrentRSS();
		experimentMemory_.push_back(static_cast<double>(memoryUsed));

		digits *= static_cast<long long int>(increaseDigits_);

		// Check for stop condition
		wxTimeSpan diff = wxDateTime::UNow() - startTime;
		elapsedMS = std::abs(diff.GetMilliseconds().GetValue());
	}

	// Only for debug: Write results to file
#if(0)
	{
		std::ofstream of("benchmark_result.txt");
		if(of.is_open())
		{
			for(size_t i = 0; i < experimentDigits_.size(); i++)
			{
				of << experimentDigits_[i] << "\t" << experimentTimes_[i] << std::endl;
			}
		}
	}
#endif

	// Testing: Throw away first 3 results
//	experimentDigits_.erase(experimentDigits_.begin(), experimentDigits_.begin()+3);
//	experimentTimes_.erase(experimentTimes_.begin(), experimentTimes_.begin()+3);

	return 0;
}

void CalcPiBenchmarkThread::runExperiment(long long int digits)
{
	double wallTime = 0;

#if defined(CP_HAVE_BOOST)
	boost::chrono::high_resolution_clock::time_point t1 = boost::chrono::high_resolution_clock::now();
#else
	wxDateTime t1 = wxDateTime::UNow();
#endif

	wxString digitsStr = wxString::Format(wxT("%lld"), digits);
	pCalculationInterface_->setDigits(digitsStr);

	// Run experiment
	pCalculationInterface_->calculate(this);

#if defined(CP_HAVE_BOOST)
	boost::chrono::duration<double> diff = boost::chrono::high_resolution_clock::now() - t1;
	wallTime = diff.count();
#else
	wxTimeSpan diff = wxDateTime::UNow() - t1;
	wallTime = std::abs(static_cast<double>(diff.GetMilliseconds().GetValue())) / 1000.0;
#endif

	experimentDigits_.push_back( static_cast<double>(digits) );
	experimentTimes_.push_back( wallTime );
}
