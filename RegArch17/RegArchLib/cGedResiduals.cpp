#include "StdAfxRegArchLib.h"
/*!
\file cGedResiduals.cpp
\brief implementation of the class for Ged conditional distribution.

\author Jean-Baptiste DURAND, Ollivier TARAMASCO
\date dec-18-2006 - Last change mar-25-2016
*/
namespace RegArchLib {
	/*!
	* \fn cGedResiduals::cGedResiduals(double theDof, bool theSimulFlag)
	* \param double theBeta: beta parameter
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cGedResiduals::cGedResiduals(double theBeta, bool theSimulFlag) : cAbstResiduals(eGed, NULL, theSimulFlag)
	{
		mDistrParameter.ReAlloc(1);
		mDistrParameter[0] = theBeta;
		MESS_CREAT("cGedResiduals")
	}

	/*!
	* \fn cGedResiduals::cGedResiduals(const cDVector* theDistrParameter, bool theSimulFlag): cAbstResiduals(eGed, theDistrParameter, theSimulFlag)
	* \param const cDVector* theDistrParameter: theDistrParameter[0] = d.o.f.
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cGedResiduals::cGedResiduals(cDVector* theDistrParameter, bool theSimulFlag) : cAbstResiduals(eGed, theDistrParameter, theSimulFlag)
	{
		MESS_CREAT("cGedResiduals")
	}

	/*!
	* \fn cGedResiduals::~cGedResiduals()
	*/
	cGedResiduals::~cGedResiduals()
	{
		MESS_DESTR("cGedResiduals")
	}

	/*!
	* \fn cAbstCondVar* cNormResiduals::::PtrCopy()
	*/

	cAbstResiduals* cGedResiduals::PtrCopy() const
	{
		cGedResiduals *mycGedResiduals = NULL;
		cDVector* myDistrParameter = new cDVector(mDistrParameter);

		bool mySimulFlag = false;

		if (mtR != NULL)
			mySimulFlag = true;

		mycGedResiduals = new cGedResiduals(myDistrParameter, mySimulFlag);

		delete myDistrParameter;

		return mycGedResiduals;
	}

	/*!
	* \fn void cGedResiduals::Generate(const uint theNSample, cDVector& theYt) const
	* \param const uint theNSample: the sample size
	* \param cDVector& theYt: the output vector
	*/
	void cGedResiduals::Generate(const uint theNSample, cDVector& theYt) const
	{
		theYt.ReAlloc(theNSample);

		double myAlpha = sqrt(gsl_sf_gamma(1 / mDistrParameter[0]) / gsl_sf_gamma(3 / mDistrParameter[0]));
		for (register uint t = 0; t < theNSample; t++)
			theYt[t] = gsl_ran_exppow(mtR, myAlpha, mDistrParameter[0]);
	}

	/*!
	* \fn void cGedResiduals::Print(ostream& theOut) const
	* \param ostream& theOut: the output stream, default cout.
	*/
#ifndef _RDLL_
	void cGedResiduals::Print(ostream& theOut) const
	{
		theOut << "Conditional Ged Distribution with: " << endl;
		theOut << " Beta=" << mDistrParameter[0] << endl;
	}
#else
	void cGedResiduals::Print(void)
	{
		Rprintf("Conditional Ged Distribution with:\n");
		Rprintf(" Beta=%d\n", mDistrParameter[0]);

	}
#endif // _RDLL_

	void cGedResiduals::SetDefaultInitPoint(void)
	{
		mDistrParameter[0] = 1.5;
	}

	static double GedDensity(double theX, double theBeta)
	{
		double myAlpha = sqrt(gsl_sf_gamma(1.0 / theBeta) / gsl_sf_gamma(3.0 / theBeta));
		return theBeta*exp(-(pow(fabs(theX) / myAlpha, theBeta))) / (2 * myAlpha*gsl_sf_gamma(1.0 / theBeta));

	}

	double cGedResiduals::LogDensity(double theX) const
	{
		double myBeta = mDistrParameter[0];
		double myGamma1 = gsl_sf_gamma(1.0 / myBeta);
		double myAlpha = sqrt(myGamma1 / gsl_sf_gamma(3.0 / myBeta));
		return log(myBeta) - pow(fabs(theX) / myAlpha, myBeta) - log(2) - log(myGamma1) - log(myAlpha);
	}

	/*!
	* \fn double cGedResiduals::GetNParam(void) const
	* \param void.
	* \brief return 2 : two parameters for Ged residuals.
	*/
	uint cGedResiduals::GetNParam(void) const
	{
		return 1;
	}

