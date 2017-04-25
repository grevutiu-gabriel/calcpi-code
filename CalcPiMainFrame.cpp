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

#include "CalcPiMainFrame.h"
#include "CalcPiWorkerThread.h"
#include "CalcPiBenchmarkThread.h"
#include "version.h"
#include "cp_config.h"
#include "CalculatorFactory.h"
#include "CalculationInterface.h"
#include "CalcPiFitBenchmarkResults.h"

enum
{
	ID_TIMER = 2000
};

CalcPiMainFrame::CalcPiMainFrame(wxWindow* parent)
	: CalcPiMainFrameBase(parent), isCalcRunning_(false), isBenchmarkRunning_(false),
	isFirstTimeIdle_(true), aTimer_(this, ID_TIMER),
	progress_(0), noProgressAvailable_(false), memoryAboveLimit_(false),
	origTCBackgroundColor_(), pCalculator_(NULL),
	pCalcPiWorkerThread_(NULL), pCalcPiBenchmarkThread_(NULL),
	pCalcPiFitBenchmarkResults_(NULL), mutex_(), maxMemoryUsed_(0)
{
	pCalcPiFitBenchmarkResults_ = new CalcPiFitBenchmarkResults();

	pAlgorithmChoice_->Clear();
	for(int i = 0; i < CalculatorFactory::getInstance().getCalculatorCount(); i++)
	{
		CalculationInterface *pCalculator = CalculatorFactory::getInstance().createCalculator(i);
		pAlgorithmChoice_->Append(pCalculator->getAlgorithmName());
		delete pCalculator;
	}

	// Workaround: It is apparently not possible to set wxFILTER_DIGITS in wxFormBuilder
	wxValidator *pValidator = pNumberOfDigitsTextCtrl_->GetValidator();
	if(pValidator != NULL)
	{
		wxTextValidator *pTextValidator = dynamic_cast<wxTextValidator*>(pValidator);
		if(pTextValidator != NULL)
			pTextValidator->SetStyle(wxFILTER_DIGITS);
	}
	pValidator = pNumberOfCoresTextCtrl_->GetValidator();
	if(pValidator != NULL)
	{
		wxTextValidator *pTextValidator = dynamic_cast<wxTextValidator*>(pValidator);
		if(pTextValidator != NULL)
			pTextValidator->SetStyle(wxFILTER_DIGITS);
	}

	numberOfDigits_ = wxT("1000");
	numberOfCores_ = wxT("1");
	InitDialog();	// Initialize validators

	pProgressGauge_->Enable(false);

	pResultListCtrl_->ClearAll();
	pResultListCtrl_->SetItemCount(0);
	SetIcon(getIcon());

	pCalculator_ = CalculatorFactory::getInstance().createCalculator(0);
	pAlgorithmChoice_->Select(0);
	selectAlgorithm(0);

	aTimer_.Start(50);
}

CalcPiMainFrame::~CalcPiMainFrame()
{
	aTimer_.Stop();
	if(pCalculator_ != NULL)
	{
		delete pCalculator_;
		pCalculator_ = NULL;
	}
	if(pCalcPiFitBenchmarkResults_ != NULL)
	{
		delete pCalcPiFitBenchmarkResults_;
		pCalcPiFitBenchmarkResults_ = NULL;
	}
}

// The following 2 methods get called by CalcPiBenchmarkThread
void CalcPiMainFrame::clearMaxMemoryUsed()
{
	wxMutexLocker lock(mutex_);
	maxMemoryUsed_ = 0;
}

size_t CalcPiMainFrame::getMaxMemoryUsed()
{
	wxMutexLocker lock(mutex_);
	return maxMemoryUsed_;
}

void CalcPiMainFrame::OnMainFrameClose( wxCloseEvent& event )
{
	aTimer_.Stop();
	Destroy();
}

