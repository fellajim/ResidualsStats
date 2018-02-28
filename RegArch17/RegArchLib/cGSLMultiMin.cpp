#include "StdAfxRegArchLib.h"

#ifndef _RDLL_
/*!
	\file cGSLMultiMin.cpp
	\brief sources for class cGSLMultiMin methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {

	cGSLMultiMin::cGSLMultiMin()
	{
		mvInitPoint.Delete() ;
		mvFdf = true ;
		mvType.mvTypeFdf = NULL ;
		mvState.mvStateFdf = NULL ;
		mvFunct.mvFunctFdf.f = NULL ;
		mvFunct.mvFunctFdf.df = NULL ;
		mvFunct.mvFunctFdf.fdf = NULL ;
		mvFunct.mvFunctFdf.params = NULL ;
		mvFunct.mvFunctFdf.n = 0 ;
		mvStepSize.mvStepDouble = 1e-2 ;
		mvTol = 1e-4 ;
	}

	cGSLMultiMin::cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function_fdf* theFunction, double theStepSize, double theTol)
	{
		mvInitPoint.ReAlloc(theX0) ;
		mvFdf = (theTypeAlgo != eSimplexNM) ;
		switch (theTypeAlgo)
		{
			case eConjugateFR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_fr ;
			break ;
			case eConjugatePR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_pr ;
			break ;
			case eBFGS :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs ;
			break ;
			case eBFGSTwo :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs2 ;
			break ;
			case eSteepest :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_steepest_descent ;
			break ;
			case eSimplexNM :
				mvType.mvTypeF = gsl_multimin_fminimizer_nmsimplex ;
			break ;

		}
	size_t myNParam = (size_t)theX0.GetSize() ;
		if (myNParam > 0)
		{	if (mvFdf)
				mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, myNParam);
			else
				mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, myNParam) ;
		}

		if (theFunction != NULL)
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = theFunction->f ;
				mvFunct.mvFunctFdf.df = theFunction->df ;
				mvFunct.mvFunctFdf.fdf = theFunction->fdf ;
				mvFunct.mvFunctFdf.params = theFunction->params ;
				mvFunct.mvFunctFdf.n = myNParam ;
			}
			else
			{	mvFunct.mvFunctF.f = theFunction->f ;
				mvFunct.mvFunctF.params = theFunction->params ;
				mvFunct.mvFunctF.n = myNParam ;
			}
		}
		else
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = NULL ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = NULL ;
				mvFunct.mvFunctFdf.n = 0 ;
			}
			else
			{	mvFunct.mvFunctF.f = NULL ;
				mvFunct.mvFunctF.params = NULL ;
				mvFunct.mvFunctF.n = 0 ;
			}
		}
		if (mvFdf)
			mvStepSize.mvStepDouble = theStepSize ;
		else
			mvStepSize.mvStepVect = new cDVector((int)myNParam, theStepSize) ;
		mvTol = theTol ;
		if (theFunction != NULL)
		{	if (mvFdf)
				gsl_multimin_fdfminimizer_set(mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
			else
				gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector()) ;
		}
	}

	cGSLMultiMin::cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function* theFunction, double theStepSize)
	{
		mvInitPoint.ReAlloc(theX0) ;
		mvFdf = (theTypeAlgo != eSimplexNM) ;
		switch (theTypeAlgo)
		{
			case eConjugateFR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_fr ;
			break ;
			case eConjugatePR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_pr ;
			break ;
			case eBFGS :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs ;
			break ;
			case eBFGSTwo :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs2 ;
			break ;
			case eSteepest :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_steepest_descent ;
			break ;
			case eSimplexNM :
				mvType.mvTypeF = gsl_multimin_fminimizer_nmsimplex ;
			break ;

		}
	size_t myNParam = (size_t)theX0.GetSize() ;
		if (myNParam > 0)
		{	if (mvFdf)
				mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, myNParam);
			else
				mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, myNParam) ;
		}
		if (theFunction != NULL)
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = theFunction->f ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = theFunction->params ;
				mvFunct.mvFunctFdf.n = myNParam ;
			}
			else
			{	mvFunct.mvFunctF.f = theFunction->f ;
				mvFunct.mvFunctF.params = theFunction->params ;
				mvFunct.mvFunctF.n = myNParam ;
			}
		}
		else
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = NULL ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = NULL ;
				mvFunct.mvFunctFdf.n = 0 ;
			}
			else
			{	mvFunct.mvFunctF.f = NULL ;
				mvFunct.mvFunctF.params = NULL ;
				mvFunct.mvFunctF.n = 0 ;
			}
		}
		if (mvFdf)
			mvStepSize.mvStepDouble = theStepSize ;
		else
			mvStepSize.mvStepVect = new cDVector((int)myNParam, theStepSize) ;
		mvTol = 1e-4 ;
		if (theFunction != NULL)
		{	if (mvFdf)
				gsl_multimin_fdfminimizer_set (mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
			else
				gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector()) ;
		}
	}

	cGSLMultiMin::cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function_fdf* theFunction, sGSLMultiMinAlgoParam* theAlgoParam)
	{
	double	myStepSize = 0.01,
			myTol = 0.01 ;
		
		if (theAlgoParam != NULL)
		{	myStepSize = theAlgoParam->mStepSize ;
			myTol = theAlgoParam->mTol ;
		}
		
		mvInitPoint.ReAlloc(theX0) ;
		mvFdf = (theTypeAlgo != eSimplexNM) ;
		switch (theTypeAlgo)
		{
			case eConjugateFR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_fr ;
			break ;
			case eConjugatePR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_pr ;
			break ;
			case eBFGS :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs ;
			break ;
			case eBFGSTwo :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs2 ;
			break ;
			case eSteepest :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_steepest_descent ;
			break ;
			case eSimplexNM :
				mvType.mvTypeF = gsl_multimin_fminimizer_nmsimplex ;
			break ;

		}
	size_t myNParam = (size_t)theX0.GetSize() ;
		if (myNParam > 0)
		{	if (mvFdf)
				mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, myNParam);
			else
				mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, myNParam) ;
		}

		if (theFunction != NULL)
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = theFunction->f ;
				mvFunct.mvFunctFdf.df = theFunction->df ;
				mvFunct.mvFunctFdf.fdf = theFunction->fdf ;
				mvFunct.mvFunctFdf.params = theFunction->params ;
				mvFunct.mvFunctFdf.n = myNParam ;
			}
			else
			{	mvFunct.mvFunctF.f = theFunction->f ;
				mvFunct.mvFunctF.params = theFunction->params ;
				mvFunct.mvFunctF.n = myNParam ;
			}
		}
		else
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = NULL ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = NULL ;
				mvFunct.mvFunctFdf.n = 0 ;
			}
			else
			{	mvFunct.mvFunctF.f = NULL ;
				mvFunct.mvFunctF.params = NULL ;
				mvFunct.mvFunctF.n = 0 ;
			}
		}
		if (mvFdf)
			mvStepSize.mvStepDouble = myStepSize ;
		else
			mvStepSize.mvStepVect = new cDVector((int)myNParam, myStepSize) ;
		mvTol = myTol ;
		if (theFunction != NULL)
		{	if (mvFdf)
		gsl_multimin_fdfminimizer_set(mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
			else
				gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector()) ;
		}
	}
	
	cGSLMultiMin::cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function* theFunction, sGSLMultiMinAlgoParam* theAlgoParam)
	{
	double	myStepSize = 0.01,
			myTol = 0.01 ;
		
		if (theAlgoParam != NULL)
		{	myStepSize = theAlgoParam->mStepSize ;
			myTol = theAlgoParam->mTol ;
		}		
		mvInitPoint.ReAlloc(theX0) ;
		mvFdf = (theTypeAlgo != eSimplexNM) ;
		switch (theTypeAlgo)
		{
			case eConjugateFR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_fr ;
			break ;
			case eConjugatePR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_pr ;
			break ;
			case eBFGS :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs ;
			break ;
			case eBFGSTwo :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs2 ;
			break ;
			case eSteepest :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_steepest_descent ;
			break ;
			case eSimplexNM :
				mvType.mvTypeF = gsl_multimin_fminimizer_nmsimplex ;
			break ;

		}
	size_t myNParam = (size_t)theX0.GetSize() ;
		if (myNParam > 0)
		{	if (mvFdf)
				mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, myNParam);
			else
				mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, myNParam) ;
		}
		if (theFunction != NULL)
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = theFunction->f ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = theFunction->params ;
				mvFunct.mvFunctFdf.n = myNParam ;
			}
			else
			{	mvFunct.mvFunctF.f = theFunction->f ;
				mvFunct.mvFunctF.params = theFunction->params ;
				mvFunct.mvFunctF.n = myNParam ;
			}
		}
		else
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = NULL ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = NULL ;
				mvFunct.mvFunctFdf.n = 0 ;
			}
			else
			{	mvFunct.mvFunctF.f = NULL ;
				mvFunct.mvFunctF.params = NULL ;
				mvFunct.mvFunctF.n = 0 ;
			}
		}
		if (mvFdf)
			mvStepSize.mvStepDouble = myStepSize ;
		else
			mvStepSize.mvStepVect = new cDVector((int)myNParam, myStepSize) ;
		mvTol = myTol ;
		if (theFunction != NULL)
		{	if (mvFdf)
				gsl_multimin_fdfminimizer_set (mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
			else
				gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector()) ;
		}
	}

	cGSLMultiMin::~cGSLMultiMin()
	{
		if (mvFdf)
		{	if (mvState.mvStateFdf != NULL)
			{	gsl_multimin_fdfminimizer_free(mvState.mvStateFdf) ;
				mvState.mvStateFdf = NULL ;
			}
		}
		else
		{	if (mvState.mvStateF != NULL)
			{	gsl_multimin_fminimizer_free(mvState.mvStateF) ;
				mvState.mvStateF = NULL ;
			}
		}
		mvInitPoint.Delete() ;
		if (mvFdf)
		{	mvType.mvTypeFdf = NULL ;
			mvFunct.mvFunctFdf.f = NULL ;
			mvFunct.mvFunctFdf.df = NULL ;
			mvFunct.mvFunctFdf.fdf = NULL ;
			mvFunct.mvFunctFdf.params = NULL ;
			mvFunct.mvFunctFdf.n = 0 ;
		}
		else
		{	mvType.mvTypeF = NULL ;
			mvFunct.mvFunctF.f = NULL ;
			mvFunct.mvFunctF.params = NULL ;
			mvFunct.mvFunctF.n = 0 ;
			delete mvStepSize.mvStepVect ;
		}
	}

	void cGSLMultiMin::SetInitPoint(cDVector& theX0)
	{
		mvInitPoint = theX0 ;
	}

	void cGSLMultiMin::SetTypeAlgo(eGSLMultiMinAlgoEnum theTypeAlgo)
	{	mvFdf = (theTypeAlgo != eSimplexNM) ;
		switch (theTypeAlgo)
		{
			case eConjugateFR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_fr ;
			break ;
			case eConjugatePR :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_conjugate_pr ;
			break ;
			case eBFGS :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs ;
			break ;
			case eBFGSTwo :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_vector_bfgs2 ;
			break ;
			case eSteepest :
				mvType.mvTypeFdf = gsl_multimin_fdfminimizer_steepest_descent ;
			break ;
			case eSimplexNM :
				mvType.mvTypeF = gsl_multimin_fminimizer_nmsimplex ;
			break ;
		}

	}
	
	void cGSLMultiMin::SetFunction(gsl_multimin_function_fdf* theFunction)
	{
		if (theFunction != NULL)
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = theFunction->f ;
				mvFunct.mvFunctFdf.df = theFunction->df ;
				mvFunct.mvFunctFdf.fdf = theFunction->fdf ;
				mvFunct.mvFunctFdf.params = theFunction->params ;
				mvFunct.mvFunctFdf.n = theFunction->n ;
				if (mvState.mvStateFdf == NULL)
					mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, mvFunct.mvFunctFdf.n);
				if (mvInitPoint.GetSize() == mvFunct.mvFunctFdf.n)
					gsl_multimin_fdfminimizer_set(mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
			}
			else
			{	mvFunct.mvFunctF.f = theFunction->f ;
				mvFunct.mvFunctF.params = theFunction->params ;
				mvFunct.mvFunctF.n = theFunction->n ;
				if (mvState.mvStateF == NULL)
					mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, mvFunct.mvFunctF.n);
				if (mvInitPoint.GetSize() == mvFunct.mvFunctF.n)
					gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector());
			}
		}

	}

	void cGSLMultiMin::SetFunction(gsl_multimin_function* theFunction)
	{
		if (theFunction != NULL)
		{	if (mvFdf)
			{	mvFunct.mvFunctFdf.f = theFunction->f ;
				mvFunct.mvFunctFdf.df = NULL ;
				mvFunct.mvFunctFdf.fdf = NULL ;
				mvFunct.mvFunctFdf.params = theFunction->params ;
				mvFunct.mvFunctFdf.n = theFunction->n ;
				if (mvState.mvStateFdf == NULL)
					mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, mvFunct.mvFunctFdf.n);
				if (mvInitPoint.GetSize() == mvFunct.mvFunctFdf.n)
					gsl_multimin_fdfminimizer_set(mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
			}
			else
			{	mvFunct.mvFunctF.f = theFunction->f ;
				mvFunct.mvFunctF.params = theFunction->params ;
				mvFunct.mvFunctF.n = theFunction->n ;
				if (mvState.mvStateF == NULL)
					mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, mvFunct.mvFunctF.n);
				if (mvInitPoint.GetSize() == mvFunct.mvFunctF.n)
					gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector());
			}
		}

	}
	
	void cGSLMultiMin::SetFunction(GSLMultiMinF theF, GSLMultiMinDf theDf, GSLMultiMinFdf theFdf, size_t theNParam, void* theOtherParam)
	{
		if (mvFdf)
		{	mvFunct.mvFunctFdf.f = theF;
			mvFunct.mvFunctFdf.df = theDf ;
			mvFunct.mvFunctFdf.fdf = theFdf ;
			mvFunct.mvFunctFdf.params = theOtherParam ;
			mvFunct.mvFunctFdf.n = theNParam ;
		}
		else
		{	mvFunct.mvFunctF.f = theF;
			mvFunct.mvFunctF.params = theOtherParam ;
			mvFunct.mvFunctF.n = theNParam ;
		}
	}
	
	void cGSLMultiMin::SetFunction(GSLMultiMinF theF,  size_t theNParam, void* theOtherParam)
	{
		if (mvFdf)
		{	mvFunct.mvFunctFdf.f = theF;
			mvFunct.mvFunctFdf.df = NULL ;
			mvFunct.mvFunctFdf.fdf = NULL ;
			mvFunct.mvFunctFdf.params = theOtherParam ;
			mvFunct.mvFunctFdf.n = theNParam ;
		}
		else
		{	mvFunct.mvFunctF.f = theF;
			mvFunct.mvFunctF.params = theOtherParam ;
			mvFunct.mvFunctF.n = theNParam ;
		}
	}
	
	void cGSLMultiMin::SetStepSize(double theStepSize)
	{
		if (mvFdf)
			mvStepSize.mvStepDouble = theStepSize ;
		else
			mvStepSize.mvStepVect = new cDVector((int)(mvFunct.mvFunctF.n), theStepSize) ;
	}

	void cGSLMultiMin::SetTol(double theTol)
	{
		mvTol = theTol ;
	}

	void cGSLMultiMin::SetAlgoParam(sGSLMultiMinAlgoParam theAlgoParam)
	{
		mvTol = theAlgoParam.mTol ;
		if (mvFdf)
			mvStepSize.mvStepDouble = theAlgoParam.mStepSize ;
		else
			mvStepSize.mvStepVect = new cDVector((int)(mvFunct.mvFunctF.n), theAlgoParam.mStepSize) ;
	}

	void cGSLMultiMin::GSLOptim(cDVector& theX, sGSLMultiMinResult& theResStruct, double theStopValue, int theMaxIter, bool theVerbose)
	{
	size_t myNParam = (size_t)mvInitPoint.GetSize() ;
		
		if (mvFdf)
		{	if (mvState.mvStateFdf  == NULL)
				mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, myNParam);
			gsl_multimin_fdfminimizer_set(mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
		}
		else
		{	if (mvState.mvStateF == NULL)
				mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, myNParam) ;
			if (mvStepSize.mvStepVect->GetSize() == 0)
				mvStepSize.mvStepVect->ReAlloc((int)myNParam, 1e-2) ;
			gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector());
		}

	int myNIter = 0,
		myStatus ;
	gsl_vector* myX ;
		do
		{	myNIter++;

			if (mvFdf)
				myStatus = gsl_multimin_fdfminimizer_iterate(mvState.mvStateFdf);
			else
				myStatus = gsl_multimin_fminimizer_iterate(mvState.mvStateF) ;

			if (myStatus)
				break;
			
			if (mvFdf)
				myStatus = gsl_multimin_test_gradient(mvState.mvStateFdf->gradient, theStopValue);
			else
			{	double myAuxSize = gsl_multimin_fminimizer_size(mvState.mvStateF);
				myStatus = gsl_multimin_test_size(myAuxSize, theStopValue) ;
			}
			if ( (myStatus == GSL_SUCCESS) && (theVerbose) )
				cout << "Minimum found at:" << endl ;
			if (theVerbose)
			{	if (mvFdf)
					myX = mvState.mvStateFdf->x ;
				else
					myX = mvState.mvStateF->x ;
				cout << "iter num " << myNIter << endl ;
				for (register uint i = 0 ; i < myNParam ; i++)
					cout << "Teta[" << i+1 << "]="<< gsl_vector_get(myX, i)  << endl ;
			double myf ;
				if (mvFdf)
					myf = mvState.mvStateFdf->f ;
				else
					myf = mvState.mvStateF->fval ;
				cout << "function=" << myf << endl ;
			}
		}
		while (myStatus == GSL_CONTINUE && myNIter < theMaxIter);
		if (mvFdf)
			myX = mvState.mvStateFdf->x ;
		else
			myX = mvState.mvStateF->x ;
		for (register uint i = 0 ; i < myNParam ; i++)
			theX[i] = gsl_vector_get(myX, i) ;
		theResStruct.mConvergence = (myStatus == GSL_SUCCESS) ;
		theResStruct.mNIter = myNIter ;

		if (mvFdf)
		{	gsl_vector* myGrad = mvState.mvStateFdf->gradient ;
			theResStruct.mGrad.ReAlloc(myGrad) ;
			theResStruct.mCritValue = Norm(theResStruct.mGrad) ;
			theResStruct.mFunctValue = mvState.mvStateFdf->f ;
		}
		else
		{	theResStruct.mGrad.Delete();
			theResStruct.mCritValue = gsl_multimin_fminimizer_size (mvState.mvStateF) ;
			theResStruct.mFunctValue = mvState.mvStateF->fval ;
		}

		sprintf(theResStruct.mConvergenceComment, "%s", gsl_strerror(myStatus)) ;
	}

	void cGSLMultiMin::GSLOptim(cDVector& theX, sGSLMultiMinResult& theResStruct, sGSLMultiMinAlgoParam theAlgoParam)
	{
	size_t myNParam = (size_t)mvInitPoint.GetSize() ;
		
		if (mvFdf)
		{	if (mvState.mvStateFdf  == NULL)
				mvState.mvStateFdf = gsl_multimin_fdfminimizer_alloc(mvType.mvTypeFdf, myNParam);
			gsl_multimin_fdfminimizer_set(mvState.mvStateFdf, &mvFunct.mvFunctFdf, mvInitPoint.GetGSLVector(), mvStepSize.mvStepDouble, mvTol);
		}
		else
		{	if (mvState.mvStateF == NULL)
				mvState.mvStateF = gsl_multimin_fminimizer_alloc(mvType.mvTypeF, myNParam) ;
			if (mvStepSize.mvStepVect->GetSize() == 0)
				mvStepSize.mvStepVect->ReAlloc((int)myNParam, 1e-2) ;
			gsl_multimin_fminimizer_set(mvState.mvStateF, &mvFunct.mvFunctF, mvInitPoint.GetGSLVector(), mvStepSize.mvStepVect->GetGSLVector());
		}

	int myNIter = 0,
		myStatus ;
	gsl_vector* myX ;
	clock_t	myStart = clock() ;
	bool myTimeBool = true ;
		do
		{	myNIter++;

			if (mvFdf)
				myStatus = gsl_multimin_fdfminimizer_iterate(mvState.mvStateFdf);
			else
				myStatus = gsl_multimin_fminimizer_iterate(mvState.mvStateF) ;

			if (myStatus)
				break;
			
			if (mvFdf)
				myStatus = gsl_multimin_test_gradient(mvState.mvStateFdf->gradient, theAlgoParam.mStopValue);
			else
			{	double myAuxSize = gsl_multimin_fminimizer_size(mvState.mvStateF);
				myStatus = gsl_multimin_test_size(myAuxSize, theAlgoParam.mStopValue) ;
			}
			if ( (myStatus == GSL_SUCCESS) && (theAlgoParam.mVerbose) )
				cout << "Minimum found at:" << endl ;
			if (theAlgoParam.mVerbose)
			{	if (mvFdf)
					myX = mvState.mvStateFdf->x ;
				else
					myX = mvState.mvStateF->x ;
				cout << "iter num " << myNIter << endl ;
				for (register uint i = 0 ; i < myNParam ; i++)
					cout << "Teta[" << i+1 << "]="<< gsl_vector_get(myX, i)  << endl ;
			double myf ;
				if (mvFdf)
					myf = mvState.mvStateFdf->f ;
				else
					myf = mvState.mvStateF->fval ;
				cout << "function=" << myf << endl ;
			}
			myTimeBool = (clock() - myStart < (clock_t)(theAlgoParam.mNMaxSeconds*CLOCKS_PER_SEC)) ;
		}
		while (myStatus == GSL_CONTINUE && myNIter < (int)theAlgoParam.mNMaxIter && myTimeBool);
		if (mvFdf)
			myX = mvState.mvStateFdf->x ;
		else
			myX = mvState.mvStateF->x ;
		for (register uint i = 0 ; i < myNParam ; i++)
			theX[i] = gsl_vector_get(myX, i) ;
		theResStruct.mConvergence = (myStatus == GSL_SUCCESS) ;
		theResStruct.mNIter = myNIter ;
		theResStruct.mComputeTime = (double)(clock() - myStart)/(double)CLOCKS_PER_SEC ;
		if (mvFdf)
		{	gsl_vector* myGrad = mvState.mvStateFdf->gradient ;
			theResStruct.mGrad.ReAlloc(myGrad) ;
			theResStruct.mCritValue = Norm(theResStruct.mGrad) ;
			theResStruct.mFunctValue = mvState.mvStateFdf->f ;
		}
		else
		{	theResStruct.mGrad.Delete();
			theResStruct.mCritValue = gsl_multimin_fminimizer_size (mvState.mvStateF) ;
			theResStruct.mFunctValue = mvState.mvStateF->fval ;
		}

		if (myTimeBool)
		{	sprintf(theResStruct.mConvergenceComment, "%s", gsl_strerror(myStatus)) ;
			theResStruct.mConvergenceStatus = myStatus ;
		}
		else
		{	sprintf(theResStruct.mConvergenceComment, "Too long time in optimization program") ;
			theResStruct.mConvergenceStatus = -100 ;
		}
	}
}

#endif // _RDLL_
