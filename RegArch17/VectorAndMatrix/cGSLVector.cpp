#include "StdAfxVectorAndMatrix.h"
/*!
	\file cGSLVector.cpp
	\brief cGSLVector class functions definitions.
	\author Ollivier TARAMASCO 
	\date oct-09-2008
*/

namespace VectorAndMatrixNameSpace
{
	using namespace ErrorNameSpace ;

static cGSLVector ourTempVector ;

	/*!
		\fn cGSLVector::cGSLVector
		\param int theSize, double theVal
		\brief initializes GSLVector size to theSize and affects theVal to all components
	*/
	cGSLVector::cGSLVector(int theSize, double theVal)
	{
		if (theSize == 0)
			mvVect = NULL ;
		else
		{	if (theSize > 0)
			{	mvVect = gsl_vector_alloc((size_t)theSize) ;
				gsl_vector_set_all(mvVect, theVal) ;
			}
			else
				cError("Size of vector must be positive") ;
		}
		MESS_CREAT("cGSLVector") ;
	}

	/*!
		\fn cGSLVector::cGSLVector
		\param int theSize, double* theVect
		\brief initializes GSLVector size to theSize and mVect[i] to theVect[i] for all i 
	*/
	cGSLVector::cGSLVector(int theSize, double* theVect)
	{
		if (theSize == 0)
			mvVect = NULL ;
		else
		{	if (theSize > 0)
				mvVect = gsl_vector_alloc((size_t)theSize) ;
			else
				cError("Size of vector must be positive") ;
		}
		for (register int i = 0 ; i < theSize ; i++)
			gsl_vector_set(mvVect, (size_t)i, theVect[i]) ;
		MESS_CREAT("cGSLVector") ;
	}

	/*!
		\fn cGSLVector::cGSLVector
		\param const gsl_vector* theVect
		\brief initializes GSLVector size to theVect->size and mVect[i] to theVect[i] for all i 
	*/
	cGSLVector::cGSLVector(const gsl_vector* theVect)
	{
		if (theVect == NULL)
			mvVect = NULL ;
		else
		{	mvVect = gsl_vector_alloc(theVect->size) ;
			for (register int i = 0 ; i < (int)theVect->size ; i++)
				gsl_vector_set(mvVect, (size_t)i, gsl_vector_get(theVect, i)) ;
		}
	}

	/*!
		\fn cGSLVector::cGSLVector
		\param const cGSLVector& theVect
		\brief initializes GSLVector size to theVect.GetSize() and mVect[i] to theVect.mVect[i] for all i 
	*/
	cGSLVector::cGSLVector(const cGSLVector& theVect)
	{
	gsl_vector* myGSLVector = theVect.GetGSLVector() ;
		if (myGSLVector == NULL)
			mvVect = NULL ;
		else
		{	mvVect = gsl_vector_alloc(myGSLVector->size) ;
			for (register int i = 0 ; i < (int)myGSLVector->size ; i++)
				gsl_vector_set(mvVect, i, gsl_vector_get(myGSLVector, i)) ;
		}
	}

	/*!
		\fn cGSLVector::~cGSLVector
		\param none
		\brief Simple destructor
	*/
	cGSLVector::~cGSLVector()
	{
		if (mvVect != NULL)
		{	gsl_vector_free(mvVect) ;
			mvVect = NULL ;
		}
		MESS_DESTR("cGSLVector") ;
 	}

	/*!
		\fn uint cGSLVector::GetSize
		\param void
		\brief returns GSLVector size 
	*/
	uint cGSLVector::GetSize(void) const 
	{
		if (mvVect == NULL)
			return 0 ;
		return (uint)mvVect->size ;
	}

	/*!
		\fn gsl_vector* cGSLVector::GetGSLVector
		\param void
		\brief returns the private member mvVect
	*/
	gsl_vector* cGSLVector::GetGSLVector(void) const
	{ 
		return mvVect ;
	} 

	/*!
		\fn gsl_vector* cGSLVector::GetVector
		\param void
		\brief returns the private member mvVect
	*/
	gsl_vector* cGSLVector::GetVector(void) const
	{ 
		return mvVect ;
	} 

	/*!
		\fn void cGSLVector::Delete
		\param void
		\brief Destructor
	*/
	void cGSLVector::Delete(void)
	{
		if (mvVect != NULL)
		{	gsl_vector_free(mvVect) ;
			mvVect = NULL ;
		}
	}

