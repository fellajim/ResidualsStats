#include "StdAfxRegArchLib.h"
/*!
\file cSkewtResiduals.cpp
\brief implementation of the class for Skewt conditional distribution.

\author Jean-Baptiste DURAND, Ollivier TARAMASCO
\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	

	static double cum_skewt(double theX, double theDof, double theGamma)
	{
		double myGamma2 = theGamma*theGamma;
		if (theX < 0)
			return 2.0/(myGamma2+1.0)*gsl_cdf_tdist_P(theGamma*theX, theDof);
		else
			return 1.0/(myGamma2+1.0)+2.0/(1.0+1.0/myGamma2)*(gsl_cdf_tdist_P(theX/theGamma, theDof) - 0.5);

	}

	static double inv_cum_skewt(double theProb, double theDof, double theGamma)
	{
	double myProbaZero = cum_skewt(0, theDof, theGamma);
		if (theProb < myProbaZero)
			return gsl_cdf_tdist_Pinv((theGamma*theGamma + 1) / 2 * theProb, theDof) / theGamma;
		else
			return  theGamma * gsl_cdf_tdist_Pinv((1 + 1 / (theGamma*theGamma)) / 2 * (theProb - myProbaZero) + 0.5, theDof);
	}


	static double sim_skewt(const gsl_rng* theGslRng, double theDof, double theGamma)
	{
		double myU = gsl_ran_flat(theGslRng, 0.0, 1.0);
		return inv_cum_skewt(myU, theDof, theGamma);
	}


	static double SkewtExpect(double theDof, double theGamma)
	{
		return 2 * (theGamma*theGamma - 1)*sqrt(theDof)*gsl_sf_gamma((theDof + 1) / 2) / (SQRT_PI*theGamma*(theDof - 1)*gsl_sf_gamma(theDof / 2));
	}

	static double SkewtVar(double theDof, double theGamma)
	{
	double myMean = SkewtExpect(theDof, theGamma);
		return theDof*(pow(theGamma, 4) - theGamma*theGamma + 1) / ((theDof - 2)*theGamma*theGamma) - myMean*myMean;
	}

	/*!
	* \fn cSkewtResiduals::cSkewtResiduals(double theDof, bool theSimulFlag)
	* \param double theDof: number of degrees of freedom
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cSkewtResiduals::cSkewtResiduals(double theDof, double theGamma, bool theSimulFlag) : cAbstResiduals(eSkewt, NULL, theSimulFlag)
	{
		mDistrParameter.ReAlloc(2);
		mDistrParameter[0] = theDof;
		mDistrParameter[1] = theGamma;
		MESS_CREAT("cSkewtResiduals")
	}


	/*!
	* \fn cSkewtResiduals::cSkewtResiduals(const cDVector* theDistrParameter, bool theSimulFlag): cAbstResiduals(eSkewt, theDistrParameter, theSimulFlag)
	* \param const cDVector* theDistrParameter: theDistrParameter[0] = d.o.f.
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cSkewtResiduals::cSkewtResiduals(cDVector* theDistrParameter, bool theSimulFlag) : cAbstResiduals(eSkewt, theDistrParameter, theSimulFlag)
	{
		MESS_CREAT("cSkewtResiduals")
	}

	/*!
	* \fn cSkewtResiduals::~cSkewtResiduals()
	*/
	cSkewtResiduals::~cSkewtResiduals()
	{
		MESS_DESTR("cSkewtResiduals")
	}

	/*!
	* \fn cAbstCondVar* cNormResiduals::::PtrCopy()
	*/

	cAbstResiduals* cSkewtResiduals::PtrCopy() const
	{
	cSkewtResiduals *mycSkewtResiduals = NULL;
	cDVector* myDistrParameter = new cDVector(mDistrParameter);

	bool mySimulFlag = false;

		if (mtR != NULL)
			mySimulFlag = true;

		mycSkewtResiduals = new cSkewtResiduals(myDistrParameter, mySimulFlag);

		delete myDistrParameter;

		return mycSkewtResiduals;
	}

	/*!
	* \fn void cSkewtResiduals::Generate(const uint theNSample, cDVector& theYt) const
	* \param const uint theNSample: the sample size
	* \param cDVector& theYt: the output vector
	*/
	void cSkewtResiduals::Generate(const uint theNSample, cDVector& theYt) const
	{
		theYt.ReAlloc(theNSample);
		if (mDistrParameter[0] <= 2.0)
			throw cError("wrong d.o.f.");

	double myMean = SkewtExpect(mDistrParameter[0], mDistrParameter[1]);
	double myStd = sqrt(SkewtVar(mDistrParameter[0], mDistrParameter[1]));

		for (register uint t = 0; t < theNSample; t++)
			theYt[t] = (sim_skewt(mtR, mDistrParameter[0], mDistrParameter[1]) - myMean) / myStd;
	}

	/*!
	* \fn void cSkewtResiduals::Print(ostream& theOut) const
	* \param ostream& theOut: the output stream, default cout.
	*/
