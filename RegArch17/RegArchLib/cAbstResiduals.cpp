#include "StdAfxRegArchLib.h"
/*!
	\file cAbstResiduals.cpp
	\brief sources for abstract class cAbstResiduals methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cAbstResiduals::cAbstResiduals(eDistrTypeEnum theDistr, cDVector* theModel, bool theSimulFlag)
	 * \param const eDistrTypeEnum theDistr: code for the conditional distribution.
	 * \param const cDVector* theModel: vector of parameters
	 * \param const bool theSimulFlag: true if created for simulation
	 */
	cAbstResiduals::cAbstResiduals(eDistrTypeEnum theDistr, cDVector* theDistrParam, bool theSimulFlag)
	{
		mvDistr = theDistr ;
		if (theSimulFlag)
		{	// random generator simulation
		gsl_rng_env_setup() ;
			mtR = gsl_rng_alloc(gsl_rng_default) ;
		#ifndef _DEBUG
			gsl_rng_set(mtR, (unsigned long int)time(NULL)) ;
		#else
			gsl_rng_set(mtR, 0) ; // Pour avoir toujours la m�me s�rie simul�e quand on teste
		#endif // _DEBUG
		}
		else
			mtR = NULL ;

		if (theDistrParam != NULL)
			mDistrParameter = *theDistrParam ;
		else
			mDistrParameter.Delete() ;
		MESS_CREAT("cAbstResiduals") ;
	}

	/*!
	 * \fn cAbstResiduals::~cAbstResiduals()
	 * \details mDistrParameter is deleted here.
	 */
	cAbstResiduals::~cAbstResiduals()
	{
		mDistrParameter.Delete() ;
		if (mtR != NULL)
		{	// random generator destructor
			gsl_rng_free(mtR) ;
			mtR = NULL ;
		}
		MESS_DESTR("cAbstResiduals") ;
	}

	/*!
	 * \fn cAbstResiduals::Delete(void)
	 * \param void
	 * \details mDistrParameter is deleted here.
	 */
	void cAbstResiduals::Delete(void)
	{
		mDistrParameter.Delete() ;
		if (mtR != NULL)
		{	// random generator destructor
			gsl_rng_free(mtR) ;
			mtR = NULL ;
		}
	}

	/*!
	 * \fn cAbstResiduals::SetSimul(void)
	 * \param void
	 */
	void cAbstResiduals::SetSimul(void)
	{
		if (mtR == NULL)
		{
			// random generator initialisation
			gsl_rng_env_setup() ;
			mtR = gsl_rng_alloc(gsl_rng_default) ;
		#ifndef _DEBUG
			gsl_rng_set(mtR, (unsigned long int)time(NULL)) ;
		#else
			gsl_rng_set(mtR, 0) ; // Pour avoir toujours la m�me s�rie simul�e quand on teste
		#endif // _DEBUG
		}
	}

	/*!
	 * \fn cAbstResiduals& cAbstResiduals::operator =(const cAbstResiduals& theSrc)
	 * \param const cAbstResiduals& theSrc
	 */


	/*!
	 * \fn inline eDistrTypeEnum cAbstResiduals::GetDistrType(void) const
	  * \param void
	 */
	eDistrTypeEnum cAbstResiduals::GetDistrType(void) const
	{	return mvDistr ;
	}

	double cAbstResiduals::Get(const uint theIndex)
	{
		if (mDistrParameter.GetSize() > theIndex)
			return mDistrParameter[theIndex] ;
		else
			throw cError("Wrong size in cAbstResiduals::Get") ;
	}

	void cAbstResiduals::Set(double theValue, const uint theIndex)
	{
		if (mDistrParameter.GetSize() <= theIndex)
		{	mDistrParameter.ReAlloc(theIndex+1) ;
		}
		mDistrParameter[theIndex] = theValue ;
	}
	
	void cAbstResiduals::ReAlloc(uint theSize)
	{
		mDistrParameter.ReAlloc(theSize);
	}

#ifndef _RDLL_
	/*!
	 * \fn ostream& cAbstResiduals::operator <<(ostream& theOut, const cAbstResiduals& theAbstResisuals)
	 * \param ostream& theOut: output (file or screen).
	 * \param const cAbstResiduals& theAbstResisuals: the residuals model.
	 * \details Uses cAbstResiduals::Print method.
	 */
	ostream& operator <<(ostream& theOut, const cAbstResiduals& theAbstResisuals)
	{
		theAbstResisuals.Print(theOut) ;
		return theOut ;
	}
