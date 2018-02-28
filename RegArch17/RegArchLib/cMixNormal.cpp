#include "StdAfxRegArchLib.h"
/*!
\file cMixNormal.cpp
\brief implementation of the class for Student conditional distribution.
*/
namespace RegArchLib {
	/*!
	* \fn cStudentResiduals::cStudentResiduals(double theDof, bool theSimulFlag)
	* \param double theDof: number of degrees of freedom
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cMixNormal::cMixNormal(double theDof, bool theSimulFlag) : cAbstResiduals(eStudent, NULL, theSimulFlag)
	{
		mDistrParameter.ReAlloc(1);
		mDistrParameter[0] = theDof;
		MESS_CREAT("cStudentResiduals")
	}

	/*!
	* \fn cStudentResiduals::cStudentResiduals(const cDVector* theDistrParameter, bool theSimulFlag): cAbstResiduals(eStudent, theDistrParameter, theSimulFlag)
	* \param const cDVector* theDistrParameter: theDistrParameter[0] = d.o.f.
	* \param bool theSimulFlag: true if created for simulation
	* \details: mvBool is initialised by ce cAbstResiduals constructor
	*/
	cMixNormal::cMixNormal(cDVector* theDistrParameter, bool theSimulFlag) : cAbstResiduals(eStudent, theDistrParameter, theSimulFlag)
	{
		MESS_CREAT("cStudentResiduals")
	}

	/*!
	* \fn cStudentResiduals::~cStudentResiduals()
	*/
	cMixNormal::~cMixNormal()
	{
		MESS_DESTR("cStudentResiduals")
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
	* \fn void cStudentResiduals::Generate(const uint theNSample, cDVector& theYt) const
	* \param const uint theNSample: the sample size
	* \param cDVector& theYt: the output vector
	*/
	void cMixNormal::Generate(const uint theNSample, cDVector& theYt) const
	{
		theYt.ReAlloc(theNSample);
		if (mDistrParameter[0] <= 2.0)
			throw cError("wrong d.o.f.");

		double myStd = sqrt(mDistrParameter[0] / (mDistrParameter[0] - 2.0));
		for (register uint t = 0; t < theNSample; t++)
			theYt[t] = gsl_ran_tdist(mtR, mDistrParameter[0]) / myStd;
	}

	/*!
	* \fn void cStudentResiduals::Print(ostream& theOut) const
	* \param ostream& theOut: the output stream, default cout.
	*/
#ifndef _RDLL_
	void cMixNormal::Print(ostream& theOut) const
	{
		theOut << "Conditional Student Distribution with " << mDistrParameter[0] << " d. o. f." << endl;
	}
#else
	void cStudentResiduals::Print(void)
	{
		Rprintf("Conditional Student Distribution with %f d.o.f.\n", mDistrParameter[0]);
	}
#endif // _RDLL_

	void cMixNormal::SetDefaultInitPoint(void)
	{
		mDistrParameter[0] = 10.0;
	}

	static double StudentLogDensity(double theX, double theDof)
	{
		return log(gsl_ran_tdist_pdf(theX, theDof));
	}

	double cMixNormal::LogDensity(double theX) const
	{
		double myStd = sqrt(mDistrParameter[0] / (mDistrParameter[0] - 2.0));
		return StudentLogDensity(theX*myStd, mDistrParameter[0]) + log(myStd);

	}

	/*!
	* \fn double cStudentResiduals::GetNParam(void) const
	* \param void.
	* \brief return 1: One parameter for St(n) residuals.
	*/
	uint cMixNormal::GetNParam(void) const
	{
		return 1;
	}

	/*!
	* \fn static void StudentGradLogDensity(double theX, double theDof, cDVector& theGrad)
	* \brief Compute the derivative of log density of a Student distribution (unstandardized)
	* with respect to the random variable (theGrad[0]) \e and the gradient
	* of log density with respect to the model parameters (other components in theGrad)
	* \param theX double: value of the random variable
	* \param theDof double: value of the distribution parameter
	* \param theGrad cDVector&: concatenation of derivatives with respect to the random variable and the model parameters
	*/
	static void MixNormalGradLogDensity(double theX, double theDof, cDVector& theGrad)
	{
		double myt2 = theX * theX;
		double myAux1 = myt2 + theDof;
		theGrad[0] = -(theDof + 1)*theX / myAux1;
		double myAux2 = log(myAux1) - gsl_sf_psi((theDof + 1) / 2.0) - log(theDof) + gsl_sf_psi(theDof / 2.0);
		theGrad[1] = -0.5*(myAux2 + (1 - myt2) / myAux1);
	}

	double cMixNormal::DiffLogDensity(double theX) const
	{
		double myDof = this->mDistrParameter[0];
		return -(myDof + 1)*theX / (theX*theX + myDof - 2.0);
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
		double	myDof = theDistrParam[0];
		double myt2 = theX*theX;
		double myAux1 = myt2 + myDof - 2;
		double myRes = -log((myt2 + myDof - 2) / (myDof - 2)) / 2;
		myRes += (gsl_sf_psi((myDof + 1) / 2.0E+0) - gsl_sf_psi(myDof / 2)) / 2;
		myRes += (myDof*myt2 - myDof + 2) / ((myDof - 2)*(myt2 + myDof - 2)) / 2;
		theGrad[theBegIndex] = myRes;
	}

	/*!
	* \fn void cStudentResiduals::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData)
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