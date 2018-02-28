#include "StdAfxWrapperNloptCpp.h"

namespace WrapperNloptCppNamespace {

	cNLOPTAlgoParam::cNLOPTAlgoParam()
	{
		this->mAlgo = NLOPT_GN_DIRECT;
		this->mfTol = 1e-3;
		this->mMaxComputeTime = 60;
		this->mMaxFuncEval = INT_MAX;
		this->mMinimisation = true;
		this->mStopVal = -1e299;
		this->mxTol = 1e-3;
		this->mVerbose = false;
	}

	cNLOPTAlgoParam::cNLOPTAlgoParam(nlopt_algorithm theAlgo, bool theMinimisation, double theStopValue, double thefTol, double thexTol, uint theMaxFuncEval, double theComputeTimeMax, bool theVerbose)
	{
		this->mAlgo = theAlgo;
		this->mfTol = thefTol;
		this->mMaxComputeTime = theComputeTimeMax;
		this->mMaxFuncEval = theMaxFuncEval;
		this->mMinimisation = theMinimisation;
		this->mStopVal = theStopValue;
		this->mxTol = thexTol;
		this->mVerbose = theVerbose;
	}

	cNLOPTAlgoParam::~cNLOPTAlgoParam()
	{
	}

	cNLOPTResult::cNLOPTResult()
	{
		mDimension = 0;
		mXOptim = NULL;
		mConvergence = false;		
		mFunctValue = 0;
		mConvergenceStatus = NLOPT_FAILURE;
		sprintf(mConvergenceString, "");
		mComputeTime = 0.0;
	}

	cNLOPTResult::cNLOPTResult(uint theDimension)
	{
		mDimension = theDimension;
		mXOptim =  new double [theDimension] ;
		mConvergence = false;
		mFunctValue = 0;
		mConvergenceStatus = NLOPT_FAILURE;
		sprintf(mConvergenceString, "");
		mComputeTime = 0.0;

	}

	cNLOPTResult::~cNLOPTResult()
	{
		if (mXOptim != NULL)
		{
			delete[] mXOptim;
			mXOptim = NULL;
			mDimension = 0;
		}

	}

	cNloptWrapperCpp::cNloptWrapperCpp()
	{
		mvOpt = NULL;
	}

	cNloptWrapperCpp::cNloptWrapperCpp(nlopt_algorithm theAlgo, uint theDimension, nlopt_func theObjective, bool theMin, void* theFuncData)
	{
		mvOpt = nlopt_create(theAlgo, theDimension);
	double* mylb = new double[theDimension] ;
	double* myub = new double[theDimension] ;
		for (register uint i = 0; i < theDimension; i++)
		{
			mylb[i] = -1e299;
			myub[i] = 1e299;
		}
		nlopt_set_lower_bounds(mvOpt, mylb);
		nlopt_set_upper_bounds(mvOpt, myub);

		delete [] mylb;
		delete [] myub;

		if (theMin)
		{
			nlopt_set_min_objective(mvOpt, theObjective, theFuncData);
		}
		else 
		{
			nlopt_set_max_objective(mvOpt, theObjective, theFuncData);
		}
	}

	cNloptWrapperCpp::~cNloptWrapperCpp()
	{
		if (mvOpt != NULL)
		{
			nlopt_destroy(mvOpt);
			mvOpt = NULL;
		}
	}

	void cNloptWrapperCpp::SetAlgorithm(nlopt_algorithm theAlgo, uint theDimension)
	{
		if (mvOpt != NULL)
			nlopt_destroy(mvOpt);
		mvOpt = nlopt_create(theAlgo, theDimension);
	double* mylb = new double[theDimension];
	double* myub = new double[theDimension];
		for (register uint i = 0; i < theDimension; i++)
		{
			mylb[i] = -1e299;
			myub[i] = 1e299;
		}
		nlopt_set_lower_bounds(mvOpt, mylb);
		nlopt_set_upper_bounds(mvOpt, myub);

		delete [] mylb;
		delete [] myub;
	}

	void cNloptWrapperCpp::SetObjectiveFunc(nlopt_func theObjective, bool theMin, void* theFuncData)
	{
		if (mvOpt != NULL)
		{
			if (theMin)
			{
				nlopt_set_min_objective(mvOpt, theObjective, theFuncData);
			}
			else
			{
				nlopt_set_max_objective(mvOpt, theObjective, theFuncData);
			}
		}
	}

