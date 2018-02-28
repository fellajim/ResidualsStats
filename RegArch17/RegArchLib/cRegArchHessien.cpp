#include "StdAfxRegArchLib.h"

namespace RegArchLib {
    /*!
     * \fn cRegArchHessien::cRegArchHessien(uint theNPast, uint theNMeanParam, uint theNVarParam, uint theNDistrParameter)
     * \param uint theNPast: maximum number of past lags used in Hessient computations
     * \param uint theNMeanParam: number of conditional mean parameters
     * \param uint theNVarParam:  number of conditional var parameters
     * \param uint theNDistrParameter:  number of conditional distribution parameters
     */
    cRegArchHessien::cRegArchHessien(uint theNPast, uint theNMeanParam, uint theNVarParam, uint theNDistrParameter)
    {
    	mvNPast = theNPast ;
    	mHessHt = new cDMatrix[theNPast] ;
    	mHessMt = new cDMatrix[theNPast] ;
    	mHessEpst = new cDMatrix[theNPast] ;
    	mvNParam = theNMeanParam + theNVarParam + theNDistrParameter ;
    	mvNMeanParam = theNMeanParam ;
    	mvNVarParam = theNVarParam ;
    	mvNDistrParameter = theNDistrParameter ;
    	for (register uint i = 0 ; i < theNPast ; i++)
    	{	mHessHt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessMt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessEpst[i].ReAlloc(mvNParam,mvNParam) ;
    	}
    	mCurrentHessVar.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessSigma.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessMu.ReAlloc(mvNParam,mvNParam) ;
		mCurrentHessDens.ReAlloc(mvNParam, mvNParam);
    	mCurrentHessEps.ReAlloc(mvNParam,mvNParam) ;
		mCurrentGradDiffLogDensity.ReAlloc(mvNParam);
    }
    
    cRegArchHessien::cRegArchHessien(cRegArchModel* theParam)
    {
    	mvNPast = mvNParam = 0 ;
    	if (theParam != NULL)
    	{	mvNPast = theParam->GetNLags() ;
    		if ( theParam->mMean != NULL)
    			mvNMeanParam = theParam->mMean->GetNParam() ;
    		else
    			mvNMeanParam = 0 ;
    		mvNVarParam = theParam->mVar->GetNParam() ;
    		mvNDistrParameter = theParam->mResids->GetNParam() ;
    		mvNParam = mvNMeanParam + mvNVarParam + mvNDistrParameter ;
    	}
    	mHessHt = new cDMatrix[mvNPast] ;
    	mHessMt = new cDMatrix[mvNPast] ;
    	mHessEpst = new cDMatrix[mvNPast] ;
    	for (register uint i = 0 ; i < mvNPast ; i++)
    	{	mHessHt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessMt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessEpst[i].ReAlloc(mvNParam,mvNParam) ;
    	}
    	mCurrentHessVar.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessSigma.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessMu.ReAlloc(mvNParam,mvNParam) ;
		mCurrentHessDens.ReAlloc(mvNParam, mvNParam);
    	mCurrentHessEps.ReAlloc(mvNParam,mvNParam) ;
		mCurrentGradDiffLogDensity.ReAlloc(mvNParam);
	}
    
    cRegArchHessien::~cRegArchHessien()
    {
    	for (register uint i = 0 ; i < mvNPast ; i++)
    	{	mHessHt[i].Delete() ;
    		mHessMt[i].Delete() ;
    		mHessEpst[i].Delete() ;
    	}
    	delete [] mHessHt ;
    	delete [] mHessMt ;
    	delete [] mHessEpst ;
    	mvNPast = 0 ;
    	mCurrentHessVar.Delete() ;
    	mCurrentHessSigma.Delete() ;
    	mCurrentHessMu.Delete() ;
    	mCurrentHessDens.Delete() ;
    	mCurrentHessEps.Delete() ;
		mCurrentGradDiffLogDensity.Delete();
    	mvNMeanParam =  mvNVarParam = mvNDistrParameter = mvNParam = 0 ;
    }
    
    void cRegArchHessien::Delete(void)
    {
    	for (register uint i = 0 ; i < mvNPast ; i++)
    	{	mHessHt[i].Delete() ;
    		mHessMt[i].Delete() ;
    		mHessEpst[i].Delete() ;
    	}
    	delete [] mHessHt ;
    	delete [] mHessMt ;
    	delete [] mHessEpst ;
    	mvNPast = 0 ;
    	mCurrentHessVar.Delete() ;
    	mCurrentHessSigma.Delete() ;
    	mCurrentHessMu.Delete() ;
    	mCurrentHessDens.Delete() ;
    	mCurrentHessEps.Delete() ;
		mCurrentGradDiffLogDensity.Delete();
		mvNMeanParam =  mvNVarParam = mvNDistrParameter = mvNParam = 0 ;
    }
    
