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

#ifndef CALCPIMAINFRAME_H
#define CALCPIMAINFRAME_H

class CalcPiWorkerThread;
class CalcPiBenchmarkThread;
class CalculationInterface;
class CalcPiFitBenchmarkResults;

#include "CalcPiMainFrameBase.h"

class CalcPiMainFrame: public CalcPiMainFrameBase
{
public:
	CalcPiMainFrame(wxWindow* parent);
	virtual ~CalcPiMainFrame();

	static wxString MPFToString(const mpf_class &p, int nrOfDigits);
	void clearMaxMemoryUsed();
	size_t getMaxMemoryUsed();

protected:
	virtual void OnMainFrameClose( wxCloseEvent& event );
	virtual void OnFileSaveResultMenuItem( wxCommandEvent& event );
	virtual void OnUpdateUIFileSaveResultMenuItem( wxUpdateUIEvent& event );
	virtual void OnExitMenuItem( wxCommandEvent& event );
	virtual void OnAboutMenuItem( wxCommandEvent& event );
	virtual void OnUpdateUIAboutMenuItem( wxUpdateUIEvent& event );
	virtual void OnMainFrameIdle( wxIdleEvent& event );
	virtual void OnTimer( wxTimerEvent &event );
	virtual void OnAlgorithmChoice( wxCommandEvent& event );
	virtual void OnNumberOfDigitsText( wxCommandEvent& event );
	virtual void OnNumberOfCoresText( wxCommandEvent& event );
	virtual void OnStartCalculationButton( wxCommandEvent& event );
	virtual void OnShowCopyrightButton( wxCommandEvent& event );
	virtual void OnBenchmarkButton( wxCommandEvent& event );

	void cleanupWorkerThread();
	void cleanupBenchmarkThread();
	void selectAlgorithm(int i);
	void updateEstimates();

	wxIcon getIcon();

private:

	bool isFirstTimeIdle_;
	wxTimer aTimer_;

	bool isCalcRunning_, isBenchmarkRunning_;
	int progress_;
	bool noProgressAvailable_, memoryAboveLimit_;
	std::string result_;
	wxColor origTCBackgroundColor_;
	CalcPiWorkerThread *pCalcPiWorkerThread_;
	CalcPiBenchmarkThread *pCalcPiBenchmarkThread_;
	CalculationInterface *pCalculator_;
	CalcPiFitBenchmarkResults *pCalcPiFitBenchmarkResults_;

	wxMutex mutex_;
	size_t maxMemoryUsed_;

	DECLARE_EVENT_TABLE()
};

#endif
