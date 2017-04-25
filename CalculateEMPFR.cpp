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
#include "CalculateEMPFR.h"
#include "ProgressIndicatorInterface.h"

const wxString CalculateEMPFR::algoName_ = wxT("E: MPFR");
const wxString CalculateEMPFR::algoDescr_ = wxT("Calculates e (base of the natural logarithm) using the routine mpfr_exp of the library MPFR.");
const wxString CalculateEMPFR::copyrightText_ = wxT("Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Free Software Foundation, Inc.\n") \
	wxT("Contributed by the AriC and Caramel projects, INRIA.\n") \
	wxT("\n") \
	wxT("This file is part of the GNU MPFR Library.\n") \
	wxT("\n") \
	wxT("The GNU MPFR Library is free software; you can redistribute it and/or modify\n") \
	wxT("it under the terms of the GNU Lesser General Public License as published by\n") \
	wxT("the Free Software Foundation; either version 3 of the License, or (at your\n") \
	wxT("option) any later version.\n") \
	wxT("\n") \
	wxT("The GNU MPFR Library is distributed in the hope that it will be useful, but\n") \
	wxT("WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY\n") \
	wxT("or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public\n") \
	wxT("License for more details.\n") \
	wxT("\n") \
	wxT("You should have received a copy of the GNU Lesser General Public License\n") \
	wxT("along with the GNU MPFR Library; see the file COPYING.LESSER.  If not, see\n") \
	wxT("http://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,\n") \
	wxT("51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.");
// The maximum exponent is 2^31, the number of digits is 646456992
const std::string CalculateEMPFR::maxNumberOfDigits_ = "646456992";

CalculateEMPFR::CalculateEMPFR() :
	pProgressIndicatorInterface_(NULL)
{
}

CalculateEMPFR::~CalculateEMPFR()
{
}
	
bool CalculateEMPFR::isMulticoreCapable()
{
	return false;
}

bool CalculateEMPFR::enableProgressBar()
{
	return false;
}

const std::string &CalculateEMPFR::getMaxNumberOfDigits()
{
	return maxNumberOfDigits_;
}

const wxString &CalculateEMPFR::getAlgorithmName()
{
	return algoName_;
}

const wxString &CalculateEMPFR::getAlgorithmDescription()
{
	return algoDescr_;
}

const wxString &CalculateEMPFR::getCopyrightText()
{
	return copyrightText_;
}

void CalculateEMPFR::setDigits(const wxString &digits)
{
	digits_ = digits;
}

void CalculateEMPFR::setCores(int c)
{
	// Don't do anything: This Calculator is not multithreaded
}

void CalculateEMPFR::calculate(ProgressIndicatorInterface *pProgressIndicatorInterface)
{
	long long int d;
	digits_.ToLongLong(&d);
	long long int d_bits = 0;	// The number of bits required for d digits
	double digits_bits_factor = std::log( 10.0 ) / std::log( 2.0 );	// This is log2(10), approx. 3.32
	d_bits = static_cast<long long int>(std::ceil( digits_bits_factor * static_cast<double>(d) ));
	mpfr_t euler, one;
	mpfr_init2(euler, d_bits);
	mpfr_init2(one, 2);
	int ret = mpfr_set_str(one, "1", 10, MPFR_RNDN);

	ret = mpfr_exp(euler, one, MPFR_RNDN);

	//mpfr_const_euler(euler, MPFR_RNDN);

//	result_.resize(d+2);
//	mpfr_snprintf(&(result_[0]), d+2, "%.*RNf", static_cast<int>(d+2), euler);
	result_.resize(d+3);
	mpfr_exp_t exponent = 0;
	mpfr_get_str(&(result_[1]), &exponent, 10, d+1, euler, MPFR_RNDN);

	for(mpfr_exp_t i = 0; i < exponent; i++)
		result_[i] = result_[i+1];
	result_[exponent] = '.';
	size_t length1 = result_.size();
	size_t length2 = strlen(&(result_[0]));
	result_.resize(d+2);

	mpfr_clear(one);
	mpfr_clear(euler);
	mpfr_free_cache();
}
