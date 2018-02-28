#include "StdAfxRegArchLib.h"
/*!
	\file cEgarch.cpp
	\brief sources for class cEgarch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {

	cEgarch::cEgarch(int theNArch, int theNGarch)
	:cAbstCondVar(eEgarch)  // call constructor of cAbstCondVar with type eEgarch
	{
		mvEspAbsEpsilont = 2/SQRT_2_PI  ; // Bon seulement pour la loi normale
		mvCste = 0.0L ;
		mvArch.ReAlloc(theNArch) ;
		mvGarch.ReAlloc(theNGarch) ;
		mvTeta=0;
		mvGamma=0;
		MESS_CREAT("cEgarch") ;
	}

	cEgarch::cEgarch(cAbstResiduals* theResiduals, int theNArch, int theNGarch)
	:cAbstCondVar(eEgarch)
	{
		if (theResiduals != NULL)
			mvEspAbsEpsilont = theResiduals->ComputeEspAbsEps() ;
		mvCste = 0.0L ;
		mvArch.ReAlloc(theNArch) ;
		mvGarch.ReAlloc(theNGarch) ;
		mvTeta=0;
		mvGamma=0;
		MESS_CREAT("cEgarch") ;
	}

	cEgarch::cEgarch(const cEgarch& theEgarch)
	:cAbstCondVar(eEgarch) 
	{	*this = theEgarch;
		MESS_CREAT("cEgarch");
	}

	cEgarch::~cEgarch()
	{
		mvArch.Delete() ;
		mvGarch.Delete() ;
		MESS_DESTR("cEgarch") ;
	}

	/*!
	 * \fn cAbstCondVar* cEgarch::PtrCopy()
	 */

	cAbstCondVar* cEgarch::PtrCopy() const
	{
	cEgarch *myEgarch = new cEgarch(*this);
		 return myEgarch;
	}
	
	void cEgarch::Delete(void)
	{	mvArch.Delete() ;
		mvGarch.Delete() ;
	}

	void cEgarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		if (theNumParam == 1)
			mvArch.ReAlloc(theSize) ;
		else
			mvGarch.ReAlloc(theSize) ;
	}

	void cEgarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		if (theNumParam == 0)
			mvArch.ReAlloc((int)theVectParam[0]) ;
		else
			mvGarch.ReAlloc((int)theVectParam[0]) ;
	}

#ifndef _RDLL_
	void cEgarch::Print(ostream& theOut) const
	{
		theOut << "EGARCH(" << mvArch.GetSize() << ", " << mvGarch.GetSize() << ") model:" << endl ;
		theOut << "Const=" << mvCste << endl ;
	register uint i ;
		for (i = 0 ; i < mvArch.GetSize() ; i++)
			theOut << "ARCH[" << i+1 << "]=" << mvArch[i] << endl ;
		for (i = 0 ; i < mvGarch.GetSize() ; i++)
			theOut << "GARCH[" << i+1 << "]=" << mvGarch[i] << endl ;
		theOut << "Teta=" << mvTeta << endl ;
		theOut << "Gamma=" << mvGamma << endl ;
	}
#else
	void cEgarch::Print(void)
	{
		Rprintf("EGARCH(%d, %d) model with:\n", mvArch.GetSize(), mvGarch.GetSize());
		Rprintf("Const=%f\n", mvCste);
	register uint i;
		for (i = 0; i < mvArch.GetSize(); i++)
			Rprintf("ARCH[%d]=%f\n", i + 1, mvArch[i]);
		for (i = 0; i < mvGarch.GetSize(); i++)
			Rprintf("GARCH[%d]=%f\n", i + 1, mvGarch[i]);
		Rprintf("Teta=%f\n", mvTeta);
		Rprintf("Gamma=%f\n", mvGamma);
	}
