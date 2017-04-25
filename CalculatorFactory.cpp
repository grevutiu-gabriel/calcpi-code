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
#include "CalculatorFactory.h"
#include "cp_config.h"

// The calculators
#include "CalculatePIChudnovskyOMP.h"
#include "CalculatePIChudnovsky.h"
#include "CalculatePIMPFR.h"
#include "CalculatePIBoost.h"
#include "CalculateEMPFR.h"

// The boost calculator is limited: The number of digits is fixed
#define DISABLE_BOOST_CALCULATOR 1

CalculatorFactory CalculatorFactory::instance_;

CalculatorFactory::CalculatorFactory()
{
}

CalculatorFactory::~CalculatorFactory()
{
}

int CalculatorFactory::getCalculatorCount()
{
#if defined(CP_HAVE_BOOST) && !defined(DISABLE_BOOST_CALCULATOR)
	return 5;
#else
	return 4;
#endif
}

CalculationInterface *CalculatorFactory::createCalculator(int i)
{
	CalculationInterface *pCalculator = NULL;

	if(i == 0)
	{
		// Chudnovsky OMP
		pCalculator = new CalculatePIChudnovskyOMP();
	}
	else if(i == 1)
	{
		// Chudnovsky
		pCalculator = new CalculatePIChudnovsky();
	}
	else if(i == 2)
	{
		// MPFR
		pCalculator = new CalculatePIMPFR();
	}
	else if(i == 3)
	{
		// boost::math::constants
		pCalculator = new CalculateEMPFR();
	}
#if defined(CP_HAVE_BOOST) && !defined(DISABLE_BOOST_CALCULATOR)
	else if(i == 4)
	{
		// boost::math::constants
		pCalculator = new CalculatePIBoost();
	}
#endif

	return pCalculator;
}

CalculatorFactory &CalculatorFactory::getInstance()
{
	return instance_;
}



