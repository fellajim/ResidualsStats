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
		mDistrParameter[1] = sigma_1*sigma_1;
		mDistrParameter[2] = sigma_2*sigma_2;

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
			theYt[t] = mut* gsl_ran_gaussian(mtR, mDistrParameter[1]) + (1-mut)*gsl_ran_gaussian(mtR, mDistrParameter[2]);
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
		double mydensity= 0.5*log(sigmaCarre)+ MixNormalLogDensity(sqrt(sigmaCarre)*theX, mDistrParameter[0], mDistrParameter[1], mDistrParameter[2]);
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
		double dDensityP = -1 / theDistrParam[2] * gsl_ran_gaussian_pdf(theX / theDistrParam[2],1)+1 / theDistrParam[1] * gsl_ran_gaussian_pdf(theX / theDistrParam[1], 1);
		double dDensitySigmax = theDistrParam[0] / pow(theDistrParam[1], 2) * (pow(theX, 2) / pow(theDistrParam[1], 2) - 1)*gsl_ran_gaussian_pdf(theX / theDistrParam[1], 1);
		double dDensitySigmay = (1- theDistrParam[0]) / pow(theDistrParam[2], 2) * (pow(theX, 2) / pow(theDistrParam[2], 2) - 1)*gsl_ran_gaussian_pdf(theX / theDistrParam[2], 1);
		double mydensity = theDistrParam[0] * gsl_ran_gaussian_pdf(theX, theDistrParam[1]) + (1 - theDistrParam[0])* gsl_ran_gaussian_pdf(theX, theDistrParam[2]);
		theGrad[theBegIndex] = dDensityP / mydensity;
		theGrad[theBegIndex+1] = dDensitySigmax / mydensity;
		theGrad[theBegIndex+2] = dDensitySigmay / mydensity;
	}

	static void GradLogDensity(double theX, cDVector& theGrad, const cDVector& theDistrParam, uint theBegIndex)
	{
		MixNormalGradLogDensity(theX, theDistrParam, theGrad, theBegIndex);
		double sigmaCarre = theDistrParam[0] * theDistrParam[1] * theDistrParam[1] + (1 - theDistrParam[0])*theDistrParam[2] * theDistrParam[2];
		double part1 = 0.5*(pow(theDistrParam[1], 2) - pow(theDistrParam[2], 2));
		double part2 = theDistrParam[0] * theDistrParam[1];
		double part3 = (1 - theDistrParam[0])* theDistrParam[2];
		theGrad[theBegIndex] = part1/sigmaCarre + (part1/sqrt(sigmaCarre))*theGrad[theBegIndex];
		theGrad[theBegIndex+1] = part2/sigmaCarre + (part2 * theX  / sqrt(sigmaCarre))*theGrad[theBegIndex+1];
		theGrad[theBegIndex+2] = part3/sigmaCarre + (part2*theX/sqrt(sigmaCarre))*theGrad[theBegIndex+2];
	}

	double cMixNormal::DiffLogDensity(double theX) const
	{
		double diffDensityYPart = -(1 - mDistrParameter[0])*theX / pow(mDistrParameter[2], 3) * gsl_ran_gaussian_pdf(theX / mDistrParameter[2], 1);
		double diffDensityXPart = -mDistrParameter[0] * theX / pow(mDistrParameter[1], 3) * gsl_ran_gaussian_pdf(theX / mDistrParameter[1], 1);
		double mydensity = mDistrParameter[0] * gsl_ran_gaussian_pdf(theX, mDistrParameter[1]) + (1 - mDistrParameter[0])* gsl_ran_gaussian_pdf(theX, mDistrParameter[2]);
		double result = (diffDensityXPart + diffDensityYPart) / mydensity;
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
	}

	void cMixNormal::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		if (theIndex >= theSrcVect.GetSize())
			throw cError("Wrong size");
		mDistrParameter[0] = theSrcVect[theIndex];
	}

	/*
	* (2*sqrt(n-2)*gamma((n+1)/2))/(sqrt(PI)*(n-1)*gamma(n/2))
	*/
	double cMixNormal::ComputeEspAbsEps(void)
	{
		double myDof = mDistrParameter[0];
		return gsl_sf_gamma((myDof + 1.0) / 2.0) / gsl_sf_gamma(myDof / 2.0)
			*2.0*sqrt(myDof - 2.0) / (myDof - 1.0) / SQRT_PI;
	}

	/*
	*  [(n^2 - 3*n + 2)*(psi0((n+1)/2) - psi0(n/2)) - n + 3]/[sqrt(PI)*sqrt(n-2)*(n-1)^2]*gamma((n+1)/2)/gamma(n/2)
	*/
	void cMixNormal::ComputeGradBetaEspAbsEps(cDVector& theGrad)
	{
		double myDof = this->mDistrParameter[0],
			myAux = myDof*myDof - 3 * myDof + 2;

		theGrad[0] = (myAux*gsl_sf_psi((myDof + 1) / 2.0) - myAux*gsl_sf_psi(myDof / 2.0) - myDof + 3.0)
			*gsl_sf_gamma((myDof + 1) / 2.0)
			/ (SQRT_PI*sqrt(myDof - 2)*(myDof*myDof - 2 * myDof + 1)*gsl_sf_gamma(myDof / 2.0));
	}

	double cMixNormal::Diff2LogDensity(double theX) const
	{
		double myDof = mDistrParameter[0];
		double myt2 = theX*theX;
		return (myDof + 1)*(myt2 - myDof + 2) / pow(myt2 + myDof - 2, 2);
	}

	void cMixNormal::GradDiffLogDensity(double theX, const cDVector& theDistrParam, cDVector& theGrad)
	{
		double myDof = theDistrParam[0];
		double myt2 = theX*theX;
		theGrad[0] = -(theX*(myt2 - 3)) / pow(myt2 + myDof - 2, 2);
	}

	static void HessLogDensity(double theX, cDMatrix& theHess, const cDVector& theDistrParam, uint theBegIndex)
	{
		double myDof = theDistrParam[0];
		double myt2 = theX*theX;
		double myAux1 = myt2 + myDof - 2;
		double myRes = ((myDof - 4)*myt2*myt2 - (myDof - 2)*(4 * myt2 - myDof + 2)) / (2 * pow((myDof - 2)*myAux1, 2));
		myRes += (gsl_sf_psi_1((myDof + 1) / 2) - gsl_sf_psi_1(myDof / 2)) / 4;
		theHess[theBegIndex][theBegIndex] = myRes;
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
		sprintf(theName[myIndex++], "Student d.o.f.");
	}



}//namespace
