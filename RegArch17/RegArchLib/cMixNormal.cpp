#include "StdAfxRegArchLib.h"
/*!
\file cMixNormal.cpp
\brief implementation of the class for Student conditional distribution.
*/
namespace RegArchLib {
	/*!
	* \fn cMixNormal::cMixNormal(double p, double sigma_1, double sigma_2, bool theSimulFlag)
	* \param double p: bernoulli proba
	* \param double sigma_1: std of the first variable
	* \param double sigma_2: std of the seconde variable
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cMixNormal::cMixNormal(double p, double sigma_1, double sigma_2, bool theSimulFlag) : cAbstResiduals(eMixNormal, NULL, theSimulFlag)
	{
		mDistrParameter.ReAlloc(3);
		mDistrParameter[0] = p;
		mDistrParameter[1] = sigma_1;
		mDistrParameter[2] = sigma_2;

		MESS_CREAT("cMixNormal")
	}

	/*!
	* \fn cMixNormal::cMixNormal(cDVector* theDistrParameter, bool theSimulFlag) : cAbstResiduals(eStudent, theDistrParameter, theSimulFlag)
	* \param const cDVector* theDistrParameter: theDistrParameter[0] = d.o.f.
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cMixNormal::cMixNormal(cDVector* theDistrParameter, bool theSimulFlag) : cAbstResiduals(eStudent, theDistrParameter, theSimulFlag)
	{
		MESS_CREAT("cMixNormal")
	}

	/*!
	* \fn cMixNormal::~cMixNormal()
	*/
	cMixNormal::~cMixNormal()
	{
		MESS_DESTR("cMixNormal")
	}

	/*!
	* \fn cAbstCondVar* cNormResiduals::::PtrCopy()
	*/

	cAbstResiduals* cMixNormal::PtrCopy() const
	{
		cMixNormal *mycMixNormal = NULL;
		cDVector* myDistrParameter = new cDVector(mDistrParameter);

		bool mySimulFlag = false;

		if (mtR != NULL)
			mySimulFlag = true;

		mycMixNormal = new cMixNormal(myDistrParameter, mySimulFlag);

		delete myDistrParameter;

		return mycMixNormal;
	}

	/*!
	* \fn void cMixNormal::Generate(const uint theNSample, cDVector& theYt) const
	* \param const uint theNSample: the sample size
	* \param cDVector& theYt: the output vector
	*/
	void cMixNormal::Generate(const uint theNSample, cDVector& theYt) const
	{
		theYt.ReAlloc(theNSample);
		int mut;
		for (register uint t = 0; t < theNSample; t++) {
			mut = gsl_ran_bernoulli(mtR, mDistrParameter[0]);
			theYt[t] = mut* gsl_ran_gaussian(mtR, mDistrParameter[1]) + (1 - mut)*gsl_ran_gaussian(mtR, mDistrParameter[2]);
		}
	}

	/*!
	* \fn vvoid cMixNormal::Print(ostream& theOut) const
	* \param ostream& theOut: the output stream, default cout.
	*/
#ifndef _RDLL_
	void cMixNormal::Print(ostream& theOut) const
	{
		theOut << "Conditional Mixed Normal Distribution with p= " << mDistrParameter[0] << ", sigma_1= "
			<< mDistrParameter[1] << "and sigma_2=" << mDistrParameter[2] << endl;
	}
#else
	void cMixNormal::Print(void)
	{
		Rprintf("Conditional Mixed Normal Distribution with p =%f, sigma_1= %f and sigma_2=%f .\n", mDistrParameter[0], mDistrParameter[1], mDistrParameter[2]);
	}
#endif // _RDLL_

	void cMixNormal::SetDefaultInitPoint(void)
	{
		mDistrParameter[0] = 0.5;
		mDistrParameter[1] = 0.2;
		mDistrParameter[2] = 0.3;
	}

	static double MixNormalLogDensity(double theX, double p, double sigma1, double sigma2)
	{
		double mydensity = p * gsl_ran_gaussian_pdf(theX, sigma1) + (1 - p)* gsl_ran_gaussian_pdf(theX, sigma2);
		return log(mydensity);
	}

	double cMixNormal::LogDensity(double theX) const
	{
		double sigmaCarre = mDistrParameter[0] * mDistrParameter[1] * mDistrParameter[1] + (1 - mDistrParameter[0])*mDistrParameter[2] * mDistrParameter[2];
		double mydensity = 0.5*log(sigmaCarre) + MixNormalLogDensity(sqrt(sigmaCarre)*theX, mDistrParameter[0], mDistrParameter[1], mDistrParameter[2]);
		return mydensity;
	}

