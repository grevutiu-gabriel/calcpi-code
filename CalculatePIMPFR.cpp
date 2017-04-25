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
#include "CalculatePIMPFR.h"
#include "ProgressIndicatorInterface.h"

const wxString CalculatePIMPFR::algoName_ = wxT("PI: MPFR");
const wxString CalculatePIMPFR::algoDescr_ = wxT("Calculates PI using the routine mpfr_const_pi of the library MPFR.");
const wxString CalculatePIMPFR::copyrightText_ = wxT("Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Free Software Foundation, Inc.\n") \
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
const std::string CalculatePIMPFR::maxNumberOfDigits_ = "646456992";

CalculatePIMPFR::CalculatePIMPFR() :
	pProgressIndicatorInterface_(NULL)
{
}

CalculatePIMPFR::~CalculatePIMPFR()
{
}
	
bool CalculatePIMPFR::isMulticoreCapable()
{
	return false;
}

bool CalculatePIMPFR::enableProgressBar()
{
	return false;
}

const std::string &CalculatePIMPFR::getMaxNumberOfDigits()
{
	return maxNumberOfDigits_;
}

const wxString &CalculatePIMPFR::getAlgorithmName()
{
	return algoName_;
}

const wxString &CalculatePIMPFR::getAlgorithmDescription()
{
	return algoDescr_;
}

const wxString &CalculatePIMPFR::getCopyrightText()
{
	return copyrightText_;
}

void CalculatePIMPFR::setDigits(const wxString &digits)
{
	digits_ = digits;
}

void CalculatePIMPFR::setCores(int c)
{
	// Don't do anything: This Calculator is not multithreaded
}

void CalculatePIMPFR::calculate(ProgressIndicatorInterface *pProgressIndicatorInterface)
{
	long long int d;
	digits_.ToLongLong(&d);
	long long int d_bits = 0;	// The number of bits required for d digits
	double digits_bits_factor = std::log( 10.0 ) / std::log( 2.0 );	// This is log2(10), approx. 3.32
	d_bits = static_cast<long long int>(std::ceil( digits_bits_factor * static_cast<double>(d) ));
	mpfr_prec_t d_bits_t = static_cast<mpfr_prec_t>(d_bits);
	mpfr_t pi;
	mpfr_init2(pi, d_bits_t);
	mpfr_const_pi(pi, MPFR_RNDN);

//	boost::chrono::high_resolution_clock::time_point t1 = boost::chrono::high_resolution_clock::now();

	result_.resize(d+3);
	//mpfr_snprintf(&(result_[0]), d+2, "%.*Ff", d+2, pi);
	mpfr_exp_t exponent = 0;
	mpfr_get_str(&(result_[1]), &exponent, 10, d+1, pi, MPFR_RNDN);

	for(mpfr_exp_t i = 0; i < exponent; i++)
		result_[i] = result_[i+1];
	result_[exponent] = '.';
	size_t length1 = result_.size();
	size_t length2 = strlen(&(result_[0]));
	result_.resize(d+2);

/*
	boost::chrono::duration<double, boost::milli> diff = boost::chrono::high_resolution_clock::now() - t1;
	char buf[1024];
	sprintf(buf, "Time used for mpfr_get_str: %f ms\n", diff);
	OutputDebugStringA(buf);


	t1 = boost::chrono::high_resolution_clock::now();
	mpfr_snprintf(&(result_[0]), d+2, "%.*RNf", static_cast<int>(d+2), pi);
/*	diff = boost::chrono::high_resolution_clock::now() - t1;
	sprintf(buf, "Time used for mpfr_snprintf: %f ms\n", diff);
	OutputDebugStringA(buf);*/
/*
	t1 = boost::chrono::high_resolution_clock::now();

	boost::multiprecision::mpfr_float pi_boost(pi);
//	pi_boost.precision(d);
//	mpfr_const_pi(pi_boost.backend().data(), MPFR_RNDN);
	std::ostringstream ostr;
	ostr << std::setprecision(d);
	ostr << pi_boost;
	std::string pi_boost_str = ostr.str();

	diff = boost::chrono::high_resolution_clock::now() - t1;
	sprintf(buf, "Time used for boost::multiprecision: %f ms\n", diff);
	OutputDebugStringA(buf);
*/

	mpfr_clear(pi);
	mpfr_free_cache();
}
