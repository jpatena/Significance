// Jaron Patena
// Calculates significance between two series of numbers
// Compile: c++ -o sig significance.cpp
// Execute: ./sig
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <boost/math/distributions/students_t.hpp>

using namespace std;

class StdDeviation
{
private:
	int max;
	double value[100];
	double mean;

public:
	double CalcMean();
	double CalcVariance();
	double CalcSampleVariance();
	int SetValues(double *p, int count);
	double CalcStdDeviation();
	double CalcSampleStdDeviation();
};


double StdDeviation::CalcMean()
{
	double sum = 0.0;
	for(int i = 0; i < max; i++)
		sum += value[i];
	return (sum / max);
}

double StdDeviation::CalcVariance()
{
	mean = CalcMean();
	double temp = 0;

	for(int i = 0; i < max; i++)
		temp += (value[i] - mean) * (value[i] - mean) ;
	return temp / max;
}

double StdDeviation::CalcSampleVariance()
{
	mean = CalcMean();
	double temp = 0;

	for(int i = 0; i < max; i++)
		temp += (value[i] - mean) * (value[i] - mean) ;
	return temp / (max - 1);
}

int StdDeviation::SetValues(double *p, int count)
{
  if(count > 100) return -1;
	max = count;

	for(int i = 0; i < count; i++)
		value[i] = p[i];
	return 0;
}

double StdDeviation::CalcStdDeviation()
{
  return sqrt(CalcVariance());
}

double StdDeviation::CalcSampleStdDeviation()
{
	return sqrt(CalcSampleVariance());
}

class StatsCalc
{
private:
	double XSeries[100];
	double YSeries[100];
	int max;
	double p;
	StdDeviation x;
	StdDeviation y;
public:
	void SetValues(double *xvalues, double *yvalues, int count);
	double CalcCovariance();
	double CalcCorrelation();
	double CalcDetermination();
	double CalcSignificance();
	double CalcPValue();
	double Equation();
};

void StatsCalc::SetValues(double *xvalues, double *yvalues, int count)
{
	for(int i = 0; i < count; i++)
	{
		XSeries[i] = xvalues[i];
		YSeries[i] = yvalues[i];
	}
	x.SetValues(xvalues, count);
	y.SetValues(yvalues, count);
	max = count; // n
}

double StatsCalc::CalcCovariance()
{
	double xmean = x.CalcMean();
	double ymean = y.CalcMean();

	double total = 0;
	for(int i = 0; i < max; i++)
		total += (XSeries[i] - xmean) * (YSeries[i] - ymean);
	return total / max;
}

double StatsCalc::CalcCorrelation() // r
{
	double cov = CalcCovariance();
	double correlation = cov / (x.CalcStdDeviation() * y.CalcStdDeviation());
	
	return correlation;
}

double StatsCalc::CalcDetermination() // r^2
{
	double rsquared = pow(CalcCorrelation(), 2);
	return rsquared;
}

double StatsCalc::CalcSignificance() // t
{
	double step1, step2, step3, step4;
	// |r(x,y)|
	step1 = fabs(CalcCorrelation());
	// sqrt(n-2)
	step2 = sqrt(max-2);
	// sqrt[1-r(x,y)^2]
	step3 = sqrt(1-CalcDetermination());
	// [|r(x,y)| * sqrt(n-2)] / sqrt[1-r(x,y)^2]
	step4 = (step1*step2)/step3;
	return step4;
}

double StatsCalc::CalcPValue() // p
{
	boost::math::students_t dist(max-2);
	p = boost::math::cdf(dist, fabs(CalcSignificance())); // absolute value t
	return p;
}

double StatsCalc::Equation()
{
	return (1-p)*2;
}

int main() // main program
{
	StatsCalc num;
	double xarr[] = { 400, 450, 1700, 1600, 1958, 2030, 125};
	double yarr[] = { 26.6, 30.2, 36.6, 48.8, 58.5, 55.6, 32.3, 288.6};

	num.SetValues(xarr,yarr,sizeof(xarr) / sizeof(xarr[0]));

	cout << "r: " << num.CalcCorrelation()  << endl;
	cout << "r^2: " << num.CalcDetermination() << endl;
	cout << "t: " <<  num.CalcSignificance() << endl;
	cout << "p: " << num.CalcPValue() << endl;
	cout << "2*(1-p): " << num.Equation() << endl;
	return 0;
}
