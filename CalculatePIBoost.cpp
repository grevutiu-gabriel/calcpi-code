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
#include "CalculatePIBoost.h"
#include "ProgressIndicatorInterface.h"

//#include <boost/math/constants/info.hpp>
#include <boost/math/constants/constants.hpp>
//#include <boost/multiprecision/detail/functions/trig.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

const wxString CalculatePIBoost::algoName_ = wxT("PI: boost");
const wxString CalculatePIBoost::algoDescr_ = wxT("Calculates PI using the boost::multiprecision library.");
const wxString CalculatePIBoost::copyrightText_ = wxT("Boost Software License - Version 1.0 - August 17th, 2003\n") \
	wxT("\n") \
	wxT("Permission is hereby granted, free of charge, to any person or organization\n") \
	wxT("obtaining a copy of the software and accompanying documentation covered by\n") \
	wxT("this license (the \"Software\") to use, reproduce, display, distribute,\n") \
	wxT("execute, and transmit the Software, and to prepare derivative works of the\n") \
	wxT("Software, and to permit third-parties to whom the Software is furnished to\n") \
	wxT("do so, all subject to the following:\n") \
	wxT("\n") \
	wxT("The copyright notices in the Software and this entire statement, including\n") \
	wxT("the above license grant, this restriction and the following disclaimer,\n") \
	wxT("must be included in all copies of the Software, in whole or in part, and\n") \
	wxT("all derivative works of the Software, unless such copies or derivative\n") \
	wxT("works are solely in the form of machine-executable object code generated by\n") \
	wxT("a source language processor.\n") \
	wxT("\n") \
	wxT("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n") \
	wxT("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n") \
	wxT("FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT\n") \
	wxT("SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE\n") \
	wxT("FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,\n") \
	wxT("ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\n") \
	wxT("DEALINGS IN THE SOFTWARE.");
const std::string CalculatePIBoost::maxNumberOfDigits_ = "1000000";

CalculatePIBoost::CalculatePIBoost() :
	pProgressIndicatorInterface_(NULL)
{
}

CalculatePIBoost::~CalculatePIBoost()
{
}
	
bool CalculatePIBoost::isMulticoreCapable()
{
	return false;
}

bool CalculatePIBoost::enableProgressBar()
{
	return false;
}

const std::string &CalculatePIBoost::getMaxNumberOfDigits()
{
	return maxNumberOfDigits_;
}

const wxString &CalculatePIBoost::getAlgorithmName()
{
	return algoName_;
}

const wxString &CalculatePIBoost::getAlgorithmDescription()
{
	return algoDescr_;
}

const wxString &CalculatePIBoost::getCopyrightText()
{
	return copyrightText_;
}

void CalculatePIBoost::setDigits(const wxString &digits)
{
	digits_ = digits;
}

void CalculatePIBoost::setCores(int c)
{
	// Don't do anything: This Calculator is not multithreaded
}

#define VERSION_1

void CalculatePIBoost::calculate(ProgressIndicatorInterface *pProgressIndicatorInterface)
{
	// The major drawback of boost::multiprecision::cpp_dec_float is that the number of digits needs
	// to be known at compile time
	const int digits_compile_time = 100000;
	long long int d;
	digits_.ToLongLong(&d);
	if(d > digits_compile_time)
		d = digits_compile_time;	// ... or else calc_pi does not terminate
	long long int d_bits = 0;	// The number of bits required for d digits
	double digits_bits_factor = std::log( 10.0 ) / std::log( 2.0 );	// This is log2(10), approx. 3.32
	d_bits = static_cast<long long int>(std::ceil( digits_bits_factor * static_cast<double>(d) ));

	//typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<digits_compile_time, boost::int32_t, std::allocator<unsigned int> > > calc_pi_mptype;
	//typedef boost::multiprecision::number<boost::multiprecision::backends::mpfr_float_backend<digits_compile_time> > calc_pi_mptype;
	typedef boost::multiprecision::number<boost::multiprecision::backends::gmp_float<digits_compile_time> > calc_pi_mptype;
	calc_pi_mptype pi_boost;

#if defined(VERSION_1)
	// Version 1 uses boost::math::constants
//	boost::math::constants::print_info_on_type<mptype>();
	pi_boost = boost::math::constants::pi<calc_pi_mptype>();
#else
	// Version 2 uses an internal and undocumented method of boost::multiprecision which may or may not be present in the future
	boost::multiprecision::default_ops::calc_pi(pi_boost.backend(), d_bits);
#endif

	std::ostringstream ostr;
	ostr << std::setprecision(d+2);
	ostr << pi_boost;
	result_ = ostr.str();
}
