#pragma once
#ifndef _CPOLYNOME_H_
#define _CPOLYNOME_H_
#include "StdAfxRegArchLib.h"



namespace RegArchLib {
	class cPolynome
	{
	public:
		cDVector* mCoeff;
	public:
		cPolynome();
		cPolynome(uint theDegree);
		cPolynome(uint theDegree, double* theCoeff);
		cPolynome(cDVector& theCoeff);
		cPolynome(const cPolynome& theSrc);
		~cPolynome();
		void Delete();
		int GetNDegree(void) const;
		void Resize(uint theDegree);
		void Print(void);
		cPolynome& operator=(const cPolynome& theSrc);
		cPolynome& operator+(cPolynome& theP);
		cPolynome& operator-(cPolynome& theP);
		cPolynome& operator*(cPolynome& theP);
		double& operator[](int theIndex) const ;
		double BackwardPolOp(const cDVector& theYt, uint theIndex0 = 0, double thePow = 1.0) const;
		friend cPolynome& operator*(double theVal, cPolynome& theP);
		friend cPolynome& operator*(cPolynome& theP, double theVal);
	};

	extern cPolynome ourPolynom;

	extern void ComputeDeltaPowD(double theD, uint theDegree, cPolynome& theRes); // compute (1 - X)^d till theDegree
	extern void IncrPowDiv(cPolynome& theNum, cPolynome& theDen, uint theOrder, cPolynome& theQuot, cPolynome& theRest);
	extern cPolynome& TrunkMult(cPolynome& theP, cPolynome& theQ, uint theMaxDegree);
//	extern double BackwardPolOp(cPolynome& theP, const cDVector& theYt, uint theIndex0 = 0, double thePow = 1.0);

} // namespace
#endif // _POLYNOME_H_
