#include "StdAfxRegArchLib.h"
/*!
	\file cUgarch.cpp
	\brief sources for class cUgarch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date  sept-26-2016 - Last change feb-26-2016
*/
namespace RegArchLib {
	/*!
	 * \fn cUgarch::cUgarch(uint theNArch, uint theNGarch):cAbstCondVar(eUgarch)
     * \param bool theExistConst
	 * \param uint theNArch: number of ARCH lags
	 * \param uint theNGarch: number of GARCH lags
	*/
	cUgarch::cUgarch(bool theExistConst, uint theNBeta, uint theNArch, uint theNGarch)
		:cAbstCondVar(eUgarch)  // call constructor of cAbstCondVar with type eUgarch
	{
		mvArch.ReAlloc(theNArch);
		mvGarch.ReAlloc(theNGarch);
		mvBetaS.ReAlloc(theNBeta);
		mvConst = 0.0;
        mvExistConst = theExistConst ;
		MESS_CREAT("cUgarch");
	}

	/*!
	 * \fn cUgarch::cUgarch(double theConst, cDVector& theArch, cDVector& theGarch):cAbstCondVar(eUgarch)
	 * \param bool theExistConst: true if const exists
	 * \param double theConst: constant part of the UGARCH(p, q) model if exists
	 * \param cDVector& theBetaS : Beta parameters
	 * \param cDVector& theGarch theArch: ARCH parameters
	 * \param cDVector& theGarch theGarch: GARCH parameters
	*/
	cUgarch::cUgarch(bool theExistConst, double theConst, cDVector& theBetaS, cDVector& theArch, cDVector& theGarch)
		:cAbstCondVar(eUgarch)
	{
		mvExistConst = theExistConst ;
        mvConst = theConst;
        mvBetaS = theBetaS ;
		mvArch = theArch;
		mvGarch = theGarch;
		MESS_CREAT("cUgarch");
	}

	/*!
	* \fn cUgarch::cUgarch(const cAbsCondVar& theGarch):cAbstCondVar(eUgarch)
	* \param cAbsCondVar& theEgarch: theGarch class
	*/
	cUgarch::cUgarch(const cUgarch& theUgarch)
		:cAbstCondVar(eUgarch)
	{
		*this = theUgarch;
		MESS_CREAT("cUgarch");
	}

	/*!
	 * \fn cUgarch::~cUgarch()
	*/
	cUgarch::~cUgarch()
	{
		mvArch.Delete();
		mvGarch.Delete();
        mvBetaS.Delete() ;
		MESS_DESTR("cUgarch");
	}

	/*!
	 * \fn cAbstCondVar* cUgarch::PtrCopy()
	 */
	cAbstCondVar* cUgarch::PtrCopy() const
	{
		cUgarch *myUgarch = new cUgarch(*this);
		return myUgarch;
	}

	/*!
	 * \fn void cUgarch::Delete(void)
	 * \param void
	 * \details Free memory
	 */
	void cUgarch::Delete(void)
	{
		mvArch.Delete();
		mvGarch.Delete();
        mvBetaS.Delete() ;
	}

