#include "StdAfxRegArchLib.h"

/*!
	\file cConstCondVar.cpp
	\brief sources for class cConstCondVar methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cConstCondVar::cConstCondVar(double theData): cAbstCondVar(eCste)
	 * \param double theData: constant variance value. Default 1.
	 */
	cConstCondVar::cConstCondVar(double theValue)
	: cAbstCondVar(eCste)  // call constructor of cAbstCondVar with type eCste
	{	
		mvCste = theValue ;
		MESS_CREAT("cConstCondVar")
	}

	/*!
	* \fn cConstCondVar::cConstCondVar(const cAbstCondVar& theConstCondVar):cAbstCondVar(eConstCondVar)
	* \param const cAbstCondVar& theArch: the recopy parameter
	*/
	cConstCondVar::cConstCondVar(const cConstCondVar& theConstCondVar)
	: cAbstCondVar(eCste)   // call constructor of cAbstCondVar with type eCste
	{
		*this = theConstCondVar;
		MESS_CREAT("cConstCondVar")
	}

	/*!
	 * \fn cConstCondVar::~cConstCondVar()
	 * \param none
	 * \details Nothing to do here.
	 */
	cConstCondVar::~cConstCondVar()
	{	
		MESS_DESTR("cConstCondVar")
	}

	/*!
	 * \fn cAbstCondVar* cConstCondVar::PtrCopy()
	 */

	cAbstCondVar* cConstCondVar::PtrCopy() const
	{
		 cConstCondVar *myConstCondVar = new cConstCondVar(*this);
		 return myConstCondVar;
	}

	/*!
	 * \fn void cConstCondVar::Delete(void)
	 * \param void
	 * \details Nothing to do here.
	 */
	void cConstCondVar::Delete(void)
	{
	}

	/*!
	 * \fn void void cConstCondVar::Print(ostream& theOut) const
	 * \param ostream& theOut: output stream (file or screen) default cout.
	 */
#ifndef _RDLL_
	void cConstCondVar::Print(ostream& theOut) const
	{
		theOut << "Constant variance model with:" << endl ;
		theOut << "\tConstant=" << mvCste << endl ;
	}
#else
	void cConstCondVar::Print(void)
	{
		Rprintf("Constant variance model with:\n");
		Rprintf("\tConstant=%f\n", mvCste);
	}
#endif // _RDLL_

	void cConstCondVar::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvCste = theVar ;
	}

	void cConstCondVar::SetDefaultInitPoint(cRegArchValue& theValue)
	{
		theValue.ComputeVar(mvCste);
	}


	/*!
	 * \fn void cConst::ReAlloc(const uint theSize, const uint theNumParam=0)
	 * \param const uint theSize: not used. Not used for cConstClass
	 * \param const uint theNumParam: not used for cConst class
	 * \details Nothing to do for cConstCondVar Class.
	 */
	void cConstCondVar::ReAlloc(const uint theSize, const uint theNumParam)
	{
	}

	/*!
	 * \fn void cConstCondVar::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the constant value is in theVectParam[0]
	 * \param const uint theNumParam: not used for cConst class
	 * \details Here, mvCste = theVectParam[0]
	 */
	 void cConstCondVar::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{	if (theVectParam.GetSize() > 0)
			mvCste = theVectParam[0] ;
		else
			throw cError("Size of theVectParam must be > 0.") ;
	}

	 /*!
	  * \fn void cConstCondVar::Set(const double theValue, const uint theIndex, const uint theNumParam)
	  * \brief fill the parameters vector
	  * \param const double theValue: the constant value.
	  * \param const uint theIndex: not used here. Default 0.
	  * \param const uint theNumParam: not used for cConstCondVar model. Default 0.
	  * \details mvCste = theValue
	  */
	void cConstCondVar::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		mvCste = theValue ;

	}

	/*!
	 * \fn void cConstCondVar::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the constant value is in theVectParam[0].
	 * \param const uint theIndex: not used here. Default 0.
	 * \param const uint theNumParam: not used for cConstCondVar model. Default 0.
	 * \details mvCste = theVectParam[0]
	 */
	void cConstCondVar::Set(const cDVector& theVectParam, const uint theNumParam)
	{	if (theVectParam.GetSize() > 0)
			mvCste = theVectParam[0] ;
		else
			throw cError("the size of theVectParam must be > 0") ;
	}

	double  cConstCondVar::Get(const uint theIndex, const uint theNumParam)
	{
		return mvCste ;
	}

	cDVector& cConstCondVar::Get(const uint theNumParam)
	{
		ourAuxVector.ReAlloc(1, mvCste);
		return ourAuxVector;
	}

	cConstCondVar& cConstCondVar::operator =(const cConstCondVar& theSrc)
	{
		mvCste = theSrc.mvCste;
		return *this;
	}

	/*!
	 * \fn double cConstCondVar::ComputeVar(const uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of the computation
	 * \param cRegArchValue& theData: past datas.
	 * \details theData must not be updated here.
	 */
	double cConstCondVar::ComputeVar(uint theDate, const cRegArchValue& theData) const
	{
		return mvCste ;
	}

	uint cConstCondVar::GetNParam(void) const
	{
		return 1 ;
	}

	uint cConstCondVar::GetNLags(void) const
	{
		return 0 ;
	}

	void cConstCondVar::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResids)
	{
		theGradData.mCurrentGradVar[theGradData.GetNMeanParam()] = 1.0 ;
	}

	void cConstCondVar::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = theDestVect.GetSize() ;
		if (theIndex >= mySize)
			throw cError("Wrong size") ;
		theDestVect[theIndex] = mvCste;
	}

	void cConstCondVar::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		if (1 + theIndex > theSrcVect.GetSize())
			throw cError("Wrong size") ;
		mvCste = theSrcVect[theIndex] ;
	}

	void cConstCondVar::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	// Hess = 0
	}

	void cConstCondVar::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
	}



}//namespace
