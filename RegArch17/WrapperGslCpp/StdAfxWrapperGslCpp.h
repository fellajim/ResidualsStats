// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#ifdef _MSC_VER // Compilateur MS Visual Studio
	#pragma warning(disable: 4996)
#endif

//#ifndef _RDLL_
	#include <gsl/gsl_multimin.h>
	#include "StdAfxError.h"
	#include "StdAfxVectorAndMatrix.h"
	#include "cGSLMultiMin.h"
	#include <ctime>

//#endif // _RDLL_

#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s



// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
