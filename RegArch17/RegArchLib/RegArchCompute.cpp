#include "StdAfxRegArchLib.h"
/*!
	\file RegArchCompute.cpp
	\brief implementation of the Simulation / Estimation procedures for general RegArchModel
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - last change feb-18-2011
*/


namespace RegArchLib {

	/*!
	 * \fn void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cRegArchValue& theData)
	 * \param const uint theNSample: size of the sample
	 * \param const cRegArchModel& theModel: the RegArch model
	 * \param cRegArchValue& theData: output parameter. The Y(t) values are stored in theData.mYt
	 */
	void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cRegArchValue& theData)
	{
	theData.ReAlloc(theNSample) ;
	theModel.mResids->Generate(theNSample, theData.mEpst) ;
	theModel.mVar->UpdateProxyVarParameters();
	if (theModel.mMean != NULL)
	theModel.mMean->UpdateProxyMeanParameters();
	for (register uint t = 0 ; t < theNSample ; t++)
	{	theData.mHt[t] = theModel.mVar->ComputeVar(t, theData) ;
	if (theModel.mMean != NULL)
	{	theData.mMt[t] = theModel.mMean->ComputeMean(t, theData) ;
	}
	theData.mUt[t] = sqrt(theData.mHt[t])*theData.mEpst[t] ;
	theData.mYt[t] = theData.mMt[t] + theData.mUt[t] ;
	}
	}

   /*!
	 * \fn void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cDVector& theYt)
	 * \param const uint theNSample: size of the sample
	 * \param const cRegArchModel& theModel: the RegArch model
	 * \param cDVector& theYt: output parameter. 
	 */
	void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cDVector& theYt)
	{
	cRegArchValue myValue(theNSample) ;
	RegArchSimul(theNSample, theModel, myValue) ;
	theYt = myValue.mYt ;

	}

   /*!
	* \fn static double ComputeLt(uint theDate, const cRegArchModel& theParam, cRegArchValue& theValue)
	* \param uint theDate: the date
	* \param const cRegArchModel& theParam: the model
	* \param cRegArchValue& theValue: output parameter.
	* \brief return the lt(theta ; Yt)
	*/
	static double ComputeLt(uint theDate, const cRegArchModel& theParam, cRegArchValue& theValue)
	{
	double myRes = 0;
	theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue);
	if (theParam.mMean != NULL)
	theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue);
	theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate];
	theValue.mEpst[theDate] = theValue.mUt[theDate] / sqrt(theValue.mHt[theDate]);
	myRes += -0.5*log(theValue.mHt[theDate]) + theParam.mResids->LogDensity(theValue.mEpst[theDate]);
	return myRes;
	}

	/*!
	 * \fn double RegArchLLH(const cRegArchModel& theParam, cDVector* theYt, cDMatrix* theXt)
	 * \param const cRegArchModel& theParam: the model
	 * \param cDVector* theYt: the observations
	 * \param cDMatrix* theXt: the regressors matrix. Default NULL
	 * \brief return the log-likelihood value
	 */
	double RegArchLLH(const cRegArchModel& theParam, cDVector* theYt, cDMatrix* theXt)
	{
	cRegArchValue myValue(theYt, theXt) ;
	return RegArchLLH(theParam, myValue) ;
	}

	/*!
	 * \fn double RegArchLLH(const cRegArchModel& theParam,cRegArchValue& theData)
	 * \param const cRegArchModel& theParam: the model
	 * \param cRegArchValue& theData: theData.mYt contains the observations.
	 * \brief return the log-likelihood value
	 */
	double RegArchLLH(const cRegArchModel& theParam, cRegArchValue& theData)
	{
	int mySize = (int)theData.mYt.GetSize() ;
	double myRes = 0 ;
	theData.mEpst = theData.mHt = theData.mMt = theData.mUt = 0.0 ;
	theParam.mVar->UpdateProxyVarParameters();
	if (theParam.mMean != NULL)
	theParam.mMean->UpdateProxyMeanParameters();
	for(register int t=0 ; t < mySize ; t++)
	{	theData.mHt[t] = theParam.mVar->ComputeVar(t, theData) ;
	if (theParam.mMean != NULL)
	theData.mMt[t] = theParam.mMean->ComputeMean(t, theData) ;
	theData.mUt[t] = theData.mYt[t] - theData.mMt[t] ;
	theData.mEpst[t] = theData.mUt[t]/sqrt(theData.mHt[t]) ;
	myRes += -0.5*log(theData.mHt[t]) + theParam.mResids->LogDensity(theData.mEpst[t]) ;
	}
	return myRes ;
	}

	void RegArchGradLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cDVector& theGradlt)
	{	theGradlt = 0.0 ;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue) ;
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue) ;
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate] ;
	double mySigmat = sqrt(theValue.mHt[theDate]) ;
		theValue.mEpst[theDate] = theValue.mUt[theDate]/mySigmat ;
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat);
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData) ;
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu)/ mySigmat ;
		theGradlt =  (-1.0/mySigmat) * theGradData.mCurrentGradSigma  + theGradData.mCurrentDiffLogDensity * theGradData.mCurrentGradEps  + theGradData.mCurrentGradDens;
	}
	
	void NumericRegArchGradLt(uint theDate, cRegArchModel& theParam, cRegArchValue* theValue, cDVector& theGradlt, double theh)
	{
	uint myNParam = theParam.GetNParam();
	cDVector myParam(myNParam);
	cDVector myParam1(myNParam);
	theParam.RegArchParamToVector(myParam);
	cRegArchModel myModel = cRegArchModel(theParam) ;
	theGradlt.ReAlloc(myNParam);
	double mylt0 = ComputeLt(theDate, theParam, theValue[myNParam]);
	myModel.VectorToRegArchParam(myParam);
	myParam1 = myParam;
	for (register uint i = 0; i < myNParam; i++)
	{
	double myh1 = abs(myParam[i] * theh);
	myParam1[i] += myh1;
	myModel.VectorToRegArchParam(myParam1);
	double mylt1 = ComputeLt(theDate, myModel, theValue[i]);
	theGradlt[i] = (mylt1 - mylt0) / myh1;
	myParam1[i] -= myh1;
	}
	}

	void RegArchLtAndGradLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, double& theLt, cDVector& theGradlt)
	{
		theGradlt = 0.0 ;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue) ;
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue) ;
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate] ;
	double mySigmat = sqrt(theValue.mHt[theDate]) ;
		theValue.mEpst[theDate] = theValue.mUt[theDate]/mySigmat ;
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat);
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData) ;
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu)/ mySigmat ;
		theGradlt = (-1.0 / mySigmat) * theGradData.mCurrentGradSigma + theGradData.mCurrentDiffLogDensity * theGradData.mCurrentGradEps + theGradData.mCurrentGradDens ;
		theLt = -0.5*log(theValue.mHt[theDate]) + theParam.mResids->LogDensity(theValue.mEpst[theDate]) ; 
	}

	void RegArchGradLLH(cRegArchModel& theParam, cRegArchValue& theData, cDVector& theGradLLH)
	{
	cRegArchGradient myGradData=cRegArchGradient(&theParam) ;
	cDVector myGradlt(myGradData.GetNParam()) ;
	theGradLLH = 0.0L ;
	theParam.mVar->UpdateProxyVarParameters();
	if (theParam.mMean != NULL)
	theParam.mMean->UpdateProxyMeanParameters();
	for (register int t = 0 ; t < (int)theData.mYt.GetSize() ; t++)
	{	RegArchGradLt(t, theParam, theData, myGradData, myGradlt) ;
	theGradLLH += myGradlt ;
	myGradData.Update();
	}
	}

	void RegArchLLHAndGradLLH(cRegArchModel& theParam, cRegArchValue& theValue, double& theLLH, cDVector& theGradLLH)
	{
	cRegArchGradient myGradData(&theParam) ;
	cDVector myGradlt(myGradData.GetNParam()) ;
	double myLt ;
		theGradLLH = 0.0L ;
		theLLH = 0.0 ;
		theParam.mVar->UpdateProxyVarParameters();
		if (theParam.mMean != NULL)
			theParam.mMean->UpdateProxyMeanParameters();
		theParam.mVar->UpdateProxyVarParameters();
		for (register int t = 0 ; t < (int)theValue.mYt.GetSize() ; t++)
		{	RegArchLtAndGradLt(t, theParam, theValue, myGradData, myLt, myGradlt) ;
			theGradLLH += myGradlt ;
			theLLH += myLt ;
			myGradData.Update();
		}
	}

	void RegArchHessLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cDMatrix& theHesslt)
	{	
	cDVector myGradlt(theGradData.GetNParam()) ;
		theHesslt = 0.0 ;
		RegArchGradLt(theDate, theParam, theValue, theGradData,  myGradlt) ;
		theParam.mVar->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids) ;
		theHessData.ComputeHessSigmatFromHessVart(theDate, theValue, theGradData);
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids) ;
		theParam.mResids->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids) ;

	double myHt = theValue.mHt[theDate];
	double mySigmat = sqrt(myHt);
	/* Hessien Eps(t, theta) */
		theHessData.mCurrentHessEps = theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradMu) / myHt;
		theHessData.mCurrentHessEps += theGradData.mCurrentGradMu * Transpose(theGradData.mCurrentGradSigma) / myHt;
		theHessData.mCurrentHessEps -= theHessData.mCurrentHessMu / mySigmat;
		theHessData.mCurrentHessEps += 2 * theValue.mUt[theDate] / (myHt * mySigmat) * theGradData.mCurrentGradSigma*Transpose(theGradData.mCurrentGradSigma);
		theHessData.mCurrentHessEps -= theValue.mUt[theDate] / myHt *theHessData.mCurrentHessSigma ;
	/* Hessien lt*/
		theHesslt = theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradSigma) / myHt;
		theHesslt -= theHessData.mCurrentHessSigma / mySigmat;
	double myDiff2LogDensity = theParam.mResids->Diff2LogDensity(theValue.mEpst[theDate]);
		theHesslt += theGradData.mCurrentGradEps * Transpose(theGradData.mCurrentGradEps)*myDiff2LogDensity;
		theHesslt += theGradData.mCurrentDiffLogDensity * theHessData.mCurrentHessEps;
		theHesslt += theHessData.mCurrentGradDiffLogDensity * Transpose(theGradData.mCurrentGradEps);
		theHesslt += theGradData.mCurrentGradEps * Transpose(theHessData.mCurrentGradDiffLogDensity);
		theHesslt += theHessData.mCurrentHessDens;
	}

	void NumericRegArchHessLt(int theDate, cRegArchModel& theParam, cRegArchValue* theValue, cRegArchGradient* theGradData, cDMatrix& theHesslt, double theh)
	{
	uint myNParam = theParam.GetNParam();
	cDVector myGradLt0 = cDVector(myNParam);
	cDVector myGradLt1 = cDVector(myNParam);
	cDVector myParam(myNParam);
	theParam.RegArchParamToVector(myParam);
	RegArchGradLt(theDate, theParam, theValue[myNParam], theGradData[myNParam], myGradLt0);
	theGradData[myNParam].Update();
	for (register uint i = 0; i < myNParam; i++)
	{
	double myh = abs(myParam[i] * theh);
	myParam[i] += myh;
	theParam.VectorToRegArchParam(myParam);
	RegArchGradLt(theDate, theParam, theValue[i], theGradData[i], myGradLt1);
	theGradData[i].Update();
	for (register uint j = i; j < myNParam; j++)
	theHesslt[i][j] = theHesslt[j][i] = (myGradLt1[j] - myGradLt0[j]) / myh;
	myParam[i] -= myh;
	}
	theParam.VectorToRegArchParam(myParam);
	}

	void RegArchGradAndHessLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cDVector& theGradlt, cDMatrix& theHesslt)
	{	
	double mylt = 0.0 ;
	theGradlt.ReAlloc(theParam.GetNParam()) ;
	theHesslt = 0.0 ;
	RegArchLtAndGradLt(theDate, theParam, theValue, theGradData, mylt, theGradlt) ;
	theParam.mVar->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids) ;
	theHessData.ComputeHessSigmatFromHessVart(theDate, theValue, theGradData);
	if (theParam.mMean != NULL)
	theParam.mMean->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids) ;
	theParam.mResids->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids) ;
	double mySigmat = sqrt(theValue.mHt[theDate]) ;
	/* Hessien Eps(t, theta) */
	theHessData.mCurrentHessEps = theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradMu) / theValue.mHt[theDate];
	theHessData.mCurrentHessEps += theGradData.mCurrentGradMu * Transpose(theGradData.mCurrentGradSigma) / theValue.mHt[theDate];
	theHessData.mCurrentHessEps -= theHessData.mCurrentHessMu / mySigmat;
	theHessData.mCurrentHessEps += 2 * theValue.mUt[theDate] / pow(mySigmat, 3)*theGradData.mCurrentGradSigma*Transpose(theGradData.mCurrentGradSigma);
	theHessData.mCurrentHessEps -= theValue.mUt[theDate] / theValue.mHt[theDate] * theHessData.mCurrentHessSigma;
	/* Hessien lt*/
	theHesslt = theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradSigma) / theValue.mHt[theDate];
	theHesslt -= theHessData.mCurrentHessSigma / mySigmat;
	double myDiff2LogDensity = theParam.mResids->Diff2LogDensity(theValue.mEpst[theDate]);
	theHesslt += theGradData.mCurrentGradEps * Transpose(theGradData.mCurrentGradEps)*myDiff2LogDensity;
	theHesslt += theGradData.mCurrentDiffLogDensity * theHessData.mCurrentHessEps;
	theHesslt += theHessData.mCurrentGradDiffLogDensity * Transpose(theGradData.mCurrentGradEps);
	theHesslt += theGradData.mCurrentGradEps * Transpose(theHessData.mCurrentGradDiffLogDensity);
	theHesslt += theHessData.mCurrentHessDens;
	}

	void RegArchLtGradAndHessLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, double& thelt, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cDVector& theGradlt, cDMatrix& theHesslt)
	{
	uint myNParam = theParam.GetNParam();
	theGradlt.ReAlloc(myNParam);
	theHesslt .ReAlloc(myNParam, myNParam);
	RegArchLtAndGradLt(theDate, theParam, theValue, theGradData, thelt, theGradlt);
	theParam.mVar->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids);
	if (theParam.mMean != NULL)
	theParam.mMean->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids);
	theParam.mResids->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids);
	theHessData.ComputeHessSigmatFromHessVart(theDate, theValue, theGradData);
	double mySigmat = sqrt(theValue.mHt[theDate]);
	/* Hessien Eps(t, theta) */
	theHessData.mCurrentHessEps = theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradMu) / theValue.mHt[theDate];
	theHessData.mCurrentHessEps += theGradData.mCurrentGradMu * Transpose(theGradData.mCurrentGradSigma) / theValue.mHt[theDate];
	theHessData.mCurrentHessEps -= theHessData.mCurrentHessMu / mySigmat;
	theHessData.mCurrentHessEps += 2 * theValue.mUt[theDate] / pow(mySigmat, 3)*theGradData.mCurrentGradSigma*Transpose(theGradData.mCurrentGradSigma);
	theHessData.mCurrentHessEps += theValue.mUt[theDate] / theValue.mHt[theDate] * theHessData.mCurrentHessSigma;
	/* Hessien lt*/
	theHesslt = theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradSigma) / theValue.mHt[theDate];
	theHesslt -= theHessData.mCurrentHessSigma / mySigmat;
	double myDiff2LogDensity = theParam.mResids->Diff2LogDensity(theValue.mEpst[theDate]);
	theHesslt += theGradData.mCurrentGradEps * Transpose(theGradData.mCurrentGradEps)*myDiff2LogDensity;
	theHesslt += theGradData.mCurrentDiffLogDensity * theHessData.mCurrentHessEps;
	theHesslt += theHessData.mCurrentGradDiffLogDensity * Transpose(theGradData.mCurrentGradEps);
	theHesslt += theGradData.mCurrentGradEps * Transpose(theHessData.mCurrentGradDiffLogDensity);
	theHesslt += theHessData.mCurrentHessDens;
	}

	void RegArchHessLLH(cRegArchModel& theParam, cRegArchValue& theValue, cDMatrix& theHessLLH)
	{
	cRegArchGradient myGradData = cRegArchGradient(&theParam);
	cRegArchHessien myHessData = cRegArchHessien(&theParam);
	uint myNParam = myHessData.GetNParam();
	cDMatrix myHesslt(myNParam, myNParam);
		theHessLLH.ReAlloc(myNParam, myNParam);
		theParam.mVar->UpdateProxyVarParameters();
		if (theParam.mMean != NULL)
			theParam.mMean->UpdateProxyMeanParameters();
		for (register int t = 0; t < (int)theValue.mYt.GetSize(); t++)
		{
			RegArchHessLt(t, theParam, theValue, myGradData, myHessData, myHesslt) ;
			theHessLLH += myHesslt;
			myGradData.Update();
			myHessData.Update();
		}
	}

	void NumericComputeJ(cRegArchModel& theModel, cRegArchValue& theValue, cDMatrix& theJ, double theh=1e-5)
	{
	//on initialise l'ensemble des variables utiles
	uint myNParam = theModel.GetNParam();
	cDVector myGradij(myNParam), myVect(myNParam), myVect0(myNParam);
		theJ.ReAlloc(myNParam, myNParam);
	uint myT = theValue.mYt.GetSize();
	cRegArchGradient myGradData(&theModel);

	cDVector* myGrad0 = new cDVector[myT];

		for (uint t = 0; t < myT; t++)
		{
			myGrad0[t].ReAlloc(myNParam);
			RegArchGradLt(t, theModel, theValue, myGradData, myGrad0[t]);
		}

		//on met � 0 la hessienne
		theJ = 0.0L;

		theModel.RegArchParamToVector(myVect0);

		myVect = myVect0;
	double myhhi;
		for (register uint i = 0; i < myNParam; i++)
		{
		//on fait la somme sur T
			myhhi = fabs(theh*myVect0[i]);
			if (myhhi < 1e-16)
				myhhi = theh;
			myVect[i] += myhhi;
			theModel.VectorToRegArchParam(myVect);
			if (theModel.mMean != NULL)
				theModel.mMean->UpdateProxyMeanParameters();
			theModel.mVar->UpdateProxyVarParameters();
			myGradData.ReInitialize();
			for (register uint t = 0; t < myT; t++)
			{
				RegArchGradLt(t, theModel, theValue, myGradData, myGradij);
				for (register uint j = i; j < myNParam; j++)
				{
					//on somme dans theHessLLH
					theJ[i][j] += (myGradij[j] - myGrad0[t][j]) / myhhi;
				}
			}
			myVect[i] -= myhhi;
		}
	//on divise par T
		for (register uint i = 0; i < myNParam; i++)
			for (register uint j = i + 1; j < myNParam; j++)
				theJ[j][i] = theJ[i][j];
		theJ /= (double)myT;
		// On d�salloue
		for (register uint t = 0; t < myT; t++)
			myGrad0[t].Delete();
		delete[] myGrad0;
	}

	void NumericComputeCov(cRegArchModel &theModel, cRegArchValue &theData, cDMatrix &theCov)
	{
	uint myNParam = theModel.GetNParam();
	theCov.ReAlloc(myNParam,myNParam);
	cDMatrix myI(myNParam, myNParam);  
	cDMatrix myJ(myNParam, myNParam);
	RegArchComputeI(theModel,theData,myI);
//	cout<< " I : " << endl;
	myI.Print();
//	cout << "J : " << endl;
	NumericComputeJ(theModel, theData, myJ);
	myJ.Print();

	theCov =  Inv(myJ) * myI * Inv(myJ);
	theCov = theCov/(int)theData.mYt.GetSize();
	}

	void RegArchComputeI(cRegArchModel &theModel,cRegArchValue &theData, cDMatrix &theI)
	{
	uint myNParam = theModel.GetNParam();

		theI.ReAlloc(myNParam, myNParam);
	cRegArchGradient myGradData(&theModel);
	cDVector myGradlt(myNParam);
	cRegArchGradient myGradlt2(myNParam);
	cDMatrix myGradltTranspose(1,myNParam);
	cDMatrix myGradI(myNParam,myNParam);
		theModel.mVar->UpdateProxyVarParameters();
		for (register int t = 0 ; t < (int)theData.mYt.GetSize() ; t++)
		{
			//Calcul du gradient de lt pour th�ta chapeau
			RegArchGradLt(t,theModel,theData,myGradData, myGradlt);
			myGradltTranspose = Transpose(myGradlt);
			myGradI += (myGradlt * myGradltTranspose);
		}
		theI = myGradI / theData.mYt.GetSize();
	}

	void RegArchComputeIAndJ(cRegArchModel &theModel,cRegArchValue &theData, cDMatrix &theI, cDMatrix &theJ)
	{
	uint myNParam = theModel.GetNParam();

	theI.ReAlloc(myNParam, myNParam);
	theJ.ReAlloc(myNParam, myNParam);
	cRegArchGradient myGradData(&theModel);
	cRegArchHessien myHessData(&theModel) ;

	cDVector myGradlt(myNParam);
	cDMatrix myHesslt(myNParam, myNParam) ;

	cRegArchGradient myGradlt2(myNParam);
	
	cDMatrix myGradltTranspose(1,myNParam);
	cDMatrix myGradI(myNParam,myNParam);
	theModel.mVar->UpdateProxyVarParameters();
	for (register int t = 0 ; t < (int)theData.mYt.GetSize() ; t++)
	{
	//Calcul du gradient et du hessien de lt pour th�ta chapeau
	RegArchGradAndHessLt(t, theModel, theData, myGradData, myHessData, myGradlt, myHesslt) ;

	myGradltTranspose = Transpose(myGradlt);
	myGradI += (myGradlt * myGradltTranspose);
	theJ -= myHesslt ;
	}
	theI = myGradI / (double)theData.mYt.GetSize();
	theJ /= (double)theData.mYt.GetSize() ;
	}

	void RegArchComputeCov(cRegArchModel& theModel, cRegArchValue& theValue, cDMatrix& theCov)
	{
	uint myNParam = theModel.GetNParam() ;
	cDMatrix myI(myNParam, myNParam) ;
	cDMatrix myJ(myNParam, myNParam) ;
	cDMatrix myInvJ(myNParam, myNParam) ;

//		RegArchComputeIAndJ(theModel, theValue, myI, myJ) ;
		RegArchComputeI(theModel, theValue, myI);
		NumericComputeJ(theModel, theValue, myJ);
		myInvJ = Inv(myJ) ;
		theCov = myInvJ * myI * myInvJ  / theValue.mYt.GetSize();
	}

	void NumericRegArchGradLLH(cRegArchModel& theModel, cRegArchValue& theValue, cDVector& theGradLLH, double theh)
	{
	double myLLH0 = RegArchLLH(theModel, theValue) ;
	int myNParam = (int)theGradLLH.GetSize() ;
	int myNLawParam = (int)theModel.mResids->GetNParam() ;
	eCondVarEnum myVarType = theModel.mVar->GetCondVarType() ;

	cDVector myVectParam(myNParam), myVect0(myNParam) ;
	
	theModel.RegArchParamToVector(myVectParam) ;
	theModel.mVar->UpdateProxyVarParameters();
	myVect0 = myVectParam ;
	for (register int i = 0 ; i < myNParam ; i++)
	{
	double myhh = fabs(theh * myVectParam[i]) ;
	if (myhh < 1e-16)
	myhh = theh ;
	myVectParam[i] += myhh ;
	theModel.VectorToRegArchParam(myVectParam) ;
	theModel.mVar->UpdateProxyVarParameters();
	
	double myLLH1 = RegArchLLH(theModel, theValue) ;
	theGradLLH[i] = (myLLH1 - myLLH0)/myhh ;
	myVectParam[i] -= myhh ;
	}
	theModel.VectorToRegArchParam(myVect0) ;
	}

	void NumericRegArchHessLLHold(cRegArchModel& theModel, cRegArchValue& theValue, cDMatrix& theHessLLH, double theh)
	{
	int myNParam = (int)theModel.GetNParam() ;
	cDVector myGrad0(myNParam), myGradij(myNParam), myVect(myNParam), myVect0(myNParam) ;
	theHessLLH.ReAlloc(myNParam, myNParam) ;
	
	theModel.RegArchParamToVector(myVect) ;
	myVect0 = myVect ;
	RegArchGradLLH(theModel, theValue, myGrad0) ; 	

	for (register int i = 0 ; i < myNParam ; i++)
	{	
	double myhhi = fabs(theh*myGrad0[i]) ;
	if (myhhi < 1e-16)
		myhhi = theh ;
	myVect[i] += myhhi ;
	theModel.VectorToRegArchParam(myVect) ;
	RegArchGradLLH(theModel, theValue, myGradij) ;
	myVect[i] -= myhhi ;
	for (register int j = 0 ; j < myNParam ; j++)
		theHessLLH[i][j] = (myGradij[j] - myGrad0[j])/myhhi ;
	}
	theModel.VectorToRegArchParam(myVect0) ;
	for (register int i = 0 ; i < myNParam-1 ; i++)
	for (register int j = i+1 ; j < myNParam ; j++)
	theHessLLH[i][j] = theHessLLH[j][i] = (theHessLLH[i][j] + theHessLLH[j][i])/2.0 ;
	}	


