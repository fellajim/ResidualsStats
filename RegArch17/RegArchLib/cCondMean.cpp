#include "StdAfxRegArchLib.h"
/*!
 \file cAbstCondVar.cpp
 \brief sources for abstract class cAbstCondVar methods.

 \author Jean-Baptiste DURAND, Ollivier TARAMASCO 
 \date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cCondMean::cCondMean(uint theNCondMean)
	 * \param uint theNCondMean: number of conditional means
	 * \details mvCondMean = theNCondMean
	 */
	cCondMean::cCondMean(uint theNCondMean)
	: mvCondMean(theNCondMean)
	{
		MESS_CREAT("cCondMean") ;
	}

	/*!
	 * \fn cCondMean::cCondMean(const cCondMean& theCondMean)
	 * \param cCondMean& theCondMean: conditional mean
	 * \details Recopy constructor
	 */
	cCondMean::cCondMean(const cCondMean& theCondMean)
	: mvCondMean(theCondMean.GetNMean())
	{
	uint myNCondMean = (uint)mvCondMean.size();
	vector<cAbstCondMean*> myCondMean = theCondMean.GetCondMean() ;
		for (register uint i = 0; i < myNCondMean; i++)
			mvCondMean.at(i) = myCondMean.at(i)->PtrCopy() ;
		MESS_CREAT("cCondMean") ;
	}

	/*!
	 * \fn cCondMean::~cCondMean()
	 * \param None
	 * \details simple destructor
	 */
	cCondMean::~cCondMean()
	{	Delete() ;
		MESS_DESTR("cCondMean") ;
	}

	/*!
	 * \fn void cCondMean::Delete(void)
	 * \param void
	 * \details free memory used par the cCondMean class
	 */
	void cCondMean::Delete(void)
	{
	uint myNCondMean = GetNMean();
		for (register uint i = 0 ; i < myNCondMean ; i++)
			if (mvCondMean[i] != NULL)
				mvCondMean[i]->Delete() ;
		mvCondMean.clear();
	}

	/*!
	 * \fn inline uint cCondMean::GetNMean(void) const
	 * \param void
	 * \brief return mvNCondMean
	 */
	uint cCondMean::GetNMean(void) const
	{
		return (uint)mvCondMean.size() ;
	}

	/*!
	 * \fn  void cCondMean::SetOneMean(uint theWhatMean, cAbstCondMean* theAbstCondMean)
	 * \param uint theWhatMean: index of the conditional mean component
	 * \param cAbstCondMean* theAbstCondMean: conditional mean component to be copied in the mCondMean array.
	 * \brief *mvCondMean[theWhatMean] = *theAbstCondMean
	 */
	void cCondMean::SetOneMean(uint theWhatMean, cAbstCondMean& theAbstCondMean)
	{
		if (theWhatMean >= GetNMean())
			throw cError("cCondMean::GetOneMean bad index") ;
		else
		{
			mvCondMean.at(theWhatMean) = theAbstCondMean.PtrCopy();

		}
	}

	/*!
	* \fn  void cCondMean::AddOneMean(cAbstCondMean* theAbstCondMean)
	* \param cAbstCondMean* theAbstCondMean: conditional mean component to be copied in the mCondMean array.
	* \brief *mvCondMean[theWhatMean] = *theAbstCondMean
	*/
	void cCondMean::AddOneMean(cAbstCondMean& theAbstCondMean)
	{
		mvCondMean.push_back(theAbstCondMean.PtrCopy());
	}

	/*!
	 * \fn cAbstCondMean** cCondMean::GetCondMean(void)
	 * \\details return mvCondMean
	 */
	vector<cAbstCondMean*> cCondMean::GetCondMean(void) const
	{	return mvCondMean ;		
	}

	/*!
	 * \fn cAbstCondMean* cCondMean::GetOneMean(uint theIndex) const
	 * \param uint theIndex: index of component to be returned
	 * \\details return mvCondMean[theWhateMean] 
	 */
	cAbstCondMean* cCondMean::GetOneMean(uint theIndex) const
	{
		if (theIndex < GetNMean())
			return mvCondMean.at(theIndex) ;
		else
			throw cError("cCondMean::GetOneMean bad index") ;
	}

	/*!
	 * \fn void cCondMean::GetCondMeanType(eCondMeanEnum* theCodeType) const
	 * \param eCondMeanEnum* theCodeType: array of all conditional mean component codes
	 * \details fill theCodeType array
	 */
	void cCondMean::GetCondMeanType(eCondMeanEnum* theCodeType) const
	{	for (register uint i = 0 ; i < GetNMean() ; i++)
			theCodeType[i] = mvCondMean[i]->GetCondMeanType()  ;
	}

	/*!
	 * \fn void cCondMean::Print(ostream& theOut) const
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 */
#ifndef _RDLL_
	void cCondMean::Print(ostream& theOut) const
	{	theOut << "Conditional mean parameters:" << endl ;
		theOut << "----------------------------" << endl ;
		for (register uint i = 0 ; i < GetNMean() ; i++)
			mvCondMean.at(i)->Print(theOut) ;
	}
