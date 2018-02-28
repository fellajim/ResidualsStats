#pragma once
#ifndef _CNORMRESIDUALS_H_
#define _CNORMRESIDUALS_H_
#include "cAbstResiduals.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
/*!
 \file cNormResiduals.h
 \brief Definition of the class for N(0, 1) conditional distribution.

 \author Jean-Baptiste DURAND, Ollivier TARAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	/*! 
	 * \class cNormResiduals
	 * \brief  Class to implement the N(0, 1) residuals
	 */

	class cNormResiduals: public cAbstResiduals
	{
	public :
		cNormResiduals(const cDVector* theDistrParameter=NULL, bool theSimulFlag=true) ; ///< a simple constructor
		virtual ~cNormResiduals() ; ///< a simple destructor
		virtual cAbstResiduals* PtrCopy() const ; /// < Return a copy of *this
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(void) ;
		void Generate(const uint theNSample, cDVector& theYt) const ; ///< Draw a sample from residual distribution 
		double LogDensity(double theX) const ;
		/** Return the number of parameters in distribution */
		uint GetNParam(void) const ;
		/** Compute the derivative of log density with respect to the random variable (theGradData[0]) \e and the gradient
            of log density with respect to the model parameters (other components in theGradData) */
		double DiffLogDensity(double theX) const;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData) const;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) const ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		double ComputeEspAbsEps(void) ;
		void ComputeGradBetaEspAbsEps(cDVector &theGrad) ;
		double Diff2LogDensity(double theX) const;
		void GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void GetParamName(uint theIndex, char** theName);
	};

}

#endif //_CNORMRESIDUALS_H_

