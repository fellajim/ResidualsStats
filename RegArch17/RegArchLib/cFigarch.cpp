#include "StdAfxRegArchLib.h"
/*!
\file cFigarch.cpp
\brief sources for class cFigarch methods.

\author Jean-Baptiste DURAND, Ollivier TARAMASCO
\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	* \fn cFigarch::cFigarch(uint theNArch, uint theNFigarch, double theFracD, uint theNTruncLag):cAbstCondVar(eFigarch)
	* \param uint theNArch: number of ARCH lags
	* \param uint theNFigarch: number of Figarch lags
	*/
	cFigarch::cFigarch(uint theNArch, uint theNGarch, double theFracD, uint theNTruncLag)
		:cAbstCondVar(eFigarch)  // call constructor of cAbstCondVar with type eFigarch
	{
		mvArch.ReAlloc(theNArch);
		mvGarch.ReAlloc(theNGarch);
		mvConst = 0.0;
		mvFracD = theFracD;
		mvNTruncLag = theNTruncLag;
		uint myNParam = theNArch + theNGarch + 2;
		mvPolArch.Resize(mvNTruncLag);
		mvGradPolArch.resize(myNParam-1);
		for (register uint i = 0; i < myNParam-1; i++)
			mvGradPolArch.at(i).Resize(mvNTruncLag);

		MESS_CREAT("cFigarch");
	}

	/*!
	* \fn cFigarch::cFigarch(double theConst, cDVector& theArch, cDVector& theFigarch):cAbstCondVar(eFigarch)
	* \param double theConst: constant part of the Figarch(p, q) model
	* \param cDVector& theFigarch theArch: ARCH parameters
	* \param cDVector& theFigarch theFigarch: Figarch parameters
	*/
	cFigarch::cFigarch(double theConst, const cDVector& theArch, const cDVector& theGarch, double theFracD, uint theNTruncLag)
		:cAbstCondVar(eFigarch)
	{
		mvConst = theConst;
		mvArch = theArch;
		mvGarch = theGarch;
		mvFracD = theFracD;
		mvNTruncLag = theNTruncLag;
		uint myNArch = theArch.GetSize();
		uint myNGarch = theGarch.GetSize();
		uint myNParam = myNArch + myNGarch + 2;
		mvPolArch.Resize(mvNTruncLag);
		mvGradPolArch.resize(myNParam-1);
		for (register uint i = 0; i < myNParam-1; i++)
			mvGradPolArch.at(i).Resize(mvNTruncLag);

		MESS_CREAT("cFigarch");
	}

	/*!
	* \fn cFigarch::cFigarch(const cAbsCondVar& theFigarch):cAbstCondVar(eFigarch)
	* \param cAbsCondVar& theEFigarch: theFigarch class
	*/
	cFigarch::cFigarch(const cFigarch& theFigarch)
		:cAbstCondVar(eNotKnown)
	{
		*this = theFigarch;
		MESS_CREAT("cFigarch");
	}

	/*!
	* \fn cFigarch::~cFigarch()
	*/
	cFigarch::~cFigarch()
	{
		mvArch.Delete();
		mvGarch.Delete();
		uint myNParam = mvGradPolArch.size();
		mvPolArch.Delete();
		if (myNParam != 0)
		{
			for (register uint i = 0; i < myNParam; i++)
				mvGradPolArch.at(i).Delete();
			mvGradPolArch.clear();
		}

		MESS_DESTR("cFigarch");
	}

	/*!
	* \fn cAbstCondVar* cFigarch::PtrCopy()
	*/
	cAbstCondVar* cFigarch::PtrCopy() const
	{
	cFigarch *myFigarch = new cFigarch(*this);
		return myFigarch;
	}

	/*!
	* \fn void cFigarch::Delete(void)
	* \param void
	* \details Free memory
	*/
	void cFigarch::Delete(void)
	{
		mvArch.Delete();
		mvGarch.Delete();
		uint myNParam = mvGradPolArch.size() ;
		mvPolArch.Delete();
		if (myNParam != 0)
		{
			for (register uint i = 0; i < myNParam; i++)
				mvGradPolArch.at(i).Delete();
			mvGradPolArch.clear();
		}
	}

