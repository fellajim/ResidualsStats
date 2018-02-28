#pragma once 
#ifndef _CSTDDEVINMEAN_H_
#define _CSTDDEVINMEAN_H_

#include "cAbstCondMean.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
/*!
	\file cStdDevInMean.h
	\brief header for class cStdDevInMean.
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	class cStdDevInMean : public cAbstCondMean
	{
	private :
		double mvStdDevInMean ;
	public :
		cStdDevInMean(double theStdDevInMean = 0.0) ;
		cStdDevInMean(const cStdDevInMean& theStdDevInMean) ; /// Recopy constructor
		~cStdDevInMean() ;
		cAbstCondMean* PtrCopy(void) const ; /// < Return a copy of *this
		void Delete(void) ;
	#ifndef _RDLL_
		void Print(ostream& theOut=cout) const ;
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void SetDefaultInitPoint(cRegArchValue& theValue);
		void Set(const double theValue, const uint theIndex = 0, const uint theNumParam = 0); ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam);
		void ReAlloc(const uint theSize, const uint theNumParam=0) ;
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ;
		cStdDevInMean& operator=(const cStdDevInMean& theSrc) ;
		void UpdateProxyMeanParameters(void) {};
		double ComputeMean(uint theDate, const cRegArchValue& theData) const ;
		uint GetNParam(void) const ; ///< Return number of parameters
		uint GetNLags(void) const ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	} ;
}

#endif // _CStdDevInMean_H_
