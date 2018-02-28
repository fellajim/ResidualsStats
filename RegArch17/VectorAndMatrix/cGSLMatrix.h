#pragma once 
#ifndef _cGSLMatrix_H_
#define _cGSLMatrix_H_
	#include <gsl/gsl_matrix.h>
	#include <gsl/gsl_linalg.h>
	#include <cstdlib>
	#include <cstring>
	#include <cmath>
	#include <cfloat>

	#ifndef _RDLL_
		#include <iostream>
		#include <iostream>
	#else
		#include <R.h>
	#endif //_RDLL_
	
	/*!
		\file cGSLMatrix.h
		\brief Header fo C++ encapsulation of GSL matrix definitions and functions.
	
		\par Details.
		\par uses gsl_matrix.
		\author Ollivier TARAMASCO 
		\date oct-09-2008
	*/

	#ifndef MIN_DBLE
		#define MIN_DBLE DBL_EPSILON
	#endif //MIN_DBLE


	namespace VectorAndMatrixNameSpace
	{

			/*!
				\brief class cGSLMatrix
				\par
				1) Encapsulation of the "C" gsl structure gsl_matrix \par
				2) Standard and usefull matrix operations declarations
			*/	

			class cGSLVector ;
			class cGSLMatrix
			{
			private :
				gsl_matrix*		mvMat ; ///< data
			public :
				cGSLMatrix(int theNRow=0, int theNCol=0, double theVal = 0.0L) ; ///< standard constructor
				cGSLMatrix(const cGSLVector& theVect) ; ///< constructor
				cGSLMatrix(const cGSLMatrix& theMat) ; ///< constructor
				virtual ~cGSLMatrix() ; ///< standard destructor
				uint GetNRow(void) const ; ///< returns matrix number of rows
				uint GetNCol(void) const ; ///< returns matrix number of columns
				inline gsl_matrix* GetMat(void) { return mvMat ;} ;
				void Delete(void) ; ///< Deletes matrix
				void ReAlloc(uint theNRow, uint theNCol, double theVal = 0.0L) ; ///< Reallocates matrix
				void ReAlloc(const cGSLVector& theVect) ; ///< Reallocates matrix
				void ReAlloc(const cGSLMatrix& theMat) ; ///< Reallocates matrix
				double* operator[](int theNRow) const ; ///< Returns address of matrix[theNRow]
				cGSLMatrix& operator =(const cGSLMatrix& theSrcMat) ; ///< Standard matrix affectation
				cGSLMatrix& operator =(const cGSLVector& theVect) ; ///< one column matrix affectation
				cGSLMatrix& operator =(double theVal) ; ///< matrix[i,j] = theVal
				cGSLMatrix& operator +=(const cGSLMatrix& theMatrix) ; ///< Standard += operation
				//template <typename T> cGSLMatrix& operator +=(T theVal) ; 
				cGSLMatrix& operator +=(double theVal) ; ///< matrix[i,j] += theVal
				cGSLMatrix& operator -=(const cGSLMatrix& theMatrix) ; ///< Standard -= operation
				cGSLMatrix& operator -=(double theVal) ; ///< matrix[i,j] -= theVal
				cGSLMatrix& operator *=(const cGSLMatrix& theRight) ; ///< Standard *= operation
				cGSLMatrix& operator *=(double theLambda) ; ///< matrix[i,j] *= theVal
				cGSLMatrix& operator /=(double theLambda) ; ///< matrix[i,j] /= theVal
			#ifndef _RDLL_
				void Print(ostream& theStream=cout) ;
			#else
				void Print(void);
			#endif //_RDLL_
				void SetRow(uint theRowNumber, const cGSLVector& theVector);
				void SetColumn(uint theColumnNumber, const cGSLVector& theVector);
			#ifdef _MSC_VER
				friend cGSLMatrix operator +(const cGSLMatrix& theLeft, const cGSLMatrix &theRight) ; ///< Standard + operation
				friend cGSLMatrix operator +(double theVal, const cGSLMatrix &theRight) ;
				friend cGSLMatrix operator +(const cGSLMatrix &theLeft, double theVal) ;
				friend cGSLMatrix operator -(const cGSLMatrix& theLeft, const cGSLMatrix &theRight) ;
				friend cGSLMatrix operator -(double theVal, const cGSLMatrix &theRight) ;
				friend cGSLMatrix operator -(const cGSLMatrix &theLeft, double theVal) ;
				friend cGSLMatrix operator *(const cGSLMatrix& theLeft, const cGSLMatrix &theRight) ;
				friend cGSLVector operator *(const cGSLMatrix& theLeft, const cGSLVector& theVect) ;
				friend cGSLMatrix operator *(const cGSLVector& theVect, const cGSLMatrix& theRight) ;
				friend cGSLMatrix operator *(const cGSLMatrix& theMat, double theLambda) ;
				friend cGSLMatrix operator *(double theLambda, const cGSLMatrix& theMat) ;
				friend cGSLMatrix operator /(const cGSLMatrix& theMat, double theLambda) ;
				friend cGSLMatrix Zeros(uint theN, uint theP) ;
				friend cGSLMatrix Identity(uint theN) ;
				friend cGSLMatrix Diag(const cGSLVector& theVect) ;
				friend cGSLVector Diag(const cGSLMatrix& theMat);
				friend void Svd(const cGSLMatrix& theMatrix, cGSLMatrix& theU, cGSLVector& theS, cGSLMatrix& theV) ;
				friend cGSLMatrix Inv(const cGSLMatrix &theMatrix) ;
				friend void ClearMatrix(cGSLMatrix& theMatrix) ;
			#ifndef _RDLL_
				friend std::ostream& operator <<(std::ostream& theStream, const cGSLMatrix& theMat) ;
			#endif //_RDLL_
				friend cGSLMatrix Transpose(const cGSLMatrix& theMatrix) ;
				friend cGSLMatrix Transpose(const cGSLVector& theVector) ;
				friend cGSLMatrix Abs(const cGSLMatrix& theMatrix);
				friend double Mini(const cGSLMatrix& theMatrix);
				friend double Maxi(const cGSLMatrix& theMatrix);
			#endif // _MSC_VER
			};// class
	#ifndef _MSC_VER
		extern cGSLMatrix operator +(const cGSLMatrix& theLeft, const cGSLMatrix &theRight) ; ///< Standard + operation
		extern cGSLMatrix operator +(double theVal, const cGSLMatrix &theRight) ;
		extern cGSLMatrix operator +(const cGSLMatrix &theLeft, double theVal) ;
		extern cGSLMatrix operator -(const cGSLMatrix& theLeft, const cGSLMatrix &theRight) ;
		extern cGSLMatrix operator -(double theVal, const cGSLMatrix &theRight) ;
		extern cGSLMatrix operator -(const cGSLMatrix &theLeft, double theVal) ;
		extern cGSLMatrix operator *(const cGSLMatrix& theLeft, const cGSLMatrix &theRight) ;
		extern cGSLVector operator *(const cGSLMatrix& theLeft, const cGSLVector& theVect) ;
		extern cGSLMatrix operator *(const cGSLVector& theVect, const cGSLMatrix& theRight) ;
		extern cGSLMatrix operator *(const cGSLMatrix& theMat, double theLambda) ;
		extern cGSLMatrix operator *(double theLambda, const cGSLMatrix& theMat) ;
		extern cGSLMatrix operator /(const cGSLMatrix& theMat, double theLambda) ;
		extern cGSLMatrix Zeros(uint theN, uint theP) ;
		extern cGSLMatrix Identity(uint theN) ;
		extern cGSLMatrix Diag(const cGSLVector& theVect) ;
		extern void Svd(const cGSLMatrix& theMatrix, cGSLMatrix& theU, cGSLVector& theS, cGSLMatrix& theV) ;
		extern cGSLMatrix Inv(const cGSLMatrix &theMatrix) ;
		extern void ClearMatrix(cGSLMatrix& theMatrix) ;
	#ifndef _RDLL_
		extern std::ostream& operator <<(std::ostream& theStream, const cGSLMatrix& theMat) ;
	#endif // _RDLL_
		extern cGSLMatrix Transpose(const cGSLMatrix& theMatrix) ;
		extern cGSLMatrix Transpose(const cGSLVector& theVector) ;
		extern cGSLMatrix Abs(const cGSLMatrix& theMatrix);
		extern double Mini(const cGSLMatrix& theMatrix);
		extern double Maxi(const cGSLMatrix& theMatrix);
	#endif // _MSC_VER
	} // namespace
#endif // _cGSLMatrix_H_

