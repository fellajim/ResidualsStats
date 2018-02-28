#pragma once 
#ifndef _CREGARCHHESSIEN_H_
#define _CREGARCHHESSIEN_H_


namespace RegArchLib {
	/*!
	 * \class cRegArchHessien
	 * \brief This class is used to store the Hessien matrix
	 */

	class cRegArchModel ;
	class cRegArchHessien
	{
	private :
		uint		mvNPast ; ///< number of past lags
		uint		mvNParam ; ///< number of parameters
		uint		mvNMeanParam ; ///< number of conditional mean parameters
		uint		mvNVarParam ; ///< number of conditional variance parameters
		uint		mvNDistrParameter ; ///< number of conditional distribution parameters
	public :
		cDMatrix*	mHessMt	; ///< matrix of past hessien of conditional mean.
		cDMatrix*	mHessHt	; ///< matrix of past hessien of conditional variance.
		cDMatrix*	mHessEpst ; ///< matrix of past hessien of standardized residuals.
		cDMatrix	mCurrentHessVar ; ///< Current hessien of conditional variance
		cDMatrix	mCurrentHessSigma ; ///< Current hessien of conditional standard deviation
		cDMatrix	mCurrentHessMu ; ///< Current hessien of conditional mean
		cDMatrix	mCurrentHessDens ; ///< Currend hessien of conditional log density.
		cDMatrix	mCurrentHessEps ; // Current hessien of conditional standardized residuals
		cDVector    mCurrentGradDiffLogDensity; // Current gradient with respect of theta of the derivative of the log density with respect of x
	public :
		cRegArchHessien(uint theNPast = 0, uint theNMeanParam = 0, uint theNVarParam = 0, uint theNDistrParameter = 0) ; ///< a creator
		cRegArchHessien(cRegArchModel* theParam) ; ///< a creator
		void Delete(void) ; ///< delete
		void ReAlloc(uint theNPast, uint theNMeanParam, uint theNVarParam, uint theNDistrParameter) ;
		void ReAlloc(cRegArchModel* theParam) ;
		virtual ~cRegArchHessien() ;
		uint GetNPast(void) ; ///< return the number of lags
		uint GetNParam(void) ;///< return the size of the gradient vector
		uint GetNMeanParam(void) ;///< return the number of mean parameters
		uint GetNVarParam(void) ;///< return the number of variance parameters
		uint GetNDistrParameter(void) ; ///< return the number of distribution parameters
		void ComputeHessSigmatFromHessVart(uint theDate, const cRegArchValue& theData, const cRegArchGradient& theGradData);
		void Update(void) ; ///< Update gradient at current date and shift stored values of past gradients towards future (right shift of indices)
	};
}
#endif // _CREGARCHHESSIEN_H_
