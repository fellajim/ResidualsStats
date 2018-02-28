#include "StdAfxRegArchLib.h"
/*!
 \file cTarch.cpp
 \brief sources for class cStdDevInMean methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	cTarch::cTarch(int theNTarch)
	:cAbstCondVar(eTarch)  // call constructor of cAbstCondVar with type eTarch
	{
		mvCste = 0.0L ;
		mvArchPos.ReAlloc(theNTarch) ;
		mvArchNeg.ReAlloc(theNTarch) ;
		MESS_CREAT("cTarch") ;
	}

	cTarch::cTarch(const cTarch& theTarch)
	{
		*this = theTarch;
		MESS_CREAT("cTarch")
	}

	cTarch::~cTarch()
	{
		mvArchPos.Delete() ;
  		mvArchNeg.Delete() ;
  		MESS_DESTR("cTarch") ;
	}

	/*!
	 * \fn cAbstCondVar* cTarch::PtrCopy()
	 */

	cAbstCondVar* cTarch::PtrCopy() const
	{
	 cTarch *myTarch = new cTarch(*this);
		 return myTarch;
	}
	
	void cTarch::Delete(void)
	{	mvArchPos.Delete() ;
		mvArchNeg.Delete() ;
	}

#ifndef _RDLL_
	void cTarch::Print(ostream& theOut) const
	{
		theOut << "TARCH(" << mvArchPos.GetSize() << ") model:" << endl ;
		theOut << "Const=" << mvCste << endl ;
		for (register uint i = 0 ; i < mvArchPos.GetSize() ; i++)
			theOut << "ARCH+[" << i+1 << "]=" << mvArchPos[i] << endl ;
		for (register uint i = 0 ; i < mvArchNeg.GetSize() ; i++)
			theOut << "ARCH-[" << i+1 << "]=" << mvArchNeg[i] << endl ;
	}
#else
	void cTarch::Print(void)
	{
		Rprintf("TARCH(%d) model with:\n", mvArchPos.GetSize());
		Rprintf("Const=%f\n", mvCste);
		for (register uint i = 0; i < mvArchPos.GetSize(); i++)
			Rprintf("ARCH+[%d]=%f\n", i+1,  mvArchPos[i]);
		for (register uint i = 0; i < mvArchNeg.GetSize(); i++)
			Rprintf("ARCH-[%d]=%f\n", i+1,  mvArchNeg[i]);
	}


