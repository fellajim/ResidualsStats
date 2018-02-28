#include "StdAfxError.h"
/*! 
	\file cError.cpp
	\brief cError class functions declaration
*/
namespace ErrorNameSpace
{
	/*!
	 \fn cError
	 \param const string& theMess
	 \brief prints theMess and exits program
	*/

#ifdef _RDLL_
	cError::cError(const char* theMess)
	{	error(theMess) ;
	}
#else
	cError::cError(const char* theMess)
	{
		if (theMess != NULL)
			cError((string)theMess) ;
		exit(0) ;
	}

	cError::cError(const string& theMess)
	{
		if (theMess.length() != 0) 
			cout << theMess.c_str() << endl ;
		exit(0) ;
	}

#endif // _RDLL_
} //namespace
