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

#ifndef CALCPIBENCHMARKTHREAD_H
#define CALCPIBENCHMARKTHREAD_H

class CalculationInterface;
class CalcPiFitBenchmarkResults;
class CalcPiMainFrame;
#include "ProgressIndicatorInterface.h"

class CalcPiBenchmarkThread : public wxThread, public ProgressIndicatorInterface
{
public:
	CalcPiBenchmarkThread(CalculationInterface *pCalculationInterface);
	virtual ~CalcPiBenchmarkThread();

	void transferBenchmarkResults(CalcPiFitBenchmarkResults *pCalcPiFitBenchmarkResults);

	void setMainFrame(CalcPiMainFrame *pCalcPiMainFrame);

	// From ProgressIndicatorInterface
	virtual void updateProgress(int progress);

protected:
	virtual wxThread::ExitCode Entry();

	void runExperiment(long long int digits);

private:
	CalculationInterface *pCalculationInterface_;
	std::vector<double> experimentDigits_, experimentTimes_, experimentMemory_;
	CalcPiMainFrame *pCalcPiMainFrame_;

	// Configuration parameters
	static const int runtimeMS_;
	static const int startDigits_;
	static const int increaseDigits_;
};

#endif