	/*!
		\fn void cGSLVector::ReAlloc
		\param int theSize, double theVal
		\brief Reallocates GSL vector size to theSize and initialises mVect[i] to theVal for all i 
	*/
	void cGSLVector::ReAlloc(int theSize, double theVal)
	{	
		if (theSize < 0)
			cError("Size of vector must be positive") ;
		Delete() ;
		if (theSize > 0)
		{	mvVect = gsl_vector_alloc((size_t)theSize) ;
			gsl_vector_set_all(mvVect, theVal) ;
		}
	}

	/*!
		\fn void cGSLVector::ReAlloc
		\param int theSize, double* theVect
		\brief Reallocates GSL vector size to theSize and initialises mVect[i] to theVect[i] for all i 
	*/
	void cGSLVector::ReAlloc(int theSize, double* theVect)
	{	
		if (theSize < 0)
			cError("Size of vector must be positive") ;

		Delete() ;
		if (theSize > 0)
			mvVect = gsl_vector_alloc((size_t)theSize) ;
		for (register int i = 0 ; i < theSize ; i++)
			gsl_vector_set(mvVect, i, theVect[i]) ;
	}

	/*!
		\fn void cGSLVector::ReAlloc
		\param const cGSLVector& theVect
		\brief Reallocates GSL vector size to theVect.GetSize() and initialises mVect[i] to theVect.mVect[i] for all i 
	*/
	void cGSLVector::ReAlloc(const cGSLVector& theVect)
	{	
		Delete() ;
	uint mySize = theVect.GetSize() ;
		if (mySize > 0)
		{	mvVect = gsl_vector_alloc((size_t)mySize) ;
	
			for (register int i = 0 ; i < (int)mySize ; i++)
				gsl_vector_set(mvVect, i, theVect[i]) ;
		}
	}

	/*!
		\fn void cGSLVector::ReAlloc
		\param const gsl_vector* theVect
		\brief Reallocates GSL vector size to theVect->size and initialises mVect[i] to theVect[i] for all i 
	*/
	void cGSLVector::ReAlloc(const gsl_vector* theVect)
	{	
		Delete() ;
	int mySize = (int)theVect->size ;
		if (mySize > 0)
		{	mvVect = gsl_vector_alloc((size_t)mySize) ;
	
			for (register int i = 0 ; i < mySize ; i++)
				gsl_vector_set(mvVect, i, gsl_vector_get(theVect, i)) ;
		}
	}

	/*!
		\fn double& cGSLVector::operator[]
		\param int theIndex
		\brief Returns mVect[i] adress 
	*/
	double& cGSLVector::operator[](int theIndex) const 
	{	if ((theIndex >= 0) && ((theIndex < (int)GetSize())) )
			return(*gsl_vector_ptr(mvVect, (size_t)theIndex)) ;
		else
			throw cError("bad index") ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator=
		\param const cGSLVector& theSrcVect
		\brief GSLVector is re-allocated (if needed) and mVect=theSrcVect.mVect
	*/
	cGSLVector& cGSLVector::operator=(const cGSLVector& theSrcVect)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (mySize != GetSize())
			ReAlloc(theSrcVect) ;
		else
		{	for (register int i = 0 ; i < (int)mySize ; i++)
				(*this)[i] = theSrcVect[i] ;
		}
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator=
		\param double theVal
		\brief mVect[i] = theVal for all i
	*/
	cGSLVector& cGSLVector::operator=(double theVal) 
	{
		if (mvVect != NULL)
			gsl_vector_set_all(mvVect, theVal) ;
		else
			cError("Vector size must be strictly positive") ;
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator=
		\param double* theVect
		\brief mVect[i] = theVect[i] for all i
		known problem: if theVect size < mVect Size
	*/
	cGSLVector& cGSLVector::operator=(double* theVect)
	{
		if (mvVect != NULL)
		{	for (register int i = 0 ; i < (int)GetSize() ; i++)
				gsl_vector_set(mvVect, i, theVect[i]) ;
		}
		else
			cError("Unknown vector size") ;

		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator =
		\param const cGSLMatrix& theMatrix
		\brief Affects the one column matrix theMatrix to *this
	*/
	cGSLVector& cGSLVector::operator =(const cGSLMatrix& theMatrix)
	{
		if (theMatrix.GetNCol() == 1)
		{
			if (GetSize() != 0)
				gsl_vector_free(mvVect) ;
		
			mvVect = gsl_vector_alloc(theMatrix.GetNRow()) ;
		
			for (register uint i = 0 ; i < theMatrix.GetNRow() ; i++)
				gsl_vector_set(mvVect, i, theMatrix[i][0]) ;
		}
		else 
			cError("Matrix must have one column") ;
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator+
		\param double theVal
		\brief mVect[i] + theVal for all i
	*/
	cGSLVector& cGSLVector::operator +(double theVal) 
	{
	
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector += theVal ;
			return ourTempVector ;
		}
		else
			cError("Unknown vector size") ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator+
		\param const cGSLVector& theVect
		\brief mVect[i] + theVect[i] for all i
	*/
	cGSLVector& cGSLVector::operator +(const cGSLVector& theVect)
	{
	
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector += theVect ;
			return ourTempVector ;
		}
		else
			cError("Unknown vector size") ;
		
	}

	/*!
		\fn cGSLVector& cGSLVector::operator+
		\param const cGSLVector& theVect
		\brief mVect[i] + theVect[i] for all i
	*/
	cGSLVector& cGSLVector::operator +(double* theVect2) 	
	{
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector += theVect2 ;
			return ourTempVector ;
		}
		else
			cError("Unknown vector size") ;
	}	

	/*!
		\fn cGSLVector& cGSLVector::operator-
		\param double theVal
		\brief mVect[i] - theVal for all i
	*/
	cGSLVector& cGSLVector::operator -(double theVal) 
	{	if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector -= theVal ;
			return ourTempVector ;
		}
		else
			cError("Unknown vector size") ;
	}	
	
	/*!
		\fn cGSLVector& cGSLVector::operator-
		\param const cGSLVector& theVect
		\brief mVect[i] - theVect[i] for all i
	*/
	cGSLVector& cGSLVector::operator -(const cGSLVector& theVect)
	{
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector -= theVect ;
			return ourTempVector ; 
		}
		else
			cError("Unknown vector size") ;
	}	
	
