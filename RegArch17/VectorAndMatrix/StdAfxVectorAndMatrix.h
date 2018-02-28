// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
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

#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés



// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
