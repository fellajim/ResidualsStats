#include "StdAfxRegArchLib.h"

namespace RegArchLib {
	cPolynome ourPolynom;

	cPolynome::cPolynome()
	{
		mCoeff = NULL;
	}

	cPolynome::cPolynome(uint theDegree)
	{
		mCoeff = new cDVector(theDegree+1);
	}

	cPolynome::cPolynome(uint theDegree, double* theCoeff)
	{
		mCoeff = new cDVector(theDegree + 1, theCoeff);
	}

	cPolynome::cPolynome(cDVector& theCoeff)
	{
		mCoeff = new cDVector(theCoeff);
	}
		
	cPolynome::cPolynome(const cPolynome &theSrc)
	{
		mCoeff = new cDVector(*(theSrc.mCoeff));
	}

	cPolynome::~cPolynome()
	{
		if (mCoeff != NULL)
		{
			delete mCoeff;
			mCoeff = NULL;
		}
	}

	void cPolynome::Delete(void)
	{
		if (mCoeff != NULL)
		{
			delete mCoeff;
			mCoeff = NULL;
		}
	}

	void cPolynome::Resize(uint theDegree)
	{
		if (mCoeff == NULL)
			mCoeff = new cDVector(theDegree + 1);
		else
			mCoeff->ReAlloc(theDegree + 1);
	}

	int cPolynome::GetNDegree(void) const
	{
		if (mCoeff == NULL)
			return -1;
		else
			return mCoeff->GetSize() - 1;
	}

	double& cPolynome::operator[](int theIndex) const
	{
		return (*mCoeff)[theIndex];
	}

	cPolynome& cPolynome::operator=(const cPolynome& theSrc)
	{
		mCoeff = new cDVector(*(theSrc.mCoeff));
		return *this;
	}

	cPolynome& cPolynome::operator+(cPolynome& theP)
	{
	uint p1 = GetNDegree();
	uint p2 = theP.GetNDegree();
	uint p = MAX(p1, p2);
		ourPolynom.Resize(p);
		for (uint i = 0; i <= p; i++)
		{
			if (i <= p1)
				(*ourPolynom.mCoeff)[i] = (*mCoeff)[i];
			if (i <= p2)
				(*ourPolynom.mCoeff)[i] += (*theP.mCoeff)[i];
		}
		return ourPolynom;
	}

	cPolynome& cPolynome::operator-(cPolynome& theP)
	{
	uint p1 = GetNDegree();
	uint p2 = theP.GetNDegree();
	uint p = MAX(p1, p2);
		ourPolynom.Resize(p);
		for (uint i = 0; i <= p; i++)
		{
			if (i <= p1)
				(*ourPolynom.mCoeff)[i] = (*mCoeff)[i];
			if (i <= p2)
				(*ourPolynom.mCoeff)[i] -= (*theP.mCoeff)[i];
		}
		return ourPolynom;
	}

	cPolynome& cPolynome::operator*(cPolynome& theP)
	{
	uint p1 = GetNDegree();
	uint p2 = theP.GetNDegree();
		uint p = p1 + p2;
		ourPolynom.Resize(p);
		for (uint i = 0; i <= p1; i++)
			for (uint j = 0; j <= p2; j++)
				ourPolynom[i + j] += (*mCoeff)[i] * (*theP.mCoeff)[j];

		return ourPolynom;
	}

#ifndef _RDLL_
	void cPolynome::Print(void)
	{
	uint myDegree = GetNDegree();
		if ((*mCoeff)[myDegree] != 0)
			cout << (*mCoeff)[myDegree] << "*x^" << myDegree;
		for (uint i = myDegree - 1; i >= 1; i--)
		{
			if ((*mCoeff)[i] > 0)
				cout << "+" << (*mCoeff)[i] << "*x^" << i;
			else
				if ((*mCoeff)[i] < 0)
					cout << (*mCoeff)[i] << "*x^" << i;
		}
		if ((*mCoeff)[0] > 0)
			cout << "+" << (*mCoeff)[0] << endl;
		else
			if ((*mCoeff)[0] < 0)
				cout << (*mCoeff)[0] << endl;
			else 
				cout << endl;
	}
#else
	void cPolynome::Print(void)
	{
		uint myDegree = GetNDegree();
		if ((*mCoeff)[myDegree] != 0)
			Rprintf("%f*x^%d", (*mCoeff)[myDegree],myDegree);
		for (uint i = myDegree - 1; i >= 1; i--)
		{
			if ((*mCoeff)[i] > 0)
				Rprintf("+%f*x^%d", (*mCoeff)[i], i);
			else
				if ((*mCoeff)[i] < 0)
					Rprintf("%f*x^%d", (*mCoeff)[i], i);
		}
		if ((*mCoeff)[0] > 0)
			Rprintf("+%f\n", (*mCoeff)[0]);
		else
			if ((*mCoeff)[0] < 0)
				Rprintf("%f\n", (*mCoeff)[0]);
			else 
				Rprintf("\n");
	}
#endif //_RDLL_

