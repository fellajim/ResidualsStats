#include "StdAfxRegArchLib.h"
/*!
 \file cRegArchModel.cpp
 \brief sources for class cRegArchModel methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {

	/*!
	 * \fn cRegArchModel::cRegArchModel()
	 * \param None
	 * \details A simple constructor
	 */
	cRegArchModel::cRegArchModel()
	{
		mMean = NULL ;
		mVar = NULL ;
		mResids = NULL ;
	}

	/*!
	 * \fn cRegArchModel::cRegArchModel(cCondMean* theMean, cAbstCondVar* theVar, cAbstResiduals* theResiduals)
	 * \param cCondMean* theMean: pointer to conditional mean model
	 * \param cAbstCondVar* theVar: pointer to conditional variance model
	 * \param cAbstResiduals* theResiduals: pointer to conditional residuals distribution model
	 * \details A simple constructor
	 */
	cRegArchModel::cRegArchModel(cCondMean& theMean, cAbstCondVar& theVar, cAbstResiduals& theResiduals)
	{	mMean = new cCondMean(theMean) ;			
		mVar = theVar.PtrCopy() ;
		mResids = theResiduals.PtrCopy() ;
		// CreateRealCondResiduals(theResiduals) ;
		
 		MESS_CREAT("cRegArchModel") ;
	}

	/*!
	 * \fn cRegArchModel::cRegArchModel(cRegArchModel& theModel)
	 * \param cRegArchModel& theModel: the source
	 * \details recopy constructor
	 */
		cRegArchModel::cRegArchModel(const cRegArchModel& theModel)
		{	if (theModel.mMean != NULL)
				mMean = new cCondMean(*theModel.mMean) ;
			else
				mMean = new cCondMean() ;
			mVar = theModel.mVar->PtrCopy() ;
			mResids = theModel.mResids->PtrCopy() ;
			// CreateRealCondResiduals(*theModel.mResids) ;

 			MESS_CREAT("cRegArchModel") ;
		}

	/*!
	 * \fn cRegArchModel::~cRegArchModel()
	 */
	cRegArchModel::~cRegArchModel()
	{	if (mMean != NULL)
		{	mMean->Delete() ;
			delete mMean ;
		}
		if (mVar != NULL)
		{	mVar->Delete() ;
			delete mVar ;
		}
		if (mResids != NULL)
		{	mResids->Delete() ;
			delete mResids ;
		}
		MESS_DESTR("cRegArchModel") ;
	}

	/*!
	 * \fn cRegArchModel& cRegArchModel::operator=(cRegArchModel& theRegArchModel)
	 * \param cRegArchModel& theRegArchModel: the source
	 * \details = operator for cRegArchModel
	 */
	cRegArchModel& cRegArchModel::operator=(cRegArchModel& theRegArchModel)
	{
		*mMean =  *(theRegArchModel.mMean) ;
		mVar = theRegArchModel.mVar->PtrCopy() ;
		mResids = theRegArchModel.mResids->PtrCopy() ;
		return *this ;
	}

	void cRegArchModel::ReAllocMean(uint theNewSize)
	{
		if (mMean != NULL)
		{
			mMean->Delete() ;
			delete mMean ;
		}
		mMean = new cCondMean(theNewSize) ;
	}

	/*!
	 * \fn void cRegArchModel::SetMean(cCondMean& theCondMean)
	 * \param cCondMean& theCondMean: the conditional mean model
	 */
	void cRegArchModel::SetMean(cCondMean& theCondMean)
	{
		if (mMean != NULL)
		{
			mMean->Delete() ;
			delete mMean ;
		}

		mMean = new cCondMean(theCondMean) ;
	}

	/*!
	 * \fn void cRegArchModel::GetNMean(void)
	 * \param void
	 * \details return the number of mean components
	 */
	int cRegArchModel::GetNMean(void)
	{
		if (mMean == NULL)
			return 0 ;
		else
			return mMean->GetNMean() ;
	}

	/*!
	 * \fn void cRegArchModel::AddOneMean(cAbstCondMean& theOneMean)
	 * \param cAbstCondMean& theOneMean: the conditional mean component model
	 * \details Add a new mean component
	 */
	void cRegArchModel::AddOneMean(cAbstCondMean& theOneMean)
	{
		if (mMean == NULL)
			mMean = new cCondMean(0);
		mMean->AddOneMean(theOneMean) ;
	}

	/*!
	 * \fn void cRegArchModel::GetOneMean(int theNumMean)
	 * \param int theNumMean: the index of the mean component
	 * \details Return theNumMean th mean component
	 */
	cAbstCondMean* cRegArchModel::GetOneMean(int theNumMean)
	{	if (mMean != NULL)
			return mMean->GetOneMean(theNumMean) ;
		else
			return NULL ;
	}

	bool cRegArchModel::IsGoodMeanType(eCondMeanEnum theMeanEnum, int theIndex)
	{
		if (mMean != NULL)
		{	cAbstCondMean* myCondMean = mMean->GetOneMean(theIndex) ;
			return (myCondMean->GetCondMeanType() == theMeanEnum) ;
		}
		else
			return false ;
	}

