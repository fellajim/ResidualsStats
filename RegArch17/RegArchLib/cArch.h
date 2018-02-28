#pragma once 
#ifndef _CARCH_H_
#define _CARCH_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"

	/*!
		\file cArch.h
		\brief Definition of the ARCH(p) class
	
		\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
		\date dec-18-2006 - last change nov-10-2009
	*/
namespace RegArchLib {
	/*! 
	 * \class cArch
	 * \brief Class to implement a 'pure' ARCH model
	 */
	class cArch: public cAbstCondVar
	{
	private :
		double mvConst ; ///< Constant part of ARCH(p) variance model.
		cDVector mvArch ; ///< Vector of ARCH coefficients. 
	public :
		cArch(uint theNArch = 0) ; ///< A simple constructor
		cArch(const cArch& theArch) ; 
		virtual ~cArch() ; ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this		
		void Delete(void) ; ///< Free memory
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar);
		void SetDefaultInitPoint(cRegArchValue& theValue);
		void Set(const double theValue, const uint theIndex = 0, const uint theNumParam = 0); ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam); 
		void ReAlloc(const uint theSize, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters
		cArch& operator=(const cArch& theSrc); ///< Standard affectation
		void UpdateProxyVarParameters(void) {}; 
		double ComputeVar(uint theDate, const cRegArchValue& theData) const; ///< Return conditional variance.
		uint GetNParam(void) const ; ///< Number of parameters in that model part
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	} ;
}

#endif // _CARCH_H_