	/*!
	* \fn uint cMixNormal::GetNParam(void) const
	* \param void.
	* \brief return 1: One parameter for St(n) residuals.
	*/
	uint cMixNormal::GetNParam(void) const
	{
		return 3;
	}

	/*!
	* \fn static void MixNormalGradLogDensity(double theX, double theDof, cDVector& theGrad)
	* \brief Compute the derivative of log density of a Student distribution (unstandardized)
	* with respect to the random variable (theGrad[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGrad)
	* \param theX double: value of the random variable
	* \param theDof double: value of the distribution parameter
	* \param theGrad cDVector&: concatenation of derivatives with respect to the random variable and the model parameters
	*/
	static void MixNormalGradLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad, uint theBegIndex)
	{
		double dDensityP = -1.0 / theDistrParam[2] * gsl_ran_gaussian_pdf(theX / theDistrParam[2], 1) + 1.0 / theDistrParam[1] * gsl_ran_gaussian_pdf(theX / theDistrParam[1], 1);
		double dDensitySigmax = theDistrParam[0] / pow(theDistrParam[1], 2) * (pow(theX, 2) / pow(theDistrParam[1], 2) - 1)*gsl_ran_gaussian_pdf(theX / theDistrParam[1], 1);
		double dDensitySigmay = (1 - theDistrParam[0]) / pow(theDistrParam[2], 2) * (pow(theX, 2) / pow(theDistrParam[2], 2) - 1)*gsl_ran_gaussian_pdf(theX / theDistrParam[2], 1);
		double mydensity = theDistrParam[0] * gsl_ran_gaussian_pdf(theX, theDistrParam[1]) + (1 - theDistrParam[0])* gsl_ran_gaussian_pdf(theX, theDistrParam[2]);
		theGrad[theBegIndex] = dDensityP / mydensity;
		theGrad[theBegIndex + 1] = dDensitySigmax / mydensity;
		theGrad[theBegIndex + 2] = dDensitySigmay / mydensity;
	}

	static void GradLogDensity(double theX, cDVector& theGrad, const cDVector& theDistrParam, uint theBegIndex)
	{
		MixNormalGradLogDensity(theX, theDistrParam, theGrad, theBegIndex);
		double sigmaCarre = theDistrParam[0] * theDistrParam[1] * theDistrParam[1] + (1 - theDistrParam[0])*theDistrParam[2] * theDistrParam[2];
		double part1 = 0.5*(pow(theDistrParam[1], 2) - pow(theDistrParam[2], 2));
		double part2 = theDistrParam[0] * theDistrParam[1];
		double part3 = (1 - theDistrParam[0])* theDistrParam[2];
		theGrad[theBegIndex] = part1 / sigmaCarre + (part1 / sqrt(sigmaCarre))*theGrad[theBegIndex];
		theGrad[theBegIndex + 1] = part2 / sigmaCarre + (part2 * theX / sqrt(sigmaCarre))*theGrad[theBegIndex + 1];
		theGrad[theBegIndex + 2] = part3 / sigmaCarre + (part2*theX / sqrt(sigmaCarre))*theGrad[theBegIndex + 2];
	}

	double cMixNormal::DiffLogDensity(double theX) const
	{
		double p = mDistrParameter[0];
		double sigmaX = mDistrParameter[1];
		double sigmaY = mDistrParameter[2];
		double sigmaNorm = sqrt(p*sigmaX*sigmaX + (1 - p)*sigmaY*sigmaY);
		double density = p*gsl_ran_gaussian_pdf(theX*sigmaNorm, sigmaX) + (1 - p)*gsl_ran_gaussian_pdf(theX*sigmaNorm, sigmaY);
		double result = -(sigmaNorm*sigmaNorm / density)*(p*theX*gsl_ran_gaussian_pdf(theX*sigmaNorm / sigmaX, 1) / pow(sigmaX, 3) + (1 - p)*theX*gsl_ran_gaussian_pdf(theX* sigmaNorm / sigmaY, 1) / pow(sigmaY, 3));
		return result;
	}


	/*!
	* \fn void cMixNormal::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData) const
	* \brief Compute the derivative of log density with respect to the random variable (theGradData[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGradData)
	* \param theDate uint: time at which gradient is computed
	* \param theValue const cRegArchValue&: value of the random variable
	* \param theGradData cRegArchGradient&: concatenation of derivatives with respect to the random variable and the model parameters
	*/
	void cMixNormal::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData) const
	{
		uint myBegIndex = theGradData.GetNMeanParam() + theGradData.GetNVarParam();
		GradLogDensity(theValue.mEpst[theDate], theGradData.mCurrentGradDens, mDistrParameter, myBegIndex);
		theGradData.mCurrentDiffLogDensity = DiffLogDensity(theValue.mEpst[theDate]);
	}

	void cMixNormal::RegArchParamToVector(cDVector& theDestVect, uint theIndex) const
	{
		if (theDestVect.GetSize() < theIndex)
			throw cError("Wrong size");
		theDestVect[theIndex] = mDistrParameter[0];
		theDestVect[theIndex + 1] = mDistrParameter[1];
		theDestVect[theIndex + 2] = mDistrParameter[2];
	}

	void cMixNormal::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		if (theIndex >= theSrcVect.GetSize())
			throw cError("Wrong size");
		mDistrParameter[0] = theSrcVect[theIndex];
		mDistrParameter[1] = theSrcVect[theIndex + 1];
		mDistrParameter[2] = theSrcVect[theIndex + 2];
	}

	double cMixNormal::ComputeEspAbsEps(void)
	{
		double p = mDistrParameter[0];
		double sigmaX = mDistrParameter[1];
		double sigmaY = mDistrParameter[2];
		double sigma = sqrt(p*sigmaX*sigmaX + (1 - p)*sigmaY*sigmaY);
		return 1 / sigma * sqrt(2.0 / PI) * ((1 - p)* sigmaY + p*sigmaX);
	}

	void cMixNormal::ComputeGradBetaEspAbsEps(cDVector& theGrad)
	{
		double p = mDistrParameter[0];
		double sigmaX = mDistrParameter[1];
		double sigmaY = mDistrParameter[2];
		double sigma = p*sigmaX*sigmaX + (1 - p)*sigmaY*sigmaY;
		theGrad[0] = sqrt(2 / PI)*((sigmaX - sigmaY) / sigma - (sigmaX*sigmaX - sigmaY*sigmaY)*(p*sigmaX + (1 - p)*sigmaY) / pow(sigma, 3 / 2));
		theGrad[1] = sqrt(2 / PI)*(p / sigma - p*sigmaX*(p*sigmaX + (1 - p)*sigmaY) / pow(sigma, 3 / 2));
		theGrad[2] = sqrt(2 / PI)*((1 - p) / sigma - (1 - p)*sigmaY*(p*sigmaX + (1 - p)*sigmaY) / pow(sigma, 3 / 2));
	}

	double cMixNormal::Diff2LogDensity(double theX) const
	{
		double p = mDistrParameter[0];
		double sigmaX = mDistrParameter[1];
		double sigmaY = mDistrParameter[2];
		double a = sqrt(p * sigmaX*sigmaX + (1 - p)* sigmaY*sigmaY);
		double fax = MixNormalLogDensity(theX, a, sigmaX, sigmaY);
		return (a*a*((a*theX*a*theX) - 1)*fax) / (pow(a*theX*fax, 2));
	}

	void cMixNormal::GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad)
	{
		double p = theDistrParam[0];
		double sigmaX = theDistrParam[1];
		double sigmaY = theDistrParam[2];
		double sigmaNorm = sqrt(p*sigmaX*sigmaX + (1 - p)*sigmaY*sigmaY);
		double a = (sigmaX*sigmaX - sigmaY*sigmaY) / (2 * sigmaNorm);
		double density = p*gsl_ran_gaussian_pdf(theX*sigmaNorm, sigmaX) + (1 - p)*gsl_ran_gaussian_pdf(theX*sigmaNorm, sigmaY);
		double dDensity = -(p*theX*gsl_ran_gaussian_pdf(sigmaNorm*theX / sigmaX, 1) / pow(sigmaX, 3) + (1 - p)*theX*gsl_ran_gaussian_pdf(sigmaNorm*theX / sigmaY, 1) / pow(sigmaY, 3));
		double dDensityP = (gsl_ran_gaussian_pdf(theX*sigmaNorm / sigmaY, 1) / pow(sigmaY, 3) - gsl_ran_gaussian_pdf(theX*sigmaNorm / sigmaX, 1) / pow(sigmaX, 3))*theX*sigmaNorm*theX*a;
		double dDensitySigmax = 0;
		double dDensitySigmay = 0;
		theGrad[0] = a*DiffLogDensity(sigmaNorm*theX) + sigmaNorm*(dDensityP / density - DiffLogDensity(sigmaNorm*theX)*(gsl_ran_gaussian_pdf(sigmaNorm*theX / sigmaX) - gsl_ran_gaussian_pdf(sigmaNorm*theX / sigmaY)) / density);
		// to do
		theGrad[1] = 0;
		theGrad[2] = 0;
	}

	static void HessLogDensity(double theX, cDMatrix& theHess, const cDVector& theDistrParam, uint theBegIndex)
	{
		double myP = theDistrParam[0];
		double mySigmaX = theDistrParam[1];
		double mySigmaY = theDistrParam[2];

		double dDensityP = -1 / theDistrParam[2] * gsl_ran_gaussian_pdf(theX / theDistrParam[2], 1) + 1 / theDistrParam[1] * gsl_ran_gaussian_pdf(theX / theDistrParam[1], 1);
		double dDensitySigmax = theDistrParam[0] / pow(theDistrParam[1], 2) * (pow(theX, 2) / pow(theDistrParam[1], 2) - 1)*gsl_ran_gaussian_pdf(theX / theDistrParam[1], 1);
		double dDensitySigmay = (1 - theDistrParam[0]) / pow(theDistrParam[2], 2) * (pow(theX, 2) / pow(theDistrParam[2], 2) - 1)*gsl_ran_gaussian_pdf(theX / theDistrParam[2], 1);
		double d2DensitySigmax = myP*gsl_ran_gaussian_pdf(theX / mySigmaX, 1) / pow(mySigmaX, 3)*(2 - 5 * pow(theX / mySigmaX, 2) + pow(theX / (mySigmaX*mySigmaX), 2));
		double d2DensitySigmay = (1 - myP)*gsl_ran_gaussian_pdf(theX / mySigmaY, 1) / pow(mySigmaY, 3)*(2 - 5 * pow(theX / mySigmaY, 2) + pow(theX / (mySigmaY*mySigmaY), 2));
		double d2DensitySigmaxP = dDensitySigmax / myP;
		double d2DensitySigmayP = -dDensitySigmay / (1 - myP);

		double sigmaCarre = myP*mySigmaX*mySigmaX + (1 - myP)*mySigmaY*mySigmaY;
		double mydensity = theDistrParam[0] * gsl_ran_gaussian_pdf(theX*sqrt(sigmaCarre), theDistrParam[1]) + (1 - theDistrParam[0])* gsl_ran_gaussian_pdf(theX*sqrt(sigmaCarre), theDistrParam[2]);

		double myDiff2P1 = -0.5*pow(mySigmaX*mySigmaX - mySigmaY*mySigmaY, 2) / (sigmaCarre*sigmaCarre);
		double myDiff2p2 = -0.25*pow(mySigmaX*mySigmaX - mySigmaY*mySigmaY, 2)*theX / pow(sqrt(sigmaCarre), 3) * dDensityP / mydensity;
		double myDiffp3 = -0.25*pow(mySigmaX*mySigmaX - mySigmaY*mySigmaY, 2)*theX*theX / sigmaCarre * pow(dDensityP / mydensity, 2) / pow(mydensity, 2);
		theHess[theBegIndex][theBegIndex] = myDiff2P1 + myDiff2p2 + myDiffp3;

		double myDiff2sxP1 = myP / sigmaCarre - (2 * myP*myP*mySigmaX*mySigmaX) / (sigmaCarre*sigmaCarre);
		double myDiff2sxP2 = (myP*theX / sqrt(sigmaCarre) + (mySigmaX*mySigmaX*myP*myP) / pow(sigmaCarre, 3 / 2))*dDensitySigmax / mydensity;
		double myDiff2sxP3 = (pow(mySigmaX*myP*theX, 2) / sigmaCarre)*(d2DensitySigmax / mydensity - pow(dDensitySigmax / mydensity, 2));
		theHess[theBegIndex + 1][theBegIndex + 1] = myDiff2sxP1 + myDiff2sxP2 + myDiff2sxP3;

		double myDiff2syP1 = (1 - myP) / sigmaCarre - (2 * (1 - myP)*(1 - myP)*mySigmaY*mySigmaY) / (sigmaCarre*sigmaCarre);
		double myDiff2syP2 = ((1 - myP)*theX / sqrt(sigmaCarre) + (mySigmaY*mySigmaY*(1 - myP)*(1 - myP)) / pow(sigmaCarre, 3 / 2))*dDensitySigmay / mydensity;
		double myDiff2syP3 = (pow(mySigmaY*(1 - myP)*theX, 2) / sigmaCarre)*(d2DensitySigmay / mydensity - pow(dDensitySigmay / mydensity, 2));
		theHess[theBegIndex + 2][theBegIndex + 2] = myDiff2syP1 + myDiff2syP2 + myDiff2syP3;

		double myDiff2psxP1 = mySigmaX*pow(mySigmaY / sigmaCarre, 2);
		double myDiff2psxP2 = ((myP * (mySigmaX*mySigmaX - mySigmaY*mySigmaY) + 2 * mySigmaY*mySigmaY)*theX*mySigmaX*dDensitySigmax) / (2 * pow(myP*(mySigmaX*mySigmaX - mySigmaY*mySigmaY) + mySigmaY*mySigmaY, 3 / 2)*mydensity);
		double myDiff2psxP3 = (mySigmaX*myP*theX*theX*(mySigmaX*mySigmaX - mySigmaY*mySigmaY) / (2 * sigmaCarre))*(d2DensitySigmaxP / mydensity - dDensitySigmax*dDensityP / pow(mydensity, 2));
		theHess[theBegIndex][theBegIndex + 1] = myDiff2psxP1 + myDiff2psxP2 + myDiff2psxP3;
		theHess[theBegIndex + 1][theBegIndex] = theHess[theBegIndex][theBegIndex + 1];

		double myDiff2psyP1 = -mySigmaY*pow(mySigmaX / sigmaCarre, 2);
		double myDiff2psyP2 = -((myP * (mySigmaX*mySigmaX - mySigmaY*mySigmaY) + mySigmaY*mySigmaY + mySigmaX*mySigmaX)*theX*mySigmaY*dDensitySigmay) / (2 * pow(myP*(mySigmaX*mySigmaX - mySigmaY*mySigmaY) + mySigmaY*mySigmaY, 3 / 2)*mydensity);
		double myDiff2psyP3 = (mySigmaY*(1 - myP)*theX*theX*(mySigmaX*mySigmaX - mySigmaY*mySigmaY) / (2 * sigmaCarre))*(d2DensitySigmayP / mydensity - dDensitySigmay*dDensityP / pow(mydensity, 2));
		theHess[theBegIndex][theBegIndex + 2] = myDiff2psyP1 + myDiff2psyP2 + myDiff2psyP3;
		theHess[theBegIndex + 2][theBegIndex] = theHess[theBegIndex][theBegIndex + 2];

		double myDiff2sxsyP1 = 2 * myP*mySigmaX*(1 - myP)*mySigmaY / pow(sigmaCarre, 2);
		double myDiff2sxsyP2 = mySigmaX*mySigmaY*myP*(1 - myP)*theX*dDensitySigmax / (pow(sigmaCarre, 3 / 2)*mydensity);
		double myDiff2sxsyP3 = (mySigmaX*myP*theX*theX*(1 - myP)*mySigmaY / sigmaCarre)*-dDensitySigmax*dDensitySigmay / pow(mydensity, 2);
		theHess[theBegIndex + 1][theBegIndex + 2] = myDiff2sxsyP1 + myDiff2sxsyP2 + myDiff2sxsyP3;
		theHess[theBegIndex + 2][theBegIndex + 1] = theHess[theBegIndex + 1][theBegIndex + 2];

	}


	void cMixNormal::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
		cDVector myGradDiffLogDens = cDVector(1);
		GradDiffLogDensity(theData.mEpst[theDate], mDistrParameter, myGradDiffLogDens);
		uint myBegIndex = theHessData.GetNMeanParam() + theHessData.GetNVarParam();
		theHessData.mCurrentGradDiffLogDensity[myBegIndex] = myGradDiffLogDens[0];
		double myX = theData.mEpst[theDate];
		HessLogDensity(myX, theHessData.mCurrentHessEps, mDistrParameter, myBegIndex);
	}

	void cMixNormal::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "p");
		sprintf(theName[myIndex++], "sigma_x");
		sprintf(theName[myIndex++], "sigma_y");
	}



}//namespace