	/*!
		\fn cGSLVector& cGSLVector::operator-
		\param const cGSLVector& theVect
		\brief mVect[i] - theVect[i] for all i
	*/
	cGSLVector& cGSLVector::operator -(double* theVect2) 	
	{
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector -= theVect2 ;
			return ourTempVector ; 
		}
		else
			cError("Unknown vector size") ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator*
		\param ouble theVal
		\brief mVect[i] * theVal for all i
	*/
	cGSLVector& cGSLVector::operator *(double theVal) 	
	{
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector *= theVal ;
			return ourTempVector ; 
		}
		else
			cError("Unknown vector size") ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator/
		\param ouble theVal
		\brief mVect[i] * theVal for all i
	*/
	cGSLVector& cGSLVector::operator /(double theVal) 	
	{
		if (mvVect != NULL)
		{	ourTempVector = *this ;
			ourTempVector /= theVal ;
			return ourTempVector ; 
		}
		else
			cError("Unknown vector size") ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator+=
		\param const cGSLVector& theVect
		\brief mVect[i] += theVect[i] for all i
	*/
	cGSLVector& cGSLVector::operator+=(const cGSLVector& theVect)
	{
	uint mySize = theVect.GetSize() ;
		if (mySize != GetSize())
			cError("Wrong size") ;

		for (register int i = 0 ; i < (int)mySize ; i++)
			(*this)[i] += theVect[i] ;
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator+=
		\param double theVal
		\brief mVect[i] += theVal for all i
	*/
	cGSLVector& cGSLVector::operator+=(double theVal)
	{
	uint mySize = GetSize() ;
		if (mySize == 0)
			cError("Wrong size") ;
	
		for (register int i = 0 ; i < (int)mySize ; i++)
			(*this)[i] += theVal ;
	
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator+=
		\param double* theVect
		\brief mVect[i] += theVect[i] for all i
		known problem if theVect size < mVect size
	*/
	cGSLVector& cGSLVector::operator+=(double* theVect)
	{
	uint mySize = GetSize() ;
		if (mySize == 0)
			cError("Wrong size") ;
	
		for (register int i = 0 ; i < (int)mySize ; i++)
			(*this)[i] += theVect[i] ;
	
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator-=
		\param const cGSLVector& theVect
		\brief mVect[i] -= theVect[i] for all i
	*/
	cGSLVector& cGSLVector::operator-=(const cGSLVector& theVect)
	{
	uint mySize = theVect.GetSize() ;
		if (mySize != GetSize())
			cError("Wrong size") ;
		for (register int i = 0 ; i < (int)mySize ; i++)
			(*this)[i] -= theVect[i] ;
	
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator-=
		\param double theVal
		\brief mVect[i] -= theVal for all i
	*/
	cGSLVector& cGSLVector::operator-=(double theVal)
	{
	uint mySize = GetSize() ;
		if (mySize == 0)
			cError("Wrong size") ;
	
		for (register int i = 0 ; i < (int)mySize ; i++)
			(*this)[i] -= theVal ;
	
		return *this ;
	}
	
	/*!
		\fn cGSLVector& cGSLVector::operator-=
		\param double* theVect
		\brief mVect[i] -= theVect[i] for all i
		known problem if theVect size < mVect size
	*/
	cGSLVector& cGSLVector::operator-=(double* theVect)
	{
	uint mySize = GetSize() ;
		if (mySize == 0)
			cError("Wrong size") ;
	
		for (register int i = 0 ; i < (int)mySize ; i++)
			(*this)[i] -= theVect[i] ;
	
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator*=
		\param double theVal
		\brief mVect[i] *= theVal for all i
	*/
	cGSLVector& cGSLVector::operator*=(double theVal)
	{
	uint mySize = GetSize() ;
		if (mySize == 0)
			cError("Wrong size") ;
		gsl_vector_scale(mvVect, theVal) ;		
		return *this ;
	}

	/*!
		\fn cGSLVector& cGSLVector::operator/=
		\param double theVal
		\brief mVect[i] *= theVal for all i
	*/
	cGSLVector& cGSLVector::operator/=(double theVal)
	{
	uint mySize = GetSize() ;
		if (mySize == 0)
			cError("Wrong size") ;
		if (theVal == 0)
			cError("Division by zero") ;
		gsl_vector_scale(mvVect, 1.0/theVal) ;		
	
		return *this ;
	}

	
#ifndef _RDLL_
	/*!
		\fn void cGSLVector::Print(std::ostream& theStream)
		\param std::ostream& theStream (default cout) 
		\brief Pront the vector
	*/
	void cGSLVector::Print(std::ostream& theStream)
	{	theStream << *this ;
	}
#else
	void cGSLVector::Print(void)
	{

	}
#endif // _RDLL_
	/*!
		\fn cGSLVector operator +
		\param double theVal, const cGSLVector& theVect
		\brief returns theVect[i] = theVect1[i] + theVal for all i
	*/
	cGSLVector operator +(double theVal, const cGSLVector& theVect)
	{	ourTempVector = theVect ;
		ourTempVector += theVal ;
 		return ourTempVector ; 
	}

	/*!
		\fn cGSLVector operator +
		\param  const  double* theVect1, cGSLVector& theVect2,
		\brief returns theVect[i] = theVect1[i] + theVect2[i] for all i
		\par known problem if theVect1 size < theVect2 size
	*/
	cGSLVector operator +(double* theVect1, const cGSLVector& theVect2)
	{	ourTempVector = theVect2 ;
		ourTempVector += *theVect1 ;
		return ourTempVector ; 
	}

	/*!
	\fn cGSLVector operator +
	\param  const cGSLVector& theVect1, double* theVect2
	\brief returns theVect[i] = theVect1[i] + theVect2[i] for all i
	\par known problem if theVect1 size < theVect2 size
	*/
	cGSLVector operator +(cGSLVector& theVect1, double* theVect2)
	{
		ourTempVector = theVect1;
		ourTempVector += *theVect2;
		return ourTempVector;
	}

	/*!
		\fn cGSLVector operator -
		\param double theVal, const cGSLVector& theVect
		\brief returns theResVect[i] = TheVal - theVect[i] for all i
	*/
	cGSLVector operator -(double theVal, const cGSLVector& theVect)
	{	ourTempVector = theVect ;
		ourTempVector *= -1.0 ;
		ourTempVector += theVal ;
		return ourTempVector ; 
	}

	/*!
		\fn cGSLVector operator -
		\param double* theVect1, const cGSLVector& theVect2
		\brief returns theResVect[i] = TheVect1[i] - theVect2[i] for all i
	*/
	cGSLVector operator -(double* theVect1, const cGSLVector& theVect2)
	{	ourTempVector.ReAlloc(theVect2.GetSize(), *theVect1) ;
		ourTempVector -= theVect2 ;
		return ourTempVector ; 
	}
	
	/*!
		\fn cGSLVector operator *
		\param double theVal, const cGSLVector& theVect
		\brief returns theResVect = theVal * theVect
	*/
	cGSLVector operator *(double theVal, const cGSLVector& theVect)
	{	ourTempVector = theVect ;
		ourTempVector *= theVal ;
		return ourTempVector ; 
	}

	/*!
		\fn cGSLVector operator /
		\param double theVal, const cGSLVector& theVect
		\brief returns theResVect[i] = theVal / theVect[i] for all i
	*/
	cGSLVector operator /(double theVal, const cGSLVector& theVect)
	{	
	uint mySize = theVect.GetSize() ;
		ourTempVector.ReAlloc(mySize, theVal) ;
		for (register uint i = 0 ; i < mySize ; i++)
			ourTempVector[i] /= theVect[i] ;
		return ourTempVector ; 
	}

	/*!
		\fn cGSLVector::SetThisWithSubVector
		\param const cGSLVector& theSrcVect, const uint theBegIndex
		\brief fills this[i] with theSrcVect[theBegIndex + i] i = 0 ... size of this
	*/
	void cGSLVector::SetThisWithSubVector(const cGSLVector& theSrcVect, const uint theBegIndex)
	{
	uint myDestSize = GetSize() ;
	uint mySrcSize = theSrcVect.GetSize() ;

	gsl_vector* mySrc = theSrcVect.GetGSLVector() ;
		if (myDestSize + theBegIndex > mySrcSize)
			throw cError("Wrong Size") ;
		for (register uint i = 0 ; i < myDestSize ; i++)
			gsl_vector_set(mvVect, i, gsl_vector_get(mySrc, i+theBegIndex)) ;
	}

	/*!
		\fn cGSLVector::SetSubVectorWithThis
		\param cGSLVector& theDestVect, uint theBegIndex
		\brief fills theSrcVect[theBegIndex + i] with *this[i], i = 0 ... size of this
	*/
	void cGSLVector::SetSubVectorWithThis(cGSLVector& theDestVect, const uint theBegIndex)
	{
	uint myDestSize = theDestVect.GetSize() ;
	uint mySrcSize = GetSize() ;

		if (myDestSize < mySrcSize + theBegIndex)
			throw cError("Wrong Size") ;
	gsl_vector* myDest = theDestVect.GetGSLVector() ;
		for (register uint i = 0 ; i < mySrcSize ; i++)
			gsl_vector_set(myDest, i+theBegIndex, gsl_vector_get(mvVect, i)) ;
	}

#ifndef _RDLL_
	/*!
		\fn std::ostream& operator <<
		\param std::ostream& theStream, const cGSLVector& theVect
		\brief prints theVect to stream theStream
	*/
	ostream& operator <<(ostream& theStream, const cGSLVector& theVect)
	{
		for (register uint i = 0 ; i < theVect.GetSize() ; i++)
			theStream << theVect[i] << std::endl ;
		return theStream ;
	}
#endif // _RDLL_

	/*!
		\fn double Norm
		\param const cGSLVector& theVect
		\brief euclidian norm
	*/
	double Norm(const cGSLVector& theVect)
	{
	double myRes = 0.0 ;
		for (register uint i = 0 ; i < theVect.GetSize() ; i++)
			myRes += theVect[i] * theVect[i] ;
		return sqrt(myRes) ;
	}

	/*!
		\fn double Sum
		\param const cGSLVector& theVect
		\brief sum
	*/

	double Sum(const cGSLVector& theVect)
	{
	double myRes = 0.0 ;
		for (register uint i = 0 ; i < theVect.GetSize() ; i++)
			myRes += theVect[i]  ;
		return myRes ;
	}

	cGSLVector Abs(const cGSLVector& theVect)
	{	ourTempVector=theVect ;
		for (register uint i = 0; i < theVect.GetSize(); i++)
			ourTempVector[i] = fabs(ourTempVector[i]);
		return(ourTempVector);
	}

	double Maxi(const cGSLVector& theVect)
	{	
		double myMaxi = theVect[0];
		for (register uint i = 1; i < theVect.GetSize(); i++)
			if (theVect[i] > myMaxi)
				myMaxi = theVect[i];
		return(myMaxi);
	}

	double Mini(const cGSLVector& theVect)
	{
		double myMini = theVect[0];
		for (register uint i = 1; i < theVect.GetSize(); i++)
			if (theVect[i] < myMini)
				myMini = theVect[i];
		return(myMini);
	}

	double* GSLVectorToDoubleStar(const cGSLVector& theVect)
	{
		return gsl_vector_ptr(theVect.GetVector(), 0);
	}

}