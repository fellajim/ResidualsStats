#include "StdAfxRegArchLib.h"
/*!
	\file cAr.cpp
	\brief sources for class cAr methods.

	\author Jean-Baptiste Durand, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cAr::cAr(const uint theNAr):cAbstCondMean(eAr)
	 * \param const uint theNAr: number of AR lags.
	 */
	cAr::cAr(const uint theNAr):cAbstCondMean(eAr)
	{
		mvAr.ReAlloc(theNAr) ;
		MESS_CREAT("cAr")
	}

	/*!
	 * \fn cAr::cAr(const cDVector& theAr):cAbstCondMean(eAr)
	 * \param const cDVector& theAr: vector of AR coefficients.
	 */
	cAr::cAr(const cDVector& theAr):cAbstCondMean(eAr)
	{
		mvAr = theAr ;
		MESS_CREAT("cAr")
	}

	/*!
	 * \fn cAr::cAr(cAbstCondMean& theAbstCondMean)
	 * \param const ccAbstCondMean& theAbstCondMean: the cAr source.
	 */
	cAr::cAr(const cAr& theAr):cAbstCondMean(eAr)
	{
		*this = theAr;
		MESS_CREAT("cAr")
	}

	/*!
	 * \fn cAr::~cAr()
	 */
	cAr::~cAr()
	{
		mvAr.Delete() ;
		MESS_DESTR("cAr")
	}

	/*!
	 * \fn cAbstCondMean cAr::PtrCopy(void)
	 * \param void
	 */
	cAbstCondMean* cAr::PtrCopy(void) const
	{
	cAr* myAr = new cAr(*this) ;

		return myAr ;
	}

	/*!
	 * \fn void cAr::Delete(void)
	 * \param void
	 */
	void cAr::Delete(void)
	{
		mvAr.Delete() ;
	}

	/*!
	 * \fn void cAr::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
#ifndef _RDLL_
	void cAr::Print(ostream& theOut) const
	{
	uint myNAr = mvAr.GetSize() ;
		theOut << "AR(" << myNAr << ") model with:" << endl ;
		for (register uint i = 0 ; i < myNAr ; i++)
			theOut << "\tAR[" << i+1 << "]=" << mvAr[i] << endl ;
	}
#else
	void cAr::Print(void)
	{
	uint myNAr = mvAr.GetSize();
		Rprintf("AR(%d) model with\n", myNAr);
		for (register uint i = 0; i < myNAr; i++)
			Rprintf("\tAR[%d]=%f\n", i + 1, mvAr[i]);
	}
#endif // _RDLL_

	void cAr::SetDefaultInitPoint(double theMean, double theVar)
	{
	uint myNAr = mvAr.GetSize() ;
		for (register uint i = 0 ; i < myNAr ; i++)
			mvAr[i] = 0.0 ;
	}

	void cAr::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		uint myNAr = mvAr.GetSize();
		for (register uint i = 0; i < myNAr; i++)
			mvAr[i] = 0.0;
	}

	/*!
	 * \fn void cAr::Set(const double theValue, const uint theIndex, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const double theValue: the value of the "theIndex" th lag. Default 0.
	 * \param const uint theIndex: the index.
	 * \param const uint theNumParam: not used for AR model. Default 0.
	 * \details mvAr[theIndex] = theValue
	 */
	void cAr::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{	if (theIndex < mvAr.GetSize())
			mvAr[theIndex] = theValue ;
		else
			throw cError("wrong index") ;
	}

	/*!
	 * \fn void cAr::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the vector of AR param
	 * \param const uint theNumParam: not used for AR model. Default 0.
	 * \details mvAr = theVectParam
	 */
	void cAr::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		mvAr = theVectParam ;
	}

	/*!
	 * \fn void cAr::ReAlloc(const uint theSize, const uint theNumParam)
	 * \param const uint theSize: new size of mvAr
	 * \param const uint theNumParam; not used here.
	 * \details new allocation of mvAr
	 */

	double  cAr::Get(const uint theIndex, const uint theNumParam)
	{
		return mvAr[theIndex] ;
	}

	cDVector&  cAr::Get(const uint theNumParam)
	{
		return mvAr;
	}

	void cAr::ReAlloc(const uint theSize, const uint theNumParam)
	{
		mvAr.ReAlloc(theSize) ;
	}

	/*!
	 * \fn void cAr::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of AR coefficients
	 * \param const uint theNumParam: not used here.
	 * \details new allocation of mvAr
	 */
	void cAr::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		mvAr = theVectParam ;
	}

	/*!
	 * \fn cAbstCondMean& cAr::operator =(cAbstCondMean& theSrc)
	 * \param cAbstCondMean& theSrc: source to be recopied
	 * \details An error occurs if theSrc is not an cAr class parameter 
	 */
	cAr& cAr::operator =(const cAr& theSrc)
	{
		mvAr = theSrc.mvAr ;
		SetCondMeanType(eAr) ;
		return *this ;
	}

	/*!
	 * \fn double cAr::ComputeMean(uint theDate, const cRegArchValue& theData) const
	 * \brief Compute conditional mean component for an AR model
	 * \param int theDate: date of the computation
	 * \param const cRegArchValue& theData: past datas.
	 * \details theData is not updated here.
	 */
	double cAr::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
	uint myp = mvAr.GetSize() ;
	double myRes = 0.0 ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvAr[i-1] * theData.mYt[theDate-i] ;
		return myRes ;
	}

	uint cAr::GetNParam(void) const
	{
		return mvAr.GetSize() ;
	}

	uint cAr::GetNLags(void) const
	{
		return 0 ;
	}

	void cAr::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData,  uint theBegIndex, cAbstResiduals* theResids)
	{
		for (register uint i = 1 ; i <= MIN(mvAr.GetSize(), theDate) ; i++)
			theGradData.mCurrentGradMu[theBegIndex+i-1] += theValue.mYt[theDate - i] ;
	}

	void cAr::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) 
	{
	// Hessien en AR = 0
	}

	void cAr::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = mvAr.GetSize() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		mvAr.SetSubVectorWithThis(theDestVect, theIndex) ;
	}

	void cAr::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (mvAr.GetSize() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvAr.SetThisWithSubVector(theSrcVect, theIndex) ;
	}

	void cAr::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		for (uint i = 0; i < mvAr.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "AR[%d]", i + 1);

		}
	}

}//namespace
