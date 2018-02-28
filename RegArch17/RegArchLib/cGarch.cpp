#include "StdAfxRegArchLib.h"
/*!
	\file cGarch.cpp
	\brief sources for class cGarch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cGarch::cGarch(uint theNArch, uint theNGarch):cAbstCondVar(eGarch)
	 * \param uint theNArch: number of ARCH lags
	 * \param uint theNGarch: number of GARCH lags
	*/
	cGarch::cGarch(uint theNArch, uint theNGarch)
		:cAbstCondVar(eGarch)  // call constructor of cAbstCondVar with type eGarch
	{
		mvArch.ReAlloc(theNArch);
		mvGarch.ReAlloc(theNGarch);
		mvConst = 0.0;
		MESS_CREAT("cGarch");
	}

	/*!
	 * \fn cGarch::cGarch(double theConst, cDVector& theArch, cDVector& theGarch):cAbstCondVar(eGarch)
	 * \param double theConst: constant part of the GARCH(p, q) model
	 * \param cDVector& theGarch theArch: ARCH parameters
	 * \param cDVector& theGarch theGarch: GARCH parameters
	*/
	cGarch::cGarch(double theConst, cDVector& theArch, cDVector& theGarch)
		:cAbstCondVar(eGarch)
	{
		mvConst = theConst;
		mvArch = theArch;
		mvGarch = theGarch;
		MESS_CREAT("cGarch");
	}

	/*!
	* \fn cGarch::cGarch(const cAbsCondVar& theGarch):cAbstCondVar(eGarch)
	* \param cAbsCondVar& theEgarch: theGarch class
	*/
	cGarch::cGarch(const cGarch& theGarch)
		:cAbstCondVar(eGarch)
	{
		*this = theGarch;
		MESS_CREAT("cGarch");
	}

	/*!
	 * \fn cGarch::~cGarch()
	*/
	cGarch::~cGarch()
	{
		mvArch.Delete();
		mvGarch.Delete();
		MESS_DESTR("cGarch");
	}

	/*!
	 * \fn cAbstCondVar* cGarch::PtrCopy()
	 */
	cAbstCondVar* cGarch::PtrCopy() const
	{
		cGarch *myGarch = new cGarch(*this);
		return myGarch;
	}

	/*!
	 * \fn void cGarch::Delete(void)
	 * \param void
	 * \details Free memory
	 */
	void cGarch::Delete(void)
	{
		mvArch.Delete();
		mvGarch.Delete();
	}

	/*!
	 * \fn void cGarch::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
#ifndef _RDLL_
	void cGarch::Print(ostream& theOut) const
	{
		uint myNArch = mvArch.GetSize();
		uint myNGarch = mvGarch.GetSize();
		theOut << "GARCH(" << myNArch << ", " << myNGarch << ") model with:" << endl;
		theOut << "\tCste=" << mvConst << endl;
		for (register uint i = 0; i < myNArch; i++)
			theOut << "\tARCH[" << i + 1 << "]=" << mvArch[i] << endl;
		for (register uint j = 0; j < myNGarch; j++)
			theOut << "\tGARCH[" << j + 1 << "]=" << mvGarch[j] << endl;
	}
#else
	void cGarch::Print(void)
	{
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
		Rprintf("GARCH(%d, %d) model with:\n", myNArch, myNGarch);
		Rprintf("\tCste=%f\n", mvConst);
		for (register uint i = 0; i < myNArch; i++)
			Rprintf("\tARCH[%d]=%f\n", i + 1, mvArch[i]);
		for (register uint j = 0; j < myNGarch; j++)
			Rprintf("\tGARCH[%d]=%f\n", j + 1, mvGarch[j]);
	}
#endif //_RDLL_

	void cGarch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvConst = theVar*0.1 ;
	uint myNArch = mvArch.GetSize() ;
	uint myNGarch = mvGarch.GetSize() ;
	uint i ;
		for (i = 0 ; i < myNArch ; i++)
			mvArch[i] = 0.1/(double)myNArch ;
		for (i = 0 ; i < myNGarch ; i++)
			mvGarch[i] = 0.8/(double)myNGarch ;
	}

	void cGarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvConst = myVar*0.1;
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
	uint i;
		for (i = 0; i < myNArch; i++)
			mvArch[i] = 0.1 / (double)myNArch;
		for (i = 0; i < myNGarch; i++)
			mvGarch[i] = 0.8 / (double)myNGarch;
	}

	/*!
	 * \fn void cGarch::ReAlloc(const uint theSize, const uint theNumParam)
	 * \param const uint theSize: new size of mvArch or mvGarch
	 * \param const uint theNumParam: 0 for mvArch, 1 for mvGarch.
	 * \details new allocation of mvArch or mvGarch
	 */
	void cGarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 1 :
				mvArch.ReAlloc(theSize) ;
			break ;
			case 2 :
				mvGarch.ReAlloc(theSize) ;
			break ;
			default :
	//			throw cError("cGarch::ReAlloc - theNumParam must be in 1, 2.") ;
			break ;
		}
	}

	/*!
	 * \fn void cGarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of Const, ARCH or GARCH coefficients
	 * \param const uint theNumParam: =0, the constant part; =1 the ARCH coefficients; =2 theGARCH Coefficients
	 * \details new allocation of mvArch or mvConst
	 */
	void cGarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0: // mvConst
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cGarch::ReAlloc - Size of theVectParam must be > 0") ;
			break ;
			case 1 : // mvArch
				mvArch = theVectParam ;
			break ;
			case 2 : // mvGarch
				mvGarch = theVectParam ;
			break ;
			default :
				throw cError("cGarch::ReAlloc - theNumParam must be in 0, 1, 2") ;
			break ;
		}
	}

	/*!
	 * \fn void cGarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const double theValue: the value of the "theIndex" th lag. Default 0.
	 * \param const uint theIndex: the index.
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, GARCH parameters
	 * \details mvArch[theIndex] = theValue or mvGarch[theIndex]= theValue or mvConst = theValue
	 */
	void cGarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				mvConst = theValue ;
			break ;
			case 1 :
				if (theIndex < mvArch.GetSize())
					mvArch[theIndex] = theValue ;
				else
					throw cError("cGarch::Set - wrong index") ;
			break ;
			case 2 :
				if (theIndex < mvGarch.GetSize())
					mvGarch[theIndex] = theValue ;
				else
					throw cError("cGarch::Set - wrong index") ;
			break ;
			default:
				throw cError("cGarch::Set - theNumParam must be in 0, 1, 2") ;
			break ;
		}
	}

	/*!
	 * \fn void cGarch::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the vector of values
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, GARCH parameters
	 * \details mvAr = theValue
	 */
	void cGarch::Set(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cArch::Set - Size of theVectParam must be > 0") ;
			break ;
			case 1 :
				mvArch = theVectParam ;
			break ;
			case 2 :
				mvGarch = theVectParam ;
			break ;
			default:
				throw cError("cGarch::Set - theNumParam must be in 0, 1, 2") ;
			break ;
		}
	}

	double  cGarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				return mvConst ;
			break ;
			case 1 :
				return mvArch[theIndex] ;
			break ;
			case 2 :
				return mvGarch[theIndex] ;
			break ;
		}
	}

	cDVector& cGarch::Get(const uint theNumParam)
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
		}
	}

	cGarch& cGarch::operator =(const cGarch& theSrc)
	{
		mvConst = theSrc.mvConst;
		mvArch = theSrc.mvArch;
		mvGarch = theSrc.mvGarch;
		return *this;
	}

	/*!
	 * \fn double cGarch::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas
	 * \details theData is not updated here.
	*/
	double cGarch::ComputeVar(uint theDate, const cRegArchValue& theData) const 
	{
	uint myp = mvArch.GetSize(),
		myq = mvGarch.GetSize() ;
	double myRes = mvConst ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] * theData.mUt[theDate-i] * theData.mUt[theDate-i] ;
		for (register uint j = 1 ; j <= MIN(myq, theDate) ; j++)
			myRes += mvGarch[j-1] * theData.mHt[theDate-j] ;

		return myRes ;

	}

	uint cGarch::GetNParam(void) const
	{
		return 1 + mvArch.GetSize() + mvGarch.GetSize() ;
	}

	uint cGarch::GetNLags(void) const
	{
		return  MAX(mvArch.GetSize(), mvGarch.GetSize()) ;
	}

	void cGarch::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize(),
		myq = mvGarch.GetSize(),
		myBegIndex = theGradData.GetNMeanParam() ;
	register uint i, j ;
		theGradData.mCurrentGradVar = 0.0L ;
		theGradData.mCurrentGradVar[myBegIndex] = 1.0 ;
	//ARCH
		for (i = 1 ; i <=  MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar[myBegIndex+i] = theValue.mUt[theDate-i]*theValue.mUt[theDate-i] ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar -= 2.0 * mvArch[i-1] * theValue.mUt[theDate-i] * theGradData.mGradMt[i-1] ;
	//GARCH
		for (j = 1; j <= MIN(myq, theDate); j++)
			theGradData.mCurrentGradVar[myBegIndex + myp + j] += theValue.mHt[theDate - j];
		for (j = 1; j <= MIN(myq, theDate); j++)
			theGradData.mCurrentGradVar += mvGarch[j-1] * theGradData.mGradHt[j-1] ;
	}

	void cGarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		theDestVect[theIndex] = mvConst ;
		mvArch.SetSubVectorWithThis(theDestVect, theIndex + 1) ;
		mvGarch.SetSubVectorWithThis(theDestVect, theIndex + 1 + mvArch.GetSize()) ;
	}

	void cGarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvConst = theSrcVect[theIndex] ;
		mvArch.SetThisWithSubVector(theSrcVect, theIndex+1) ;
		mvGarch.SetThisWithSubVector(theSrcVect, theIndex + 1 + mvArch.GetSize()) ;
	}

	void cGarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize(), 
		myq = mvGarch.GetSize() ;
	uint myBegIndex = theGradData.GetNMeanParam();
		theHessData.mCurrentHessVar = 0.0;
	register uint i, j ;
	// ARCH
		cDMatrix myMat = theHessData.mCurrentHessVar;
		for (i = 1; i <= MIN(myp, theDate); i++)
			myMat.SetRow(myBegIndex + i, -2 * theData.mUt[theDate - i] * theGradData.mGradMt[i - 1]);
		theHessData.mCurrentHessVar += myMat + Transpose(myMat);
		for (i = 1; i <= MIN(myp, theDate); i++)
			theHessData.mCurrentHessVar -= 2.0 * mvArch[i - 1] * theData.mUt[theDate - i] * theHessData.mHessMt[i - 1];
	// GARCH
		myMat = 0.0;
		for (j = 1; j <= MIN(myq, theDate); j++)
			myMat.SetRow(myBegIndex + myp + j, theGradData.mGradHt[j - 1]);
		theHessData.mCurrentHessVar += myMat + Transpose(myMat);

		for (j = 1; j <= MIN(myq, theDate); j++)
			theHessData.mCurrentHessVar += mvGarch[j - 1] * theHessData.mHessHt[j - 1];
	}
	
	void cGarch::GetParamName(uint theIndex, char** theName)
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
	}

}//namespace