#endif // _RDLL_

	template<class T>
	static T* TemplateCreateRealCondResiduals(cAbstResiduals& theAbstCondResiduals)
	{
		T*	myCondResiduals = new T();
		*(cAbstResiduals *)myCondResiduals = theAbstCondResiduals;
		return myCondResiduals;
	}
	/*!
	* \fn template<class T> static T* TemplateCreateRealCondResiduals(const cDVector* theDistrParam, const bool theSimulFlag)
	* \param const cDVector* theDistrParam: vector of parameters
	* \param const bool theSimulFlag: true if created for simulation
	*/
	template<class T>
	static T* TemplateCreateRealCondResiduals(cDVector* theDistrParam, bool theSimulFlag)
	{
		T*	myCondResiduals = new T(theDistrParam, theSimulFlag);
		return myCondResiduals;
	}

	/*!
	* \fn cAbstResiduals* CreateRealCondResiduals(eDistrTypeEnum theType, cDVector* theDistrParam, bool theSimulFlag)
	* \param eDistrTypeEnum theType: type of conditional residuals.
	* \param cDVector* theDistrParam: distribution parameters. Default NULL
	* \param bool theSimulFlag. True if created for simulation. Default true.
	* \details
	* This function has to be changed when adding a new conditional residuals type.
	*/
	cAbstResiduals* CreateRealCondResiduals(eDistrTypeEnum theType, cDVector* theDistrParam, bool theSimulFlag)
	{
		switch (theType)
		{
		case eNormal:
			return TemplateCreateRealCondResiduals<cNormResiduals>(NULL, theSimulFlag);
			break;
		case eStudent:
			if (theDistrParam == NULL)
			{
				cDVector myParam = cDVector(1, 3.0);
				return TemplateCreateRealCondResiduals<cStudentResiduals>(&myParam, theSimulFlag);
			}
			else
				return TemplateCreateRealCondResiduals<cStudentResiduals>(theDistrParam, theSimulFlag);
			break;

		case eGed:
			if (theDistrParam == NULL)
			{
				cDVector myParam = cDVector(1, 1.5);
				return TemplateCreateRealCondResiduals<cGedResiduals>(&myParam, theSimulFlag);
			}
			else
				return TemplateCreateRealCondResiduals<cGedResiduals>(theDistrParam, theSimulFlag);
			break;
			break;
		case eSkewt:
			if (theDistrParam == NULL)
			{
				cDVector myParam = cDVector(2, 3);
				myParam[1] = 1.0;
				return TemplateCreateRealCondResiduals<cGedResiduals>(&myParam, theSimulFlag);
			}
			else
				return TemplateCreateRealCondResiduals<cGedResiduals>(theDistrParam, theSimulFlag);
			break;

		default:
			cError myError("CreateRealCondResiduals: unknown conditional distribution type");
			break;
		}
	}

	/*!
	* \fn cAbstResiduals* CreateRealCondResiduals(eDistrTypeEnum theType, cDVector* theDistrParam, bool theSimulFlag)
	* \param eDistrTypeEnum theType: type of conditional residuals.
	* \param cDVector* theDistrParam: distribution parameters. Default NULL
	* \param bool theSimulFlag. True if created for simulation. Default true.
	* \details
	* This function has to be changed when adding a new conditional residuals type.
	*/
	cAbstResiduals* CreateRealCondResiduals(cAbstResiduals& theAbstCondResiduals)
	{
		switch (theAbstCondResiduals.GetDistrType())
		{
		case eNormal:
			return TemplateCreateRealCondResiduals<cNormResiduals>(theAbstCondResiduals);
			break;
		case eStudent:
			return TemplateCreateRealCondResiduals<cStudentResiduals>(theAbstCondResiduals);
			break;
		case eGed:
			return TemplateCreateRealCondResiduals<cGedResiduals>(theAbstCondResiduals);
			break;
		case eSkewt:
			return TemplateCreateRealCondResiduals<cSkewtResiduals>(theAbstCondResiduals);
			break;
			break;

		default:
			cError myError("CreateRealCondResiduals: unknown conditional distribution type");
			break;
		}
	}

} // namespace

