// Test.cpp : définit le point d'entrée pour l'application console.
//

#include "StdAfxTestWrapperGSL.h"
 
using namespace std;
using namespace WrapperGslCppNameSpace;

/* Paraboloid centered on (p[0],p[1]), with
scale factors (p[2],p[3]) and minimum p[4] */

double
my_f(const gsl_vector *v, void *params)
{
	double x, y;
	double *p = (double *)params;

	x = gsl_vector_get(v, 0);
	y = gsl_vector_get(v, 1);

	return p[2] * (x - p[0]) * (x - p[0]) +
		p[3] * (y - p[1]) * (y - p[1]) + p[4];
}

/* The gradient of f, df = (df/dx, df/dy). */
void
my_df(const gsl_vector *v, void *params,
	gsl_vector *df)
{
	double x, y;
	double *p = (double *)params;

	x = gsl_vector_get(v, 0);
	y = gsl_vector_get(v, 1);

	gsl_vector_set(df, 0, 2.0 * p[2] * (x - p[0]));
	gsl_vector_set(df, 1, 2.0 * p[3] * (y - p[1]));
}

/* Compute both f and df together. */
void
my_fdf(const gsl_vector *x, void *params,
	double *f, gsl_vector *df)
{
	*f = my_f(x, params);
	my_df(x, params, df);
}



int _tmain(int argc, _TCHAR* argv[])
{
sGSLMultiMinResult myResGSLStruct;
sGSLMultiMinAlgoParam myGSLAlgoParam;
	myGSLAlgoParam.mAlgoType = eBFGS;
	myGSLAlgoParam.mNMaxIter = 1000;
	myGSLAlgoParam.mStepSize = 0.0001;
	myGSLAlgoParam.mStopValue = 0.000001;
	myGSLAlgoParam.mTol = 0.000001;
	myGSLAlgoParam.mVerbose = true;
	myGSLAlgoParam.mNMaxSeconds = 1000;

double myPar[5] = { 1.0, 2.0, 10.0, 20.0, 30.0 };
	gsl_multimin_function_fdf myFunct;
	myFunct.df = my_df;
	myFunct.f = my_f;
	myFunct.fdf = my_fdf;
	myFunct.n = 2;
	myFunct.params = myPar;

cDVector myInitPoint(2);
	myInitPoint[0] = 5.0;
	myInitPoint[1] = 7.0;
cGSLMultiMin myGSLMultiMin(myInitPoint, myGSLAlgoParam.mAlgoType);
	
	myGSLMultiMin.SetFunction(&myFunct);
	myGSLMultiMin.SetAlgoParam(myGSLAlgoParam);
cDVector myX(2);
	myGSLMultiMin.GSLOptim(myX, myResGSLStruct, myGSLAlgoParam);
	cout << endl << myResGSLStruct.mConvergenceComment << endl;
	cout << "Solution:" << endl;
	cout << myX << endl;





}
