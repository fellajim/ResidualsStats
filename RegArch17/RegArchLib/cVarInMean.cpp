#include "StdAfxRegArchLib.h"
/*!
 \file cVarInMean.cpp
 \brief sources for class cVarInMean methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	cVarInMean::cVarInMean(double theVarInMean):cAbstCondMean(eVarInMean)
	{
		mvVarInMean = theVarInMean ;
		MESS_CREAT("cVarInMean") ;
	}

	/*!
	 * \fn cVarInMean::cVarInMean(cAbstCondMean& theAbstCondMean)
	 * \param cVarInMean cAbstCondMean& theAbstCondMean: the cVarInMean source.
	 */
	cVarInMean::cVarInMean(const cVarInMean& theAbstCondMean) :cAbstCondMean(eVarInMean)
	{
		*this = theAbstCondMean ;
		MESS_CREAT("cVarInMean") ;
	}

	cVarInMean::~cVarInMean()
	{
		MESS_DESTR("cVarInMean") ;
	}


	/*!
	* \fn cAbstCondMean cVarInMean::PtrCopy(void)
	* \param void
	*/
	cAbstCondMean* cVarInMean::PtrCopy(void) const
	{
	cVarInMean* myVarInMean = new cVarInMean(*this);

		return myVarInMean;
	}


	void cVarInMean::Delete(void)
	{
		MESS_DESTR("cVarInMean") ;
	}


#ifndef _RDLL_	
	void cVarInMean::Print(ostream& theOut) const
	{
		theOut << "Variance In Mean model with:" << endl ;
			theOut << "\tDelta = " << mvVarInMean << endl ;
	}
#else		
		void cVarInMean::Print(void)
		{
			Rprintf("Variance In Mean model with:\n");
			Rprintf("\tDelta = %f\n", mvVarInMean);
		}
#endif //_RDLL_
	

	void cVarInMean::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvVarInMean = 0.0 ;
	}

	void cVarInMean::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		mvVarInMean = 0.0;
	}

	void cVarInMean::Set(const cDVector& theVectParam, const uint theNumParam) 
	{
		mvVarInMean = theVectParam[0] ;
	}

	void cVarInMean::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		mvVarInMean = theValue ;
	}

	double cVarInMean::Get(const uint theIndex, const uint theNumParam)
	{
		return mvVarInMean ;
	}

	cDVector& cVarInMean::Get(const uint theNumParam)
	{
		ourAuxVector.ReAlloc(1, mvVarInMean);
		return ourAuxVector;
	}

	void cVarInMean::ReAlloc(const uint theSize, const uint theNumParam)
	{
	}

	void cVarInMean::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
	}

	cVarInMean& cVarInMean::operator =(const cVarInMean& theSrc)
	{
		mvVarInMean = theSrc.mvVarInMean ;
		return *this ;
	} 

	double cVarInMean::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
		return mvVarInMean * theData.mHt[theDate] ; 
	}

	uint cVarInMean::GetNParam(void) const
	{
		return 1 ;
	}

	uint cVarInMean::GetNLags(void) const
	{
		return 0 ;
	}

	void cVarInMean::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
		theDestVect[theIndex] = mvVarInMean ;
	}

	void  cVarInMean::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		mvVarInMean = theSrcVect[theIndex] ;
	}

	void cVarInMean::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
		theGradData.mCurrentGradMu[theBegIndex] += theData.mHt[theDate] ;
		theGradData.mCurrentGradMu += mvVarInMean * theGradData.mCurrentGradVar ;
	}

	void cVarInMean::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) 
	{
	uint myNParam = theGradData.GetNParam();
	cDMatrix myMat = cDMatrix(myNParam, myNParam);
		myMat.SetRow(theBegIndex, theGradData.mCurrentGradVar);
		theHessData.mCurrentHessMu += myMat+Transpose(myMat) ;
		theHessData.mCurrentHessMu += mvVarInMean * theHessData.mCurrentHessVar;
	}

	void cVarInMean::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "DELTA");
	}


}//namespace
