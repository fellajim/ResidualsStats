#pragma once


#include <nlopt.hpp>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <climits>

#ifdef _MESS_CREAT
	#ifdef _DEBUG
		#ifndef _RDLL_
			#define MESS_CREAT(p)  std::cout << "creation de " << p << std::endl ;
			#define MESS_DESTR(p)  std::cout << "destruction de " << p << std::endl ;
		#else
			#define MESS_CREAT(p) Rprintf("Creation de %s\n", p) ;
			#define MESS_DESTR(p)  Rprintf("Destruction de %s\n", p) ;
		#endif // _RDLL
	#else
		#define MESS_CREAT(p) ///< used for debugging 
		#define MESS_DESTR(p) ///< used for debugging
	#endif //_DEBUG
#else
	#define MESS_CREAT(p) ///< used for debugging
	#define MESS_DESTR(p) ///< used for debugging
#endif //_MESS_CREAT

#ifdef _DEBUG
	#ifdef _MESS
		#ifdef _RDLL_
			#define MESS(p) Rprintf("%s\n", p) ;
		#else
			#define MESS(p) std:cout << p << std:endl;
		#endif //_RDLL
	#else
		#define MESS(p)
	#endif // _MESS
#else
	#define MESS(p)
#endif //_DEBUG

#ifndef _UINT_
	#define _UINT_
	typedef unsigned int uint;
#endif // _UINT_


namespace WrapperNloptCppNamespace
{
	class cNLOPTResult
	{
	public :
		uint			mDimension;
		bool			mConvergence;
		double*			mXOptim;
		double			mFunctValue;
		nlopt_result	mConvergenceStatus;
		char			mConvergenceString[100];
		double			mComputeTime;
	public :
		cNLOPTResult();
		cNLOPTResult(uint theDimension);
		~cNLOPTResult();
	}; // class cNLOPTResult

	class cNLOPTAlgoParam
	{
	public :
		nlopt_algorithm mAlgo;
		bool mMinimisation;
		double mStopVal;
		double mfTol;
		double mxTol;
		double mMaxFuncEval;
		double mMaxComputeTime;
		bool mVerbose;

	public :
		cNLOPTAlgoParam();
		cNLOPTAlgoParam(nlopt_algorithm theAlgo, bool theMinimisation=true, double theStopValue=1e200, double thefTol=1e-3, double thexTol = 1e-3, uint theMaxFuncEval=INT_MAX, double theComputeTimeMax=60, bool theVerbose=false);
		virtual ~cNLOPTAlgoParam();

	}; // class cNLOPTAlgoParam
	
	class cNloptWrapperCpp
	{
	private:
		nlopt_opt mvOpt;
		
	public:
		cNloptWrapperCpp();
		cNloptWrapperCpp(nlopt_algorithm theAlgo, uint theDimension, nlopt_func theObjective, bool theMin=true, void* theFuncData=NULL);
		~cNloptWrapperCpp();
		void SetAlgorithm(nlopt_algorithm theAlgo, uint theDimension);
		void SetObjectiveFunc(nlopt_func theObjective, bool theMin = true, void* theFuncData = NULL);
		void SetBounds(const double* theLB=NULL, const double* theUB=NULL);
		void AddEqualityConstraint(nlopt_func theConstraint, void* theConstrData, double theTol = 1e-8);
		void AddInequalityConstraint(nlopt_func theConstraint, void* theConstrData, double theTol = 1e-8);
		void Optimize(double* theInitPoint, uint theDimension, cNLOPTAlgoParam& theAlgoParam, cNLOPTResult& theResult);

	}; // class cNloptWrapperCpp
} // namespace WrapperNloptCppNamespace
