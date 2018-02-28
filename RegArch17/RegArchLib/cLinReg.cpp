#include "StdAfxRegArchLib.h"
/*!
	\file cLinReg.cpp
	\brief sources for class cLinReg methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	cLinReg::cLinReg(int theNLinReg):cAbstCondMean(eLinReg)
	{
		mvBeta.ReAlloc(theNLinReg) ;
		MESS_CREAT("cLinReg")
	}

	cLinReg::cLinReg(const cDVector& theBeta):cAbstCondMean(eLinReg)
	{
		mvBeta = theBeta ;
		MESS_CREAT("cLinReg")
	}

	/*!
	 * \fn cLinReg::cLinReg(cAbstCondMean& theAbstCondMean)
	 * \param const cAbstCondMean& theAbstCondMean: the cLinReg source.
	 */
	cLinReg::cLinReg(const cLinReg& theLinReg) :cAbstCondMean(eLinReg)
	{
		*this = theLinReg;
		MESS_CREAT("cLinReg")
	}

	cLinReg::~cLinReg()
	{
		mvBeta.Delete() ;
		MESS_DESTR("cLinReg") ;
	}

	/*!
	 * \fn cAbstCondMean cLinReg::PtrCopy(void)
	 * \param void
	 */
	cAbstCondMean* cLinReg::PtrCopy(void) const
	{
	cLinReg* myLinReg = new cLinReg(*this) ;

		return myLinReg ;
	}

	void cLinReg::Delete(void)
	{
		mvBeta.Delete() ;
	}

#ifndef _RDLL_
	void cLinReg::Print(ostream& theOut) const
	{	cout << "Linear Regression with:" << endl ;
		for (register uint i = 0 ; i < mvBeta.GetSize() ; i++)
				theOut << "\tLinReg[" << i+1 << "] = " << mvBeta[i] << endl ;
	}
#else
	void cLinReg::Print(void)
	{
		Rprintf("Linear Regression with:\n");
		for (register uint i = 0; i < mvBeta.GetSize(); i++)
			Rprintf("\tLinReg[%d]=%f", i + 1, mvBeta[i]);

	}
#endif // _RDLL_

	void cLinReg::SetDefaultInitPoint(double theMean, double theVar)
	{
		for (register uint i = 0 ; i < mvBeta.GetSize() ; i++)
			mvBeta[i] = 0.0 ;
	}

	void cLinReg::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		mvBeta = Inv(Transpose(theValue.mXt)*theValue.mXt)*Transpose(theValue.mXt)*theValue.mYt;
	}

	void cLinReg::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		mvBeta = theVectParam ;
	}

	void cLinReg::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		mvBeta.ReAlloc(1) ;
		mvBeta[0] = theValue ;
	}

	double cLinReg::Get(const uint theIndex, const uint theNumParam)
	{
		return mvBeta[theIndex] ;
	}

	cDVector& cLinReg::Get(const uint theNumParam)
	{
		return mvBeta;
	}

	void cLinReg::ReAlloc(const cDVector& theParam, const uint theNumParam)
	{
		mvBeta.ReAlloc((int)theParam[0]) ;
	}

	void cLinReg::ReAlloc(const uint theParam, const uint theNumParam)
	{
		mvBeta.ReAlloc(theParam) ;
	}

	cLinReg& cLinReg::operator =(const cLinReg &theSrc)
	{
		mvBeta =theSrc.mvBeta ;
		return *this ;
	} 

	double cLinReg::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
		int myp = (int)mvBeta.GetSize() ;
		if (myp > 0)
		{	
			double myRes = 0.0 ;
				for (register int i = 0 ; i < myp ; i++)
						myRes += mvBeta[i] * theData.mXt[theDate][i] ; 
				return myRes ;
		}
		else 
			return 0.0 ;
	}

	uint cLinReg::GetNParam(void) const
	{
		return (int)mvBeta.GetSize() ;
	}

	uint cLinReg::GetNLags(void) const
	{
		return 0 ;
	}

	void cLinReg::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = mvBeta.GetSize() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		mvBeta.SetSubVectorWithThis(theDestVect, theIndex) ;
	}

	void cLinReg::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (mvBeta.GetSize() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvBeta.SetThisWithSubVector(theSrcVect, theIndex) ;
	}

	void cLinReg::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{	for (register int i = 0 ; i < (int)mvBeta.GetSize()  ; i++)
			theGradData.mCurrentGradMu[theBegIndex+i] += theData.mXt[theDate][i];
	}

	void cLinReg::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) 
	{
	// Hess = 0 
	}

	void cLinReg::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		for (uint i = 0; i < mvBeta.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "LINREG[%d]", i + 1);

		}
	}
}//namespace
