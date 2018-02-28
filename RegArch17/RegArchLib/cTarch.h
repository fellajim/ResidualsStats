#pragma once 
#ifndef _CTARCH_H_
#define _CTARCH_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

/*!
	\file cTarch.h
	\brief Definition of the TARCH(p) class
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - last change feb-18-2011
*/

namespace RegArchLib {

	/**
	 * Class to implement TARCH conditional variance
	 * (including additive constant)
	 */
	class cTarch : public cAbstCondVar
	{
	private :
		double mvCste ; ///< constant component of conditional variance
		cDVector mvArchPos ; ///< vector of TARCH coefficients.
		cDVector mvArchNeg ; ///< vector of TARCH coefficients.
	public :
		cTarch(int theNTarch = 0) ; ///< a simple constructor
		cTarch(const cTarch& theTarch);
		virtual ~cTarch() ; ///< a simple destructor
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this				
		void Delete(void) ; ///< delete
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void SetDefaultInitPoint(cRegArchValue& theValue);
		/** Change model parameters.
		 * Vector of parameters is resized to size theParam[0] */
		void ReAlloc(const uint theSize, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters

		/** Change the components for conditional variance.
		 theSrc cannot be const due to the dynamic_cast */
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam);
		void UpdateProxyVarParameters(void) {}; 
		cTarch& operator=(const cTarch& theSrc) ;
		/** Return conditional variance.
		 * \param theDate Date for conditional variance computation
		 * \param theValue Instance where the dataset is stored */
		double ComputeVar(uint theDate, const cRegArchValue& theValue) const ;
	
		uint GetNParam(void) const ; ///< Return number of parameters

		/** Return the number of steps in the past required, 
		 * \e e.g., for likelihood computation */
		uint GetNLags(void) const ;

		/** Conversion from a cTarch instance to a vector of parameters */
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0) ;

		/** Change parameter of cTarch instance according to given vector of parameters */
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;

		/** Compute gradient of conditional variance with respect to parameters
		 * \param theDate Date of interest for the process 
		 * \parame theValue Instance where the dataset is stored
		 * \param theGradData Instance where the result is stored (in theGradData.mCurrentGradVar
		 * \param theResids conditional distribution used only for non normal EGTARCH models
		*/
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);		
		void GetParamName(uint theIndex, char** theName);
	} ;
}
#endif // _CTARCH_H_
