#include "StdAfxRegArchLib.h"
/*!
\file cArfima.cpp
\brief sources for class cAr methods.

\author Jean-Baptiste Durand, Ollivier TARAMASCO
\date feb-18-2016 - Last change feb-18-2016
*/

namespace RegArchLib {
	/*!
	* \fn cArfimar(uint theNAr = 0, uint theNMa = 0, double theFracD = 0, uint theNTruncLag = 100):cAbstCondMean(eArfima)
	* \param const uint theNAr: number of AR lags.
	*/
	
	cArfima::cArfima(uint theNAr, uint theNMa, double theFracD , uint theNTruncLag) :cAbstCondMean(eArfima)
	{
		mvAr.ReAlloc(theNAr);
		mvMa.ReAlloc(theNMa);
		mvFracD = theFracD;
		mvNTruncLag = theNTruncLag;
		mvPolMa.Resize(mvNTruncLag);
	uint myNParam = theNAr + theNMa + 1;
		
		mvGradPolMa.resize(myNParam);
		for (register uint i = 0; i < myNParam; i++)
			mvGradPolMa.at(i).Resize(mvNTruncLag);
		MESS_CREAT("cArfima")
	}

	cArfima::cArfima(const cDVector& theAr, const cDVector& theMa, double theFracD, uint theNTruncLag) :cAbstCondMean(eArfima)
	{
		mvAr = theAr;
		mvMa = theMa;
		mvFracD = theFracD;
		mvNTruncLag = theNTruncLag;
		mvPolMa.Resize(mvNTruncLag);
		uint myNParam = theAr.GetSize() + theMa.GetSize() + 1;
		mvGradPolMa.resize(myNParam);
		for (register uint i = 0; i < myNParam; i++)
			mvGradPolMa.at(i).Resize(mvNTruncLag);
		MESS_CREAT("cArfima")
	}

	cArfima::cArfima(const cArfima& theArfima) :cAbstCondMean(eUnknown)
	{	
		*this = theArfima;
		MESS_CREAT("cAr")
	}

	cArfima:: ~cArfima()
	{
	uint myNParam = mvAr.GetSize() + mvMa.GetSize() + 1;
		mvAr.Delete();
		mvMa.Delete();
		mvPolMa.Delete();
		if (mvGradPolMa.size() != 0)
		{	
			for (register uint i = 0; i < myNParam; i++)
				mvGradPolMa.at(i).Delete();
			mvGradPolMa.clear();
		}
		MESS_DESTR("cArfima")
	}

	cAbstCondMean* cArfima::PtrCopy(void) const
	{
	cArfima* myArfima = new cArfima(*this);

		return myArfima;
	}

