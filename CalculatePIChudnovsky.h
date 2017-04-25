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

#ifndef CALCULATEPICHUDNOVSKY_H
#define CALCULATEPICHUDNOVSKY_H

class ProgressIndicatorInterface;

#include "CalculationInterface.h"

class CalculatePIChudnovsky: public CalculationInterface
{
public:
	CalculatePIChudnovsky();
	virtual ~CalculatePIChudnovsky();
	
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

	typedef struct {
		unsigned long max_facs;
		unsigned long num_facs;
		unsigned long *fac;
		unsigned long *pow;
	} fac_t[1];

	typedef struct {
		long int fac;
		long int pow;
		long int nxt;
	} sieve_t;

protected:


	void my_sqrt_ui(mpf_t r, unsigned long x);
	void my_div(mpf_t r, mpf_t y, mpf_t x);
	void fac_show(CalculatePIChudnovsky::fac_t f);
	void bs_mul(mpz_t r, long int a, long int b);
	void fac_remove_gcd(mpz_t p, fac_t fp, mpz_t g, fac_t fg);
	void bs(unsigned long a, unsigned long b, unsigned gflag, long int level);

	void updateProgressBS(int progress);
	void updateProgress(int progress);

private:
	wxString digits_;
	std::string result_;

	mpz_t    gcd, mgcd;
	mpf_t t1, t2;

	sieve_t *sieve;
	unsigned long int sieve_size;

	int out;
	mpz_t *pstack, *qstack, *gstack;
	fac_t *fpstack, *fgstack;
	long int top;
	double progress, percent;
	long gcd_time;

	ProgressIndicatorInterface *pProgressIndicatorInterface_;
	static const double progressBSFactor_;
	static const int progressDiv_, progressMul_;
	static const wxString algoName_, algoDescr_, copyrightText_;
	static const std::string maxNumberOfDigits_;
};

#endif
