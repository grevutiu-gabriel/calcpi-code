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
#include "CalcPiVirtualListControl.h"

CalcPiVirtualListControl::CalcPiVirtualListControl(wxWindow* parent)
	: wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_VIRTUAL | wxLC_NO_HEADER | wxLC_SINGLE_SEL)
{
	// Set font
	wxFont myFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE,
		wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString );

	if(myFont.IsFixedWidth())
	{
	}
	this->SetFont( myFont );
}

CalcPiVirtualListControl::~CalcPiVirtualListControl()
{
}

void CalcPiVirtualListControl::setResult(const std::string *result)
{
	const int arbitraryIncrement = 20;	// Don't know why this is necessary
	result_ = result;

	this->ClearAll();
	wxString str;
	str.Append(L'O', 75);	// 50 digits plus 4 spaces plus 21 digits for position
	wxSize colSize = this->GetTextExtent(str);
	this->AppendColumn(wxT(""), wxLIST_FORMAT_LEFT);
	this->SetColumnWidth(0, colSize.GetWidth() + arbitraryIncrement);

	size_t numberOfDigits = result->length();
	long int itemCount = static_cast<long int>((numberOfDigits - 2L) / 50L);
	this->SetItemCount(itemCount);

}

wxString CalcPiVirtualListControl::OnGetItemText(long item, long column) const
{
	if(column != 0)
		return "";
	long long item_times_50 = static_cast<long long>(item) * 50LL;
	wxString itemString = wxString(result_->substr(static_cast<size_t>(2LL + item_times_50), 50));
	for(int i = 0; i < 5; i++)
	{
		itemString.insert((4 - i)*10, 1, L' ');
	}
	itemString.Append( wxString::Format(wxT(" : % 18lld"), item_times_50) );
	return itemString;
}
