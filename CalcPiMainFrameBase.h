///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CalcPiMainFrameBase__
#define __CalcPiMainFrameBase__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/gauge.h>
#include "CalcPiVirtualListControl.h"
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CalcPiMainFrameBase
///////////////////////////////////////////////////////////////////////////////
class CalcPiMainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxPanel* pMainFramePanel_;
		wxStaticBoxSizer* pConfigurationSizer_;
		wxStaticText* m_staticText1;
		wxChoice* pAlgorithmChoice_;
		
		wxStaticText* m_staticText2;
		wxTextCtrl* pNumberOfDigitsTextCtrl_;
		
		wxStaticText* pNumberOfCoresStaticText_;
		wxTextCtrl* pNumberOfCoresTextCtrl_;
		
		wxButton* pStartCalculationButton_;
		wxStaticText* m_staticText5;
		wxTextCtrl* pAlgorithmDescriptionTextCtrl_;
		wxButton* pShowCopyrightButton_;
		wxStaticBoxSizer* pPerformanceSizer_;
		wxStaticText* m_staticText4;
		wxTextCtrl* pElapsedTextCtrl_;
		wxStaticText* m_staticText6;
		wxTextCtrl* pCPUTimeTextCtrl_;
		wxStaticText* m_staticText7;
		wxTextCtrl* pMemoryUsedTextCtrl_;
		wxStaticText* m_staticText8;
		wxTextCtrl* pEstdTimeUsedTextCtrl_;
		wxStaticText* m_staticText9;
		wxTextCtrl* pEstdMemUsedTextCtrl_;
		
		wxButton* pBenchmarkButton_;
		wxGauge* pProgressGauge_;
		wxTextCtrl* pResultTextCtrl_;
		CalcPiVirtualListControl *pResultListCtrl_;
		wxStatusBar* pMainFrameStatusBar_;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMainFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMainFrameIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnFileSaveResultMenuItem( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateUIFileSaveResultMenuItem( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnExitMenuItem( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutMenuItem( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateUIAboutMenuItem( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnAlgorithmChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNumberOfDigitsText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNumberOfCoresText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStartCalculationButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowCopyrightButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBenchmarkButton( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString numberOfDigits_; 
		wxString numberOfCores_; 
		
		CalcPiMainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Calc Pi"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 825,569 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~CalcPiMainFrameBase();
	
};

#endif //__CalcPiMainFrameBase__