#ifndef _RDLL_
	void cSkewtResiduals::Print(ostream& theOut) const
	{
		theOut << "Conditional Skew T Distribution with:" << endl;
		cout << " d.o.f. = " << mDistrParameter[0] << endl;
		cout << " gamma = " << mDistrParameter[1] << endl;
	}
#else
	void cSkewtResiduals::Print(void)
	{
		Rprintf("Conditional Skewt Distribution with:\n");
		Rprintf(" d.o.f. = %f\n", mDistrParameter[0]);
		Rprintf(" gamma = %f\n", mDistrParameter[0]);
	}
#endif // _RDLL_

	void cSkewtResiduals::SetDefaultInitPoint(void)
	{
		mDistrParameter[0] = 10.0;
		mDistrParameter[1] = 1.5;
	}

	static double SkewtLogDensity(double theX, double theDof, double theGamma)
	{
	double myRes;
		if (theX < 0)
			myRes = 2.0 / (theGamma + 1 / theGamma)*gsl_ran_tdist_pdf(theX*theGamma, theDof);
		else
			myRes = 1.0 / (theGamma + 1 / theGamma)*gsl_ran_tdist_pdf(theX / theGamma, theDof);
			return log(myRes);
	}

	double cSkewtResiduals::LogDensity(double theX) const
	{
	double myEsp = SkewtExpect(mDistrParameter[0], mDistrParameter[1]);
	double myStd = sqrt(SkewtVar(mDistrParameter[0], mDistrParameter[1]));
		return log(myStd) + SkewtLogDensity(theX*myStd + myEsp, mDistrParameter[0], mDistrParameter[1]);
	}

	/*!
	* \fn double cSkewtResiduals::GetNParam(void) const
	* \param void.
	* \brief return 1: One parameter for St(n) residuals.
	*/
	uint cSkewtResiduals::GetNParam(void) const
	{
		return 2;
	}

	/*!
	* \fn static void SkewtGradLogDensity(double theX, double theDof, cDVector& theGrad)
	* \brief Compute the derivative of log density of a Skewt distribution (unstandardized)
	* with respect to the random variable (theGrad[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGrad)
	* \param theX double: value of the random variable
	* \param theDof double: value of the distribution parameter
	* \param theGrad cDVector&: concatenation of derivatives with respect to the random variable and the model parameters
	*/
	static void SkewtGradLogDensity(double theX, double theDof, cDVector& theGrad)
	{
	}

	double cSkewtResiduals::DiffLogDensity(double theX) const
	{
		return 0;
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
	}

	/*!
	* \fn void cSkewtResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData)
	* \brief Compute the derivative of log density with respect to the random variable (theGradData[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGradData)
	* \param theDate uint: time at which gradient is computed
	* \param theValue const cRegArchValue&: value of the random variable
	* \param theGradData cRegArchGradient&: concatenation of derivatives with respect to the random variable and the model parameters
	*/
	void cSkewtResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData) const
	{
	}

	void cSkewtResiduals::RegArchParamToVector(cDVector& theDestVect, uint theIndex) const
	{
		theDestVect[theIndex] = mDistrParameter[0];
		theDestVect[theIndex+1] = mDistrParameter[1];
	}

	void cSkewtResiduals::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		mDistrParameter[0] = theSrcVect[theIndex];
		mDistrParameter[1] = theSrcVect[theIndex+1];
	}

	double cSkewtResiduals::ComputeEspAbsEps(void)
	{
		return 0;
	}

	void cSkewtResiduals::ComputeGradBetaEspAbsEps(cDVector& theGrad)
	{
	}

	double cSkewtResiduals::Diff2LogDensity(double theX) const
	{
		return 0.0;
	}

	void cSkewtResiduals::GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad)
	{
	}

	static void HessLogDensity(double theX, cDMatrix& theHess, const cDVector& theDistrParam, uint theBegIndex)
	{
	}

	void cSkewtResiduals::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	}

	void cSkewtResiduals::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "SkewT d.o.f.");
		sprintf(theName[myIndex++], "SkewT GAMMA");
	}

}//namespace
