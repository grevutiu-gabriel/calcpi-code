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

#ifndef CALCULATEEMPFR_H
#define CALCULATEEMPFR_H

class ProgressIndicatorInterface;

#include "CalculationInterface.h"

class CalculateEMPFR: public CalculationInterface
{
public:
	CalculateEMPFR();
	virtual ~CalculateEMPFR();

	virtual bool isMulticoreCapable();
	virtual bool enableProgressBar();
	virtual const std::string &getMaxNumberOfDigits();
	virtual const wxString &getAlgorithmName();
	virtual const wxString &getAlgorithmDescription();
	virtual const wxString &getCopyrightText();
	virtual void setDigits(const wxString &digits);
	virtual void setCores(int c);
	virtual void calculate(ProgressIndicatorInterface *pProgressIndicatorInterface);
	virtual const std::string &getResult() { return result_; }

private:
	wxString digits_;
	std::string result_;
	ProgressIndicatorInterface *pProgressIndicatorInterface_;
	static const wxString algoName_, algoDescr_, copyrightText_;
	static const std::string maxNumberOfDigits_;
};

#endif
