#pragma once 
#ifndef _CABSTCONDVAR_H_
#define _CABSTCONDVAR_H_

#include "RegArchDef.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"
/*!
	\file cAbstCondVar.h
	\brief Definition of the abstract class to implement conditional variance
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - last change feb-18-2011
*/

namespace RegArchLib {
	
	class cAbstResiduals ;

	/*! 
	 * \class cAbstCondVar
	 * \brief Abstract class to implement a conditional variance model
	 */
	class cAbstCondVar
	{
	private :
		eCondVarEnum	mvCondVar ; ///< Conditional variance type code
	public :
		cAbstCondVar(eCondVarEnum theType = eNotKnown) ; ///< A simple constructor
		virtual ~cAbstCondVar() = 0;  ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const = 0 ; /// < Return a copy of *this		
		eCondVarEnum GetCondVarType(void) const ; ///< Return the variance type code
		void SetCondVarType(eCondVarEnum theType);
		virtual void Delete(void) = 0 ; ///< delete
	#ifndef _RDLL_
		virtual void Print(ostream& theOut = cout) const = 0; ///< print the parameters
		friend ostream& operator <<(ostream& theOut, const cAbstCondVar& theAbstCondVar); ///< Print the parameters
	#else
		virtual void Print(void) = 0;
	#endif //_RDLL_
		virtual void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) = 0 ; ///< Set model parameters.
		virtual void Set(const cDVector& theVectParam, const uint theNumParam=0) = 0 ; ///< Set model parameters.
		virtual double Get(const uint theIndex, const uint theNumParam) = 0 ;
		virtual cDVector& Get(const uint theNumParam) = 0;
		virtual void ReAlloc(const uint theSize, const uint theNumParam=0) = 0 ; ///< Allocation of the model parameters
		virtual void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) = 0 ; ///< Allocation of the model parameters
		virtual void UpdateProxyVarParameters(void) = 0; ///<  Only for Fractal variance models 
		virtual double ComputeVar(uint theDate, const cRegArchValue& theData) const=0 ; ///< Return conditional variance.
		/** Return the number of parameters in CondVar */
		virtual uint GetNParam(void) const = 0 ;
		virtual uint GetNLags(void) const = 0 ; ///< Number of past gradients required to compute gradient at current time t.
		virtual void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals) = 0 ;
		virtual void NumericComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals, double theh = 1e-3);
		virtual void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0)  = 0 ;
		virtual void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) = 0 ;
		virtual void SetDefaultInitPoint(double theMean, double theVar)=0;
		virtual void SetDefaultInitPoint(cRegArchValue& theValue) = 0;
		virtual void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals) = 0;
		virtual void GetParamName(uint theIndex, char** theName) = 0;
	};
	cAbstCondVar* CreateRealCondVar(eCondVarEnum theType);
	cAbstCondVar* CreateRealCondVar(cAbstCondVar& theAbstCondVar);
}
#endif // _CABSTCONDVAR_H_
