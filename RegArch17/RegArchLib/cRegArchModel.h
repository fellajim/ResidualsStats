#pragma once 
#ifndef _CREGARCHMODEL_
#define _CREGARCHMODEL_

#include "cCondMean.h"
#include "cAbstCondVar.h"
#include "cAbstResiduals.h"

/*!
 \file cRegArchModel.h
 \brief header for class cRegArchModel.

 \author Jean-Baptiste DURAND, Ollivier TARAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*! 
	 * \class cRegArchModel
	 * \brief  Class to implement a general RegArch model
	 */
	class cRegArchModel
	{
	public :
		cCondMean*		mMean	; ///< conditional mean model
		cAbstCondVar*	mVar	; ///< conditional variance model
		cAbstResiduals*	mResids	; ///< conditional residuals distribution model
	public :
		cRegArchModel();
		cRegArchModel(cCondMean& theMean, cAbstCondVar& theVar, cAbstResiduals& theResiduals) ; ///< A simple constructor
		cRegArchModel(const cRegArchModel& theModel) ; ///< Recopy constructor
		virtual ~cRegArchModel() ; ///< Destructor
		cRegArchModel& operator=(cRegArchModel& theRegArchModel) ; ///< = operator for cRegArchModel Class
		int GetNMean(void) ;
		void ReAllocMean(uint theNewSize) ; ///< Change the number of components for conditional mean
		void SetMean(cCondMean& theCondMean) ; ///< Change all components for conditional mean
		void AddOneMean(cAbstCondMean& theOneMean) ; ///< Add one component for conditional mean
		cAbstCondMean*  GetOneMean(int theNumMean) ;
		bool IsGoodMeanType(eCondMeanEnum theMeanEnum, int theIndex) ;
		void SetVar(cAbstCondVar& theCondVar) ; ///< Add (unique) component for conditional mean
		cAbstCondVar* GetVar(void) ;
		void SetResid(cAbstResiduals& theCondResids) ; ///< Add (unique) distribution for the distribution of residuals
		cAbstResiduals* GetResid(void) ; ///< Add (unique) distribution for the distribution of residuals
	#ifdef _RDLL_
		void PrintMean(void) ; ///< Print conditional mean model
		void PrintVar(void) ;  ///< Print conditional var model
		void PrintResiduals(void) ; ///< Print conditional residuals distribution model
		void Print(void); ///< Print the whole model
	#else
		void PrintMean(ostream& theOut=cout) const; ///< Print conditional mean model
		void PrintVar(ostream& theOut=cout) const;  ///< Print conditional var model
		void PrintResiduals(ostream& theOut=cout) const; ///< Print conditional residuals distribution model
		void Print(ostream& theOut=cout) const; ///< Print the whole model
	#endif //_RDLL
		void SetDefaultInitPoint(cRegArchValue& theValue) ;
		uint GetNParam(void) const ; ///< Return the total number of model parameters
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time t.
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData) ;
		void RegArchParamToVector(cDVector& theDestVect) const ;
		void VectorToRegArchParam(const cDVector& theSrcParam) ;
		void GetParamName(char**& theName);
	} ;
}
#endif // _CREGARCHMODEL_
