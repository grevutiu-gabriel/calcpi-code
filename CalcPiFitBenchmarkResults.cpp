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
#include "CalcPiFitBenchmarkResults.h"

#if defined(CP_HAVE_EIGEN)
// Eigen
#	include <Eigen/Dense>
#	include <unsupported/Eigen/NonLinearOptimization>

/**
 * Functor that calculates the function we use to approximate the benchmark results
 *
 * Remark: Since this code is not speed-critical, it is not optimized at all.
 * Possible speedups:
 *  - Set ValuesAtCompileTime to 4 (or whatever the correct value for another function is)
 *  - Don't use the numerical differentiator and implement a proper df() method
 */
struct cpfb_functor
{
	typedef double Scalar;
	enum {
		InputsAtCompileTime = Eigen::Dynamic,
		ValuesAtCompileTime = Eigen::Dynamic
	};
	typedef Eigen::Matrix<Scalar,InputsAtCompileTime,1> InputType;
	typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
	typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

	cpfb_functor(int inputs, int values) :
		m_inputs(inputs), m_values(values)
	{}

	// Calculate the value of f(x) with the current solution of our parameters par
	double func(const InputType &par, double x) const
	{
		return par[0] + par[1]*x + par[2]*std::exp(par[3]*x);// + std::exp(par[4]*x*x);
	}

	int operator()(const InputType &x, ValueType &fvec) const
	{
		for (int i = 0; i < values(); i++)
		{
			double y_calc = func(x, xValues_[i]);
			fvec[i] = std::log(xValues_[i])*(yValues_[i] - y_calc);
		}
		return 0;
	}

	int inputs() const { return m_inputs; }
	int values() const { return m_values; }

	const int m_inputs, m_values;
	std::vector<double> xValues_, yValues_;
};

struct cpfb_functor_ln
{
	typedef double Scalar;
	enum {
		InputsAtCompileTime = Eigen::Dynamic,
		ValuesAtCompileTime = Eigen::Dynamic
	};
	typedef Eigen::Matrix<Scalar,InputsAtCompileTime,1> InputType;
	typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
	typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

	cpfb_functor_ln(int inputs, int values) :
		m_inputs(inputs), m_values(values)
	{}

	// Calculate the value of f(x) with the current solution of our parameters par
	double func(const InputType &par, double x) const
	{
		return par[0] + par[1]*x + par[2]*std::log((par[3]*par[3]+1.e-12)*x);
	}

	int operator()(const InputType &x, ValueType &fvec) const
	{
		for (int i = 0; i < values(); i++)
		{
			double y_calc = func(x, xValues_[i]);
			fvec[i] = std::log(xValues_[i])*(yValues_[i] - y_calc);
		}
		return 0;
	}

	int inputs() const { return m_inputs; }
	int values() const { return m_values; }

	const int m_inputs, m_values;
	std::vector<double> xValues_, yValues_;
};

#endif

CalcPiFitBenchmarkResults::CalcPiFitBenchmarkResults() :
	resultsValid_(false)
{
}

CalcPiFitBenchmarkResults::~CalcPiFitBenchmarkResults()
{
}

void CalcPiFitBenchmarkResults::invalidateResults()
{
	resultsValid_ = false;
	fitResultsTime_.clear();
	fitResultsMemory_.clear();
}

