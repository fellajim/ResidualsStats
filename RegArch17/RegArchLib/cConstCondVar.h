#pragma once 
#ifndef _CCONSTCONDVAR_H_
#define _CCONSTCONDVAR_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

/*!
	\file cConstCondVar.h
	\brief Definition of the constant variance class
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*! 
	 * \class cConstCondVar
	 * \brief Class to implement a constant conditional variance model
	 */
	class cConstCondVar: public cAbstCondVar
	{
	private :
		double mvCste ; //< Value of constant variance
	public :
		cConstCondVar(double theValue=1.0) ; ///< A simple constructor
		cConstCondVar(const cConstCondVar& theConstCondVar);
		virtual ~cConstCondVar() ; ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this		
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void SetDefaultInitPoint(cRegArchValue& theValue);
		void Delete(void); ///< Delete
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam); 
		void ReAlloc(const uint theSize, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters
		cConstCondVar& operator=(const cConstCondVar& theSrc); ///< Standard affectation
		void UpdateProxyVarParameters(void) {}; 
		double ComputeVar(uint theDate, const cRegArchValue& theData) const ; ///< Return conditional variance.
		uint GetNParam(void) const ; ///< Number of parameters in that model part
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time t.
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	} ;
}
#endif //_CCONSTCONDVAR_H_