	void cArfima::Delete(void)
	{
	uint myNParam = mvAr.GetSize() + mvMa.GetSize() + 1;
		mvAr.Delete();
		mvMa.Delete();
		mvPolMa.Delete();
		if (mvGradPolMa.size() != 0)
		{
			for (register uint i = 0; i < mvGradPolMa.size(); i++)
				mvGradPolMa.at(i).Delete();
			mvGradPolMa.clear();
		}
	}

#ifndef _RDLL_
	void cArfima::Print(ostream& theOut) const
	{
	uint myNAr = mvAr.GetSize();
	uint myNMa = mvMa.GetSize();
		theOut << "ARFIMA(" << myNAr << ", d, " << myNMa << ") model with:" << endl;
		for (register uint i = 0; i < myNAr; i++)
			theOut << "\tAR[" << i + 1 << "]=" << mvAr[i] << endl;
		theOut << "\td=" << mvFracD << endl;
		for (register uint i = 0; i < myNMa; i++)
			theOut << "\tMA[" << i + 1 << "]=" << mvMa[i] << endl;
	}
#else
	void cArfima::Print(void)
	{
	uint myNAr = mvAr.GetSize();
	uint myNMa = mvMa.GetSize();
		Rprintf("ARFIMA(%d, d, ) model with:\n", myNAr, myNMa);
		for (register uint i = 0; i < myNAr; i++)
			Rprintf("\tAR[%d]=%f\n", i + 1, mvAr[i]);
		Rprintf("\td=%f\n", mvFracD);
		for (register uint i = 0; i < myNMa; i++)
			Rprintf("\tMA[%d]=%f\n", i + 1, mvMa[i]);
	}
#endif // _RDLL_
	void  cArfima::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvAr = 0.1;
		mvMa = 0.1;
		mvFracD = 0.5;

	}

	void  cArfima::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		mvAr = 0.1;
		mvMa = 0.1;
		mvFracD = 0.5;

	}

	void  cArfima::ReAlloc(const uint theSize, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			mvAr.ReAlloc(theSize);
			break;
		case 1:
			mvMa.ReAlloc(theSize);
			break;
		default:
			throw cError("cArfima::ReAlloc - theNumParam must be in 0 or 1");
			break;
		}
	}
	
	void  cArfima::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0: // mvAr
			mvAr = theVectParam;
			break;
		case 1: // mvMa
			mvMa = theVectParam;
			break;
		default:
			throw cError("cArfima::ReAlloc - theNumParam must be in 0 or 1");
			break;
		}

	}
	
	void  cArfima::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0: // AR
			if (theIndex < mvAr.GetSize())
				mvAr[theIndex] = theValue;
			else
				throw cError("cArfima::Set - wrong index");
			break;
		case 1:
			if (theIndex < mvMa.GetSize())
				mvMa[theIndex] = theValue;
			else
				throw cError("cArfima::Set - wrong index");
			break;
		case 2:
			mvFracD = theValue;
			break;

		default:
			throw cError("cArfima::Set - theNumParam must be in 0, 1, 2");
			break;

		}
	}
	
	void  cArfima::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			mvAr = theVectParam;
			break;
		case 1:
			mvMa = theVectParam;
			break;
		case 2:
			if (theVectParam.GetSize() > 0)
				mvFracD = theVectParam[0];
			else
				throw cError("cArfima::Set - Size of theVectParam must be > 0");
			break;
		default:
			throw cError("cArfima::Set - theNumParam must be in 0, 1, 2");
			break;
		}
	}
	
	double  cArfima::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			return mvAr[theIndex];
			break;
		case 1:
			return mvMa[theIndex];
			break;
		case 2:
			return mvFracD;
			break;
		}

	}

	cDVector& cArfima::Get(const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			return mvAr;
			break;
		case 1:
			return mvMa;
			break;
		case 2:
			ourAuxVector.ReAlloc(1, mvFracD);
			return ourAuxVector;
			break;
		}

	}

	cArfima& cArfima::operator =(const cArfima& theSrc)
	{
		if (GetCondMeanType() == eUnknown)
		{
			mvAr = cDVector(theSrc.mvAr);
			mvMa = cDVector(theSrc.mvMa);
			mvPolMa = cPolynome(theSrc.mvPolMa);
			SetCondMeanType(eArfima);
		}
		else
		{
			mvAr =theSrc.mvAr;
			mvMa = theSrc.mvMa ;
			mvPolMa = theSrc.mvPolMa;
		}
		mvFracD = theSrc.mvFracD;
		mvNTruncLag = theSrc.mvNTruncLag;
		
	uint myNParam = mvAr.GetSize() + mvMa.GetSize() + 1;
		mvGradPolMa.resize(myNParam);
		for (uint i = 0; i < myNParam; i++)
			mvGradPolMa.at(i) = theSrc.mvGradPolMa.at(i);
		return *this;
	}
	
	void  cArfima::UpdateProxyMeanParameters(void)
	{
/*
		cout << "UpdateProxyMeanParameters" << endl;
		Print();
		cout << endl;
*/
	uint myNAr = GetNAr();
	uint myNMa = GetNMa();
	
	cPolynome myPhi(myNAr);
	cPolynome myTeta(myNMa);
		myPhi[0] = myTeta[0] = 1.0;
		for (register uint i = 1; i <= myNAr; i++)
			myPhi[i] = -mvAr[i-1];
		for (register uint i = 1; i <= myNMa; i++)
			myTeta[i] = mvMa[i-1];
	cPolynome myDelta(0);
		ComputeDeltaPowD(-mvFracD, mvNTruncLag, myDelta);
	cPolynome myPol1(0), myRest(0);
		IncrPowDiv(myTeta, myPhi, mvNTruncLag+myNAr, myPol1, myRest);
	cPolynome myPol2(0);
		IncrPowDiv(myPol1, myPhi, mvNTruncLag + myNAr, myPol2, myRest);
	cPolynome myPol3(0);
			myPol3 = TrunkMult(myDelta, myPol2, mvNTruncLag);
		mvPolMa = TrunkMult(myDelta, myPol1, mvNTruncLag);
		mvPolMa[0] = 0.0;
	cPolynome myPol4(0);
		IncrPowDiv(myDelta, myPhi, mvNTruncLag + myNAr, myPol4, myRest);
	cPolynome myPol5(mvNTruncLag);
		for (register uint i = 1; i <= mvNTruncLag; i++)
			myPol5[i] = 1 / (double)i;
	cPolynome myPol6(0);
		myPol6 = TrunkMult(myPol5, myDelta, mvNTruncLag);
	cPolynome myPol7(0);
		myPol7 = TrunkMult(myPol6, myPol1, mvNTruncLag);
	cPolynome myPolX(1);
		myPolX[1] = 1;
		for (uint i = 0; i < myNAr; i++)
		{
			myPol3 = TrunkMult(myPol3, myPolX, mvNTruncLag);
			mvGradPolMa.at(i) = myPol3;
		}
		for (uint i = myNAr; i < myNAr + myNMa; i++)
		{
			myPol4 = TrunkMult(myPol4, myPolX, mvNTruncLag);
			mvGradPolMa.at(i) = myPol4;
		}
		mvGradPolMa.at(myNAr + myNMa) = myPol7;
	}

	double  cArfima::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
	
		return mvPolMa.BackwardPolOp(theData.mUt, theDate);

	}
	
	uint  cArfima::GetNParam(void) const
	{
		return mvAr.GetSize() + mvMa.GetSize() + 1;
	}
	
	uint  cArfima::GetNAr(void) const
	{
		return mvAr.GetSize();
	}

	uint  cArfima::GetNMa(void) const
	{
		return mvMa.GetSize();
	}
		
	uint  cArfima::GetNLags(void) const
	{
		return MAX(MAX(mvMa.GetSize(), mvAr.GetSize()), mvNTruncLag);
	}

	void  cArfima::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