#endif // -RDLL-

	void cTarch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvCste = theVar*0.1 ;
	uint myNArch = mvArchPos.GetSize() ;
	uint i ;
		for (i = 0 ; i < myNArch ; i++)
			mvArchPos[i] = 0.45/(double)myNArch ;
		for (i = 0 ; i < myNArch ; i++)
			mvArchNeg[i] = 0.45/(double)myNArch ;
	}

	void cTarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvCste = myVar*0.1;
	uint myNArch = mvArchPos.GetSize();
	uint i;
		for (i = 0; i < myNArch; i++)
			mvArchPos[i] = 0.45 / (double)myNArch;
		for (i = 0; i < myNArch; i++)
			mvArchNeg[i] = 0.45 / (double)myNArch;
	}

	void cTarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		mvArchPos.ReAlloc(theSize) ;
		mvArchNeg.ReAlloc(theSize) ;
	}

	void cTarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		mvArchPos.ReAlloc((int)theVectParam[0]) ;
		mvArchNeg.ReAlloc((int)theVectParam[0]) ;
	}

	void cTarch::Set(const cDVector& thecDVector, const uint thePlace)
	{
		if (thePlace == 0)
			mvCste = thecDVector[0] ;
		else
			if (thePlace == 1)
				mvArchPos = thecDVector ;
			else
				mvArchNeg = thecDVector ;
	}

	void cTarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
	uint mySize ;
		switch (theNumParam)
		{	case 0 :
				mvCste = theValue ;
			break ;
			case 1 :
			{	mySize = mvArchPos.GetSize() ;
				if (mySize < theIndex)
				{	mvArchPos.ReAlloc(theIndex+1) ;
					mvArchNeg.ReAlloc(theIndex+1) ;
				}
				mvArchPos[theIndex]= theValue ;
			}
			break ;
			case 2 :
			{	mySize = mvArchNeg.GetSize() ;
				if (mySize < theIndex)
				{	mvArchPos.ReAlloc(theIndex+1) ;
					mvArchNeg.ReAlloc(theIndex+1) ;
				}
				mvArchNeg[theIndex]= theValue ;
			}
			break ;
		}
	}

	double cTarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				return mvCste ;
			break ;
			case 1 :
			return mvArchPos[theIndex] ;
			break ;
			case 2 :
				return mvArchNeg[theIndex] ;
			break ;
		}
	}

	cDVector& cTarch::Get(const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			ourAuxVector.ReAlloc(1, mvCste);
			return ourAuxVector;
			break;
		case 1:
			return mvArchPos;
			break;
		case 2:
			return mvArchNeg;
			break;
		}
	}

	cTarch& cTarch::operator =(const cTarch& theSrc)
	{
		mvArchPos = theSrc.mvArchPos;
		mvArchNeg = theSrc.mvArchNeg;
		mvCste = theSrc.mvCste;
		return *this ;
	}

	double cTarch::ComputeVar(uint theDate, const cRegArchValue& theValue) const
	{
	uint myp = mvArchPos.GetSize() ;
	double myRes = mvCste ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			if (theValue.mUt[theDate-i] > 0)
				myRes += mvArchPos[i-1] * theValue.mUt[theDate-i] * theValue.mUt[theDate-i] ;
			else
				myRes += mvArchNeg[i-1] * theValue.mUt[theDate-i] * theValue.mUt[theDate-i] ;

		return myRes ;
	}

	uint cTarch::GetNParam(void) const
	{
		return 1 + 2*mvArchPos.GetSize() ;
	}

	uint cTarch::GetNLags(void) const
	{
		return mvArchPos.GetSize() ;
	}

	void cTarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		theDestVect[theIndex] = mvCste ;
		mvArchPos.SetSubVectorWithThis(theDestVect, theIndex+1) ;
		mvArchNeg.SetSubVectorWithThis(theDestVect, theIndex+1+mvArchPos.GetSize()) ;

	}

	void cTarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvCste = theSrcVect[theIndex] ;
		mvArchPos.SetThisWithSubVector(theSrcVect, theIndex+1) ;
		mvArchNeg.SetThisWithSubVector(theSrcVect, theIndex+1+mvArchPos.GetSize()) ;
	}

	void cTarch::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArchPos.GetSize() ;
		theGradData.mCurrentGradVar = 0.0L ;
	int myBegIndex = (int)theGradData.GetNMeanParam() ;
		theGradData.mCurrentGradVar[myBegIndex] = 1.0 ;
	register uint i ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			if (theData.mUt[theDate-i] > 0)
				theGradData.mCurrentGradVar[myBegIndex+i] = theData.mUt[theDate - i]*theData.mUt[theDate - i] ;
			else
				theGradData.mCurrentGradVar[myBegIndex+myp+i] = theData.mUt[theDate - i]*theData.mUt[theDate - i] ;

		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			if (theData.mUt[theDate-i] > 0)
				theGradData.mCurrentGradVar -= 2.0 * mvArchPos[i-1] * theData.mUt[theDate - i] * theGradData.mGradMt[i-1] ;
			else
				theGradData.mCurrentGradVar -= 2.0 * mvArchNeg[i-1] * theData.mUt[theDate - i] * theGradData.mGradMt[i-1] ;

	}
	
	void cTarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArchPos.GetSize();
	uint myBegIndex = theGradData.GetNMeanParam();
		theHessData.mCurrentHessVar = 0.0;
	register uint i;
	cDMatrix myMat = theHessData.mCurrentHessVar;
		for (i = 1; i <= MIN(myp, theDate); i++)
			if (theData.mUt[theDate-i] > 0)
				myMat.SetRow(myBegIndex + i, -2 * theData.mUt[theDate - i] * theGradData.mGradMt[i - 1]);
			else
				myMat.SetRow(myBegIndex + i + myp, -2 * theData.mUt[theDate - i] * theGradData.mGradMt[i - 1]);

		theHessData.mCurrentHessVar += myMat + Transpose(myMat);

		for (i = 1; i <= MIN(myp, theDate); i++)
		{
			theHessData.mCurrentHessVar -= 2.0 * mvArchPos[i - 1] * theData.mUt[theDate - i] * theHessData.mHessMt[i - 1];
			theHessData.mCurrentHessVar += 2.0 * mvArchNeg[i - 1] * theGradData.mGradMt[i - 1] * Transpose(theGradData.mGradMt[i - 1]);
		}

	}
		
	void cTarch::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
		for (uint i = 0; i < mvArchPos.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH+[%d]", i + 1);

		}
		for (uint i = 0; i < mvArchNeg.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH-[%d]", i + 1);

		}
	}



}//namespace