	cPolynome& operator *(cPolynome& theP, double theVal)
	{
	uint p = theP.GetNDegree();
		ourPolynom.Resize(p);

		for (uint i = 0; i <= p; i++)
			(*ourPolynom.mCoeff)[i] = (*theP.mCoeff)[i] * theVal;
		return ourPolynom;
	}

	cPolynome& operator *(double theVal, cPolynome& theP)
	{
	uint p = theP.GetNDegree();
		ourPolynom.Resize(p);

		for (uint i = 0; i <= p ; i++)
			(*ourPolynom.mCoeff)[i] = (*theP.mCoeff)[i] * theVal;
		return ourPolynom;
	}

	void ComputeDeltaPowD(double theD, uint theDegree, cPolynome& theRes)
	{
		theRes.Resize(theDegree);
		theRes[0] = 1.0;
		for (uint i = 1; i <= theDegree; i++)
			theRes[i] = -theRes[i - 1] * (theD - (double)(i - 1)) / (double)i;

	}

	void IncrPowDiv(cPolynome& theNum, cPolynome& theDen, uint theOrder, cPolynome& theQuot, cPolynome& theRest)
	{
		//Division of two polynomials by increasing powers

		uint p1 = theNum.GetNDegree();

		theQuot.Resize(theOrder);
		theRest.Resize(theOrder + p1);
		cPolynome myAux(theOrder + p1);
		cPolynome myMonome(theOrder + p1);

		for (uint i = 0; i <= p1; i++)
			theRest[i] = theNum[i];
		uint j = 0;
		while (j <= theOrder)
		{
			uint k = j;
			while (theRest[k] == 0)
				k++;
			(*myMonome.mCoeff)[k] = (*theQuot.mCoeff)[k] = theRest[k] / theDen[0];
			myAux = myMonome * theDen;
			(*myMonome.mCoeff)[k] = 0;
			theRest = theRest - myAux;
			j++;
		}
		myAux.Delete();
		myMonome.Delete();
	}


	cPolynome& TrunkMult(cPolynome& theP, cPolynome& theQ, uint theMaxDegree)
	{
		ourPolynom.Resize(theMaxDegree);
		for (register int i = 0; i <= theP.GetNDegree(); i++)
			for (register int j = 0; j <= theQ.GetNDegree(); j++)
				if (i + j <= (int)theMaxDegree)
					(*ourPolynom.mCoeff)[i + j] += (*theP.mCoeff)[i] * (*theQ.mCoeff)[j];
		return ourPolynom;
	}

	double cPolynome::BackwardPolOp(const cDVector& theYt, uint theIndex0, double thePow) const
	{
	int myDegree = GetNDegree();
	double myRes = (*this)[0];
		if (thePow == 1.0)
		{
			for (register int i = 1; i <= MIN(myDegree, (int)theIndex0); i++)
				myRes += (*this)[i] * theYt[(int)theIndex0 - i];
		}
		else
		{
			for (register int i = 1; i <= MIN(myDegree, (int)theIndex0); i++)
				myRes += (*this)[i] * pow(theYt[(int)theIndex0 - i], thePow);
		}

		return myRes;
	}
} //namespace
