///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CommonIncludes.h"

#include "CalcPiMainFrameBase.h"

///////////////////////////////////////////////////////////////////////////

CalcPiMainFrameBase::CalcPiMainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem31;
	m_menuItem31 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Save result") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem31 );
	
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	m_menubar1->Append( m_menu1, wxT("File") ); 
	
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem3 );
	
	m_menubar1->Append( m_menu2, wxT("Help") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	pMainFramePanel_ = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pMainFramePanel_->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	pConfigurationSizer_ = new wxStaticBoxSizer( new wxStaticBox( pMainFramePanel_, wxID_ANY, wxT("Configuration") ), wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Algorithm:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer6->Add( m_staticText1, 0, wxALIGN_CENTER|wxALL, 3 );
	
	wxArrayString pAlgorithmChoice_Choices;
	pAlgorithmChoice_ = new wxChoice( pMainFramePanel_, wxID_ANY, wxDefaultPosition, wxDefaultSize, pAlgorithmChoice_Choices, 0 );
	pAlgorithmChoice_->SetSelection( 0 );
	bSizer6->Add( pAlgorithmChoice_, 0, wxALIGN_CENTER|wxALL, 3 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 3 );
	
	m_staticText2 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Number of digits:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer6->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 3 );
	
	pNumberOfDigitsTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	pNumberOfDigitsTextCtrl_->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	pNumberOfDigitsTextCtrl_->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &numberOfDigits_ ) );
	
	bSizer6->Add( pNumberOfDigitsTextCtrl_, 3, wxALIGN_CENTER|wxALL, 3 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	pNumberOfCoresStaticText_ = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Number of cores:"), wxDefaultPosition, wxDefaultSize, 0 );
	pNumberOfCoresStaticText_->Wrap( -1 );
	bSizer6->Add( pNumberOfCoresStaticText_, 0, wxALIGN_CENTER|wxALL, 3 );
	
	pNumberOfCoresTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	pNumberOfCoresTextCtrl_->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	pNumberOfCoresTextCtrl_->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &numberOfCores_ ) );
	
	bSizer6->Add( pNumberOfCoresTextCtrl_, 2, wxALIGN_CENTER|wxALL, 3 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	pStartCalculationButton_ = new wxButton( pMainFramePanel_, wxID_ANY, wxT("Go!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( pStartCalculationButton_, 0, wxALIGN_CENTER|wxALL, 3 );
	
	pConfigurationSizer_->Add( bSizer6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Algorithm description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer5->Add( m_staticText5, 0, wxALIGN_CENTER|wxALL, 3 );
	
	pAlgorithmDescriptionTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer5->Add( pAlgorithmDescriptionTextCtrl_, 1, wxALL|wxEXPAND, 3 );
	
	pShowCopyrightButton_ = new wxButton( pMainFramePanel_, wxID_ANY, wxT("Show Copyright/License"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( pShowCopyrightButton_, 0, wxALL, 3 );
	
	pConfigurationSizer_->Add( bSizer5, 1, wxEXPAND, 5 );
	
	bSizer4->Add( pConfigurationSizer_, 0, wxALL|wxEXPAND, 3 );
	
	pPerformanceSizer_ = new wxStaticBoxSizer( new wxStaticBox( pMainFramePanel_, wxID_ANY, wxT("Performance") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 6, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->AddGrowableCol( 3 );
	fgSizer1->AddGrowableCol( 5 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Elapsed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 3 );
	
	pElapsedTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( pElapsedTextCtrl_, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 3 );
	
	m_staticText6 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Accumulated CPU time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 3 );
	
	pCPUTimeTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( pCPUTimeTextCtrl_, 1, wxALL|wxEXPAND, 3 );
	
	m_staticText7 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Max. memory used:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 3 );
	
	pMemoryUsedTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( pMemoryUsedTextCtrl_, 1, wxALL|wxEXPAND, 3 );
	
	m_staticText8 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Estimated time used:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer1->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 3 );
	
	pEstdTimeUsedTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( pEstdTimeUsedTextCtrl_, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 3 );
	
	m_staticText9 = new wxStaticText( pMainFramePanel_, wxID_ANY, wxT("Estimated memory used:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 3 );
	
	pEstdMemUsedTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( pEstdMemUsedTextCtrl_, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 3 );
	
	pBenchmarkButton_ = new wxButton( pMainFramePanel_, wxID_ANY, wxT("Benchmark!"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( pBenchmarkButton_, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 3 );
	
	pPerformanceSizer_->Add( fgSizer1, 1, wxEXPAND, 3 );
	
	bSizer4->Add( pPerformanceSizer_, 0, wxALL|wxEXPAND, 3 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( pMainFramePanel_, wxID_ANY, wxT("Progress") ), wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	pProgressGauge_ = new wxGauge( pMainFramePanel_, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	pProgressGauge_->SetMinSize( wxSize( -1,30 ) );
	
	bSizer7->Add( pProgressGauge_, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 3 );
	
	sbSizer2->Add( bSizer7, 1, wxEXPAND, 5 );
	
	bSizer4->Add( sbSizer2, 0, wxALL|wxEXPAND, 3 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( pMainFramePanel_, wxID_ANY, wxT("Result") ), wxHORIZONTAL );
	
	pResultTextCtrl_ = new wxTextCtrl( pMainFramePanel_, wxID_ANY, wxT("3."), wxDefaultPosition, wxSize( 20,-1 ), wxTE_READONLY );
	sbSizer3->Add( pResultTextCtrl_, 0, wxALL, 3 );
	
	pResultListCtrl_ = new CalcPiVirtualListControl(pMainFramePanel_);
	sbSizer3->Add( pResultListCtrl_, 1, wxALL|wxEXPAND, 3 );
	
	bSizer4->Add( sbSizer3, 1, wxALL|wxEXPAND, 3 );
	
	pMainFramePanel_->SetSizer( bSizer4 );
	pMainFramePanel_->Layout();
	bSizer4->Fit( pMainFramePanel_ );
	bSizer1->Add( pMainFramePanel_, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	pMainFrameStatusBar_ = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CalcPiMainFrameBase::OnMainFrameClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( CalcPiMainFrameBase::OnMainFrameIdle ) );
	this->Connect( m_menuItem31->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnFileSaveResultMenuItem ) );
	this->Connect( m_menuItem31->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( CalcPiMainFrameBase::OnUpdateUIFileSaveResultMenuItem ) );
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnExitMenuItem ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnAboutMenuItem ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( CalcPiMainFrameBase::OnUpdateUIAboutMenuItem ) );
	pAlgorithmChoice_->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnAlgorithmChoice ), NULL, this );
	pNumberOfDigitsTextCtrl_->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CalcPiMainFrameBase::OnNumberOfDigitsText ), NULL, this );
	pNumberOfCoresTextCtrl_->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CalcPiMainFrameBase::OnNumberOfCoresText ), NULL, this );
	pStartCalculationButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalcPiMainFrameBase::OnStartCalculationButton ), NULL, this );
	pShowCopyrightButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalcPiMainFrameBase::OnShowCopyrightButton ), NULL, this );
	pBenchmarkButton_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalcPiMainFrameBase::OnBenchmarkButton ), NULL, this );
}

CalcPiMainFrameBase::~CalcPiMainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CalcPiMainFrameBase::OnMainFrameClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( CalcPiMainFrameBase::OnMainFrameIdle ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnFileSaveResultMenuItem ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( CalcPiMainFrameBase::OnUpdateUIFileSaveResultMenuItem ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnExitMenuItem ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnAboutMenuItem ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( CalcPiMainFrameBase::OnUpdateUIAboutMenuItem ) );
	pAlgorithmChoice_->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CalcPiMainFrameBase::OnAlgorithmChoice ), NULL, this );
	pNumberOfDigitsTextCtrl_->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CalcPiMainFrameBase::OnNumberOfDigitsText ), NULL, this );
	pNumberOfCoresTextCtrl_->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CalcPiMainFrameBase::OnNumberOfCoresText ), NULL, this );
	pStartCalculationButton_->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalcPiMainFrameBase::OnStartCalculationButton ), NULL, this );
	pShowCopyrightButton_->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalcPiMainFrameBase::OnShowCopyrightButton ), NULL, this );
	pBenchmarkButton_->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CalcPiMainFrameBase::OnBenchmarkButton ), NULL, this );
	
}