	void cNloptWrapperCpp::SetBounds(const double* theLB, const double* theUB)
	{
		if (mvOpt != NULL)
		{
			if (theLB != NULL)
			{
				nlopt_set_lower_bounds(mvOpt, theLB);
			}
			if (theUB != NULL)
			{
				nlopt_set_upper_bounds(mvOpt, theUB);
			}
		}
	}

	void cNloptWrapperCpp::AddEqualityConstraint(nlopt_func theConstraint, void* theConstrData, double theTol)
	{
		if (mvOpt != NULL)
		{
			nlopt_add_equality_constraint(mvOpt, theConstraint, theConstrData, theTol);
		}
	}

	void cNloptWrapperCpp::AddInequalityConstraint(nlopt_func theConstraint, void* theConstrData, double theTol)
	{
		if (mvOpt != NULL)
		{
			nlopt_add_inequality_constraint(mvOpt, theConstraint, theConstrData, theTol);
		}
	}

	void cNloptWrapperCpp::Optimize(double* theInitPoint, uint theDimension, cNLOPTAlgoParam& theAlgoParam, cNLOPTResult& theResult)
	{
		if (theResult.mXOptim == NULL)
			theResult.mXOptim = new double[theDimension];
		for (register uint i = 0; i < theDimension; i++)
			theResult.mXOptim[i] = theInitPoint[i];
		theResult.mDimension = theDimension;
		nlopt_set_stopval(mvOpt, theAlgoParam.mStopVal);
		nlopt_set_ftol_abs(mvOpt, theAlgoParam.mfTol);
		nlopt_set_ftol_rel(mvOpt, theAlgoParam.mfTol);
		nlopt_set_xtol_abs(mvOpt, &theAlgoParam.mxTol);
		nlopt_set_xtol_rel(mvOpt, theAlgoParam.mxTol);
		nlopt_set_maxeval(mvOpt, theAlgoParam.mMaxFuncEval);
		nlopt_set_maxtime(mvOpt, theAlgoParam.mMaxComputeTime);
	clock_t myStart = clock();
		theResult.mConvergenceStatus = nlopt_optimize(mvOpt, theResult.mXOptim, &theResult.mFunctValue);
		theResult.mComputeTime = (double)(clock() - myStart) / (double)CLOCKS_PER_SEC;
		if ((theResult.mConvergenceStatus >= NLOPT_SUCCESS) && (theResult.mConvergenceStatus < NLOPT_MAXEVAL_REACHED))
			theResult.mConvergence = true;
		else
			theResult.mConvergence = false;
		switch (theResult.mConvergenceStatus)
		{
			case NLOPT_FAILURE :
				sprintf(theResult.mConvergenceString, "generic failure");
			break;
			case NLOPT_INVALID_ARGS :
				sprintf(theResult.mConvergenceString, "Invalide arguments");
			break;
			case NLOPT_OUT_OF_MEMORY:
				sprintf(theResult.mConvergenceString, "Out of memory");
			break;
			case NLOPT_ROUNDOFF_LIMITED:
				sprintf(theResult.mConvergenceString, "Roundoff limited");
			break;
			case NLOPT_FORCED_STOP :
				sprintf(theResult.mConvergenceString, "Stop forced");
			break;
			case NLOPT_SUCCESS :
				sprintf(theResult.mConvergenceString, "Generic success");
			break;
			case NLOPT_STOPVAL_REACHED :
				sprintf(theResult.mConvergenceString, "Stop value reached");
			break;
			case NLOPT_FTOL_REACHED :
				sprintf(theResult.mConvergenceString, "fTol reached");
			break;
			case NLOPT_XTOL_REACHED :
				sprintf(theResult.mConvergenceString, "xTol reached");
			break;
			case NLOPT_MAXEVAL_REACHED :
				sprintf(theResult.mConvergenceString, "max function eval reached");
			break;
			case NLOPT_MAXTIME_REACHED :
				sprintf(theResult.mConvergenceString, "max time reached");
			break;
			default :
			break;
		}


	}

} // namespace

