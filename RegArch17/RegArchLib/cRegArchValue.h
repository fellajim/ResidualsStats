#pragma once 
#ifndef _CREGARCHVALUE_H_
#define _CREGARCHVALUE_H_
#include "RegArchDef.h"
/*!
 * \file cRegArchValue.h
 * \brief Definition of the cRegArchValue class.
 * \par Details.
 * 
 * This class is used for computing the conditional mean, variance and
 * residuals for a regression ARCH model.
 *
 * \author Jean-Baptiste DURAND, Ollivier TARAMASCO
 * \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
/*! 
 * \class cRegArchValue
 * \brief Class to store computational data
 */
class cRegArchValue
{
    public :
        cDVector	mYt	; ///< Vector of data.
	cDMatrix	mXt	; ///< Matrix of regressors if any.
	cDVector	mMt	; ///< Vector of conditional mean.
	cDVector	mHt	; ///< Vector of conditional variance.
	cDVector	mUt	; ///< Vector of residuals.
	cDVector	mEpst ; ///< Vector of standardized residuals.
    public :
	cRegArchValue(uint theSize, cDMatrix* theXt=NULL) ; ///< Creator
	cRegArchValue(cDVector* theYt=NULL, cDMatrix* theXt=NULL) ; ///< Creator
	virtual ~cRegArchValue() ; ///< Destructor
	void Delete(void) ;
	void ReAlloc(uint theSize=0) ; ///< Memory reallocation
	void ReAlloc(cDVector& theYt) ;///< Memory reallocation
	void ReAlloc(cDVector& theYt, cDMatrix& theXt) ;///< Memory reallocation
	void ReAllocXt(uint theNRow, uint theNCol) ;
    #ifndef _RDLL_	
        void PrintValue(ostream& theOut=cout, bool theHeader=true, char* theSep="\t") ;///< Print the datas
    #endif // _RDLL_
	void ComputeMeanAndVar(double& theMean, double& theVar) ;
	void ComputeVar(double& theVar);

};

#ifndef _RDLL_
    extern ostream& operator <<(ostream& theOut, cRegArchValue& theData) ;///< Print the datas
#endif //_RDLL_
    
}

#endif //_CREGARCHVALUE_H_
