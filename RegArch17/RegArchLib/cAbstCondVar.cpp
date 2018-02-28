#include "StdAfxRegArchLib.h"

/*!
	\file cAbstCondVar.cpp
	\brief sources for abstract class cAbstCondVar methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {


	/*!
	 * \fn cAbstCondVar::cAbstCondVar(eCondVarEnum theType)
	 * \param eCondVarEnum theType: Conditional variance type code. Default eNotKnown.
	 */
	cAbstCondVar::cAbstCondVar(eCondVarEnum theType)
	{	mvCondVar = theType ;
	}

	/*!
	 * \fn cAbstCondVar::~cAbstCondVar()
	 * \details Nothing to do here
	 */
	cAbstCondVar::~cAbstCondVar()
	{
	}

	/*!
	 * \fn inline eCondVarEnum cAbstCondVar::GetCondVarType(void) const
	 * \param void
	 * \brief return the real conditional variance type code.
	*/
	eCondVarEnum cAbstCondVar::GetCondVarType(void) const
	{	return mvCondVar ;
	}

	void cAbstCondVar::SetCondVarType(eCondVarEnum theType)
	{
		mvCondVar = theType;
	}

#ifndef _RDLL_
	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cAbstCondVar& theAbstCondVar)
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 * \param const cAbstCondVar& theAbstCondVar: the variance model.
	 * \details Uses cAbstCondVar::Print
	 */
	ostream& operator <<(ostream& theOut, const cAbstCondVar& theAbstCondVar)
	{	theAbstCondVar.Print(theOut) ;
		return theOut ;
	}
#endif // _RDLL_

	void cAbstCondVar::NumericComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals, double theh)
	{
	uint myNParam = GetNParam();
	cDVector myParam(myNParam);
		RegArchParamToVector(myParam);
	double myF0 = ComputeVar(theDate, theData);
		for (register uint i = 0; i < myNParam; i++)
		{
			double myh = fabs(theh*myParam[i]);
			if (myh < 1e-16)
				myh = theh;
			myParam[i] += myh;
			VectorToRegArchParam(myParam);
			UpdateProxyVarParameters();
			double myF1 = ComputeVar(theDate, theData);
			theGradData.mCurrentGradVar[i] = (myF1 - myF0) / myh;
			myParam[i] -= myh;
		}
	}

	/*!
	* \fn template<class T> static T* TemplateCreateOneRealCondVar(cAbstCondVar& theAbstCondVar)
	* \param cAbstCondVar& theAbstCondVar
	*/
	template<class T>
	static T* TemplateCreateOneRealCondVar(cAbstCondVar& theAbstCondVar)
	{
	T*	mySrc = static_cast<T *>(&theAbstCondVar);
		if (mySrc)
		{
			T* myCondVar = new T(*mySrc);
			return myCondVar;
		}
		else
		{
			throw cError("Wrong Conditional Variance in TemplateCreateOneRealCondVar");
		}
	}

	/*!
	* \fn template<class T> static T* TemplateCreateOneRealCondVar(void)
	* \param void
	*/
	template<class T>
	static T* TemplateCreateOneRealCondVar(void)
	{
		T*	myCondVar = new T();
		return myCondVar;
	}


	/*!
	* \fn CreateRealCondVar* CreateRealCondVar(eCondVarEnum theType)
	* \param theType: type of conditional variance.
	* \par Details This function has to be changed when adding a new conditional variance type.
	*/
	cAbstCondVar* CreateRealCondVar(eCondVarEnum theType)
	{
		switch (theType)
		{
		case eCste:
			return TemplateCreateOneRealCondVar<cConstCondVar>();
			break;
		case eArch:
			return TemplateCreateOneRealCondVar<cArch>();
			break;
		case eGarch:
			return TemplateCreateOneRealCondVar<cGarch>();
			break;
		case eEgarch:
			return TemplateCreateOneRealCondVar<cEgarch>();
			break;
		case eAparch:
			return TemplateCreateOneRealCondVar<cAparch>();
			break;
		case eTarch:
			return TemplateCreateOneRealCondVar<cTarch>();
			break;
		case eFigarch:
			return TemplateCreateOneRealCondVar<cFigarch>();
			break;
		case eUgarch:
			return TemplateCreateOneRealCondVar<cUgarch>();
			break;
		case eNotKnown:
		default:
			throw cError("unknown conditional variance type");
			break;
		}
	}

	cAbstCondVar* CreateRealCondVar(cAbstCondVar& theAbstCondVar)
	{
		switch (theAbstCondVar.GetCondVarType())
		{
		case eCste:
			return TemplateCreateOneRealCondVar<cConstCondVar>(theAbstCondVar);
			break;
		case eArch:
			return TemplateCreateOneRealCondVar<cArch>(theAbstCondVar);
			break;
		case eGarch:
			return TemplateCreateOneRealCondVar<cGarch>(theAbstCondVar);
			break;
		case eEgarch:
			return TemplateCreateOneRealCondVar<cEgarch>(theAbstCondVar);
			break;
		case eAparch:
			return TemplateCreateOneRealCondVar<cAparch>(theAbstCondVar);
			break;
		case eTarch:
			return TemplateCreateOneRealCondVar<cTarch>(theAbstCondVar);
			break;
		case eFigarch:
			return TemplateCreateOneRealCondVar<cFigarch>(theAbstCondVar);
			break;
		case eUgarch:
			return TemplateCreateOneRealCondVar<cUgarch>(theAbstCondVar);
			break;

		default:
			throw cError("CreateOneRealCondVar: unknown conditional var type");
			break;
		}
	}

}//namespace
