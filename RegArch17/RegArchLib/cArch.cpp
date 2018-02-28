#include "StdAfxRegArchLib.h"

/*!
	\file cArch.cpp
	\brief sources for class cArch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cArch::cArch(uint theNArch):cAbstCondVar(eArch)
	 * \param uint theNArch: number of lags
	 */
	cArch::cArch(uint theNArch)
	:cAbstCondVar(eArch)   // call constructor of cAbstCondVar with type eArch
	{
		mvArch.ReAlloc(theNArch) ;
		MESS_CREAT("cArch")
	}

	/*!
	* \fn cArch::cArch(const cAbstCondVar& theArch):cAbstCondVar(eArch)
	* \param const cAbstCondVar& theArch: the recopy parameter
	*/
	cArch::cArch(const cArch& theArch)
		:cAbstCondVar(eArch)   // call constructor of cAbstCondVar with type eArch
	{
		*this = theArch;
		MESS_CREAT("cArch")
	}

	/*!
	 * \fn cArch::~cArch()
	 */
	cArch::~cArch()
	{	mvArch.Delete() ;
  		MESS_DESTR("cArch") ;
	}

	/*!
	 * \fn void cArch::Delete(void)
	 * \param void
	 * \details Delete mvVar
	 */
	void cArch::Delete(void)
	{	mvArch.Delete() ;
	}

	/*!
	 * \fn cAbstCondVar* cArch::PtrCopy()
	 */
	cAbstCondVar* cArch::PtrCopy() const
	{
		 cArch *mycArch = new cArch(*this);
		 return mycArch;
	}

#ifdef _RDLL_
	void cArch::Print(void)
	{
	uint myNArch = mvArch.GetSize();
		Rprintf("ARCH(%d) model with:\n", myNArch);
		Rprintf("\tCste=%f\n",mvConst);
		for (register uint i = 0; i < mvArch.GetSize(); i++)
			Rprintf("\tARCH[%d]=%f\n", i + 1, mvArch[i]);
	}
#else
	/*!
	* \fn void cArch::Print(ostream& theOut) const
	* \param ostream& theOut: output stream (file or screen). Default cout.
	*/
	void cArch::Print(ostream& theOut) const
	{
		uint myNArch = mvArch.GetSize();
		theOut << "ARCH(" << myNArch << ") model with:" << endl;
		theOut << "\tCste=" << mvConst << endl;
		for (register uint i = 0; i < mvArch.GetSize(); i++)
			theOut << "\tARCH[" << i + 1 << "]=" << mvArch[i] << endl;
	}