#ifndef _DEBUG1
	uint myNAr = GetNAr();
	uint myNMa = GetNMa();
	register uint i;
		for (i = 0; i < myNAr + myNMa + 1; i++)
			theGradData.mCurrentGradMu[theBegIndex + i] += mvGradPolMa.at(i).BackwardPolOp(theValue.mUt, theDate);
		for (i = 0; i < myNAr + myNMa + 1; i++)
		{
			for (register uint t = 0; t < mvNTruncLag; t++)
				theGradData.mCurrentGradMu[theBegIndex + i] -= mvPolMa[t+1]* theGradData.mGradMt[t][i];
		}
#else
		NumericComputeGrad(theDate, theValue, theGradData, theBegIndex, theResiduals);
#endif //_DEBUG

	}

	void  cArfima::NumericComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals, double theh)
	{
	uint myNAr = GetNAr();
	uint myNMa = GetNMa();
		UpdateProxyMeanParameters();
	double myF0 = ComputeMean(theDate, theValue);
		for (uint i = 0; i < myNAr; i++)
		{
		double myh = fabs(mvAr[i] * theh);
			myh = MAX(myh, 1e-8);
			mvAr[i] += myh;
			UpdateProxyMeanParameters();
		double myF1 = ComputeMean(theDate, theValue);
			theGradData.mCurrentGradMu[theBegIndex + i] += (myF1 - myF0) / myh;
			mvAr[i] -= myh;
		}
		for (uint i = 0; i < myNMa; i++)
		{
		double myh = fabs(mvMa[i] * theh);
			myh = MAX(myh, 1e-8);
			mvMa[i] += myh;
			UpdateProxyMeanParameters();
		double myF1 = ComputeMean(theDate, theValue);
			theGradData.mCurrentGradMu[theBegIndex + i + myNAr] += (myF1 - myF0) / myh;
			mvMa[i] -= myh;
		}
	double myh = fabs(mvFracD * theh);
		myh = MAX(myh, 1e-8);
		mvFracD += myh;
		UpdateProxyMeanParameters();
	double myF1 = ComputeMean(theDate, theValue);
		theGradData.mCurrentGradMu[theBegIndex  + myNAr + myNMa] += (myF1 - myF0) / myh;
		mvFracD -= myh;
	}

	void  cArfima::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam();
		if (theDestVect.GetSize() < mySize + theIndex)
				throw cError("Wrong size");
		mvAr.SetSubVectorWithThis(theDestVect, theIndex);
		mvMa.SetSubVectorWithThis(theDestVect, theIndex + mvAr.GetSize());
		theDestVect[theIndex + mvAr.GetSize() + mvMa.GetSize()] = mvFracD;

	}
	
	void  cArfima::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize();
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size");
		mvAr.SetThisWithSubVector(theSrcVect, theIndex);
		mvMa.SetThisWithSubVector(theSrcVect, theIndex + mvAr.GetSize());
		mvFracD = theSrcVect[theIndex + mvAr.GetSize() + mvMa.GetSize()];


	}

	void  cArfima::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals)
	{

	}
	
	void cArfima::GetParamName(uint theIndex, char** theName)
	{
	uint myIndex = theIndex;
		for (uint i = 0; i < mvAr.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "AR[%d]", i + 1);

		}
		for (uint i = 0; i < mvMa.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "MA[%d]", i + 1);

		}
		sprintf(theName[myIndex++ + 1], "Frac. d");

	}


} // namespace