#ifndef _RDLL_
	/*!
	* \fn void cFigarch::Print(ostream& theOut) const
	* \param ostream& theOut: the output stream, default cout.
	*/
	void cFigarch::Print(ostream& theOut) const
	{
		uint myNArch = mvArch.GetSize();
		uint myNGarch = mvGarch.GetSize();
		theOut << "FIGARCH(" << myNArch << ", d, " << myNGarch << ") model with:" << endl;
		theOut << "\tCste=" << mvConst << endl;
		for (register uint i = 0; i < myNArch; i++)
			theOut << "\tARCH[" << i + 1 << "]=" << mvArch[i] << endl;
		for (register uint j = 0; j < myNGarch; j++)
			theOut << "\tGARCH[" << j + 1 << "]=" << mvGarch[j] << endl;
		theOut << "\td=" << mvFracD << endl;
	}
#else
	/*!
	* \fn void cFigarch::Print()
	*/
	void cFigarch::Print()
	{
		uint myNArch = mvArch.GetSize();
		uint myNGarch = mvGarch.GetSize();
		Rprintf("FIGARCH(%d, d,%d) model with:", myNArch, myNGarch);
		Rprintf("\tCste=%f\n", mvConst);
		for (register uint i = 0; i < myNArch; i++)
			Rprintf("\tARCH[%d]=%f\n", i + 1, mvArch[i]);
		for (register uint j = 0; j < myNGarch; j++)
			Rprintf("\tGARCH[%d]=%f\n", j + 1, mvGarch[j]);
		Rprintf("\td=%f\n", mvFracD);
	}
#endif //_RDLL_

	void cFigarch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvConst = theVar*0.1;
		uint myNArch = mvArch.GetSize();
		uint myNFigarch = mvGarch.GetSize();
		uint i;
		for (i = 0; i < myNArch; i++)
			mvArch[i] = 0.1 / (double)myNArch;
		for (i = 0; i < myNFigarch; i++)
			mvGarch[i] = 0.8 / (double)myNFigarch;
		mvFracD = .5;
	}

	void cFigarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvConst = myVar*0.1;
	uint myNArch = mvArch.GetSize();
	uint myNFigarch = mvGarch.GetSize();
	uint i;
		for (i = 0; i < myNArch; i++)
			mvArch[i] = 0.1 / (double)myNArch;
		for (i = 0; i < myNFigarch; i++)
			mvGarch[i] = 0.8 / (double)myNFigarch;
		mvFracD = .5;
	}

	/*!
	* \fn void cFigarch::ReAlloc(const uint theSize, const uint theNumParam)
	* \param const uint theSize: new size of mvArch or mvGarch
	* \param const uint theNumParam: 0 for mvArch, 1 for mvGarch.
	* \details new allocation of mvArch or mvGarch
	*/
	void cFigarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 1:
			mvArch.ReAlloc(theSize);
			break;
		case 2:
			mvGarch.ReAlloc(theSize);
			break;
		default:
