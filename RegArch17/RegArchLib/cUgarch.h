#pragma once 
#ifndef _CUGARCH_H_
#define _CUGARCH_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
/*!
	\file cUgarch.h
	\brief Definition of the GARCH(p, q) class
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date sept-26-2016 - Last change feb-26-2016
*/
namespace RegArchLib {

	/*! 
	 * \class cUgarch
	 * \brief Class to implement a UGARCH(p, q) model
	 */
	class cUgarch: public cAbstCondVar
	{
	private :
        bool   mvExistConst ; ///< true if there is a constant
		double mvConst ; ///< Constant part of GARCH(p, q) variance model.
		cDVector mvBetaS ; ///< vector of betas in the variance equation
        cDVector mvArch ; ///< Vector of ARCH coefficients. 
		cDVector mvGarch ; ///< Vector of GARCH coefficients.
	public :
		cUgarch(bool theExistConst=true, uint theNBetas = 0, uint theNArch = 0, uint theNGarch=0) ; ///< A simple constructor
		cUgarch(bool theExistConst, double theConst, cDVector& theBetaS, cDVector& theArch, cDVector& theGarch) ; ///< Another constructor
		cUgarch(const cUgarch& theGarch);
		virtual ~cUgarch() ; ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this				
		void Delete(void) ; /// Delete
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void SetDefaultInitPoint(cRegArchValue& theValue);
		void ReAlloc(const uint theSize, const uint theNumParam = 0); ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam);
		cUgarch& operator=(const cUgarch& theSrc); ///< Standard affectation
		void UpdateProxyVarParameters(void) {}; 
		double ComputeVar(uint theDate, const cRegArchValue& theData) const;	///< Return conditional variance.
		uint GetNParam(void) const ; ///< Number of parameters in that model part
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time t.
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	} ;

}
#endif // _CUGARCH_H_