#else
	void cCondMean::Print(void)
	{
		Rprintf("Conditional mean parameters:\n");
		Rprintf("----------------------------\n");
		for (register uint i = 0; i < GetNMean(); i++)
			mvCondMean.at(i)->Print();
	}
#endif // _RDLL_

	/*!
	 * \fn double cCondMean::Get(uint theNumMean, uint theIndex, uint theNumParam)
	 * \param uint theNumMean: index of conditional mean
	 * \param uint theNumParam: index of parameter
	 */
	double cCondMean::Get(uint theNumMean, uint theIndex, uint theNumParam)
	{
		if (theNumMean < GetNMean() )
			return mvCondMean.at(theNumMean)->Get(theIndex, theNumParam) ;
		else
			throw cError("cCondMean::Get bad index");
	}
	
	void cCondMean::SetDefaultInitPoint(double theMean, double theVar)
	{
		for (register uint i = 0; i < GetNMean(); i++)
			mvCondMean[i]->SetDefaultInitPoint(theMean, theVar) ;
	}

	void cCondMean::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		for (register uint i = 0; i < GetNMean(); i++)
			mvCondMean[i]->SetDefaultInitPoint(theValue);
	}

	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cCondMean& theCondMean)
	 * \param ostream& theOut: output (file or screen).
	 * \param const cCondMean& theCondMean: the conditional mean class to be printed.
	 */
#ifndef _RDLL_
	ostream& operator <<(ostream& theOut, const cCondMean& theCondMean)
	{	theOut << "Conditional mean parameters:" << endl ;
		theOut << "----------------------------" << endl ;
		for (register uint i = 0 ; i < theCondMean.GetNMean() ; i++)
		{	theCondMean.mvCondMean.at(i)->Print(theOut) ;
			theOut << endl ;
		}
		return theOut ;
	}
#endif //_RDLL_

	/*!
	 * \fn cCondMean& cCondMean::operator =(cCondMean& theSrc)
	 * \param cCondMean& theSrc: source class
	 */
	cCondMean& cCondMean::operator =(const cCondMean& theSrc)
	{	Delete() ;
	uint myNMean = theSrc.GetNMean();
		mvCondMean.resize(myNMean) ;
		for (register uint i = 0 ; i < myNMean ; i++)
			mvCondMean.at(i) = theSrc.GetOneMean(i)->PtrCopy() ;
		return *this ;
	}

	void cCondMean::UpdateProxyMeanParameters(void)
	{
	uint myNMean = GetNMean();
		for (uint i = 0; i < myNMean; i++)
			mvCondMean.at(i)->UpdateProxyMeanParameters();
	}

	/*!
	 * \fn double cCondMean::ComputeMean(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas.
	 * \details Compute the value of the conditional mean at date theDate. 
	 * theData is not updated here.
	 */
	double cCondMean::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
	double myMean = 0.0 ;
	uint myNMean = GetNMean() ;
		for (uint i = 0 ; i < myNMean ; i++)
			myMean += mvCondMean.at(i)->ComputeMean(theDate, theData) ;
		return myMean ;
	}

	uint cCondMean::GetNParam(void) const
	{
	uint myNParam = 0 ;
		for (register uint i = 0 ; i < GetNMean() ; i++)
			myNParam += mvCondMean[i]->GetNParam() ;
		return myNParam ;
	}

	uint cCondMean::GetNLags(void) const
	{
	uint myNLags = 0 ;
		for (register uint i = 0 ; i < GetNMean() ; i++)
			myNLags = MAX(myNLags, mvCondMean[i]->GetNLags());
		return myNLags ;
	}

	void cCondMean::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResids)
	{
	uint myIndex = 0 ;
		theGradData.mCurrentGradMu = 0.0L ;
		for (register uint i = 0 ; i < GetNMean() ; i++)
		{	mvCondMean[i]->ComputeGrad(theDate, theValue, theGradData, myIndex, theResids) ;
			myIndex += mvCondMean[i]->GetNParam() ;
		}
	}

	void cCondMean::RegArchParamToVector(cDVector& theDestVect, uint theIndex) const
	{
	uint myIndexCour = theIndex ;
		for (register uint i = 0; i < GetNMean(); i++)
		{	mvCondMean[i]->RegArchParamToVector(theDestVect, myIndexCour) ;
			myIndexCour += mvCondMean[i]->GetNParam() ;
		}
	}

	void cCondMean::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint myIndexCour = theIndex ;
		for (register uint i = 0; i < GetNMean(); i++)
		{	mvCondMean[i]->VectorToRegArchParam(theSrcVect, myIndexCour) ;
			myIndexCour += mvCondMean[i]->GetNParam() ;
		}
	}

	void cCondMean::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myIndex = 0;
		theHessData.mCurrentHessMu = 0.0;
		for (register uint i = 0; i < GetNMean(); i++)
		{
			mvCondMean[i]->ComputeHess(theDate, theData, theGradData, theHessData, myIndex, theResiduals);
			myIndex += mvCondMean[i]->GetNParam();
		}
	}

	void cCondMean::GetParamName(char** theName)
	{
	uint myIndex = 0;
		for (uint i = 0; i < GetNMean(); i++)
		{
			mvCondMean[i]->GetParamName(myIndex, theName);
			myIndex += mvCondMean[i]->GetNParam();

		}
	
	}



}//namespace
