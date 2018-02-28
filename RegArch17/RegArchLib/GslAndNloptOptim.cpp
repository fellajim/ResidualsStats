#include "StdAfxRegArchLib.h"
/*!
	\file GslOptim.cpp
	\brief sources for class cGslOptim methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
/*
#ifdef _RDLL_
	double GslLLHFunction(int n, double *par, void *ex)
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)ex ;
	cRegArchModel* myParam = myParamOptim->mParam ;
	cRegArchValue* myValue = myParamOptim->mValue ;

	cDVector myTeta = cDVector(n, par) ;

		myParam->VectorToRegArchParam(myTeta) ;
		return -1.0*RegArchLLH(*myParam, *myValue) ;
	}
#else
*/
	double GslLLHFunction(const gsl_vector* theParam, void* theData)
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)theData ;
	cRegArchModel* myParam = myParamOptim->mParam ;
	cRegArchValue* myValue = myParamOptim->mValue ;

	cDVector myTeta(theParam) ;

		myParam->VectorToRegArchParam(myTeta) ;
		return -1.0*RegArchLLH(*myParam, *myValue) ;
	}

	double NloptLLHFunction(uint theSize, double* theParam, void*theData)
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)theData;
	cRegArchModel* myParam = myParamOptim->mParam;
	cRegArchValue* myValue = myParamOptim->mValue;
	cDVector myTeta(theSize, theParam);
	double myLLH = RegArchLLH(*myParam, *myValue);
	bool myVerbose = myParamOptim->mVerbose;
		myParam->VectorToRegArchParam(myTeta);
		if (myVerbose)
		{
			(myParamOptim->mNFuncEval)++;
		#ifndef _RDLL_
				cout << "#Function evaluation: " << myParamOptim->mNFuncEval << " - Function value: " << myLLH << endl;
		#else
			Rprintf("#Function evaluation: %d Function value: %f\n", myParamOptim->mNFuncEval, myLLH);
		#endif //_RDLL_

		}
		return myLLH;

	}
//#endif // _RDLL_

/*
#ifdef _RDLL_
	void GslGradLLHFunction(int n, double *par, double *gr, void *ex)
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)ex ;
	cRegArchModel* myParam = myParamOptim->mParam ;
	cRegArchValue* myValue = myParamOptim->mValue ;

	cDVector myTeta(n, par) ;
	cDVector myGradLLH(n, gr) ;

		myParam->VectorToRegArchParam(myTeta) ;
		RegArchGradLLH(*myParam, *myValue, myGradLLH) ;
		for (register int i = 0 ; i < n ; i++)
			gr[i] = -myGradLLH[i] ;
	}
#else
*/
	void GslGradLLHFunction(const gsl_vector* theParam, void* theData, gsl_vector* theGrad)
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)theData ;
	cRegArchModel* myParam = myParamOptim->mParam ;
	cRegArchValue* myValue = myParamOptim->mValue ;

	cDVector myTeta(theParam) ;
	cDVector myGradLLH(theGrad) ;

		myParam->VectorToRegArchParam(myTeta) ;
		RegArchGradLLH(*myParam, *myValue, myGradLLH) ;
		for (register int i = 0 ; i < (int)theGrad->size ; i++)
			gsl_vector_set(theGrad, i, -myGradLLH[i]) ;
	}

	double NloptLLHAndGradLLHFunction(uint theSize, double* theParam, double* theGrad, void* theData)
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)theData;
	cRegArchModel* myParam = myParamOptim->mParam;
	cRegArchValue* myValue = myParamOptim->mValue;
	bool myVerbose = myParamOptim->mVerbose;
	cDVector myTeta(theSize, theParam);
	cDVector myGradLLH(theSize);
	double myLLH;
		myParam->VectorToRegArchParam(myTeta);
		RegArchLLHAndGradLLH(*myParam, *myValue, myLLH, myGradLLH);
		if (theGrad == NULL)
		{
			theGrad = (double *)calloc(theSize, sizeof(double));
		}
		for (register uint i = 0; i < theSize; i++)
			theGrad[i] = myGradLLH[i] ;
		if (myVerbose)
		{
			(myParamOptim->mNFuncEval)++;
		#ifndef _RDLL_
			cout << "#Function evaluation: " << myParamOptim->mNFuncEval << " - Function value: " << myLLH << endl;
		#else
			Rprintf("#Function evaluation: %d Function value: %f\n", myParamOptim->mNFuncEval, myLLH);
		#endif //_RDLL_
		}
		return myLLH;
	}

	void GslLLHAndGradLLHFunction(const gsl_vector* theParam, void* theData, double* theLLH, gsl_vector* theGrad) 
	{
	pParamOptimPtr myParamOptim = (pParamOptimPtr)theData ;
	cRegArchModel* myParam = myParamOptim->mParam ;
	cRegArchValue* myValue = myParamOptim->mValue ;

	cDVector myTeta(theParam) ;
	cDVector myGradLLH(theGrad) ;

		myParam->VectorToRegArchParam(myTeta) ;
		RegArchLLHAndGradLLH(*myParam, *myValue, *theLLH, myGradLLH) ;
		for (register int i = 0 ; i < (int)theGrad->size ; i++)
			gsl_vector_set(theGrad, i, -myGradLLH[i]) ;
		*theLLH *= -1.0 ;
	}
//#endif //_RDLL_
} //namespace