#endif // _RDLL_
	cArch& cArch::operator =(const cArch& theSrc)
	{
		mvConst = theSrc.mvConst;
		mvArch = theSrc.mvArch;
		return *this;
	}

	void cArch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvConst = theVar*0.1 ;
	uint myNArch = mvArch.GetSize() ;
	uint i ;
		for (i = 0 ; i < myNArch ; i++)
			mvArch[i] = 0.8/(double)myNArch ;
	}

	void cArch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvConst = myVar*0.1;
		uint myNArch = mvArch.GetSize();
		uint i;
		for (i = 0; i < myNArch; i++)
			mvArch[i] = 0.8 / (double)myNArch;
	}

	/*!
	 * \fn void cArch::ReAlloc(const uint theSize, const uint theNumParam)
	 * \param const uint theSize: new size of mvAr
	 * \param const uint theNumParam: not used here.
	 * \details new allocation of mvArch
	 */
	void cArch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		mvArch.ReAlloc(theSize) ;
	}

	/*!
	 * \fn void cArch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of Const or ARCH coefficients
	 * \param const uint theNumParam: =0, the constant part; =1 the ARCH coefficients
	 * \details new allocation of mvArch or mvConst
	 */
	void cArch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0: // mvConst
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cArch::ReAlloc - Size of theVectParam must be > 0") ;
			break ;
			case 1 : // mvArch
				mvArch = theVectParam ;
			break ;
			default :
	//			throw cError("cArch::ReAlloc - theNumParam must be in 0, 1") ;
			break ;
		}
	}

	/*!
	 * \fn void cArch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const double theValue: the value of the "theIndex" th lag. Default 0.
	 * \param const uint theIndex: the index.
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters
	 * \details mvArch[theIndex] = theValue or mvConst = theValue
	 */
	void cArch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				mvConst = theValue ;
			break ;
			case 1 :
				if (theIndex < mvArch.GetSize())
					mvArch[theIndex] = theValue ;
				else
					throw cError("cArch::Set - wrong index") ;
			break ;
			default:
				throw cError("cArch::Set - theNumParam must be in 0, 1") ;
			break ;
		}
	}

	double  cArch::Get(const uint theIndex, const uint theNumParam)
	{
		double res = 0.0;
		switch (theNumParam)
		{	case 0 :
				res = mvConst ;
			break ;
			case 1 :
				res = mvArch[theIndex] ;
			break ;
		}
		return res;
	}

	cDVector& cArch::Get(const uint theNumParam)
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
		}
	}

	/*!
	 * \fn void cArch::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the vector of values
	 * \param const uint theNumParam: =0, mvConst; =1, mvArch
	 * \details mvArch = theVectParam or mvConst = theVectParam[0]
	 */
	void cArch::Set(const cDVector& theVectParam, const uint theNumParam)
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
			default:
				throw cError("cArch::Set - theNumParam must be in 0, 1") ;
			break ;
		}
	}

	/*!
	 * \fn double cArch::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas
	 * \details theData is not updated here.
	*/
	double cArch::ComputeVar(uint theDate, const cRegArchValue& theDatas) const
	{
	uint myp = mvArch.GetSize() ;
	double myRes = mvConst ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] * theDatas.mUt[theDate-i] * theDatas.mUt[theDate-i] ;
		return myRes ;
	}

	uint cArch::GetNParam(void) const
	{
		return 1 + mvArch.GetSize() ;
	}

	uint cArch::GetNLags(void) const
	{
		return mvArch.GetSize() ;
	}

	void cArch::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize() ;
		theGradData.mCurrentGradVar = 0.0L ;
	uint myBegIndex = theGradData.GetNMeanParam() ;
		theGradData.mCurrentGradVar[myBegIndex] = 1.0 ;
	register uint i ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar[myBegIndex+i] = theValue.mUt[theDate - i]*theValue.mUt[theDate - i] ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar -= 2.0 * mvArch[i-1] * theValue.mUt[theDate - i] * theGradData.mGradMt[i-1] ;
	}

	void cArch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		theDestVect[theIndex] = mvConst ;
		mvArch.SetSubVectorWithThis(theDestVect, theIndex+1) ;
	}

	void cArch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvConst = theSrcVect[theIndex] ;
		mvArch.SetThisWithSubVector(theSrcVect, theIndex+1) ;
	}

	void cArch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize();
	uint myBegIndex = theGradData.GetNMeanParam();
		theHessData.mCurrentHessVar = 0.0;
	register uint i;
	cDMatrix myMat = theHessData.mCurrentHessVar;
		for (i = 1; i <= MIN(myp, theDate); i++)
			myMat.SetRow(myBegIndex + i, -2 * theData.mUt[theDate - i] * theGradData.mGradMt[i - 1]);
		theHessData.mCurrentHessVar += myMat + Transpose(myMat);

		for (i = 1; i <= MIN(myp, theDate); i++)
		{
			theHessData.mCurrentHessVar -= 2.0 * mvArch[i - 1] * theData.mUt[theDate - i] * theHessData.mHessMt[i - 1];
			theHessData.mCurrentHessVar += 2.0 * mvArch[i - 1] * theGradData.mGradMt[i - 1] * Transpose(theGradData.mGradMt[i - 1]);
		}
	}

	void cArch::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH[%d]", i + 1);

		}
	}

}//namespace