#ifdef _RDLL_
	void cRegArchModel::PrintMean(void)
	{
		if (mMean != NULL)
			mMean->Print();
	}

	void cRegArchModel::PrintVar(void)
	{
		if (mVar != NULL)
			mVar->Print();
	}

	void cRegArchModel::PrintResiduals(void)
	{
		if (mResids != NULL)
			mResids->Print();
	}

	void cRegArchModel::Print(void)
	{
		Rprintf("Regression with ARCH type residuals parameters:\n");
		Rprintf("-----------------------------------------------\n");
		PrintResiduals();
		Rprintf("\n");
		Rprintf("\n");
		Rprintf("\nConditional variance parameters:\n");
		Rprintf("--------------------------------\n");
		PrintVar();
		Rprintf("\n");
	}

#else

	/*!
	* \fn void cRegArchModel::PrintMean(ostream& theOut) const
	* \param ostream& theOut: output stream (screen or file). Default: cout
	*/
	void cRegArchModel::PrintMean(ostream& theOut) const
    {	if (mMean != NULL)
    		mMean->Print(theOut) ;
    }

	/*!
	* \fn void cRegArchModel::PrintVar(ostream& theOut) const
	* \param ostream& theOut: output stream (screen or file). Default: cout
	*/
	void cRegArchModel::PrintVar(ostream& theOut) const
	{
		if (mVar != NULL)
			mVar->Print(theOut);
	}
	/*!
	* \fn void cRegArchModel::PrintResiduals(ostream& theOut) const
	* \param ostream& theOut: output stream (screen or file). Default: cout
	*/
	void cRegArchModel::PrintResiduals(ostream& theOut) const
	{
		if (mResids != NULL)
			mResids->Print(theOut);
	}

	/*!
	* \fn void cRegArchModel::Print(ostream& theOut) const
	* \param ostream& theOut: output stream (screen or file). Default: cout
	*/
	void cRegArchModel::Print(ostream& theOut) const
	{
		theOut << "Regression with ARCH type residuals parameters:" << endl;
		theOut << "-----------------------------------------------" << endl;
		PrintResiduals(theOut);
		theOut << endl;
		PrintMean(theOut);
		theOut << endl;
		theOut << "Conditional variance parameters:" << endl;
		theOut << "--------------------------------" << endl;
		PrintVar(theOut);
		theOut << endl;

	}