void CalcPiMainFrame::OnFileSaveResultMenuItem( wxCommandEvent& event )
{
	wxFileDialog fileDialog(this, wxT("Save result in text file"),
		wxEmptyString, wxEmptyString, wxT("Text files (*.txt)|*.txt|All files (*.*)|*.*"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	int returnValue = fileDialog.ShowModal();

	if(returnValue == wxID_OK)
	{
		wxFileOutputStream fos(fileDialog.GetPath());
		if(fos.IsOk())
		{
			fos.Write(result_.c_str(), result_.length());
		}
	}
}

void CalcPiMainFrame::OnUpdateUIFileSaveResultMenuItem( wxUpdateUIEvent& event )
{
	event.Enable((!isCalcRunning_) && (result_.length() > 0));
}

void CalcPiMainFrame::OnExitMenuItem( wxCommandEvent& event )
{
	Close();
}

void CalcPiMainFrame::OnAboutMenuItem( wxCommandEvent& event )
{
	wxAboutDialogInfo info;
	info.SetName(wxT(CALC_PI_NAME) wxT(" ") wxT(CALC_PI_ARCHITECTURE_STRING));
	info.SetVersion(CALC_PI_VERSION_STRING);
	info.SetIcon(getIcon());

	wxString descrString;
	descrString = wxT("This program calculates pi and other constants.\n");
	descrString.Append(wxT("Built with: ") wxT(CP_STRINGIFY(CP_COMPILER_NAME)) wxT(" ") wxT(CP_STRINGIFY(CP_COMPILER_VERSION)) wxT("\n"));
	descrString.Append(wxT("Linked with:\n - ") wxVERSION_STRING wxT("\n"));
#if defined(CP_HAVE_MPIR)
	descrString.Append(wxT(" - MPIR "));
	descrString.Append(mpir_version);
#else
	descrString.Append(wxT(" - GMP "));
	descrString.Append(gmp_version);
#endif
#if defined(CP_HAVE_BOOST)
	wxString boostVersion;
	boostVersion.Printf("%d.%d.%d", BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
	descrString.Append(wxT("\n - boost "));
	descrString.Append(boostVersion);
#endif
	descrString.Append(wxT("\nIcon (C) by Oxygen"));

	info.SetDescription(descrString);
	info.SetCopyright(wxT("(C) ") wxT(CALC_PI_COPYRIGHT_YEAR) wxT(" ") wxT(CALC_PI_COPYRIGHT_NAME));

	wxAboutBox(info);
}

void CalcPiMainFrame::OnUpdateUIAboutMenuItem( wxUpdateUIEvent& event )
{
	// Disable the menu if a calculation is running
	event.Enable(!isCalcRunning_ && !isBenchmarkRunning_);
}

void CalcPiMainFrame::OnMainFrameIdle( wxIdleEvent& event )
{
	if(isFirstTimeIdle_)
	{
		isFirstTimeIdle_ = false;

#if defined(CP_WIN32)
		origTCBackgroundColor_ = wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK);
#else
		origTCBackgroundColor_ = pEstdMemUsedTextCtrl_->GetBackgroundColour();
#endif
	}
}

void CalcPiMainFrame::OnTimer( wxTimerEvent &event )
{
	if(isBenchmarkRunning_ && pCalcPiBenchmarkThread_ != NULL)
	{
		pProgressGauge_->Pulse();

		size_t processMemory = getCurrentRSS();
		{
			wxMutexLocker lock(mutex_);
			maxMemoryUsed_ = std::max(maxMemoryUsed_, processMemory);
		}

		if(!pCalcPiBenchmarkThread_->IsRunning())
		{
			pCalcPiBenchmarkThread_->transferBenchmarkResults(pCalcPiFitBenchmarkResults_);
			cleanupBenchmarkThread();

			pProgressGauge_->SetValue(0);

			pAlgorithmChoice_->Enable();
			pNumberOfDigitsTextCtrl_->Enable();
			if(pCalculator_->isMulticoreCapable())
				pNumberOfCoresTextCtrl_->Enable();
			else
				pNumberOfCoresTextCtrl_->Disable();
			pStartCalculationButton_->Enable();
			pShowCopyrightButton_->Enable();
			pBenchmarkButton_->Enable();

			updateEstimates();
		}
	}
	else if(isCalcRunning_ && pCalcPiWorkerThread_ != NULL)
	{
		int progress = pCalcPiWorkerThread_->getProgress();
		if(progress != progress_)
		{
			progress_ = progress;
			pProgressGauge_->SetValue(progress);
		}
		if(noProgressAvailable_)
		{
			pProgressGauge_->Pulse();
		}

		size_t processMemory = getCurrentRSS();
		maxMemoryUsed_ = std::max(maxMemoryUsed_, processMemory);
		//extern "C" size_t getPeakRSS();

		if(!pCalcPiWorkerThread_->IsRunning())
		{
			wxString timeElapsed = wxString::Format(wxT("%f s"), pCalcPiWorkerThread_->getDuration());
			pElapsedTextCtrl_->SetValue(timeElapsed);
			wxULongLong memUsedUL = maxMemoryUsed_;
			wxString maxMemUsedStr = wxFileName::GetHumanReadableSize(memUsedUL);
			pMemoryUsedTextCtrl_->SetValue(maxMemUsedStr);

			// CPU user time not supported on all platforms
			if(pCalcPiWorkerThread_->getCPUUserTime() > 0)
			{
				wxString cpuUserTimeStr = wxString::Format(wxT("%f s"), pCalcPiWorkerThread_->getCPUUserTime());
				pCPUTimeTextCtrl_->SetValue(cpuUserTimeStr);
			}
			else
			{
				pCPUTimeTextCtrl_->Clear();
			}

			cleanupWorkerThread();

			if(noProgressAvailable_)
				pProgressGauge_->SetValue(0);
			else
				pProgressGauge_->SetValue(100);

			result_ = pCalculator_->getResult();
			pResultListCtrl_->setResult(&result_);
			pResultTextCtrl_->SetValue( wxString(result_.substr(0, 2)) );
			pAlgorithmChoice_->Enable();
			pNumberOfDigitsTextCtrl_->Enable();
			if(pCalculator_->isMulticoreCapable())
				pNumberOfCoresTextCtrl_->Enable();
			else
				pNumberOfCoresTextCtrl_->Disable();
			pStartCalculationButton_->Enable();
			pShowCopyrightButton_->Enable();
			pBenchmarkButton_->Enable();
		}
	}
}


wxString CalcPiMainFrame::MPFToString(const mpf_class &p, int nrOfDigits)
{
	char buf[1024];
	gmp_snprintf(buf, 1024, "%.*Ff", nrOfDigits, p.get_mpf_t());
	return wxString(buf);
}

void CalcPiMainFrame::OnAlgorithmChoice( wxCommandEvent& event )
{
	selectAlgorithm(pAlgorithmChoice_->GetSelection());
	pCalcPiFitBenchmarkResults_->invalidateResults();
	updateEstimates();
}

void CalcPiMainFrame::OnNumberOfDigitsText( wxCommandEvent& event )
{
	updateEstimates();
}

void CalcPiMainFrame::OnNumberOfCoresText( wxCommandEvent& event )
{
	pCalcPiFitBenchmarkResults_->invalidateResults();
	updateEstimates();
}

void CalcPiMainFrame::OnShowCopyrightButton( wxCommandEvent& event )
{
	if(pCalculator_ != NULL)
	{
		wxMessageBox(pCalculator_->getCopyrightText(), wxT("Algorithm copyright/license"),
			wxOK | wxICON_INFORMATION, this);
	}
}

void CalcPiMainFrame::OnStartCalculationButton( wxCommandEvent& event )
{
	if(isCalcRunning_)
	{
		// Shouldn't happen, as button is disabled
	}
	else
	{
		// Update validators
		bool isOK = Validate();
		bool isOK2 = TransferDataFromWindow();
		if(isOK && isOK2)
		{
			// Check for memory above limit
			if(memoryAboveLimit_)
			{
				int retVal = wxMessageBox(wxT("The estimated memory consumption of the algorithm exceeds the physical memory. ") \
					wxT("This means that the algorithm will run a lot slower than estimated as the memory will ") \
					wxT("be swapped out onto disk. The program might even crash if the virtual memory is exhausted.\n") \
					wxT("Are you sure you want to continue?"),
					wxT("Physical memory exceeded"),
					wxYES_NO | wxICON_QUESTION);
				if(retVal != wxYES)
					return;
			}

			// Check for number of digits
			mpz_class digitsUser, digitsAlgorithm;
			digitsUser.set_str( numberOfDigits_.ToStdString().c_str(), 10);
			digitsAlgorithm.set_str( pCalculator_->getMaxNumberOfDigits().c_str(), 10);
			if(cmp( digitsUser, digitsAlgorithm) > 0)
			{
				wxString digitsText;
				digitsText = wxT("Number of digits too high for selected algorithm.\n");
				digitsText.Append(wxT("The algorithm \""));
				digitsText.Append(pCalculator_->getAlgorithmName());
				digitsText.Append(wxT("\" supports up to "));
				digitsText.Append(pCalculator_->getMaxNumberOfDigits().c_str());
				digitsText.Append(wxT(" digits."));
				wxMessageBox(digitsText, wxT("Limit exceeded"),
					wxOK | wxICON_ERROR);
				return;
			}
			pCalculator_->setDigits(numberOfDigits_);
			long numberOfCoresI = 1;
			numberOfCores_.ToLong( &numberOfCoresI );
			pCalculator_->setCores( numberOfCoresI );
			noProgressAvailable_ = !(pCalculator_->enableProgressBar());
			pCalcPiWorkerThread_ = new CalcPiWorkerThread(pCalculator_);
			pCalcPiWorkerThread_->Create();
			pCalcPiWorkerThread_->Run();
			isCalcRunning_ = true;
			pResultListCtrl_->ClearAll();
			maxMemoryUsed_ = 0;

			// Disable GUI elements
			pAlgorithmChoice_->Disable();
			pNumberOfDigitsTextCtrl_->Disable();
			pNumberOfCoresTextCtrl_->Disable();
			pStartCalculationButton_->Disable();
			pShowCopyrightButton_->Disable();
			pBenchmarkButton_->Disable();
		}
	}
}

void CalcPiMainFrame::OnBenchmarkButton( wxCommandEvent& event )
{
	if(isBenchmarkRunning_)
	{
		// Shouldn't happen, as button is disabled
	}
	else
	{
		long numberOfCoresI = 1;
		numberOfCores_.ToLong( &numberOfCoresI );
		pCalculator_->setCores( numberOfCoresI );
		noProgressAvailable_ = !(pCalculator_->enableProgressBar());
		pCalcPiBenchmarkThread_ = new CalcPiBenchmarkThread(pCalculator_);
		pCalcPiBenchmarkThread_->setMainFrame(this);
		pCalcPiBenchmarkThread_->Create();
		pCalcPiBenchmarkThread_->Run();
		isBenchmarkRunning_ = true;
		pResultListCtrl_->ClearAll();
		maxMemoryUsed_ = 0;

		// Disable GUI elements
		pAlgorithmChoice_->Disable();
		pNumberOfDigitsTextCtrl_->Disable();
		pNumberOfCoresTextCtrl_->Disable();
		pStartCalculationButton_->Disable();
		pShowCopyrightButton_->Disable();
		pBenchmarkButton_->Disable();
	}
}

void CalcPiMainFrame::cleanupWorkerThread()
{
	if(pCalcPiWorkerThread_ != NULL)
	{
		pCalcPiWorkerThread_->Wait();
		delete pCalcPiWorkerThread_;
		pCalcPiWorkerThread_ = NULL;
		isCalcRunning_ = false;
	}
}

void CalcPiMainFrame::cleanupBenchmarkThread()
{
	if(pCalcPiBenchmarkThread_ != NULL)
	{
		pCalcPiBenchmarkThread_->Wait();
		delete pCalcPiBenchmarkThread_;
		pCalcPiBenchmarkThread_ = NULL;
		isBenchmarkRunning_ = false;
	}
}

void CalcPiMainFrame::selectAlgorithm(int i)
{
	if(pCalculator_ != NULL)
	{
		delete pCalculator_;
		pCalculator_ = NULL;
	}

	pCalculator_ = CalculatorFactory::getInstance().createCalculator(i);
	if(pCalculator_ == NULL)
	{
		// Shouldn't happen...
		pAlgorithmDescriptionTextCtrl_->Clear();
		pStartCalculationButton_->Disable();
		return;
	}

	TransferDataFromWindow();
	pStartCalculationButton_->Enable();
	pAlgorithmDescriptionTextCtrl_->SetValue(pCalculator_->getAlgorithmDescription());
	if(pCalculator_->isMulticoreCapable())
	{
		pNumberOfCoresTextCtrl_->Enable();
		numberOfCores_ = wxString::Format(wxT("%d"), wxThread::GetCPUCount());
	}
	else
	{
		pNumberOfCoresTextCtrl_->Disable();
		numberOfCores_ = wxT("1");
	}
	TransferDataToWindow();
}

void CalcPiMainFrame::updateEstimates()
{
	if(pCalcPiFitBenchmarkResults_->getResultsValid())
	{
		double digits = 0;
		TransferDataFromWindow();
		numberOfDigits_.ToDouble(&digits);
		double estdTime = pCalcPiFitBenchmarkResults_->estimateTime(digits);
		double estdMemory = pCalcPiFitBenchmarkResults_->estimateMemory(digits);

		wxString estdTimeString;	//wxString::Format( wxT("%.3f s"), estdTime ) );
		std::ostringstream ostr;
		ostr << std::setprecision(2) << std::fixed << estdTime << " s";
		estdTimeString = ostr.str();
		wxTimeSpan estdTS(0, 0, static_cast<wxLongLong>(estdTime), 0 );
		if(estdTS.GetWeeks() > 1)
			estdTimeString = estdTS.Format(wxT("%E weeks"));
		else if(estdTS.GetDays() > 1)
			estdTimeString = estdTS.Format(wxT("%D days"));
		else if(estdTS.GetHours() > 1)
			estdTimeString = estdTS.Format(wxT("%H hours"));
		else if(estdTS.GetMinutes() > 1)
			estdTimeString = estdTS.Format(wxT("%M minutes"));

		pEstdTimeUsedTextCtrl_->SetValue( estdTimeString );

		size_t totalMemory = getMemorySize();
		wxULongLong estdMemoryUL = static_cast<wxULongLong>(estdMemory);
		wxString estdMemoryStr = wxFileName::GetHumanReadableSize(estdMemoryUL);

		memoryAboveLimit_ = (static_cast<double>(totalMemory) < estdMemory);
		if(memoryAboveLimit_)
			pEstdMemUsedTextCtrl_->SetBackgroundColour(*wxRED);
		else
			pEstdMemUsedTextCtrl_->SetBackgroundColour(origTCBackgroundColor_);

		pEstdMemUsedTextCtrl_->SetValue(estdMemoryStr);
	}
	else
	{
		wxString notAvailableText = wxT("N/A (run benchmark)");
		pEstdTimeUsedTextCtrl_->SetValue(notAvailableText);
		pEstdMemUsedTextCtrl_->SetValue(notAvailableText);
		pEstdMemUsedTextCtrl_->SetBackgroundColour(origTCBackgroundColor_);
		memoryAboveLimit_ = false;
	}
}

// Icon: From resource on Win32, from PNG otherwise
#if !defined(CP_WIN32)
#include "res/png/calc_pi_png.h"
#endif

wxIcon CalcPiMainFrame::getIcon()
{
#if defined(CP_WIN32)
	wxIcon icon(wxT("aaaaaaaa"));
#else
	wxMemoryInputStream istream(calc_pi_png, sizeof(calc_pi_png));
	wxImage myimage_img(istream, wxBITMAP_TYPE_PNG);
	wxBitmap myimage_bmp(myimage_img);
	wxIcon icon;
	icon.CopyFromBitmap(myimage_bmp);
#endif

	return icon;
}

BEGIN_EVENT_TABLE( CalcPiMainFrame, CalcPiMainFrameBase )
	EVT_TIMER(ID_TIMER, CalcPiMainFrame::OnTimer)
END_EVENT_TABLE()
