#pragma once 
#ifndef _CCONST_H_
#define _CCONST_H_
/*!
 \file cConst.h
 \brief header for class cConst.

 \author Jean-Baptiste Durand, Ollivier TARAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
#include "cAbstCondMean.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"


namespace RegArchLib {
	/*! 
	 * \class cConst
	 * \brief  Class to implement a constant mean model
	 */
	class cConst: public cAbstCondMean
	{
	private :
		double	mvConst	; ///< Const value
	public :
		cConst(double theVal = 0.0L) ; ///< A simple constructor
		cConst(const cConst& theConst) ; /// Recopy constructor
		virtual ~cConst() ; ///< A simple destructor
		cAbstCondMean* PtrCopy(void) const ; /// < Return a copy of *this
		void Delete(void) ;  ///< Free memory
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void SetDefaultInitPoint(cRegArchValue& theValue);
		void Set(const double theValue, const uint theIndex = 0, const uint theNumParam = 0); ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam);
		void ReAlloc(const uint theSize, const uint theNumParam) ; ///< New memory allocation of parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< New memory allocation of parameters
		cConst& operator=(const cConst& theSrc); ///< Standard affectation
		void UpdateProxyMeanParameters(void) {};
		double ComputeMean(uint theDate, const cRegArchValue& theData) const; ///< Compute the conditional mean value
		uint GetNParam(void) const ;
		uint GetNLags(void) const ;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	};
}
#endif //_CCONST_H_