#endif // _RDLL_

	void cEgarch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvCste = log(theVar) ;
	register uint i ;
		for (i = 0 ; i < mvArch.GetSize() ; i++)
			mvArch[i] = 0.0 ;
		for (i = 0 ; i < mvGarch.GetSize() ; i++)
			mvGarch[i] = 0.0;
		mvTeta = mvGamma = 0.0 ;
	}

	void cEgarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvCste = log(myVar);
	register uint i;
		for (i = 0; i < mvArch.GetSize(); i++)
			mvArch[i] = 0.0;
		for (i = 0; i < mvGarch.GetSize(); i++)
			mvGarch[i] = 0.0;
		mvTeta = mvGamma = 0.0;
	}

	void cEgarch::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				mvEspAbsEpsilont = theVectParam[0] ;
			break ;
			case 1 :
				mvCste = theVectParam[0] ;
			break ;
			case 2 :
				mvArch = theVectParam ;
			break ;
			case 3:
				mvGarch = theVectParam ;
			case 4:
				mvTeta = theVectParam[0];
			default :
				mvGamma = theVectParam[0];
			break ;
		}
	}

	void cEgarch::Set(const double theValue, const uint theIndex, const uint theNumParam) 
	{
	uint mySize ;
		switch (theNumParam)
		{	case 0 :
				mvEspAbsEpsilont = theValue ;
			break ;
			case 1 :
				mvCste = theValue ;
			break ;
			case 2 :
			{	mySize = mvArch.GetSize() ;
				if (mySize < theIndex)
					mvArch.ReAlloc(theIndex+1) ;
				mvArch[theIndex]= theValue ;
			}
			break ;
			case 3 :
			{	mySize = mvGarch.GetSize() ;
				if (mySize < theIndex)
					mvGarch.ReAlloc(theIndex+1) ;
				mvGarch[theIndex]= theValue ;
			}
			case 4:
				mvTeta = theValue ;
			break ;

			case 5:
				mvGamma = theValue;
			break ;

			default :
			break ;
		}
	}

	double  cEgarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				return mvCste ;
			break ;		
			case 1 :
				return mvArch[theIndex] ;
			break ;
			case 2 :
				return mvGarch[theIndex] ;
			break ;
			case 3 :
				return mvTeta ;
			break ;
			case 4 :
				return mvGamma ;
			break ;
		}
	}

	cDVector& cEgarch::Get(const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			ourAuxVector.ReAlloc(1, mvCste);
			return ourAuxVector;
			break;
		case 1:
			return mvArch;
			break;
		case 2:
			return mvGarch;
			break;
		case 3:
			ourAuxVector.ReAlloc(1, mvTeta);
			return ourAuxVector;
			break;
		case 4:
			ourAuxVector.ReAlloc(1, mvGamma);
			return ourAuxVector;
			break;
		}
	}

	cEgarch& cEgarch::operator =(const cEgarch& theSrc)
	{
		mvArch = theSrc.mvArch;
		mvGarch = theSrc.mvGarch;
		mvCste = theSrc.mvCste;
		mvGamma = theSrc.mvGamma;
		mvTeta = theSrc.mvTeta;
		mvEspAbsEpsilont = theSrc.mvEspAbsEpsilont;
		return *this ;
	}

	double cEgarch::ComputeVar(uint theDate, const cRegArchValue& theValue) const
	{
	uint myp = mvArch.GetSize(),
		myq = mvGarch.GetSize() ;
	double myRes = mvCste ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] *( mvTeta*theValue.mEpst[theDate-i] + mvGamma * (fabs(theValue.mEpst[theDate-i])- mvEspAbsEpsilont)) ;
		for (register uint j = 1 ; j <= MIN(myq, theDate) ; j++)
			myRes += mvGarch[j-1] * log(theValue.mHt[theDate-j]) ;

		return exp(myRes) ;
	}

	uint cEgarch::GetNParam(void) const 
	{
		return mvArch.GetSize() + mvGarch.GetSize() + 3 ;
	}

	uint cEgarch::GetNLags(void) const
	{
		return MAX(mvArch.GetSize(), mvGarch.GetSize()) ;
	}

	void cEgarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam(),
		 k = theIndex ;

		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		theDestVect[k++] = mvCste ;
		mvArch.SetSubVectorWithThis(theDestVect, k) ;
		k += mvArch.GetSize() ;
		mvGarch.SetSubVectorWithThis(theDestVect, k) ;
		k += mvGarch.GetSize() ;
		theDestVect[k++] = mvTeta	;
		theDestVect[k] = mvGamma	;
	}

	void cEgarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize(),
		k = theIndex					;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvCste = theSrcVect[k++] ;
		mvArch.SetThisWithSubVector(theSrcVect, k) ;
		k += mvArch.GetSize() ;
		mvGarch.SetThisWithSubVector(theSrcVect, k) ;
		k += mvGarch.GetSize() ;
		mvTeta = theSrcVect[k++] ;
		mvGamma = theSrcVect[k] ;
	}

	void cEgarch::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize(),
		myq = mvGarch.GetSize(),
		myBegIndex = theGradData.GetNMeanParam() ;
	register uint i, j ;
		theGradData.mCurrentGradVar = 0.0L ;
	// CSTE
		theGradData.mCurrentGradVar[myBegIndex] = 1.0 ;
	// ARCH
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar[myBegIndex+i] = mvTeta*theData.mEpst[theDate-i] + mvGamma*(fabs(theData.mEpst[theDate-i]) - mvEspAbsEpsilont) ;
		myBegIndex += myp ;
	// GARCH
		for (j = 1 ; j <= MIN(myq, theDate) ; j++)
			theGradData.mCurrentGradVar[myBegIndex+j] = log(theData.mHt[theDate-j]) ;
		myBegIndex += myq ;
	// Teta et Gamma
		for ( i = 1 ; i <= MIN(myp, theDate) ; i++)
		{	theGradData.mCurrentGradVar[myBegIndex+1] += mvArch[i-1] * theData.mEpst[theDate-i] ;
			theGradData.mCurrentGradVar[myBegIndex+2] += mvArch[i-1] * (fabs(theData.mEpst[theDate-i]) - mvEspAbsEpsilont) ;
		}

	// Et la suite
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar += mvArch[i-1]*
			(mvTeta + mvGamma*SIGN(theData.mEpst[theDate-i])) 
				* theGradData.mGradEpst[i-1] ;

		for (j = 1 ; j <= MIN(myq, theDate) ; j++)
			theGradData.mCurrentGradVar += mvGarch[j-1] / theData.mHt[theDate-j] * theGradData.mGradHt[j-1] ;

		// Et maintenant le gradient par rapport au(x) param�tre(s) du r�sidu

	int myNLawParam = (int)theGradData.GetNDistrParameter() ;
		if (myNLawParam > 0)
		{
		double myAux = -mvGamma * Sum(mvArch) ;
		cDVector myAuxGrad(myNLawParam) ;

			theResiduals->ComputeGradBetaEspAbsEps(myAuxGrad) ;
			myBegIndex = theGradData.GetNParam() - myNLawParam ;
			for (int i = 0 ; i < myNLawParam ; i++)
				theGradData.mCurrentGradVar[myBegIndex+i] += myAuxGrad[i]*myAux ;
		}

	// On a calcul�  grad Nu = grad(Log h), il faut calculer grad h	
	
		theGradData.mCurrentGradVar *= theData.mHt[theDate] ; 
	}

	void cEgarch::SetEspAbsEps(double theEspAbsEps)
	{
		mvEspAbsEpsilont = theEspAbsEps ;
	}

	void cEgarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
		// TO DO

	}

	void cEgarch::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH[%d]", i + 1);

		}
		for (uint i = 0; i < mvGarch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "GARCH[%d]", i + 1);

		}
		sprintf(theName[myIndex++], "THETA");
		sprintf(theName[myIndex++], "GAMMA");
	}
	



}//namespace
