/*!
 * \file cRegArchValue.cpp
 * \brief declaration of the cRegArchValue class methods.
 * \par Details.
 * 
 * This class is used for computing the conditional mean, variance and
 * residuals for a regression ARCH model.
 *
 * \author Jean-Baptiste DURAND, Ollivier TARAMASCO
 * \date dec-18-2006 - Last change feb-18-2011
*/

#include "StdAfxRegArchLib.h"
namespace RegArchLib {
    /*!
     * \fn cRegArchValue::cRegArchValue(uint theSampleSize, cDMatrix* theXt)
     * \param uint theSampleSize: size of the sample.
     * \param cDMatrix* theXt: the regressors matrix if any
    */
    cRegArchValue::cRegArchValue(uint theSampleSize, cDMatrix* theXt)
    {
        if (theSampleSize > 0)
        {	mYt.ReAlloc(theSampleSize) ;
            if ( theXt == NULL )
                mXt.Delete() ;
            else
            {   if ( theXt->GetMat() == NULL )
                    mXt.Delete() ;
                else
                    mXt = *theXt ;
            }
            mMt.ReAlloc(theSampleSize) ;
            mHt.ReAlloc(theSampleSize) ;
            mUt.ReAlloc(theSampleSize) ;
            mEpst.ReAlloc(theSampleSize) ;
        }
        else
        {   mYt.Delete();
            mXt.Delete() ;
            mMt.Delete() ;
            mHt.Delete() ;
            mUt.Delete() ;
            mEpst.Delete() ;
        }
        MESS_CREAT("cRegArchValue") ;
    }

    /*!
     * \fn cRegArchValue::cRegArchValue(uint theSampleSize, cDMatrix* theXt)
     * \param cDVector* theYt: vector of Y(t)
     * \param cDMatrix* theXt: the regressors matrix if any
    */
    cRegArchValue::cRegArchValue(cDVector* theYt, cDMatrix* theXt)
    {
        if (theYt == NULL)
        {	mYt.Delete() ;
            mXt.Delete() ;
            mMt.Delete() ;
            mHt.Delete() ;
            mUt.Delete() ;
            mEpst.Delete() ;
        }
        else
        {	mYt = *theYt ;
        uint mySampleSize = mYt.GetSize() ;
            mMt.ReAlloc(mySampleSize) ;
            mHt.ReAlloc(mySampleSize) ;
            mUt.ReAlloc(mySampleSize) ;
            mEpst.ReAlloc(mySampleSize) ;		
            if (theXt == NULL)
                mXt.Delete() ;
            else
            {   if ( theXt->GetMat() == NULL )
                    mXt.Delete() ;
                else
                    mXt = *theXt ;
            }
        }
        MESS_CREAT("cRegArchValue") ;
    }

    /*!
     * \fn cRegArchValue::~cRegArchValue()
    */
    cRegArchValue::~cRegArchValue()
    {   mYt.Delete();
        mXt.Delete() ;
        mMt.Delete() ;
        mHt.Delete() ;
        mUt.Delete() ;
        mEpst.Delete() ;
        MESS_DESTR("cRegArchValue") ;
    }

    void cRegArchValue::Delete(void)
    {
        this->~cRegArchValue() ;
    }
    /*!
     * \fn void cRegArchValue::ReAlloc(uint theSize)
     * \param uint theSize: new size of datas
    */
    void cRegArchValue::ReAlloc(uint theSize)
    {   if (theSize > 0)
        {   if (mYt.GetSize() != theSize)
                mYt.ReAlloc(theSize) ;
            if (mXt.GetNRow() != theSize)
            {   if (mXt.GetNCol() > 0)
                    mXt.ReAlloc(theSize, mXt.GetNCol()) ;
            }
            if (mMt.GetSize() != theSize)
                mMt.ReAlloc(theSize) ;
            if (mHt.GetSize() != theSize)
                mHt.ReAlloc(theSize) ;
            if (mUt.GetSize() != theSize)
                mUt.ReAlloc(theSize) ;
            if (mEpst.GetSize() != theSize)
                mEpst.ReAlloc(theSize) ;
        }
        else
        {	mYt.Delete() ;
            ClearMatrix(mXt) ;
            mMt.Delete() ;
            mHt.Delete() ;
            mUt.Delete() ;
            mEpst.Delete() ;
        }
    }

