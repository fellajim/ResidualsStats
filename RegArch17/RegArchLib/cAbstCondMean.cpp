#include "StdAfxRegArchLib.h"
/*!
	\file cAbstCondMean.cpp
	\brief Sources for abstract class cAbstCondMean methods.
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	/*!
	 * \fn cAbstCondMean::cAbstCondMean(const eCondMeanEnum theType)
	 * \param const eCondMeanEnum theType. Code of the conditional mean type. Default value: eUnknown
	 * \details Set the real type code of conditional mean component
	 */
	cAbstCondMean::cAbstCondMean(const eCondMeanEnum theType)
	{
		mvCondMeanType = theType ;
  		MESS_CREAT("cAbstCondMean")
	}

	/*!
	 * \fn cAbstCondMean::~cAbstCondMean()
	 */
	cAbstCondMean::~cAbstCondMean()
	{
		mvCondMeanType = eUnknown ;
  		MESS_DESTR("cAbstCondMean")
	}

	/*!
	 * \fn  inline eCondMeanEnum cAbstCondMean::GetCondMeanType(void)
	 * \param void
	 * \return a eCondMeanEnum value.
	 */
	eCondMeanEnum cAbstCondMean::GetCondMeanType(void) const
	{
		return mvCondMeanType ;
	}

	/*!
	 * \fn  void cAbstCondMean::SetCondMeanType(eCondMeanEnum theType)
	 * \param eCondMeanEnum theType
	 * \details mvCondMeanType = theType
	 */
	void cAbstCondMean::SetCondMeanType(eCondMeanEnum theType)
	{	mvCondMeanType = theType ;
	}


#ifndef _RDLL_
	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean)
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 * \param const cAbstCondMean& theAbstCondMean: the mean component model.
	 * \details Uses cAbstCondMean::Print method.
	 */
	ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean)
	{
		theAbstCondMean.Print(theOut) ;
		return theOut ;
	}
#endif // _RDLL_

	template<class T>
	static T* TemplateCreateOneRealCondMean(void)
	{
		T*	myCondMean = new T();
		return myCondMean;
	}

	template<class T>
	static T* TemplateCreateOneRealCondMean(cAbstCondMean& theAbstCondMean)
	{
	T* mySrc = static_cast<T *>(&theAbstCondMean);
	
	if (mySrc)
	{
		T* myCondMean;
		myCondMean = new T(*mySrc);
		return myCondMean;
	}
	else
		throw cError("Wrong contional mean in TemplateCreateOneRealCondMean");
	}

	/*!
	* \fn cAbstCondMean* CreateOneRealCondMean(const eCondMeanEnum theType)
	* \param const eCondMeanEnum theType: code of the real condtional mean component.
	* \details This function has to be changed when adding a new conditional mean type.
	*/
	cAbstCondMean* CreateOneRealCondMean(const eCondMeanEnum theType)
	{
		switch (theType)
		{
		case eConst:
			return TemplateCreateOneRealCondMean<cConst>();
			break;
		case eAr:
			return TemplateCreateOneRealCondMean<cAr>();
			break;
		case eMa:
			return TemplateCreateOneRealCondMean<cMa>();
			break;
		case eLinReg:
			return TemplateCreateOneRealCondMean<cLinReg>();
			break;
		case eStdDevInMean:
			return TemplateCreateOneRealCondMean<cStdDevInMean>();
			break;
		case eVarInMean:
			return TemplateCreateOneRealCondMean<cVarInMean>();
			break;
		case eArfima:
			return TemplateCreateOneRealCondMean<cArfima>();
			break;
		default:
			throw cError("CreateOneRealCondMean: unknown conditional mean type");
			break;
		}
	}

	cAbstCondMean* CreateOneRealCondMean(cAbstCondMean& theAbstCondMean)
	{
		switch (theAbstCondMean.GetCondMeanType())
		{
		case eConst:
			return TemplateCreateOneRealCondMean<cConst>(theAbstCondMean);
			break;
		case eAr:
			return TemplateCreateOneRealCondMean<cAr>(theAbstCondMean);
			break;
		case eMa:
			return TemplateCreateOneRealCondMean<cMa>(theAbstCondMean);
			break;
		case eLinReg:
			return TemplateCreateOneRealCondMean<cLinReg>(theAbstCondMean);
			break;
		case eStdDevInMean:
			return TemplateCreateOneRealCondMean<cStdDevInMean>(theAbstCondMean);
			break;
		case eVarInMean:
			return TemplateCreateOneRealCondMean<cVarInMean>(theAbstCondMean);
			break;
		case eArfima:
			return TemplateCreateOneRealCondMean<cArfima>(theAbstCondMean);
			break;
		default:
			throw cError("CreateOneRealCondMean: unknown conditional mean type");
			break;
		}
	}

	
}//namespace
