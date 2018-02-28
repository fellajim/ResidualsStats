#include "StdAfxRegArchLib.h"
/*!
 \file cMa.cpp
 \brief sources for class cMa methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cMa::cMa(uint theNMa):cAbstCondMean(eMa)
	 * \param int theNMa: number of lags.
	 */

	cMa::cMa(uint theNMa):cAbstCondMean(eMa)
	{
		mvMa.ReAlloc(theNMa) ;
		MESS_CREAT("cMa")
	}

	/*!
	 * \fn cMa::cMa(const cDVector& theMa):cAbstCondMean(eMa)
	 * \param const cDVector& theAr: vector of AR coefficients.
	 */
	cMa::cMa(const cDVector& theMa):cAbstCondMean(eMa)
	{
		mvMa = theMa ;
		MESS_CREAT("cMa")
	}

	/*!
	 * \fn cMa::cMa(cAbstCondMean& theAbstCondMean)
	 * \param const cAbstCondMean& theAbstCondMean: the cMa source.
	 */
	cMa::cMa(const cMa& theMa):cAbstCondMean(eMa)
	{
		*this = theMa;
		MESS_CREAT("cMa")
	}

	/*!
	 * \fn cMa::~cMa()
	 */
	cMa::~cMa()
	{
		mvMa.Delete() ;
		MESS_DESTR("cMa")
	}

	/*!
	 * \fn cAbstCondMean cMa::PtrCopy(void)
	 * \param void
	 */
	cAbstCondMean* cMa::PtrCopy(void) const
	{
	cMa* myMa = new cMa(*this) ;

		return myMa ;
	}

	/*!
	 * \fn void cMa::Delete(void)
	 * \param void
	 */
	void cMa::Delete(void)
	{
		mvMa.Delete() ;
	}

	/*!
	 * \fn void cMa::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
#ifndef _RDLL_
	void cMa::Print(ostream& theOut) const
	{
	uint myNMa = mvMa.GetSize();
		theOut << "MA(" << myNMa << ") mean with:" << endl ;
		for (register uint i = 0 ; i <  myNMa ; i++)
			theOut << "\tMA[" << i+1 << "]=" << mvMa[i] << endl ;
	}
#else
	void cMa::Print(void)
	{
	uint myNMa = mvMa.GetSize();
		Rprintf("MA(%d) model with:", myNMa);
		for (register uint i = 0; i < myNMa; i++)
			Rprintf("\tMA[%d]=%f", i + 1, mvMa[i]);
	}
#endif //_RDLL_

	void cMa::SetDefaultInitPoint(double theMean, double theVar)
	{
	uint myNMa = mvMa.GetSize() ;
		for (register uint i = 0 ; i < myNMa ; i++)
			mvMa[i] = 0.0 ;
	}

	void cMa::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		uint myNMa = mvMa.GetSize();
		for (register uint i = 0; i < myNMa; i++)
			mvMa[i] = 0.0;
	}

	/*!
	 * \fn void cMa::Set(const double theValue, const uint theIndex=0, const uint theNumParam)
	 * \param const double theValue: the theIndex th value
	 * \param const uint theIndex: the index
	 * \param const uint theNumParam: not used here
	 * \details mvMa[theIndex] = theValue
	 */
	void cMa::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		if (theIndex >= mvMa.GetSize())
			throw cError("Bad index") ;
		else
			mvMa[theIndex]=theValue ;
	}

	/*!
	 * \fn void cMa::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of MA coefficients
	 * \param const uint theNumParam: not used here
	 * \details mvMa = theVectParam
	 */
	void cMa::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		mvMa=theVectParam ;
	}

	double cMa::Get(const uint theIndex, const uint theNumParam)
	{
		return mvMa[theIndex] ;
	}

	cDVector& cMa::Get(const uint theNumParam)
	{
		return mvMa;
	}

	/*!
	 * \fn void cMa::ReAlloc(const uint theSize, const uint theNumParam)
	 * \param const uint theSize: new size of mvMA
	 * \param const uint theNumParam; not used here.
	 * \details new allocation of mvMa 
	 */
	void cMa::ReAlloc(const uint theSize, const uint theNumParam)
	{
		mvMa.ReAlloc(theSize) ;
	}

	/*!
	 * \fn void cAr::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of AR coefficients
	 * \param const uint theNumParam: not used here.
	 * \details new allocation of mvAr
	 */
	void cMa::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		mvMa = theVectParam ;
	}

	/*!
	 * \fn cAbstCondMean& cAr::operator =(cAbstCondMean& theSrc)
	 * \param cAbstCondMean& theSrc: source to be recopied
	 * \details An error occurs if theSrc is not an cAr class parameter
	 */
	cMa& cMa::operator =(const cMa& theSrc)
	{
		mvMa = theSrc.mvMa ;
		return *this ;
	}

	/*!
	 * \fn cMa::ComputeMean(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of the computation
	 * \param cRegArchValue& theData: past datas.
	 * \details theData is not updated here.
	 */
	double cMa::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
	uint myq = mvMa.GetSize() ;

	double myRes = 0.0 ;
		for (register uint i = 1 ; i <= MIN(myq, theDate) ; i++)
			myRes += mvMa[i-1] * theData.mUt[theDate-i] ;
		return myRes ;
	}

	uint cMa::GetNParam(void) const
	{
		return mvMa.GetSize() ;
	}
	
	uint cMa::GetNLags(void) const
	{
		return mvMa.GetSize() ;
	}

	void cMa::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResids)
	{
	uint myq = mvMa.GetSize() ;
	register uint i ;
		for (i = 1 ; i <= MIN(myq, theDate) ; i++)
			theGradData.mCurrentGradMu[theBegIndex+i-1] += theValue.mUt[theDate - i] ;
		for (i = 0 ; i < MIN(myq, theDate) ; i++)
			theGradData.mCurrentGradMu -=  mvMa[i] * theGradData.mGradMt[i] ;
	}

	void cMa::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) 
	{
	uint myq = mvMa.GetSize();
	uint myNParam = theGradData.GetNParam();
	cDMatrix myMat1(myNParam, myNParam);
	register uint i ;
		for (i = 0; i < MIN(myq, theDate); i++)
		{	
			myMat1.SetColumn(theBegIndex + i, theGradData.mGradMt[i]);
		}
		theHessData.mCurrentHessMu -= myMat1 + Transpose(myMat1);
		for (i = 0; i < MIN(myq, theDate); i++)
			theHessData.mCurrentHessMu -= mvMa[i] * theHessData.mHessMt[i];
	}

	void cMa::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = mvMa.GetSize() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		mvMa.SetSubVectorWithThis(theDestVect, theIndex) ;
	}

	void cMa::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (mvMa.GetSize() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvMa.SetThisWithSubVector(theSrcVect, theIndex) ;
	}

	void cMa::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		for (uint i = 0; i < mvMa.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "MA[%d]", i + 1);

		}
	}

}//namespace