void CalcPiFitBenchmarkResults::setBenchmarkResults(const std::vector<double> &experimentDigits,
	const std::vector<double> &experimentTimes,
	const std::vector<double> &experimentMemory)
{
	const int inputs = 4;
	int values = static_cast<int>(experimentDigits.size());
	resultsValid_ = false;

	if(values < 2)	// Insufficient amount of results
		return;

	fitResultsTime_.resize(inputs);
	fitResultsMemory_.resize(inputs);
	for(int i = 0; i < inputs; i++)
	{
		fitResultsTime_[i] = 0;
		fitResultsMemory_[i] = 0;
	}

#if defined(CP_HAVE_EIGEN)
	if(values <= inputs)
#else
	if(true)
#endif
	{
		// Use a linear "fit": This is in no way (at least AFAIK) a least squares fit
		double meanAscent = 0;
		for(int i = 0; i < values; i++)
		{
			meanAscent += (experimentTimes[i] / experimentDigits[i]);
		}
		meanAscent /= static_cast<double>(values);
		fitResultsTime_[1] = meanAscent;
		resultsValid_ = true;
	}
#if defined(CP_HAVE_EIGEN)
	else
	{
		Eigen::LevenbergMarquardtSpace::Status info = Eigen::LevenbergMarquardtSpace::NotStarted;
		Eigen::VectorXd x;
		x.resize(inputs);
		x.setZero();

		// Fit times
		{
			cpfb_functor functor(inputs, values);
			functor.xValues_ = experimentDigits;
			functor.yValues_ = experimentTimes;
			Eigen::NumericalDiff<cpfb_functor> numDiff(functor);
			Eigen::LevenbergMarquardt< Eigen::NumericalDiff<cpfb_functor> > lm(numDiff);
			info = lm.minimize(x);
		}
		for(int i = 0; i < inputs; i++)
			fitResultsTime_[i] = x(i);

		// Fit memory
		info = Eigen::LevenbergMarquardtSpace::NotStarted;
		x.setZero();

		{
			cpfb_functor_ln functor(inputs, values);
			functor.xValues_ = experimentDigits;
			functor.yValues_ = experimentMemory;
			Eigen::NumericalDiff<cpfb_functor_ln> numDiff(functor);
			Eigen::LevenbergMarquardt< Eigen::NumericalDiff<cpfb_functor_ln> > lm(numDiff);
			info = lm.minimize(x);
		}
		for(int i = 0; i < inputs; i++)
			fitResultsMemory_[i] = x(i);
		resultsValid_ = true;

		// Other tests: Memory, quadratic fit
		Eigen::MatrixXd A;
		Eigen::VectorXd b;
		A.resize(values, 3);
		b.resize(values);
		for(int i = 0; i < values; i++)
		{
			A(i, 0) = 1.0;
			A(i, 1) = experimentDigits[i];
			A(i, 2) = experimentDigits[i]*experimentDigits[i];
			b(i) = experimentMemory[i];
		}
		Eigen::Vector3d x_mem_quad = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

		// Time, linear fit
		A.resize(values, 2);
		b.resize(values);
		for(int i = 0; i < values; i++)
		{
			A(i, 0) = 1.0;
			A(i, 1) = experimentDigits[i];
			b(i) = experimentTimes[i];
		}
		Eigen::Vector2d x_lin = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

		A.resize(values, 3);
		b.resize(values);
		for(int i = 0; i < values; i++)
		{
			A(i, 0) = 1.0;
			A(i, 1) = experimentDigits[i];
			A(i, 2) = experimentDigits[i]*experimentDigits[i];
			b(i) = experimentTimes[i];
		}
		Eigen::Vector3d x_quad = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

		// Only for debugging: output to a gnuplot script
#	if(0)
		std::ofstream of("../gnuplot_calcpi_fit.txt");
		if(of.is_open())
		{
			of << "set multiplot layout 1, 2" << std::endl;
			of << "set   autoscale                        # scale axes automatically" << std::endl;
			of << "unset log                              # remove any log-scaling" << std::endl;
			of << "unset label                            # remove any previous labels" << std::endl;
			of << "set xtic auto                          # set xtics automatically" << std::endl;
			of << "set ytic auto                          # set ytics automatically" << std::endl;
			of << "set title \"Calc_Pi benchmark results and fit: Time\"" << std::endl;
			of << "set xlabel \"Number of digits\"" << std::endl;
			of << "set ylabel \"Time\"" << std::endl;
			of << "set logscale x 2" << std::endl;
			of << "set logscale y 2" << std::endl;
			of << "plot " << fitResultsTime_[0] << " + x*" << fitResultsTime_[1]
//				<< " + x**" << fitResultsTime_[2]
//				<< " + x*x*" << fitResultsTime_[2]
//				<< " + " << fitResultsTime_[2] << "*log(x*" << fitResultsTime_[3]
				<< " + " << fitResultsTime_[2] << "*exp(x*" << fitResultsTime_[3]
//				<< " + " << fitResultsTime_[2] << "*x**" << fitResultsTime_[3]
//				<< " + exp(x*" << fitResultsTime_[3]
//				<< ") + exp(x*x*" << fitResultsTime_[4]
				<< ") title \"Exp fit function\" with lines, \\"
				<< std::endl;

			of << " " << x_lin(0) << " + x*" << x_lin(1) << " title \"Linear fit function\" with lines, \\" << std::endl;
			of << " " << x_quad(0) << " + x*" << x_quad(1) << " + x*x*" << x_quad(2) << " title \"Quadratic fit function\" with lines, \\" << std::endl;

			of << " '-' using 1:2 title \"Experiment\" with lines" << std::endl;
			for(size_t i = 0; i < experimentDigits.size(); i++)
			{
				of << experimentDigits[i] << "\t" << experimentTimes[i]<< "\t" << (experimentTimes[i]-experimentDigits[i]*x_lin(1)-x_lin(0)) << std::endl;
			}
			of << "e" << std::endl;

			of << "set title \"Calc_Pi benchmark results and fit: Memory\"" << std::endl;
			of << "set xlabel \"Number of digits\"" << std::endl;
			of << "set ylabel \"Memory\"" << std::endl;
			of << "set logscale x 2" << std::endl;
			of << "set logscale y 2" << std::endl;
			of << "plot " << fitResultsMemory_[0] << " + x*" << fitResultsMemory_[1]
				<< " + " << fitResultsMemory_[2] << "*log(x*" << fitResultsMemory_[3]*fitResultsMemory_[3]
				<< ") title \"Exp fit function\" with lines, \\"
				<< std::endl;
			of << " " << x_mem_quad(0) << " + x*" << x_mem_quad(1) << " + x*x*" << x_mem_quad(2) << " title \"Quadratic fit function\" with lines, \\" << std::endl;
			of << " '-' using 1:2 title \"Experiment\" with lines" << std::endl;
			for(size_t i = 0; i < experimentDigits.size(); i++)
			{
				of << experimentDigits[i] << "\t" << experimentMemory[i] << std::endl;
			}
			of << "e" << std::endl;

			of << "unset multiplot" << std::endl;
		}
#	endif

	}
#endif
}

double CalcPiFitBenchmarkResults::estimateTime(double digits)
{
	return std::max(0.0, fitResultsTime_[0] + fitResultsTime_[1]*digits
		+ fitResultsTime_[2]*std::exp(fitResultsTime_[3]*digits));
}

double CalcPiFitBenchmarkResults::estimateMemory(double digits)
{
	return std::max(0.0, fitResultsMemory_[0] + fitResultsMemory_[1]*digits
		+ fitResultsMemory_[2]*std::log((fitResultsMemory_[3]*fitResultsMemory_[3] + +1.e-12)*digits));
	return 0;
}
