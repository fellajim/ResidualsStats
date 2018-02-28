#pragma once 
#ifndef _CABSTCONDMEAN_H_
#define _CABSTCONDMEAN_H_

#include "RegArchDef.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"
/*!
	\file cAbstCondMean.h
	\brief Definition of the abstract class to implement a conditional mean component
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - last change feb-18-2011
*/

namespace RegArchLib {

	class cAbstResiduals ;

	/*! 
	 * \class cAbstCondMean
	 * \brief Abstract class definition of a conditional mean model component
	 */
	class cAbstCondMean
	{
	private :
		eCondMeanEnum	mvCondMeanType ; ///<  Code for the mean type
	public :
		cAbstCondMean(const eCondMeanEnum theType=eUnknown) ; ///< A constructor
		virtual ~cAbstCondMean() ; ///< Destructor
		virtual cAbstCondMean* PtrCopy(void) const = 0 ; /// < Return a copy of *this
		eCondMeanEnum GetCondMeanType(void) const ; ///< Return the mean type code
		void SetCondMeanType(eCondMeanEnum theType)  ;
		virtual void Delete(void) = 0;
	#ifndef _RDLL_
		virtual void Print(ostream& theOut = cout) const = 0; ///< print the parameters
	#else
		virtual void Print(void) = 0;
	#endif //_RDLL_
		friend   ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean) ; ///< Print the parameters
		virtual void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) = 0 ; ///< Set model parameters.
		virtual void Set(const cDVector& theVectParam, const uint theNumParam=0) = 0 ; ///< Set model parameters.
		virtual double Get(const uint theIndex, const uint theNumParam) = 0 ;
		virtual cDVector& Get(const uint theNumParam) = 0;
		virtual void ReAlloc(const uint theSize, const uint theNumParam=0) = 0 ; ///< Allocation of the model parameters
		virtual void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) = 0 ; ///< Allocation of the model parameters
		virtual void UpdateProxyMeanParameters(void) = 0;
		virtual double ComputeMean(uint theDate, const cRegArchValue& theData) const = 0 ; ///< Compute conditional mean.
		virtual uint GetNParam(void) const = 0 ; ///< Number of Parameters
		virtual uint GetNLags(void) const = 0 ; ///< Number of past gradients required to compute gradient at current time t.
		virtual void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) = 0 ;
		virtual void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0) = 0 ;
		virtual void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) = 0 ;
		virtual void SetDefaultInitPoint(double theMean, double theVar)=0 ;
		virtual void SetDefaultInitPoint(cRegArchValue& theValue) = 0;
		virtual void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) = 0;
		virtual void GetParamName(uint theIndex, char** theName) = 0;
	} ;
	extern cAbstCondMean* CreateOneRealCondMean(const eCondMeanEnum theType);
	extern cAbstCondMean* CreateOneRealCondMean(cAbstCondMean& theAbstCondMean);
}

#endif // _CABSTCONDMEAN_H_