    /*!
     * \fn void cRegArchValue::ReAlloc(cDVector& theYt)
     * \param cDVector& theYt: vector of new datas
    */
    void cRegArchValue::ReAlloc(cDVector& theYt)
    {
    uint mySize = theYt.GetSize() ;
        ReAlloc(mySize) ;
        if (mySize > 0)
            mYt = theYt ;
    }

    /*!
     * \fn void cRegArchValue::ReAlloc(cDVector& theYt, cDMatrix& theXt)
     * \param cDVector& theYt: vector of new datas
     * \param cDMatrix& theXt: matrix of regressors 
    */
    void cRegArchValue::ReAlloc(cDVector& theYt, cDMatrix& theXt) // Xt T rows, p columns
    {
    uint mySize = theYt.GetSize() ;
        ReAlloc(mySize) ;
        if (mySize > 0) 
        {	mYt = theYt ;
            if (mySize == theXt.GetNRow())
                mXt = theXt;
        }
    }

    void cRegArchValue::ReAllocXt(uint theNRow, uint theNCol)
    {
        mXt.ReAlloc(theNRow, theNCol) ;
    }

    #ifndef _RDLL_
    /*!
     * \fn void cRegArchValue::PrintValue(ostream& theOut, bool theHeader, char* theSep)
     * \param ostream& theOut: output stream (screen or file). Default, cout.
     * \param bool theHeader: true if an header line is printed. Default, true
     * \param char* theSep: separator character. Default, "\t".
    */
    void cRegArchValue::PrintValue(ostream& theOut, bool theHeader, char* theSep)
    {
    uint mySize = mYt.GetSize() ;
    uint myNbCol = 0 ;
        if (theHeader)
        {   theOut << "Yt" << theSep ;
            if (mXt.GetNRow() > 0)
            {   myNbCol = mXt.GetNCol() ;
                for (register uint j = 0 ; j < myNbCol ; j++)
                    theOut << "Xt[ ," << j+1 << "]" << theSep ;
            }
            theOut << "Mt" << theSep << "Ht" << theSep << "Ut" << theSep << "Epst" << endl ;
        }			
        for (register int t = 0 ; t < mySize ; t++)
        {	theOut << mYt[t] << theSep ;
            for (register uint j = 0 ; j < myNbCol ; j++)
                theOut << mXt[t][j] << theSep ;
            theOut << mMt[t] << theSep << mHt[t] << theSep << mUt[t] << theSep << mEpst[t] << endl ;
        }
    }

    /*!
     * \fn ostream& operator <<(ostream& theOut, cRegArchValue& theData)
     * \param ostream& theOut: output stream (screen or file). Default, cout.
     * \param cRegArchValue& theData: datas to be printed.
     * \details Uses cRegArchValue::PrintValue with default values.
    */
    ostream& operator <<(ostream& theOut, cRegArchValue& theData)
    {   theData.PrintValue(theOut) ;
        return theOut ;
    }
    #endif //_RDLL_

    void cRegArchValue::ComputeMeanAndVar(double& theMean, double& theVar)
    {
        theMean = theVar = 0.0 ;
        for (register uint i = 0 ; i < mYt.GetSize() ; i++)
        {	theMean = ((double)i*theMean + mYt[i])/(double)(i+1) ;
            theVar = ((double)i*theVar + mYt[i]*mYt[i])/(double)(i+1) ;
        }
        theVar -= theMean * theMean ;
    }

	void cRegArchValue::ComputeVar(double& theVar)
	{
		theVar = 0.0;
		for (register uint i = 0; i < mYt.GetSize(); i++)
		{
			theVar = ((double)i*theVar + mUt[i] * mUt[i]) / (double)(i + 1);
		}
	}

} //Namespace