	double cGedResiduals::DiffLogDensity(double theX) const
	{
		double myBeta = mDistrParameter[0];
		double myAlpha = sqrt(gsl_sf_gamma(1.0 / myBeta) / gsl_sf_gamma(3.0 / myBeta));

		return -myBeta*pow(fabs(theX), myBeta) / (theX*pow(myAlpha, myBeta));
	}

	/*!
	* \fn static void GradLogDensity(double theX, cDVector& theGrad, cDVector& theDistrParam)
	* \brief Compute the derivative of log density with respect to the random variable (theGrad[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGrad)
	* \param theX double: value of the random variable
	* \param theGrad cDVector&: concatenation of derivatives with respect to the random variable and the model parameters
	* \param theDistrParam cDVector&: value of the distribution parameters
	*/
	static void GradLogDensity(double theX, cDVector& theGrad, const cDVector& theDistrParam, uint theBegIndex)
	{
		double myBeta = theDistrParam[0];
		double myGamma1 = gsl_sf_gamma(1.0 / myBeta);
		double myGamma3 = gsl_sf_gamma(3.0 / myBeta);
		double myPsi01 = gsl_sf_psi(1.0 / myBeta);
		double myPsi03 = gsl_sf_psi(3.0 / myBeta);
		double myAlpha = sqrt(myGamma1 / myGamma3);
		double myXpBeta = pow(fabs(theX), myBeta);
		double myAlphapBeta = pow(myAlpha, myBeta);
		double myBeta2 = myBeta*myBeta;
		/*
		double myRes = -(((myBeta*(myPsi01 - 3 * myPsi03) - myBeta2*log(myGamma1 / myGamma3))*myXpBeta + 2 * log(fabs(theX))*myBeta2*myXpBeta +
					myAlphapBeta*(3 * myPsi03 - 3 * myPsi01) - 2 * myBeta*myAlphapBeta) / (myAlphapBeta*myBeta2)) / 2.0;
		*/

		double myPart1 = myXpBeta*myBeta*(2 * myBeta*log(fabs(theX)) + myPsi01 - 3.0*myPsi03 - 2 * myBeta*log(myAlpha));
		double myPart2 = (3 * (myPsi03 - myPsi01) - 2 * myBeta)*myAlphapBeta;
		double myRes = -(myPart1 + myPart2) / (2 * myAlphapBeta*myBeta2);
		theGrad[theBegIndex] = myRes;
	}