//			throw cError("cFigarch::ReAlloc - theNumParam must be in 1, 2.");
			break;
		}
	}

	/*!
	* \fn void cFigarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	* \param const cDVector& theVectParam: the vector of Const, ARCH or Figarch coefficients
	* \param const uint theNumParam: =0, the constant part; =1 the ARCH coefficients; =2 theFigarch Coefficients
	* \details new allocation of mvArch or mvConst
	*/
	void cFigarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0: // mvConst
			if (theVectParam.GetSize() > 0)
				mvConst = theVectParam[0];
			else
				throw cError("cFigarch::ReAlloc - Size of theVectParam must be > 0");
			break;
		case 1: // mvArch
			mvArch = theVectParam;
			break;
		case 2: // mvGarch
			mvGarch = theVectParam;
			break;
		case 3:
			mvFracD = theVectParam[0];
			break;
		default:
			throw cError("cFigarch::ReAlloc - theNumParam must be in 0 .. 3");
			break;
		}
	}

	/*!
	* \fn void cFigarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	* \brief fill the parameters vector
	* \param const double theValue: the value of the "theIndex" th lag. Default 0.
	* \param const uint theIndex: the index.
	* \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, Figarch parameters
	* \details mvArch[theIndex] = theValue or mvGarch[theIndex]= theValue or mvConst = theValue
	*/
	void cFigarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			mvConst = theValue;
			break;
		case 1:
			if (theIndex < mvArch.GetSize())
				mvArch[theIndex] = theValue;
			else
				throw cError("cFigarch::Set - wrong index");
			break;
		case 2:
			if (theIndex < mvGarch.GetSize())
				mvGarch[theIndex] = theValue;
			else
				throw cError("cFigarch::Set - wrong index");
			break;
		case 3:
			mvFracD = theValue;
			break;
		default:
			throw cError("cFigarch::Set - theNumParam must be in 0 .. 3");
			break;
		}
	}

	/*!
	* \fn void cFigarch::Set(const cDVector& theVectParam, const uint theNumParam)
	* \brief fill the parameters vector
	* \param const cDVector& theVectParam: the vector of values
	* \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, Figarch parameters
	* \details mvAr = theValue
	*/
	void cFigarch::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			if (theVectParam.GetSize() > 0)
				mvConst = theVectParam[0];
			else
				throw cError("cFigarch::Set - Size of theVectParam must be > 0");
			break;
		case 1:
			mvArch = theVectParam;
			break;
		case 2:
			mvGarch = theVectParam;
			break;
		case 3:
			if (theVectParam.GetSize() > 0)
				mvFracD = theVectParam[0];
			else
				throw cError("cFigarch::Set - Size of theVectParam must be > 0");
			break;
		default:
			throw cError("cFigarch::Set - theNumParam must be in 0, 1, 2");
			break;
		}
	}

	double  cFigarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			return mvConst;
			break;
		case 1:
			return mvArch[theIndex];
			break;
		case 2:
			return mvGarch[theIndex];
			break;
		case 3:
			return mvFracD;
			break;
		}
	}

	cDVector& cFigarch::Get(const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			ourAuxVector.ReAlloc(1, mvConst);
			return ourAuxVector;
			break;
		case 1:
			return mvArch;
			break;
		case 2:
			return mvGarch;
			break;
		case 3:
			ourAuxVector.ReAlloc(1, mvFracD);
			return ourAuxVector;
			break;
		}
	}

	cFigarch& cFigarch::operator =(const cFigarch& theSrc)
	{
		mvArch = theSrc.mvArch;
		mvGarch = theSrc.mvGarch;
		mvPolArch = theSrc.mvPolArch;
		SetCondVarType(eFigarch);
		mvConst = theSrc.mvConst;
		mvFracD = theSrc.mvFracD;
		mvNTruncLag = theSrc.mvNTruncLag;
		uint myNParam = mvArch.GetSize() + mvGarch.GetSize() + 2;
		mvGradPolArch.resize(myNParam-1);
		for (uint i = 0; i < myNParam-1; i++)
			mvGradPolArch.at(i).Resize(mvNTruncLag);
		mvOmega0 = theSrc.mvOmega0;
		return *this;
	}

	static void ComputePhiAndTeta(cFigarch& theFigarch, cPolynome& thePhi, cPolynome& theTeta)
	{
		uint myNArch = theFigarch.GetNArch();
		uint myNGarch = theFigarch.GetNGarch();
		thePhi.Resize(MAX(myNArch, myNGarch));
		theTeta.Resize(myNGarch);
		for (register uint i = 0; i < myNArch; i++)
			thePhi[i + 1] -= theFigarch.Get(i, 1);
		for (register uint j = 0; j < myNGarch; j++)
		{
			thePhi[j + 1] -= theFigarch.Get(j, 2);
			theTeta[j + 1] = -theFigarch.Get(j, 2);
		}
		thePhi[0] = theTeta[0] = 1.0;
	}

	static void ComputePolForGrad(cPolynome& thePhi, cPolynome& theTeta, double theD, uint theNMaxLag, cPolynome& theARMAPol, cPolynome& thePolGradAlpha, cPolynome& thePolGradBeta, cPolynome& thePolGradD)
	{
		cPolynome myDeltaD(theNMaxLag + 2);
		ComputeDeltaPowD(theD, theNMaxLag + 2, myDeltaD);
		cPolynome myPolAux1(theNMaxLag + 2), myRest(0);
		IncrPowDiv(myDeltaD, theTeta, theNMaxLag + 2, myPolAux1, myRest);
		thePolGradAlpha = -1 * myPolAux1;
		cPolynome myPolAux2(theNMaxLag + 2);
		IncrPowDiv(myPolAux1, theTeta, theNMaxLag + 2, myPolAux2, myRest);
		thePolGradBeta = myPolAux2 - myPolAux1;
		cPolynome myLogDeltaD(theNMaxLag + 2);
		for (register uint i = 1; i <= theNMaxLag + 2; i++)
			myLogDeltaD[i] = -1 / (double)i;
		cPolynome myPolAux3(theNMaxLag + 2), myPolAux4(theNMaxLag + 2);
		myPolAux3 = TrunkMult(myPolAux1, thePhi, theNMaxLag + 2);
		myPolAux4 = TrunkMult(myPolAux3, myLogDeltaD, theNMaxLag + 2);
		theARMAPol = -1 * myPolAux3;
		thePolGradD = -1 * myPolAux4;
	}

	static void UpdateAllArchParam(cArch* thecArch, double theConst, cDVector& theARCH, cDVector& theGARCH, uint theNLag, double theFracD)
	{
	cPolynome myDeltaD(theNLag + 2);
		ComputeDeltaPowD(theFracD, theNLag + 2, myDeltaD);
	uint myNArch = theARCH.GetSize();
	uint myNGarch = theGARCH.GetSize();
	cPolynome myPhi(MAX(myNArch, myNGarch));
		for (register uint i = 1; i <= myNArch; i++)
			myPhi[i] -= theARCH[i - 1];
		for (register uint i = 1; i <= myNGarch; i++)
			myPhi[i] -= theGARCH[i - 1];
		myPhi[0] = 1;
	cPolynome myAux(theNLag + 2);
		myAux = TrunkMult(myPhi, myDeltaD, theNLag + 2);
	cPolynome myTeta(myNGarch);
		for (register uint i = 1; i <= myNGarch; i++)
			myTeta[i] = -theGARCH[i - 1];
		myTeta[0] = 1;
	cPolynome myQuot(theNLag), myRest(theNLag);
		IncrPowDiv(myAux, myTeta, theNLag, myQuot, myRest);
		for (uint i = 1; i <= theNLag; i++)
			thecArch->Set(-myQuot[i], i - 1, 1);
	double mySum = Sum(theGARCH);
	double myOmega0 = theConst / (1 - mySum);
		thecArch->Set(myOmega0, 0, 0);
	}

	void cFigarch::UpdateProxyVarParameters(void)
	{
	cPolynome myPhi(0), myTeta(0);
		ComputePhiAndTeta(*this, myPhi, myTeta);
	cPolynome myDelta(0);
		ComputeDeltaPowD(mvFracD, mvNTruncLag, myDelta);
	cPolynome myPol1(0), myRest(0);
		IncrPowDiv(myPhi, myTeta, mvNTruncLag + myTeta.GetNDegree(), myPol1, myRest);
		mvPolArch = TrunkMult(myPol1, myDelta, mvNTruncLag);
		mvOmega0 = mvConst / (1 - Sum(mvGarch));
	cPolynome myPol2(0);
		IncrPowDiv(myDelta, myTeta, mvNTruncLag + myTeta.GetNDegree(), myPol2, myRest);
	cPolynome myPol3 = myPol2;
	uint myNArch = mvArch.GetSize();
	cPolynome myPolX(1);
		myPolX[1] = 1;
		for (uint i = 0; i < myNArch; i++)
		{
			myPol2 = TrunkMult(myPol2, myPolX, mvNTruncLag);
			mvGradPolArch[i] = myPol2;
		}
	cPolynome myPol4(0);
		IncrPowDiv(mvPolArch, myTeta, mvNTruncLag + myTeta.GetNDegree(), myPol4, myRest);
	uint myNGarch = mvGarch.GetSize();
		for (uint i = 0; i < myNGarch; i++)
		{
			myPol3 = TrunkMult(myPol3, myPolX, mvNTruncLag);
			myPol4 = TrunkMult(myPol4, myPolX, mvNTruncLag);
			mvGradPolArch[myNArch+i] = myPol3-myPol4;
		}
	cPolynome myPol6(mvNTruncLag);
		for (uint i = 1; i <= mvNTruncLag; i++)
			myPol6[i] = 1 / (double)i;
		mvGradPolArch[myNArch + myNGarch] = TrunkMult(myPol6, mvPolArch, mvNTruncLag);
		mvPolArch[0] = 0;

	}

	/*!
	* \fn double cFigarch::ComputeVar(uint theDate, const cReFigarchValue& theData) const
	* \param int theDate: date of computation
	* \param const cReFigarchValue& theData: past datas
	* \details theData is not updated here.
	*/
	double cFigarch::ComputeVar(uint theDate, const cRegArchValue& theData) const
	{
		double myVar = mvOmega0 - mvPolArch.BackwardPolOp(theData.mUt, theDate, 2);
		return myVar;

	}

	uint cFigarch::GetNParam(void) const
	{
		return 2 + mvArch.GetSize() + mvGarch.GetSize();
	}

	uint cFigarch::GetNArch(void) const
	{
		return mvArch.GetSize();
	}

	uint cFigarch::GetNGarch(void) const
	{
		return mvGarch.GetSize();
	}

	uint cFigarch::GetNLags(void) const
	{
		uint myMax = MAX(mvArch.GetSize(), mvGarch.GetSize());
		myMax = MAX(myMax, mvNTruncLag);
		return  myMax;
	}

	void cFigarch::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
	double mySum = Sum(mvGarch);
		theGradData.mCurrentGradVar = 0.0L;
	uint myBegIndex = theGradData.GetNMeanParam();
		theGradData.mCurrentGradVar[myBegIndex] = 1.0/(1-mySum);
	register uint i;
		for (i = 0; i < myNArch + myNGarch + 1 ; i++)
			theGradData.mCurrentGradVar[myBegIndex + i + 1] += mvGradPolArch[i].BackwardPolOp(theValue.mUt, theDate, 2);
	double myAux = mvConst / pow(1 - mySum, 2);
		for (i = 0; i < myNGarch; i++)
			theGradData.mCurrentGradVar[myBegIndex + myNArch + i + 1] += myAux;
		for (i = 1; i < myNArch + myNGarch + 2; i++)
		{
			for (register uint t = 0; t < MIN(mvNTruncLag, theDate); t++)
				theGradData.mCurrentGradVar[myBegIndex+i] -= 2*mvPolArch[t+1]*theValue.mUt[theDate-t]*theGradData.mGradMt[t][i];
		}

	}

	void  cFigarch::NumericComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals, double theh)
	{
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
		UpdateProxyVarParameters();
		double myF0 = ComputeVar(theDate, theValue);
		double myh = fabs(mvConst * theh);
		myh = MAX(myh, 1e-8);
		mvConst += myh;
		UpdateProxyVarParameters();
		double myF1 = ComputeVar(theDate, theValue);
		theGradData.mCurrentGradVar[theBegIndex] += (myF1 - myF0) / myh;
		mvConst -= myh;

		for (uint i = 0; i < myNArch; i++)
		{
			myh = fabs(mvArch[i] * theh);
			myh = MAX(myh, 1e-8);
			mvArch[i] += myh;
			UpdateProxyVarParameters();
			myF1 = ComputeVar(theDate, theValue);
			theGradData.mCurrentGradVar[theBegIndex + i + 1] += (myF1 - myF0) / myh;
			mvArch[i] -= myh;
		}
		for (uint i = 0; i < myNGarch; i++)
		{
			myh = fabs(mvGarch[i] * theh);
			myh = MAX(myh, 1e-8);
			mvGarch[i] += myh;
			UpdateProxyVarParameters();
			myF1 = ComputeVar(theDate, theValue);
			theGradData.mCurrentGradVar[theBegIndex + i + myNArch + 1] += (myF1 - myF0) / myh;
			mvGarch[i] -= myh;
		}
		myh = fabs(mvFracD * theh);
		myh = MAX(myh, 1e-8);
		mvFracD += myh;
		UpdateProxyVarParameters();
		myF1 = ComputeVar(theDate, theValue);
		theGradData.mCurrentGradVar[theBegIndex + myNArch + myNGarch + 1] += (myF1 - myF0) / myh;
		mvFracD -= myh;
	}

	void cFigarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam();
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size");
		theDestVect[theIndex] = mvConst;
		mvArch.SetSubVectorWithThis(theDestVect, theIndex + 1);
		mvGarch.SetSubVectorWithThis(theDestVect, theIndex + 1 + mvArch.GetSize());
		theDestVect[theIndex + 1 + mvArch.GetSize() + mvGarch.GetSize()] = mvFracD;
	}

	void cFigarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		uint mySize = theSrcVect.GetSize();
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size");
		mvConst = theSrcVect[theIndex];
		mvArch.SetThisWithSubVector(theSrcVect, theIndex + 1);
		mvGarch.SetThisWithSubVector(theSrcVect, theIndex + 1 + mvArch.GetSize());
		mvFracD = theSrcVect[theIndex + 1 + mvArch.GetSize() + mvGarch.GetSize()];
	}

	void cFigarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	}

	void cFigarch::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH[%d]", i + 1);

		}
		for (uint i = 0; i < mvGarch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "GARCH[%d]", i + 1);

		}
		sprintf(theName[myIndex++], "Frac. d");
	}

}//namespace
