// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#ifdef _MSC_VER // Compilateur MS VC
	#include "TargetVerVectorAndMatrix.h"
#endif // _MSC_VER_

#ifdef _CLI_
	#ifndef _DLLEXPORT_
		#define  __declspec(dllexport)
	#endif // _DLLEXPORT_
#else
	#define _DLLEXPORT_
#endif // _CLI_

#include "StdAfxError.h"
#include "cGSLVector.h"
#include "cGSLMatrix.h"
//#ifdef _RDLL_
//	#undef ftrunc
//	#include "StdAfxGslEquiv.h"
//	using namespace GslEquivNameSpace ;
//#endif // _RDLL_

#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s



// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
