#pragma once 
#ifndef _REGARCHDEF_H_
#define _REGARCHDEF_H_
/*!
	\file RegArchDef.h
	\brief Common definitions for all the classes.
	
	\author Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/


namespace RegArchLib {
	using namespace ErrorNameSpace;
	using namespace VectorAndMatrixNameSpace ;
	using namespace std;
	using namespace WrapperGslCppNameSpace ;
	using namespace WrapperNloptCppNamespace;

	#define cDVector cGSLVector 
	#define cDMatrix cGSLMatrix  


	#ifndef MIN
		#define MIN(p, q) ( (p) < (q) ? (p): (q) ) ///< Definition of MIN if doesn't exist.
	#endif // MIN

	#ifndef MAX
		#define MAX(p, q) ( (p) > (q) ? (p): (q) ) ///< Definition of MAX if doesn't exist.
	#endif // MAX

	#ifndef SIGN
		#define SIGN(p) ( (p) > 0 ? 1: -1 )
	#endif // SIGN

	#ifndef PI
		#define PI 3.1415926535897931159979634685
	#endif //PI

	#ifndef SQRT_PI
		#define SQRT_PI 1.7724538509055161039640324816
	#endif // SQRT_PI

	#ifndef SQRT_2_PI
		#define SQRT_2_PI 2.50662827463100050241576528481 ///< useful value
	#endif //SQRT_2_PI

	#ifndef LOG_SQRT_2_PI
		#define LOG_SQRT_2_PI 0.918938533204672741780329736406
	#endif // LOG_SQRT_2_PI

	#ifndef LOG_PI
		#define LOG_PI 1.1447298858494001638774761886
	#endif // LOG_PI

	#ifndef MAX_COND_MEAN
		#define MAX_COND_MEAN 10
	#endif //MAXCONDMEAN

	/*!
	 *	\enum eCondMeanEnum
	 *	\brief different type of conditional means.
	 */
		typedef enum eCondMeanEnum
		{	eUnknown=0,
			eConst,
			eAr,
			eMa,
			eLinReg, // + X(t)*beta
			eStdDevInMean, // + lambda * Sigma(t)
			eVarInMean, // + Lambda * Sigma(t)^2
			eArfima
		}eCondMeanEnum ;

	/*!
	 *	\enum eCondVarEnum
	 *	\brief different type of conditional variances.
	 */
	typedef enum eCondVarEnum
	{	eNotKnown=0,
		eCste,
		eArch,
		eGarch,
		eEgarch,
		eAparch,
		eTarch,
		eFigarch,
		eUgarch
	}eCondVarEnum ;

	/*!
	 *	\enum eDistrTypeEnum
	 *	\brief different type of conditional distribution.
	 */

	typedef enum eDistrTypeEnum
	{	eUndefined=0,
		eNormal,
		eStudent,
		eGed, // Generalized error distribution http://en.wikipedia.org/wiki/Exponential_power_distribution
		eSkewt	// Skewed Student-t http://cran.r-project.org/doc/packages/skewt.pdf
	}eDistrTypeEnum ;


	// POUR DEBOGUER
	#ifdef _MESS_CREAT
		#ifdef _DEBUG
			#define MESS_CREAT(p)  std::cout << "creation de " << p << std::endl ;
			#define MESS_DESTR(p)  std::cout << "destruction de " << p << std::endl ;
		#else
			#define MESS_CREAT(p) ///< used for debugging 
			#define MESS_DESTR(p) ///< used for debugging
		#endif //_DEBUG
	#else
		#define MESS_CREAT(p) ///< used for debugging
		#define MESS_DESTR(p) ///< used for debugging
	#endif //_MESS_CREAT

	extern cDVector ourAuxVector;
} //namespace


#endif // _REGARCHDEF_H_

