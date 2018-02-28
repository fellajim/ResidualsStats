// Test.cpp : définit le point d'entrée pour l'application console.
//

#include "StdAfxTestCplusPlus.h"
 
using namespace std;
using namespace ErrorNameSpace;
using namespace VectorAndMatrixNameSpace;
using namespace WrapperGslCppNameSpace;
using namespace WrapperNloptCppNamespace;
using namespace RegArchLib ;





int _tmain(int argc, _TCHAR* argv[])
{

ifstream myFile("CACWE.txt", ios::in) ;
int myNObs = 0;
double myRt;
int myd1, myd2, myd3, myd4, myd5;
char myBuf[500];
while (myFile.getline(myBuf, 500))
{
	myNObs++;
}
myFile.close();
myFile.open("CACWE.txt", ios::in);
int kk = 0;
cDVector myYt(myNObs);
cDMatrix myXt(myNObs, 5);
while (myFile.getline(myBuf, 500))
{
	sscanf(myBuf, "%lf %d %d %d %d %d", &myRt, &myd1, &myd2, &myd3, &myd4, &myd5);
	myYt[kk] = myRt*100;
	myXt[kk][0] = myd1;
	myXt[kk][1] = myd2;
	myXt[kk][2] = myd3;
	myXt[kk][3] = myd4;
	myXt[kk][4] = myd5;
	kk++;
}



/*
cAr	myAr(2);

	myAr.Set(.8, 0);
	myAr.Set(-.2, 1);
cAr myAr1 = myAr;

cArfima myArfima(2, 2, .2, 100);
	myArfima.Set(.3, 0, 0);
	myArfima.Set(.2, 1, 0);
	myArfima.Set(.4, 0, 1);
	myArfima.Set(.5, 1, 1);
*/
	/*
	myArfima.UpdateProxyMeanParameters();
	myArfima.Print();
	myArfima.PolMaPrint();
	myArfima.PolGradPrint();
	return 0;
	*/

cLinReg myDummy(5);
cUgarch myUgarch(false, 5, 1, 1);

/*
cFigarch myFigarch(1, 1, 0.5, 100);
	myFigarch.Set(.1, 0, 0);
	myFigarch.Set(.1, 0, 1);
	myFigarch.Set(.8, 0, 2);
*/

/*	
	myFigarch.UpdateProxyVarParameters();
	myFigarch.Print();
	myFigarch.PolArchPrint();
	myFigarch.PolGradPrint();
	return 0;
*/



/*
cMa myMa(1) ;
	myMa.Set(0.8, 0) ;
//	myMa.Set(0.6, 1) ;
*/

cConst myConst(10);

cCondMean myCondMean ;

    //myCondMean.AddOneMean(myConst);
    //myCondMean.AddOneMean(myDummy);
	//myCondMean.AddOneMean(myArfima);
	//myCondMean.SetOneMean(0, myAr) ;

	//myCondMean.SetOneMean(1, myMa) ;

/*	cCondMean myCondMean1 = cCondMean(myCondMean) ;
	cout << "CondMean :" << myCondMean << endl ;
	cout << "CondMean1 : " <<  myCondMean1 << endl ;
	myCondMean1.Delete() ;
//	cout << "CondMean :" << myCondMean << endl ;
//	cout << "CondMean1 : " <<  myCondMean1 << endl ;
	myCondMean1 = myCondMean ;
//	cout << "CondMean1 : " <<  myCondMean1 << endl ;
*/
/*
cGarch myGarch(1,1) ;
	myGarch.Set(0.1, 0, 0) ;
	myGarch.Set(0.1, 0, 1) ;
	myGarch.Set(0.8, 0, 2) ;
*/

//cConstCondVar myConstVar(1.0) ;

/*
cConst myConst(100.0);
*/

//cStudentResiduals myStudent(5, true) ;
cNormResiduals myNormResid ;
/*
cSkewtResiduals mySkewt(10, 1.5);
cGedResiduals myGedResiduals(1.5);
*/

/*
cEgarch myEgarch(&myNormResid, 1, 1);
myEgarch.Set(-0.1217, 0, 1); // cste
myEgarch.Set(0.06046, 0, 2); // ARCH
myEgarch.Set(0.985106, 0, 3); // GARCH
myEgarch.Set(3.198596, 0, 4); // TETA
myEgarch.Set(1.0, 0, 5); //GAMMA
//myEgarch.Print();
*/

cConstCondVar myConstVar(1.0) ;
/*
cConst myConstMean(10.0);
cArch myArch(1);
	myArch.Set(0.4, 0, 0);
	myArch.Set(0.5, 0, 1);
	
cStdDevInMean myStdDevInMean(.6);
cVarInMean myVarInMean(.6);
*/
cRegArchModel myModel ;

//	myModel.AddOneMean(myConst);
//	myModel.AddOneMean(myArfima);
//	myModel.AddOneMean(myAr);
	myModel.AddOneMean(myDummy);
//	myModel.PrintMean();

//	myModel.SetMean(myCondMean);
	
//	myModel.AddOneMean(myMa);
//	myModel.SetResid(myStudent) ;
//	myModel.SetResid(mySkewt);
//	myModel.SetResid(myGedResiduals);

//	myModel.SetMean(myCondMean);
	myModel.SetResid(myNormResid);
	myModel.SetVar(myUgarch);


//	myModel.SetVar(myGarch);
//	myModel.SetVar(myConstVar);	
//	myModel.SetVar(myFigarch) ;
//	myModel.SetVar(myEgarch);

//	cout << "Modele : " ;
//	myModel.Print() ;
//	return 0 ;
	
/*
cRegArchModel* myModele1 = new cRegArchModel(myModele) ;
	cout << "Modele1 : " ;
	myModele1->Print() ;
cRegArchModel myModele2 = *myModele1 ;
	cout << "Modele2 : " ;
	myModele2.Print() ;
	delete myModele1 ;
	
cArch myArch(1) ;
	myArch.Set(0.1, 0, 0) ;
	myArch.Set(0.9, 0, 1) ;

cAparch myAparch(1, 1) ;
	myAparch.Set(0.1, 0, 0) ; // cste ;
	myAparch.Set(1.3, 0, 1) ; // Delta ;
	myAparch.Set(0.1, 0, 2) ; // Arch ;
	myAparch.Set(0.3, 0, 3) ; // Gamma ;
	myAparch.Set(0.8, 0, 4) ; // Garch ;


cTarch myTarch(1) ;
	myTarch.Set(0.1, 0, 0) ;
	myTarch.Set(0.8, 0, 1) ;
	myTarch.Set(0.7, 0, 2) ;
//	myTarch.Print(cout) ;


*/
//cVarInMean myVarInMean(0.4) ;



//uint myNSimul = 1000;
//cRegArchValue myValue(myNSimul) ;
cRegArchValue myValue(&myYt, &myXt);
//	RegArchSimul(myNSimul, myModel, myValue) ;

/*
cRegArchGradient myGradient = cRegArchGradient(&myModel);
cRegArchHessien myHessien = cRegArchHessien(&myModel);
uint myDate = 30;
	
	myGedResiduals.ComputeGrad(myDate, myValue, myGradient);
	cout << "Gradient : " << endl << myGradient.mCurrentGradEps;

	
double myBetah = myGedResiduals.Get(0);
double myh = 1e-5;
	myh *= myBetah;
double myF0 = myGedResiduals.LogDensity(myValue.mYt[myDate]);
	myBetah += myh;
	myGedResiduals.Set(myBetah, 0);
double myF1 = myGedResiduals.LogDensity(myValue.mYt[myDate]);
	myGedResiduals.Set(myBetah - myh, 0);
	cout << "Gradient Numerique: " << endl << (myF1 - myF0) / myh << endl;

	myGedResiduals.ComputeHess(myDate, myValue, myGradient, myHessien, &myNormResid);
	cout << "Hessien : " << endl << myHessien.mCurrentHessEps;
	myBetah += myh;
	myGedResiduals.Set(myBetah, 0);
	double myF2 = myGedResiduals.LogDensity(myValue.mYt[myDate]);
	cout << "Hessien Numerique: " << endl << (myF2 - 2* myF1 + myF0) / pow(myh,2) << endl;

	return 0;
*/
/*
ofstream myFile;
	myFile.open("GED.txt");
	myFile << myValue << endl;
	myFile.close();
	return 0;
*/

	
cRegArchModel myResModel(myModel);
	myModel.SetDefaultInitPoint(myValue);

	cDMatrix myMat = Transpose(myXt)*myXt;
	cDMatrix myMat1 = Inv(myMat);
	cDVector myBetad = myMat1 * Transpose(myXt) * myYt;


uint myNParam = myModel.GetNParam();
cNLOPTResult myResNloptStruct(myNParam);
cNLOPTAlgoParam* myNloptAlgoParam = new cNLOPTAlgoParam();
	myNloptAlgoParam->mAlgo = NLOPT_LD_TNEWTON;
	myNloptAlgoParam->mMaxComputeTime = 600;
	myNloptAlgoParam->mMinimisation = false;
	myNloptAlgoParam->mStopVal = 1e299;
	myNloptAlgoParam->mfTol = 1e-6;
	myNloptAlgoParam->mxTol = 1e-6;
	myNloptAlgoParam->mVerbose = true;

	
cDVector myInitPoint = cDVector(myNParam, 0.0) ;
// Ox Garch init value
/*

Parameter      Starting Value
Cst(M)               0.010000
Cst(V)               0.040000
d-Figarch            0.450000
ARCH(Phi1)           0.100000
GARCH(Beta1)         0.400000


/*
ARFIMA Ox estimation
d-Arfima             0.227278   0.032408    7.013  0.0000
AR(1)                0.783563   0.022611    34.65  0.0000
MA(1)                0.401927   0.013616    29.52  0.0000
Cst(V)               0.992453   0.014035    70.71  0.0000

Log Likelihood   :-14151.533



	myInitPoint[0] = 0.783563;
	myInitPoint[1] = 0.401927;
	myInitPoint[2] = 0.227278;
	myInitPoint[3] = 0.992453;
	*/
	myModel.RegArchParamToVector(myInitPoint);

	for (uint jj = 0; jj <5; jj++)
		myInitPoint[jj] = myBetad[jj];

	myResModel.VectorToRegArchParam(myInitPoint);

	cout << "Modele initial : " ;
	myResModel.Print() ;

/*
	cout << "Model Ox :" << endl;
	myResModel.Print();
*/

double myLLH = RegArchLLH(myResModel, myValue);
	cout << "LLH=" << myLLH << endl;
//	return 0;


/*
cRegArchGradient myGradient(&myModel), myGradient1(&myModel);
cDVector myGradLt(myNParam);
double myLt;
	myModel.mVar->UpdateProxyVarParameters();
	for (register uint t = 0; t < 200; t++)
	{
		RegArchLtAndGradLt(t, myModel, myValue, myGradient, myLt, myGradLt);
		myGradient.Update();
	}

	myFigarch.UpdateProxyVarParameters();
	myFigarch.ComputeGrad(200, myValue, myGradient, &myNormResid);
	cout << "Gradient theorique : " << endl;
	cout << myGradient.mCurrentGradVar << endl;

	myFigarch.NumericComputeGrad(200, myValue, myGradient1, 1, &myNormResid);
	cout << "Gradient diff finie : " << endl;
	cout << myGradient1.mCurrentGradVar << endl;
	return 0;
*/

sGSLMultiMinResult myResGSLStruct;
sGSLMultiMinAlgoParam myGSLAlgoParam;
	myGSLAlgoParam.mAlgoType = eConjugateFR;
	myGSLAlgoParam.mNMaxIter = 1000;
	myGSLAlgoParam.mStepSize = 0.0001;
	myGSLAlgoParam.mStopValue = 0.000001;
	myGSLAlgoParam.mTol = 0.000001;
	myGSLAlgoParam.mVerbose = false;
	myGSLAlgoParam.mNMaxSeconds = 1000;

	RegArchEstim(myModel, myValue, myResGSLStruct, myResModel, &myInitPoint, &myGSLAlgoParam);
	cout << endl << myResGSLStruct.mConvergenceComment << endl;
	myResModel.Print();

//	myResModel.RegArchParamToVector(myInitPoint);
	RegArchEstim(myModel, myValue, myResNloptStruct, myResModel, NULL, myNloptAlgoParam);

	cout << endl << myResNloptStruct.mConvergenceString << endl << "LLH = " << myResNloptStruct.mFunctValue << endl << "Temps : " << myResNloptStruct.mComputeTime << " s" << endl;
	myResModel.Print();
	cDMatrix myTable;
	RegArchStatTable(myResModel, myValue, myTable);
char** myParamName = NULL;
	myModel.GetParamName(myParamName);
	cout << "Parameter\tValue\tStd.Err\t t stat\tP Value" << endl;
	for (uint i = 0; i < myTable.GetNRow(); i++)
		cout << myParamName[i] << "\t" << myTable[i][0] << "\t" << myTable[i][1] << "\t" << myTable[i][2] << "\t" << myTable[i][3] << endl;


	return 0;
/*
cRegArchGradient myGradient = cRegArchGradient(&myModel);
cRegArchHessien myHessien = cRegArchHessien(&myModel);
uint myNParam = myModel.GetNParam();
cDVector myGradLt = cDVector(myNParam);
cDMatrix myHessLt = cDMatrix(myNParam, myNParam);
double mylt = 0;
cDVector myGradNum = cDVector(myNParam);
cRegArchValue* myValueGrad = new cRegArchValue[myNParam+1];
cRegArchGradient* myGradientHess = new cRegArchGradient[myNParam + 1];

for (uint j = 0; j <= myNParam; j++)
	{
		myValueGrad[j].ReAlloc(myValue.mYt, myValue.mXt);
		myGradientHess[j].ReAlloc(&myModel);
	}


cDMatrix myHessNum = cDMatrix(myNParam, myNParam);
cDMatrix myDiff = cDMatrix(myNParam, myNParam);
cDVector myDiffGrad = cDVector(myNParam);
	cout.precision(3);
for (register uint t = 0; t < myNSimul; t++)
	{
			
		NumericRegArchGradLt(t, myModel, myValueGrad, myGradNum);
		RegArchGradLt(t, myModel, myValue, myGradient, myGradLt);
		myDiffGrad = myGradLt - myGradNum;
		for (register uint i = 0; i < myNParam; i++)
		myDiffGrad[i] = 100 * abs(myDiffGrad[i] / myGradNum[i]);
			

		NumericRegArchHessLt(t, myModel, myValueGrad, myGradientHess, myHessNum);
		RegArchHessLt(t, myModel, myValue, myGradient, myHessien, myHessLt);
		myDiff = 100*(myHessNum - myHessLt) ;
		for (register uint i = 0; i < myNParam; i++)
			for (register uint j = 0; j < myNParam; j++)
				myDiff[i][j] = abs(myDiff[i][j] / myHessNum[i][j]);

		
		cout << "t=" << t << endl;
//		cout << myDiffGrad << endl;
		cout << myDiff << endl;
//		cout << myGradient.mCurrentGradMu << endl;
//		cout << myHessien.mCurrentHessVar << endl;
//		cout << myHessLt << endl;
//		cout << myHessien.mCurrentHessDens << endl;
		
		myGradient.Update();
		myHessien.Update();
		cout << endl;
		cout << "Maxi = " << Maxi(Abs(myDiff)) << "\n" << endl;
	}

*/
	exit(0);

/*
//redéfinition de myModel avec valeur empiriques
cRegArchModel myModelEmpi;
cRegArchValue myValueEmpi(myNSimul);
myValueEmpi.ReAlloc(myValue.mYt);
double meanEmpi, varEmpi ;
myValueEmpi.ComputeMeanAndVar(meanEmpi,varEmpi);
cConstCondVar myConstVarEmpi(varEmpi);
cConst myConstMeanEmpi(meanEmpi);
cCondMean myCondMeanEmpi ;
myCondMeanEmpi.AddOneMean(myConstMeanEmpi);
myModelEmpi.SetMean(myCondMeanEmpi);
myModelEmpi.SetVar(myConstVarEmpi);
myModelEmpi.SetResid(myNormResid);

//cRegArchModel myResModel(myModel);
cRegArchModel myResModel(myModelEmpi);


double myMeanSerie, myVarSerie ;
myValue.ComputeMeanAndVar(myMeanSerie, myVarSerie) ;
myResModel.mMean->SetDefaultInitPoint(myMeanSerie, myVarSerie) ;
myResModel.mVar->SetDefaultInitPoint(myMeanSerie, myVarSerie);
myResModel.Print() ;

		ofstream fichier("../test.txt", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
 
        if(fichier)
        {
                fichier <<	myValue.mYt << endl; 
                fichier.close();
        }
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl; 


*/


}
