#pragma once 
#ifndef _GSLANDNLOPTOPTIM_H_
#define _GSLANDNLOPTOPTIM_H_
#include "RegArchDef.h"
#include "cGSLVector.h"
/*!
	\file gslOptim.h
	\brief Some definitions of gsl optimization functions
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	typedef struct sParamOptimStruct
	{	cRegArchModel*		mParam ;
		cRegArchValue*		mValue ;
		bool				mVerbose;
		int					mNFuncEval;
	}sParamOptimStruct, *pParamOptimPtr ;

//	#ifdef _RDLL_
//		double GslLLHFunction(int n, double *par, void *ex) ;
//		void GslGradLLHFunction(int n, double *par, double *gr, void *ex);
//	#else
		double GslLLHFunction(const gsl_vector* theParam, void* theData) ;
		double NloptLLHFunction(uint theSize, double* theParam, void* theData);
		void GslGradLLHFunction(const gsl_vector* theParam, void* theData, gsl_vector* theGrad) ;
		double NloptLLHAndGradLLHFunction(uint theSize, double* theParam, double* theGrad, void* theData);
		void GslLLHAndGradLLHFunction(const gsl_vector* theParam, void* theData, double* theLLH, gsl_vector* theGrad)  ;
//	#endif //_RDLL_
}


#endif // _GSLANDNLOPTOPTIM_H_
