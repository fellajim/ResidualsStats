#pragma once 
#ifndef _CGEDRESIDUALS_H_
#define _CGEDRESIDUALS_H_

#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_psi.h>
#include "cAbstResiduals.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

/*!
\file cGedResiduals.h
\brief Definition of the class for Ged conditional distribution.

\author Jean-Baptiste DURAND, Ollivier TARAMASCO
\date dec-18-2006 - Last change mar-08-2016
*/


namespace RegArchLib {

	/*!
	* \class cGedResiduals
	* \brief  Class to implement a Student distribution with unitary variance for residuals
	*/
	class _DLLEXPORT_ cGedResiduals : public cAbstResiduals
	{
	public:
		cGedResiduals(double theAlpha, double theBeta, bool theSimulFlag = true); ///< A constructor
		cGedResiduals(cDVector* theDistrParameter = NULL, bool theSimulFlag = true);	///< A constructor
		virtual ~cGedResiduals(); ///< A simple destructor
		virtual cAbstResiduals* PtrCopy() const; /// < Return a copy of *this
		void Print(ostream& theOut) const; ///< Print the distribution type
		void SetDefaultInitPoint(void);
		void Generate(const uint theNSample, cDVector& theYt) const; ///< Draw a sample from residual distribution 
		double LogDensity(double theX) const;
		/** Return the number of parameters in distribution */
		uint GetNParam(void) const;
		/** Compute the derivative of log density with respect to the random variable (theGradData[0]) \e and the gradient
		of log density with respect to the model parameters (other components in theGradData) */
		double DiffLogDensity(double theX) const;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData) const;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) const;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0);
		double ComputeEspAbsEps(void);
		void ComputeGradBetaEspAbsEps(cDVector &theGrad);
		double Diff2LogDensity(double theX) const;
		void GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
	};
}
#endif //_CGEDRESIDUALS_H_