#endif //_RDLL_
	/*!
	 * \fn void cRegArchModel::SetVar(cAbstCondVar& theCondVar)
	 * \param cCondVar& theCondVar: the conditional variance model
	 */
	void cRegArchModel::SetVar(cAbstCondVar& theCondVar)
	{
		if (mVar != NULL)
		{
			mVar->Delete() ;
			delete mVar ;
		}
//		mVar = theCondVar.PtrCopy() ;
		mVar = CreateRealCondVar(theCondVar);

	}

	cAbstCondVar* cRegArchModel::GetVar(void)
	{
		return mVar  ;
	}

	/*!
	 * \fn void cRegArchModel::SetResid(cAbstResiduals& theCondResiduals)
	 * \param cAbstResiduals& theCondResiduals: the conditional residuals model
	 */
	void cRegArchModel::SetResid(cAbstResiduals& theCondResiduals)
	{
		if (mResids != NULL)
		{
			mResids->Delete() ;
			delete mResids ;
		}
/*	eDistrTypeEnum myDistrType = theCondResiduals.GetDistrType() ;
	bool mySimulFlag = theCondResiduals.GetSimulFlag() ;
		mResids = CreateRealCondResiduals(myDistrType, &(theCondResiduals.mDistrParameter), mySimulFlag) ;
*/
		mResids = theCondResiduals.PtrCopy() ;
		// CreateRealCondResiduals(theCondResiduals) ;
	}

	cAbstResiduals*  cRegArchModel::GetResid(void)
	{
			return mResids ;
	}

	void cRegArchModel::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		if (mMean != NULL)
		{
			mMean->SetDefaultInitPoint(theValue);
			for (register uint t = 0; t < theValue.mYt.GetSize(); t++)
			{
				theValue.mMt[t] = mMean->ComputeMean(t, theValue);
				theValue.mUt[t] = theValue.mYt[t] - theValue.mMt[t];
			}
		}
		else
		{
			for (register uint t = 0; t < theValue.mYt.GetSize(); t++)
			{
				theValue.mUt[t] = theValue.mYt[t];
			}
		}
		mVar->SetDefaultInitPoint(theValue) ;
		mResids->SetDefaultInitPoint() ;
	}

	uint cRegArchModel::GetNParam(void) const
    {
    	if (mMean != NULL)
    		return mMean->GetNParam() + mVar->GetNParam() + mResids->GetNParam() ;
    	else
    		return mVar->GetNParam() + mResids->GetNParam() ;
    }

	uint cRegArchModel::GetNLags(void) const
    {
    	if (mMean == NULL)
    		return mVar->GetNLags() ;
    	else
    		return MAX(mMean->GetNLags(), mVar->GetNLags()) ;
    }

	void cRegArchModel::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData)
    {
    }

	void cRegArchModel::RegArchParamToVector(cDVector& theDestVect) const
    {
    uint myIndex = 0 ;
    	if (mMean != NULL)
    	{	mMean->RegArchParamToVector(theDestVect, myIndex) ;
    		myIndex += mMean->GetNParam() ;
    	}
    	mVar->RegArchParamToVector(theDestVect, myIndex) ;
     	myIndex += mVar->GetNParam() ;
    	mResids->RegArchParamToVector(theDestVect, myIndex) ;
    }

	void cRegArchModel::VectorToRegArchParam(const cDVector& theSrcParam)
    {
    uint myIndex = 0 ;
    	if (mMean != NULL)
    	{	mMean->VectorToRegArchParam(theSrcParam, myIndex) ;
    		myIndex += mMean->GetNParam() ;
    	}
    	mVar->VectorToRegArchParam(theSrcParam, myIndex) ;
    	myIndex += mVar->GetNParam() ;
    	mResids->VectorToRegArchParam(theSrcParam, myIndex) ;
    
    	if ((mVar->GetCondVarType() == eEgarch) && (mResids->GetNParam() > 0))
    	{	
    	cEgarch* myVar = dynamic_cast<cEgarch *>(mVar) ;
    		if (myVar != NULL)
    		{	
    		double myAux = mResids->ComputeEspAbsEps() ;
    			myVar->SetEspAbsEps(myAux) ;
    		}
    	}
    
    }

	void cRegArchModel::GetParamName(char**& theName)
	{
	uint myNParam = GetNParam();
		theName = new char*[myNParam];
		for (uint i = 0; i < myNParam; i++)
			theName[i] = new char[255];
		mMean->GetParamName(theName);
	uint myIndex = mMean->GetNParam();
		mVar->GetParamName(myIndex, theName);
		myIndex += mVar->GetNParam();
		mResids->GetParamName(myIndex, theName);
		
	}

}//namespace

