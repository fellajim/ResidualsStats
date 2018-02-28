#include "StdAfxRegArchLib.h"
/*!
 \file cStdDevInMean.cpp
 \brief sources for class cStdDevInMean methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	cStdDevInMean::cStdDevInMean(double theStdDevInMean):cAbstCondMean(eStdDevInMean)
	{
		mvStdDevInMean = theStdDevInMean ;
		MESS_CREAT("cStdDevInMean") ;
	}

	/*!
	 * \fn cStdDevInMean::cStdDevInMean(cAbstCondMean& theAbstCondMean)
	 * \param const cAbstCondMean& theAbstCondMean: the cStdDevInMean source.
	 */
	cStdDevInMean::cStdDevInMean(const cStdDevInMean& theStdDevInMean) :cAbstCondMean(eStdDevInMean)
	{
		*this = theStdDevInMean;
		MESS_CREAT("cStdDevInMean") ;
	}

	cStdDevInMean::~cStdDevInMean()
	{
		MESS_DESTR("cStdDevInMean") ;
	}

	/*!
	 * \fn cAbstCondMean cStdDevInMean::PtrCopy(void)
	 * \param void
	 */
	cAbstCondMean* cStdDevInMean::PtrCopy(void) const
	{
	cStdDevInMean* myStdDevInMean = new cStdDevInMean(*this) ;

		return myStdDevInMean ;
	}

	void cStdDevInMean::Delete(void)
	{
		MESS_DESTR("cStdDevInMean") ;
	}

#ifndef _RDLL_
	void cStdDevInMean::Print(ostream& theOut) const
	{
		theOut << "Standard Deviation In Mean model with:" << endl ;
		theOut << "\tDelta = " << mvStdDevInMean << endl ;
	}
#else
	void cStdDevInMean::Print(void)
	{
		Rprintf("Standard Deviation In Mean model with:%d\n") ;
		Rprintf("\tDelta = %f\n", mvStdDevInMean) ;
	}
#endif // _RDLL_


	void cStdDevInMean::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvStdDevInMean = 0.0 ;
	}

	void cStdDevInMean::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		mvStdDevInMean = 0.0;
	}

	void cStdDevInMean::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		mvStdDevInMean = theVectParam[0] ;
	}

	void cStdDevInMean::Set(const double theValue, const uint theIndex, const uint theNumParam) 
	{
		mvStdDevInMean = theValue ;
	}

	double cStdDevInMean::Get(const uint theIndex, const uint theNumParam)
	{
		return mvStdDevInMean ;
	}

	cDVector& cStdDevInMean::Get(const uint theNumParam)
	{
		ourAuxVector.ReAlloc(1, mvStdDevInMean);
		return ourAuxVector;
	}

	void cStdDevInMean::ReAlloc(const uint theSize, const uint theNumParam)
	{
	}

	void cStdDevInMean::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
	}

	cStdDevInMean& cStdDevInMean::operator =(const cStdDevInMean& theSrc)
	{
		mvStdDevInMean = theSrc.mvStdDevInMean ;
		return *this ;
	} 

	double cStdDevInMean::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
		return mvStdDevInMean * sqrt(theData.mHt[theDate]) ; 
	}

	uint cStdDevInMean::GetNParam(void) const
	{
		return 1 ;
	}
	
	uint cStdDevInMean::GetNLags(void) const
	{
		return 0 ;
	}

	void cStdDevInMean::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
		theDestVect[theIndex] = mvStdDevInMean ;
	}

	void  cStdDevInMean::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		mvStdDevInMean = theSrcVect[theIndex] ;
	}

	void cStdDevInMean::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
		theGradData.mCurrentGradMu[theBegIndex] += sqrt(theData.mHt[theDate]) ;
		theGradData.mCurrentGradMu += mvStdDevInMean * theGradData.mCurrentGradSigma ;
	}

	void cStdDevInMean::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
	uint myNParam = theGradData.GetNParam();
	cDMatrix myMat = cDMatrix(myNParam, myNParam);
		myMat.SetRow(theBegIndex, theGradData.mCurrentGradSigma);
		
		theHessData.mCurrentHessMu += myMat + Transpose(myMat) ;
		theHessData.mCurrentHessMu += mvStdDevInMean * theHessData.mCurrentHessSigma;
	}

	void cStdDevInMean::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "DELTA");
	}

}//namespace