	/*!
	* \fn void cGedResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData)
	* \brief Compute the derivative of log density with respect to the random variable (theGradData[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGradData)
	* \param theDate uint: time at which gradient is computed
	* \param theValue const cRegArchValue&: value of the random variable
	* \param theGradData cRegArchGradient&: concatenation of derivatives with respect to the random variable and the model parameters
	*/
	void cGedResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData) const
	{
		uint myBegIndex = theGradData.GetNMeanParam() + theGradData.GetNVarParam();
		GradLogDensity(theValue.mEpst[theDate], theGradData.mCurrentGradDens, mDistrParameter, myBegIndex);
		theGradData.mCurrentDiffLogDensity = DiffLogDensity(theValue.mEpst[theDate]);
	}

	void cGedResiduals::RegArchParamToVector(cDVector& theDestVect, uint theIndex) const
	{
		if (theDestVect.GetSize() < theIndex - 1)
			throw cError("Wrong size");
		theDestVect[theIndex] = mDistrParameter[0];
	}

	void cGedResiduals::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		if (1 + theIndex > theSrcVect.GetSize())
			throw cError("Wrong size");
		mDistrParameter[0] = theSrcVect[theIndex];
	}

	double cGedResiduals::ComputeEspAbsEps(void)
	{
		double myBeta = mDistrParameter[0];
		return gsl_sf_gamma(2 / myBeta) / sqrt(gsl_sf_gamma(1 / myBeta)*gsl_sf_gamma(3 / myBeta));
	}

	void cGedResiduals::ComputeGradBetaEspAbsEps(cDVector& theGrad)
	{
		double myBeta = mDistrParameter[0];
		double myGamma1 = gsl_sf_gamma(1.0 / myBeta);
		double myGamma2 = gsl_sf_gamma(2.0 / myBeta);
		double myGamma3 = gsl_sf_gamma(3.0 / myBeta);
		double myPsi01 = gsl_sf_psi(1.0 / myBeta);
		double myPsi02 = gsl_sf_psi(2.0 / myBeta);
		double myPsi03 = gsl_sf_psi(3.0 / myBeta);
		theGrad[0] = ((sqrt(myGamma1*myGamma3)*(3 * myGamma2*myPsi03 + myGamma2*(myPsi01 - 4 * myPsi02))) / (myBeta*myBeta * myGamma1*myGamma3)) / 2.0;
	}

	double cGedResiduals::Diff2LogDensity(double theX) const
	{
		double myBeta = mDistrParameter[0];
		double myAlpha = sqrt(gsl_sf_gamma(1.0 / myBeta) / gsl_sf_gamma(3.0 / myBeta));
		double myXpBeta = pow(fabs(theX), myBeta);
		double myAlphapBeta = pow(myAlpha, myBeta);
		return -myBeta*(myBeta - 1.0)*myXpBeta / (theX*theX*myAlphapBeta);
	}

	void cGedResiduals::GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad)
	{
		double myBeta = theDistrParam[0];
		double myGamma1 = gsl_sf_gamma(1.0 / myBeta);
		double myGamma3 = gsl_sf_gamma(3.0 / myBeta);
		double myPsi01 = gsl_sf_psi(1.0 / myBeta);
		double myPsi03 = gsl_sf_psi(3.0 / myBeta);
		double myAlpha = sqrt(myGamma1 / myGamma3);
		double myXpBeta = pow(fabs(theX), myBeta);
		double myAlphapBeta = pow(myAlpha, myBeta);

		theGrad[0] = -((((-3 * myPsi03) + myPsi01 - myBeta*log(myGamma1 / myGamma3) + 2)*myXpBeta + 2 * myBeta*log(fabs(theX))*myXpBeta) / (theX*myAlphapBeta)) / 2.0;
	}

	static void HessLogDensity(double theX, cDMatrix& theHess, const cDVector& theDistrParam, uint theBegIndex)
	{
		double myBeta = theDistrParam[0];
		double myGamma1 = gsl_sf_gamma(1.0 / myBeta);
		double myGamma3 = gsl_sf_gamma(3.0 / myBeta);
		double myPsi01 = gsl_sf_psi(1.0 / myBeta);
		double myPsi03 = gsl_sf_psi(3.0 / myBeta);
		double myPsi11 = gsl_sf_psi_1(1.0 / myBeta);
		double myPsi13 = gsl_sf_psi_1(3.0 / myBeta);
		double myAlpha = sqrt(myGamma1 / myGamma3);
		double myXpBeta = pow(fabs(theX), myBeta);
		double myAlphapBeta = pow(myAlpha, myBeta);
		double myBeta2 = myBeta*myBeta;
		double myBeta3 = myBeta*myBeta2;
		double myBeta4 = myBeta2*myBeta2;
		double myLogX = log(fabs(theX));
		double myLogAlpha2 = 2.0*log(myAlpha);

		theHess[theBegIndex][theBegIndex] = -(((myBeta*(18 * myPsi13 - 2 * myPsi11) + myBeta2*(9 * myPsi03*myPsi03 - 6 * myPsi01*myPsi03 + myPsi01*myPsi01) + myBeta3*(6 * myLogAlpha2*myPsi03
			- 2 * myLogAlpha2 * myPsi01) + myBeta4*myLogAlpha2*myLogAlpha2)*myXpBeta + myLogX*(myBeta3*(4 * myPsi01 - 12 * myPsi03) - 4 * myBeta4*myLogAlpha2)*myXpBeta
			+ 4 * myBeta4*myLogX*myLogX * myXpBeta + myAlphapBeta*(6 * myPsi11 - 18 * myPsi13) + myBeta*myAlphapBeta*(12 * myPsi01 - 12 * myPsi03) + 4 * myAlphapBeta*myBeta2)
			/ (myAlphapBeta*myBeta4)) / 4.0;
	}

	void cGedResiduals::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
		cDVector myGradDiffLogDens = cDVector(1);
		GradDiffLogDensity(theData.mEpst[theDate], mDistrParameter, myGradDiffLogDens);
		uint myBegIndex = theHessData.GetNMeanParam() + theHessData.GetNVarParam();
		theHessData.mCurrentGradDiffLogDensity[myBegIndex] = myGradDiffLogDens[0];
		double myX = theData.mEpst[theDate];
		HessLogDensity(myX, theHessData.mCurrentHessEps, mDistrParameter, myBegIndex);
	}

	void cGedResiduals::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "GED BETA");
	}
}//namespace
