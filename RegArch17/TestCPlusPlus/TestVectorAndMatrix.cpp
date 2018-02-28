//============================================================================
// Name        : Tests.cpp
// Author      : J.B. Durand & O. Taramasco
// Version     :
// Copyright   : Cecill
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
using namespace std;

#include <vector>
#include <gsl/gsl_math.h>
#include "StdAfxError.h"
#include "StdAfxVectorAndMatrix.h"
#include "StdAfxTestCPlusPlus.h"

//#include "cGSLVector.h"
//#include "cGSLMatrix.h"

using namespace ErrorNameSpace;
using namespace VectorAndMatrixNameSpace;

#ifdef WIN32
int _xtmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif //WIN32
{
	unsigned int i;

	cGSLVector vec(3), res_produit_vec(0);
	cGSLMatrix mat(2,3), res_produit_mat(0);
	std::vector<double> std_vec(0);


	for(i = 0; i < 3; i++)
		vec[i] = i+1;

	cGSLVector vec1 = vec;
	vec1.Print();


	cout << "Multiplication du vecteur :" << endl;
	vec.Print();

	for(i = 0; i < 3; i++)
	{
		(mat[0])[i] = i + 1;
		(mat[1])[i] = 3 + i + 1;
	}
	cout << "... par la matrice : " << endl;
	mat.Print();

	res_produit_mat = mat;
	res_produit_mat *= vec;
	res_produit_vec = res_produit_mat;

	cout << "Resultat : " << endl;
	res_produit_vec.Print();

	std_vec.resize(vec.GetSize());
	for(i=0; i < std_vec.size(); i++)
	{
		std_vec[i] = gsl_expm1((double)(vec)[i]);
		vec[i] = std_vec[i];
	}

	cout << "Exponentielle - 1 du vecteur initial : " <<endl;
	vec.Print();
	mat *= 2;
	cout << "nouveau produit :" << endl;
	cout << mat << endl;
	cGSLMatrix mat1 ;
	mat1 = mat + mat;
	cout << "mat1' : " << endl;
	cout << Transpose(mat1) << endl ;
	cGSLMatrix mat2(2, 2);
	mat2[0][0] = 1;
	mat2[0][1] = 2;
	mat2[1][0] = 3;
	mat2[1][1] = 4;
	cout << "mat2 :" << endl;
	cout << mat2 << endl;
	cGSLMatrix invMat2;
	invMat2 = Inv(mat2);
	cout << "invMat2 :" << endl;
	cout << invMat2 << endl;
	cout << "produit :" << endl;
	cout << mat2*invMat2 << endl;
	cout << "triple produit : " << endl;
	cout << mat2*invMat2*mat2 << endl;

	return 0;
}
