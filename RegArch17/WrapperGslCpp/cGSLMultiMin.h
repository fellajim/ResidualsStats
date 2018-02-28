#pragma once 
#ifndef _CGSLMULTIMIN_H_
#define _CGSLMULTIMIN_H_

	#include "cGSLVector.h"
	#include <gsl/gsl_multimin.h>

	/*!
		\file cGSLMultiMin.h
		\brief Definition of the cGSLMultimMin class
	
		\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
		\date dec-18-2006 - Last change feb-18-2011
	*/

	#ifndef cDVector
		#define cDVector cGSLVector 
		#define cDMatrix cGSLMatrix  
	#endif //cDVector

	namespace WrapperGslCppNameSpace 
	{
        using namespace VectorAndMatrixNameSpace;

		typedef enum eGSLMultiMinAlgoEnum
		{	eConjugateFR, ///< Fletcher-Reeves conjugate gradient algorithm
			eConjugatePR, ///< Polak-Ribiere conjugate gradient algorithm
			eBFGS, ///< vector Broyden-Fletcher-Goldfarb-Shanno (BFGS) algorithm
			eBFGSTwo, ///< the most efficient vector Broyden-Fletcher-Goldfarb-Shanno BFGS algorithm
			eSteepest, ///< the steepest descent algorithm 
			eSimplexNM, ///< Simplex algorithm of Nelder and Mead
			eSimplexNMTwo, ///< More efficient Simplex algorithm of Nelder and Mead
			eSimplexNMTwoRand ///< More efficient Simplex algorithm of Nelder and Mead with random initialization.
		}eGSLMultiMinAlgoEnum ;

		typedef struct sGSLMultiMinResult
		{	uint		mNIter						;
			bool		mConvergence				;
			double		mCritValue					;
			cDVector	mGrad						;
			double		mFunctValue					;
			double		mComputeTime				;
			int			mConvergenceStatus			;
			char		mConvergenceComment[100]	;
		}sGSLMultiMinResult ;

		typedef struct sGSLMultiMinAlgoParam
		{	eGSLMultiMinAlgoEnum	mAlgoType		;
			double					mStepSize		;
			double					mTol			;
			double					mStopValue		;
			uint					mNMaxIter		;
			uint					mNMaxSeconds	;
			bool					mVerbose		;
		}sGSLMultiMinAlgoParam ;

		typedef double (*GSLMultiMinF)(const gsl_vector * x, void * params)  ;
		typedef void (*GSLMultiMinDf)(const gsl_vector * x, void * params, gsl_vector * g)  ;
		typedef void (*GSLMultiMinFdf)(const gsl_vector * x, void * params, double * f, gsl_vector * g) ;

		class cGSLMultiMin
		{
			private :
				cDVector mvInitPoint ;
				bool mvFdf ;
				union uType
				{	const gsl_multimin_fdfminimizer_type* mvTypeFdf ;
					const gsl_multimin_fminimizer_type* mvTypeF ;
				}mvType ;	
				union uMinimizer
				{	gsl_multimin_fdfminimizer* mvStateFdf ;
					gsl_multimin_fminimizer* mvStateF ;
				}mvState ;
				union uFunct
				{	gsl_multimin_function_fdf mvFunctFdf ;
					gsl_multimin_function mvFunctF ;
				} mvFunct ;
				union uStepSize
				{	double mvStepDouble ;
					cDVector* mvStepVect ;
				}mvStepSize ;
				double mvTol ;
		
			public :
				cGSLMultiMin() ;
				cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function_fdf* theFunction = NULL, double theStepSize = 0.01, double theTol=0.1) ;
				cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function* theFunction, double theStepSize=0.1) ;
				cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function_fdf* theFunction, sGSLMultiMinAlgoParam* theAlgoParam) ;
				cGSLMultiMin(cDVector& theX0, eGSLMultiMinAlgoEnum theTypeAlgo, gsl_multimin_function* theFunction, sGSLMultiMinAlgoParam* theAlgoParam) ;
				void SetInitPoint(cDVector& theX0) ;
				void SetTypeAlgo(eGSLMultiMinAlgoEnum theTypeAlgo) ;
				void SetFunction(gsl_multimin_function_fdf* theFunction) ;
				void SetFunction(gsl_multimin_function* theFunction) ;
				void SetFunction(GSLMultiMinF theF, GSLMultiMinDf theDf, GSLMultiMinFdf theFdf, size_t theNParam, void* theOtherParam) ;
				void SetFunction(GSLMultiMinF theF, size_t theNParam, void* theOtherParam) ;
				void SetStepSize(double theStepSize) ;
				void SetTol(double theTol) ;
				void SetAlgoParam(sGSLMultiMinAlgoParam theAlgoParam) ;
				void GSLOptim(cDVector& theX, sGSLMultiMinResult& theResStruct, double theStopValue=1e-6, int theMaxIter = 100, bool theVerbose = false) ;
				void GSLOptim(cDVector& theX, sGSLMultiMinResult& theResStruct, sGSLMultiMinAlgoParam theAlgoParam) ;
				virtual ~cGSLMultiMin() ;
		};
	} // namespace
#endif //_CGSLMULTIMIN_H_
