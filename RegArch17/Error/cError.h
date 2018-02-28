#pragma once
#ifndef _CERROR_H_
	#define _CERROR_H_
	/*!
		\file cError.h
		\brief Header for error class declarations
	*/

	#ifdef _RDLL_
		#include <R.h>
	#else
		#include <cstdlib>
		#include <cstring>
		#include <iostream>
		using namespace std ;
	#endif // _RDLL_
	#ifndef NULL
		#define NULL 0
	#endif // NULL

	namespace ErrorNameSpace
	{
		/*!
			\class cError
			\brief Only used to exit program with an output message
		*/

		class cError
		{
		public :
			cError(const char* theMess) ;
			#ifndef _RDLL_
				cError(const string& theMess) ;
			#endif // _RDLL_
		} ;
	}

#endif //_CERROR_H_

