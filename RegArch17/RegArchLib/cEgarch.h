#pragma once 
#ifndef _CEGARCH_H_
#define _CEGARCH_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

/*!
	\file cEgarch.h
	\brief Definition of the EGARCH(p,q) variance class
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

/**
 * Mod�le EGARCH(p, q)
 *
 * ln h(t) = Cste + Somme_{i=1 � p} Arch(i)*(Teta*Eps(t-i) + Gamma*(abs(Eps(t-i)-E|Esp(t-i)|) 
 *				+ Somme_{j=1 � q} Garch(j)* ln h(t-j)
 *
 */
namespace RegArchLib {
	class cEgarch : public cAbstCondVar
	{
	private :
		double		mvEspAbsEpsilont ;
		double		mvCste ;
		cDVector	mvArch ;
		cDVector	mvGarch ;
		double		mvTeta ;
		double		mvGamma ;
	public :
		cEgarch(int theNArch = 0, int theNGarch=0) ;
		cEgarch(cAbstResiduals* theResiduals, int theNArch = 0, int theNGarch=0) ;
		cEgarch(const cEgarch& theEgarch) ;
		virtual ~cEgarch() ;
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this				
		void Delete(void) ;
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
		cEgarch& operator=(const cEgarch& theSrc) ;
		void UpdateProxyVarParameters(void) {}; 
		double ComputeVar(uint theDate, const cRegArchValue& theValue) const ;
		uint GetNParam(void) const ; ///< Number of parameters in that model part
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time t.
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals) ;
		void SetEspAbsEps(double theEspAbsEps) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	} ;

}
#endif // _CEGARCH_H_