	/*!
	 * \fn void cUgarch::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
#ifndef _RDLL_
	void cUgarch::Print(ostream& theOut) const
	{
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
    uint myNBeta = mvBetaS.GetSize() ;
		theOut << "UGARCH(" << myNArch << ", " << myNGarch << ") model with:" << endl;
		if (mvExistConst)
        {   theOut << "\tCste=" << mvConst << endl;
		}
        for (register uint i = 0 ; i < myNBeta ; i++)
            theOut << "\tBeta[" << i + 1 << "]=" << mvBetaS[i] << endl ;
        for (register uint i = 0; i < myNArch; i++)
			theOut << "\tARCH[" << i + 1 << "]=" << mvArch[i] << endl;
		for (register uint j = 0; j < myNGarch; j++)
			theOut << "\tGARCH[" << j + 1 << "]=" << mvGarch[j] << endl;
	}
#else
	void cUgarch::Print(void)
	{
	uint myNArch = mvArch.GetSize();
	uint myNUgarch = mvGarch.GetSize();
    uint myNBeta = mvBetaS.GetSize() ;
    	Rprintf("UGARCH(%d, %d) model with:\n", myNArch, myNUgarch);
		if (mvExistConst)
        {   Rprintf("\tCste=%f\n", mvConst);
		}
        for (register uint i = 0; i < myNBeta; i++)
			Rprintf("\tBeta[%d]=%f\n", i + 1, mvBetaS[i]);
		for (register uint i = 0; i < myNArch; i++)
			Rprintf("\tARCH[%d]=%f\n", i + 1, mvArch[i]);
		for (register uint j = 0; j < myNUgarch; j++)
			Rprintf("\tGARCH[%d]=%f\n", j + 1, mvGarch[j]);
	}
#endif //_RDLL_

	void cUgarch::SetDefaultInitPoint(double theMean, double theVar)
	{
	   if (mvExistConst)
       {    mvConst = theVar*0.1 ;
            mvBetaS = 0.0 ;
       }
       else
       {
            mvBetaS = theVar*0.1 ;
       }
	uint myNArch = mvArch.GetSize() ;
	uint myNGarch = mvGarch.GetSize() ;
	uint i ;
		for (i = 0 ; i < myNArch ; i++)
			mvArch[i] = 0.1/(double)myNArch ;
		for (i = 0 ; i < myNGarch; i++)
			mvGarch[i] = 0.8/(double)myNGarch;
	}

	void cUgarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		if (mvExistConst)
		{
			mvConst = myVar*0.1;
			mvBetaS = 0.0;
		}
		else
		{
			mvBetaS = myVar*0.1;
		}
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
	uint i;
		for (i = 0; i < myNArch; i++)
			mvArch[i] = 0.1 / (double)myNArch;
		for (i = 0; i < myNGarch; i++)
			mvGarch[i] = 0.8 / (double)myNGarch;
	}

	/*!
	 * \fn void cUgarch::ReAlloc(const uint theSize, const uint theNumParam)
	 * \param const uint theSize: new size of mvArch or mvGarch
	 * \param const uint theNumParam: 0 for mvArch, 1 for mvGarch.
	 * \details new allocation of mvArch or mvGarch
	 */
	void cUgarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		switch (theNumParam)
		{
			case 1:
				mvBetaS.ReAlloc(theSize);
			break;
			case 2 :
				mvArch.ReAlloc(theSize) ;
			break ;
			case 3 :
				mvGarch.ReAlloc(theSize) ;
			break ;
			default :
	//			throw cError("cUgarch::ReAlloc - theNumParam must be in 1, 2.") ;
			break ;
		}
	}

	/*!
	 * \fn void cUgarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of Const, ARCH or GARCH coefficients
	 * \param const uint theNumParam: =0, the constant part; =1 the ARCH coefficients; =2 theGARCH Coefficients
	 * \details new allocation of mvArch or mvConst
	 */
	void cUgarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0: // mvConst
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cUgarch::ReAlloc - Size of theVectParam must be > 0") ;
			break ;
			case 1: // mvArch
				mvBetaS = theVectParam;
			break;
			case 2 : // mvArch
				mvArch = theVectParam ;
			break ;
			case 3 : // mvGarch
				mvGarch = theVectParam ;
			break ;
			default :
				throw cError("cUgarch::ReAlloc - theNumParam must be in 0, 1, 2") ;
			break ;
		}
	}

	/*!
	 * \fn void cUgarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const double theValue: the value of the "theIndex" th lag. Default 0.
	 * \param const uint theIndex: the index.
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, GARCH parameters
	 * \details mvArch[theIndex] = theValue or mvGarch[theIndex]= theValue or mvConst = theValue
	 */
	void cUgarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				mvConst = theValue ;
			break ;
			case 1:
				if (theIndex < mvBetaS.GetSize())
					mvBetaS[theIndex] = theValue;
				else
					throw cError("cUgarch::Set - wrong index");
				break;
			case 2 :
				if (theIndex < mvArch.GetSize())
					mvArch[theIndex] = theValue ;
				else
					throw cError("cUgarch::Set - wrong index") ;
			break ;
			case 3 :
				if (theIndex < mvGarch.GetSize())
					mvGarch[theIndex] = theValue ;
				else
					throw cError("cUgarch::Set - wrong index") ;
			break ;
			case 4 :
				mvExistConst = (bool)theValue;
			break;
			default:
				throw cError("cUgarch::Set - theNumParam must be in 0..4") ;
			break ;
		}
	}

	/*!
	 * \fn void cUgarch::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the vector of values
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, GARCH parameters
	 * \details mvAr = theValue
	 */
	void cUgarch::Set(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cUgarch::Set - Size of theVectParam must be > 0") ;
			break ;
			case 1:
				mvBetaS = theVectParam;
				break;
			case 2 :
				mvArch = theVectParam ;
			break ;
			case 3 :
				mvGarch = theVectParam ;
			break ;
			case 4:
				if (theVectParam.GetSize() > 0)
					mvExistConst = (bool)theVectParam[0];
				else
					throw cError("cUgarch::Set - Size of theVectParam must be > 0");
			break;
			default:
				throw cError("cUgarch::Set - theNumParam must be in 0..4") ;
			break ;
		}
	}

	double  cUgarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				return mvConst ;
			break ;
			case 1:
				return mvBetaS[theIndex];
				break;
			case 2 :
				return mvArch[theIndex] ;
			break ;
			case 3 :
				return mvGarch[theIndex] ;
			break ;
			case 4:
				return (double)mvExistConst;
			break;
		}
	}

	cDVector& cUgarch::Get(const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			ourAuxVector.ReAlloc(1, mvConst);
			return ourAuxVector;
		break;
		case 1:
			return mvBetaS;
		break;
		case 2:
			return mvArch;
		break;
		case 3:
			return mvGarch;
		break;
		case 4:
			ourAuxVector.ReAlloc(1, (double)mvExistConst);
			return ourAuxVector;
		break;
		}
	}

	cUgarch& cUgarch::operator =(const cUgarch& theSrc)
	{
		mvConst = theSrc.mvConst;
		mvBetaS = theSrc.mvBetaS;
		mvArch = theSrc.mvArch;
		mvGarch = theSrc.mvGarch;
		mvExistConst = theSrc.mvExistConst;

		return *this;
	}

	/*!
	 * \fn double cUgarch::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas
	 * \details theData is not updated here.
	*/
	double cUgarch::ComputeVar(uint theDate, const cRegArchValue& theData) const 
	{
	uint	myp = mvArch.GetSize(),
			myq = mvGarch.GetSize(),
			myNBeta = mvBetaS.GetSize();
	double myRes = 0;
		if (mvExistConst)
			myRes = mvConst;

		for (register uint i = 0; i < myNBeta ; i++)
			myRes += mvBetaS[i] * theData.mXt[theDate][i] ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] * theData.mUt[theDate-i] * theData.mUt[theDate-i] ;
		for (register uint j = 1 ; j <= MIN(myq, theDate) ; j++)
			myRes += mvGarch[j-1] * theData.mHt[theDate-j] ;

		return myRes ;

	}

	uint cUgarch::GetNParam(void) const
	{
		return  mvBetaS.GetSize() + mvArch.GetSize() + mvGarch.GetSize() + 1*(mvExistConst) ;
	}

	uint cUgarch::GetNLags(void) const
	{
		return  MAX(mvArch.GetSize(), mvGarch.GetSize()) ;
	}

	void cUgarch::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize(),
		myq = mvGarch.GetSize(),
		myNBeta = mvBetaS.GetSize(),
		myBegIndex = theGradData.GetNMeanParam() ;

	register uint i, j ;
		theGradData.mCurrentGradVar = 0.0L ;
	// CONST
		if (mvExistConst)
			theGradData.mCurrentGradVar[myBegIndex] = 1.0;
		else
			myBegIndex--;
	// Betas
		for (i = 1; i <= myNBeta; i++)
			theGradData.mCurrentGradVar[myBegIndex + i] = theValue.mXt[theDate][i - 1];
	//ARCH
		for (i = 1 ; i <=  MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar[myBegIndex + myNBeta + i] = theValue.mUt[theDate-i]*theValue.mUt[theDate-i] ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar -= 2.0 * mvArch[i-1] * theValue.mUt[theDate-i] * theGradData.mGradMt[i-1] ;
	//GARCH
		for (j = 1; j <= MIN(myq, theDate); j++)
			theGradData.mCurrentGradVar[myBegIndex + myNBeta + myp + j] += theValue.mHt[theDate - j];
		for (j = 1; j <= MIN(myq, theDate); j++)
			theGradData.mCurrentGradVar += mvGarch[j-1] * theGradData.mGradHt[j-1] ;
	}

	void cUgarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam(),
		myNBeta = mvBetaS.GetSize(),
		myNArch = mvArch.GetSize(),
		myIndex = theIndex ;

		if (theDestVect.GetSize() < mySize + myIndex)
			throw cError("Wrong size") ;
		if (mvExistConst)
			theDestVect[myIndex] = mvConst;
		else
			myIndex--;
		mvBetaS.SetSubVectorWithThis(theDestVect, myIndex + 1);
		mvArch.SetSubVectorWithThis(theDestVect, myIndex + 1 + myNBeta) ;
		mvGarch.SetSubVectorWithThis(theDestVect, myIndex + 1 + myNBeta + myNArch) ;
	}

	void cUgarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize(),
		myNBeta = mvBetaS.GetSize(),
		myNArch = mvArch.GetSize(),
		myIndex = theIndex ;

		if (GetNParam() + myIndex > mySize)
			throw cError("Wrong size") ;
		if (mvExistConst)
			mvConst = theSrcVect[myIndex];
		else
			myIndex--;
		mvBetaS.SetThisWithSubVector(theSrcVect, myIndex + 1);
		mvArch.SetThisWithSubVector(theSrcVect, myIndex + myNBeta + 1) ;
		mvGarch.SetThisWithSubVector(theSrcVect, myIndex + 1 + myNBeta + myNArch) ;
	}

	void cUgarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize(), 
		myq = mvGarch.GetSize(),
		myNBeta = mvBetaS.GetSize() ;

	uint myBegIndex = theGradData.GetNMeanParam();
		theHessData.mCurrentHessVar = 0.0;
	register uint i, j ;
	if (!mvExistConst)
		myBegIndex--;
	// ARCH
		cDMatrix myMat = theHessData.mCurrentHessVar;
		for (i = 1; i <= MIN(myp, theDate); i++)
			myMat.SetRow(myBegIndex + myNBeta + i, -2 * theData.mUt[theDate - i] * theGradData.mGradMt[i - 1]);
		theHessData.mCurrentHessVar += myMat + Transpose(myMat);
		for (i = 1; i <= MIN(myp, theDate); i++)
			theHessData.mCurrentHessVar -= 2.0 * mvArch[i - 1] * theData.mUt[theDate - i] * theHessData.mHessMt[i - 1];
	// GARCH
		myMat = 0.0;
		for (j = 1; j <= MIN(myq, theDate); j++)
			myMat.SetRow(myBegIndex + myNBeta + myp + j, theGradData.mGradHt[j - 1]);
		theHessData.mCurrentHessVar += myMat + Transpose(myMat);

		for (j = 1; j <= MIN(myq, theDate); j++)
			theHessData.mCurrentHessVar += mvGarch[j - 1] * theHessData.mHessHt[j - 1];
	}
	
	void cUgarch::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		if (mvExistConst) 
			sprintf(theName[myIndex++], "CST VAR");


		for (uint i = 0; i < mvBetaS.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "UGARCH BETA[%d]", i + 1);

		}

		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH[%d]", i + 1);

		}
		for (uint i = 0; i < mvGarch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "GARCH[%d]", i + 1);

		}
	}



}//namespace