//#ifndef _RDLL_
	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue, sGSLMultiMinResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, eGSLMultiMinAlgoEnum theAlgo, double theStopValue, int theMaxIter, bool theVerbose)
	{
	uint myNParam = theModel.GetNParam() ;

		if (theModel.mMean != NULL)
			theResModel.SetMean(*(theModel.mMean)) ;
		else
			theResModel.mMean = NULL ;

		theResModel.SetVar(*(theModel.mVar)) ;
		theResModel.SetResid(*(theModel.mResids)) ;

		if (theInitPoint == NULL)
		{
			theResModel.SetDefaultInitPoint(theValue);		
		}
		else
		{	if (theInitPoint->GetGSLVector() == NULL)
				theResModel.SetDefaultInitPoint(theValue) ;
			else
				theResModel.VectorToRegArchParam(*theInitPoint) ;
		}
	cDVector myX0(myNParam) ;
		theResModel.RegArchParamToVector(myX0) ;
	cGSLMultiMin myMultiMin(myX0, theAlgo) ;

	sParamOptimStruct myOtherParam ;
		myOtherParam.mParam = &theResModel ;
		myOtherParam.mValue = &theValue ;

	gsl_multimin_function_fdf myFunct ;
		myFunct.df = GslGradLLHFunction ;
		myFunct.f = GslLLHFunction ;
		myFunct.fdf = GslLLHAndGradLLHFunction ;
		myFunct.n = myNParam ;
		myFunct.params = &myOtherParam ;

		myMultiMin.SetFunction(&myFunct) ;

	cDVector myX1(myNParam) ;

	sGSLMultiMinResult myGSLRes ;
 		myMultiMin.GSLOptim(myX1, theResStruct, theStopValue, theMaxIter, theVerbose) ;

		theResModel.VectorToRegArchParam(myX1) ;
	
	}

	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue,  sGSLMultiMinResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, sGSLMultiMinAlgoParam* theAlgoParam)
	{
	uint myNParam = theModel.GetNParam() ;

		if (theModel.mMean != NULL)
			theResModel.SetMean(*(theModel.mMean)) ;
		else
			theResModel.mMean = NULL ;

		theResModel.SetVar(*(theModel.mVar)) ;
		theResModel.SetResid(*(theModel.mResids)) ;

		if (theInitPoint == NULL)
		{
			theResModel.SetDefaultInitPoint(theValue);		
		}
		else
		{	if (theInitPoint->GetGSLVector() == NULL)
				theResModel.SetDefaultInitPoint(theValue) ;
			else
				theResModel.VectorToRegArchParam(*theInitPoint) ;
		}
	cDVector myX0(myNParam) ;
		theResModel.RegArchParamToVector(myX0) ;
	
	sGSLMultiMinAlgoParam* myAlgoParam ;
	bool myExist = (theAlgoParam != NULL) ;	
		if (!myExist)
		{
			myAlgoParam = (sGSLMultiMinAlgoParam *)malloc(sizeof(sGSLMultiMinAlgoParam)) ;
			myAlgoParam->mAlgoType = eBFGSTwo ;
			myAlgoParam->mNMaxIter = 200 ;
			myAlgoParam->mStepSize = 0.01 ;
			myAlgoParam->mTol = 0.01 ;
			myAlgoParam->mVerbose = false ;
		}
		else
		{
			myAlgoParam = theAlgoParam ;
		}
	cGSLMultiMin myMultiMin(myX0, myAlgoParam->mAlgoType) ;

	sParamOptimStruct myOtherParam ;
		myOtherParam.mParam = &theResModel ;
		myOtherParam.mValue = &theValue ;

	gsl_multimin_function_fdf myFunct ;
		myFunct.df = GslGradLLHFunction ;
		myFunct.f = GslLLHFunction ;
		myFunct.fdf = GslLLHAndGradLLHFunction ;
		myFunct.n = myNParam ;
		myFunct.params = &myOtherParam ;

		myMultiMin.SetFunction(&myFunct) ;

	cDVector myX1(myX0) ;
	
		myMultiMin.GSLOptim(myX1, theResStruct, *myAlgoParam) ;

		if (!myExist)
			free(myAlgoParam) ;

		theResModel.VectorToRegArchParam(myX1) ;
	
	}

	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue, cNLOPTResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, cNLOPTAlgoParam* theAlgoParam)
	{
	uint myNParam = theModel.GetNParam();

		if (theModel.mMean != NULL)
			theResModel.SetMean(*(theModel.mMean));
		else
			theResModel.mMean = NULL;

		theResModel.SetVar(*(theModel.mVar));
		theResModel.SetResid(*(theModel.mResids));

		if (theInitPoint == NULL)
		{
			theResModel.SetDefaultInitPoint(theValue);
		}
		else
		{
			if (theInitPoint->GetGSLVector() == NULL)
				theResModel.SetDefaultInitPoint(theValue);
			else
			theResModel.VectorToRegArchParam(*theInitPoint);
		}
	cDVector myX0(myNParam);
		theResModel.RegArchParamToVector(myX0);

	cNLOPTAlgoParam* myAlgoParam = new cNLOPTAlgoParam();
	bool myExist = (theAlgoParam != NULL);
		if (!myExist)
		{
			myAlgoParam = new cNLOPTAlgoParam();
		}
		else
		{
			myAlgoParam->mAlgo = theAlgoParam->mAlgo;
			myAlgoParam->mfTol = theAlgoParam->mfTol;
			myAlgoParam->mMaxComputeTime = theAlgoParam->mMaxComputeTime;
			myAlgoParam->mMaxFuncEval = theAlgoParam->mMaxFuncEval;
			myAlgoParam->mMinimisation = theAlgoParam->mMinimisation;
			myAlgoParam->mStopVal = theAlgoParam->mStopVal;
			myAlgoParam->mxTol = theAlgoParam->mxTol;
			myAlgoParam->mVerbose = theAlgoParam->mVerbose;
		}

	cNloptWrapperCpp myOptim = cNloptWrapperCpp();

	sParamOptimStruct myOtherParam;
		myOtherParam.mParam = &theResModel;
		myOtherParam.mValue = &theValue;
		myOtherParam.mVerbose = myAlgoParam->mVerbose;
		myOtherParam.mNFuncEval = 0;

		myOptim.SetAlgorithm(myAlgoParam->mAlgo, myNParam);
		myOptim.SetObjectiveFunc((nlopt_func)NloptLLHAndGradLLHFunction, false, &myOtherParam);

	double* myX1 = GSLVectorToDoubleStar(myX0);


		myOptim.Optimize(myX1, myNParam, *myAlgoParam, theResStruct);

		if (!myExist)
			free(myAlgoParam);
		cDVector myXOptim(myNParam, theResStruct.mXOptim);
			theResModel.VectorToRegArchParam(myXOptim);
	}

	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue, cNLOPTResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, nlopt_algorithm theAlgo, double theStopValue, double thefTol, double thexTol, double theNMaxSec, int theMaxFuncEval, bool theMinimisation, bool theVerbose)
	{
	cNLOPTAlgoParam* myAlgoParam = new cNLOPTAlgoParam();
	myAlgoParam->mAlgo = theAlgo;
	myAlgoParam->mfTol = thefTol;
	myAlgoParam->mMaxComputeTime = theNMaxSec;
	myAlgoParam->mMaxFuncEval = theMaxFuncEval;
	myAlgoParam->mMinimisation = theMinimisation;
	myAlgoParam->mStopVal = theStopValue;
	myAlgoParam->mxTol = thexTol;
	myAlgoParam->mVerbose = theVerbose;
	
	RegArchEstim(theModel, theValue, theResStruct, theResModel, theInitPoint, myAlgoParam);
	delete myAlgoParam;
	}

//#endif // _RDLL_

	void RegArchStatTable(cRegArchModel &theModel, cRegArchValue& theValue, cDMatrix& theTable)
	{
	uint myNParam = theModel.GetNParam();
	theTable.ReAlloc(myNParam, 4);
	cDVector myTeta(myNParam);
		theModel.RegArchParamToVector(myTeta);
		for (register uint i = 0; i < myNParam; i++)
			theTable[i][0] = myTeta[i];
	cDMatrix myCov(myNParam, myNParam);
		RegArchComputeCov(theModel, theValue, myCov);

//	cout << "Matrice de covariance:" << endl << myCov << endl;

		for (register uint i = 0; i < myNParam; i++)
		{
			theTable[i][1] = sqrt(myCov[i][i]);
			theTable[i][2] = theTable[i][0] / theTable[i][1];
			theTable[i][3] = (1.0-gsl_cdf_gaussian_P(fabs(theTable[i][2]), 1.0))*2.0;
		}
	}
}