    void cRegArchHessien::ReAlloc(uint theNPast, uint theNMeanParam, uint theNVarParam, uint theNDistrParameter) 
    {
    	Delete() ;
    	mvNPast = theNPast ;
    	mHessHt = new cDMatrix[theNPast] ;
    	mHessMt = new cDMatrix[theNPast] ;
    	mHessEpst = new cDMatrix[theNPast] ;
    	mvNParam = theNMeanParam + theNVarParam + theNDistrParameter ;
    	mvNMeanParam = theNMeanParam ;
    	mvNVarParam = theNVarParam ;
    	mvNDistrParameter = theNDistrParameter ;
    	for (register uint i = 0 ; i < theNPast ; i++)
    	{	mHessHt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessMt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessEpst[i].ReAlloc(mvNParam,mvNParam) ;
    	}
    	mCurrentHessVar.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessSigma.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessMu.ReAlloc(mvNParam,mvNParam) ;
		mCurrentHessDens.ReAlloc(mvNParam, mvNParam);
    	mCurrentHessEps.ReAlloc(mvNParam,mvNParam) ;
		mCurrentGradDiffLogDensity.ReAlloc(mvNParam);
    }
    
    void cRegArchHessien::ReAlloc(cRegArchModel* theParam)
    {
    	mvNPast = mvNParam = 0 ;
    	if (theParam != NULL)
    	{	mvNPast = theParam->GetNLags() ;
    		mvNMeanParam = theParam->mMean->GetNParam() ;
    		mvNVarParam = theParam->mVar->GetNParam() ;
    		mvNDistrParameter = theParam->mResids->GetNParam() ;
    		mvNParam = mvNMeanParam + mvNVarParam + mvNDistrParameter ;
    	}
    	mHessHt = new cDMatrix[mvNPast] ;
    	mHessMt = new cDMatrix[mvNPast] ;
    	mHessEpst = new cDMatrix[mvNPast] ;
    	for (register uint i = 0 ; i < mvNPast ; i++)
    	{	mHessHt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessMt[i].ReAlloc(mvNParam,mvNParam) ;
    		mHessEpst[i].ReAlloc(mvNParam,mvNParam) ;
    	}
    	mCurrentHessVar.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessSigma.ReAlloc(mvNParam,mvNParam) ;
    	mCurrentHessMu.ReAlloc(mvNParam,mvNParam) ;
		mCurrentHessDens.ReAlloc(mvNParam, mvNParam);
    	mCurrentHessEps.ReAlloc(mvNParam,mvNParam) ;
		mCurrentGradDiffLogDensity.ReAlloc(mvNParam);
	}
    
    uint cRegArchHessien::GetNPast(void)
    {
    	return mvNPast ;
    }
    
    uint cRegArchHessien::GetNParam(void)
    {
    	return mvNParam ;
    }
    
    uint cRegArchHessien::GetNMeanParam(void)
    {
    	return mvNMeanParam ;
    }
    
    uint cRegArchHessien::GetNVarParam(void)
    {
    	return mvNVarParam ;
    }
    
    uint cRegArchHessien::GetNDistrParameter(void)
    {
    	return mvNDistrParameter ;
    }
    
    void cRegArchHessien::Update(void)
    {
    	if (mvNPast > 0)
    	{	for (register int i = (int)mvNPast - 1 ; i > 0 ; i--)
    		{	mHessHt[i] = mHessHt[i-1] ;
    			mHessMt[i] = mHessMt[i-1] ;
    			mHessEpst[i] = mHessEpst[i-1] ;
    		}
    		mHessMt[0] = mCurrentHessMu  ;
    		mHessHt[0] = mCurrentHessVar ;
    		mHessEpst[0] = mCurrentHessEps;
    	}
    }

	void cRegArchHessien::ComputeHessSigmatFromHessVart(uint theDate, const cRegArchValue& theData, const cRegArchGradient& theGradData)
	{
	double mySigmat = sqrt(theData.mHt[theDate]);
		mCurrentHessSigma = mCurrentHessVar/(2*mySigmat) - theGradData.mCurrentGradVar * Transpose(theGradData.mCurrentGradVar) /(4.0*mySigmat*theData.mHt[theDate]) ;
	}
}//namespace
