#include "StdAfxRegArchLib.h"

/*!
 \file cNormResiduals.cpp
 \brief implementation of the class for N(0, 1) conditional distribution.

 \author Jean-Baptiste DURAND, Ollivier TARAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cNormResiduals::cNormResiduals(const cDVector* theDistrParameter, bool theSimulFlag):cAbstResiduals(eNormal, NULL, theSimulFlag)
	 * \param bool theSimulFlag: true if created for simulation
	 * \details: mvBool is initialised by cAbstResiduals constructor and theDistrParameter is never used.
	 */
	cNormResiduals::cNormResiduals(const cDVector* theDistrParameter, bool theSimulFlag):cAbstResiduals(eNormal, NULL, theSimulFlag)
	{
		MESS_CREAT("cNormResiduals")
	}

	/*!
	 * \fn cNormResiduals::~cNormResiduals
	 * \details: nothing to do here
	 */
	cNormResiduals::~cNormResiduals()
	{
		MESS_DESTR("cNormResiduals")
	}

	/*!
	 * \fn cAbstCondVar* cNormResiduals::::PtrCopy()
	 */

	cAbstResiduals* cNormResiduals::PtrCopy() const
	{
		cNormResiduals *mycNormResiduals = NULL ;
		bool mySimulFlag = false ;

		if (mtR != NULL)
			mySimulFlag = true ;

		mycNormResiduals = new cNormResiduals(NULL, mySimulFlag);

		return mycNormResiduals;
	}

	/*!
	 * \fn void cNormResiduals::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
#ifndef _RDLL_
	void cNormResiduals::Print(ostream& theOut) const
	{
		theOut << "Conditional standardized normal distribution" << endl ;

	}
#else
	void cNormResiduals::Print(void)
	{
		Rprintf("Conditional standardized normal distribution\n");

	}
#endif // _RDLL_


	/*!
	 * \fn void cNormResiduals::Generate(const uint theNSample, cDVector& theYt) const 
	 * \brief Draw a sample of N(0, 1) residuals.
	 * \param const uint theNSample: the sample size.
	 * \param cDVector& theYt: output parameter
	 * \details: theYt is reallocated to size theNSample here.
	 */

	void cNormResiduals::SetDefaultInitPoint(void) 
	{
	}
	
	void cNormResiduals::Generate(const uint theNSample, cDVector& theYt) const 
	{
		theYt.ReAlloc(theNSample) ;


		for (register uint t = 0 ; t < theNSample ; t++)
			theYt[t] = gsl_ran_ugaussian(mtR) ;
	}

	/*!
	 * \fn double cNormResiduals::LogDensity(double theX) const
	 * \param double theX: the point where density is computed.
	 * \brief Compute the log density of N(0, 1)
	 */
	double cNormResiduals::LogDensity(double theX) const
	{
		return(-LOG_SQRT_2_PI - theX*theX/2.0) ;
	}

	/*!
	 * \fn double cNormResiduals::GetNParam(void)
	 * \param void.
	 * \brief return 0: no parameter for N(0,1) residuals.
	 */
	uint cNormResiduals::GetNParam(void) const
	{
		return 0 ;
	}

	/*!
	 * \fn static void GradLogDensity(double theX, cDVector& theGrad)
	 * \brief Compute the derivative of log density of a Gaussian distribution with respect to the random variable (theGrad[0])
	 * \e and the gradient of log density with respect to the model parameters (other components in theGrad)
	 * \param theX double: value of the random variable
	 * \param theGrad cDVector&: concatenation of derivatives with respect to the random variable and the model parameters
	 */
	static void GradLogDensity(double theX, cDVector& theGrad)
	{
		theGrad[0] = -theX ;
	}

	/*!
	* \fn cNormResiduals::DiffLogDensity(double theX) const
	* \brief Compute the derivative of log density of a Gaussian distribution with respect to the random variable 
	* \param theX double: value of the random variable
	*/
	double cNormResiduals::DiffLogDensity(double theX) const
	{
		return -theX;
	}

	/*!
	 * \fn void cNormResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData)
	 * \brief Compute the derivative of log density with respect to the random variable (theGradData[0]) \e and the gradient
	 * of log density with respect to the model parameters (other components in theGradData)
	 * \param theDate uint: time at which gradient is computed
	 * \param theValue const cRegArchValue&: value of the random variable
	 * \param theGradData cRegArchGradient&: concatenation of derivatives with respect to the random variable and the model parameters
	 */
	void cNormResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData) const
	{
		theGradData.mCurrentDiffLogDensity = DiffLogDensity(theValue.mEpst[theDate]);
	}

	void cNormResiduals::RegArchParamToVector(cDVector& theDestVect, uint theIndex) const
	{
	}

	void cNormResiduals::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	}

	double cNormResiduals::ComputeEspAbsEps(void)
	{
		return 2.0/SQRT_2_PI ;
	}

	void cNormResiduals::ComputeGradBetaEspAbsEps(cDVector& theGrad)
	{
	}

	double cNormResiduals::Diff2LogDensity(double theX) const
	{
		return -1.0;
	}

	void cNormResiduals::GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad)
	{

	}

	void cNormResiduals::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{

	}

	void cNormResiduals::GetParamName(uint theIndex, char** theName)
	{
	}

}//namespace
