#pragma once 
#ifndef _CREGARCHGRADIENT_H_
#define _CREGARCHGRADIENT_H_
/*!
	\file cRegArchGradient.h
	\brief Definition of the cRegArchGradient class
	This class is used to compute the gradient of the LLH

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \class cRegArchGradient
     * \brief Data structure to store histories of gradients for empirical residuals,
     * conditional means and variances as stacks
     *
	 * Gradients at past times are stored in stacks mGradHt, mGradMt, mGradEpst
	 *
     * mGradHt[k] contains gradient at time t-k-1 where t is current time
     *
     * Gradients at current time are stored in cDVector
     *
     * Gradients are structured into three blocks: mean parameters, variance parameters
     * and residual probability density function parameters
     *
     * Stacks can be shifted using Update when moving forward into the future (t=t+1)
	 */

	class cRegArchModel ;
	class cRegArchGradient
	{
	private :
		uint		mvNPast ; ///< number of past lags
		uint		mvNParam ; ///< number of parameters
		uint		mvNMeanParam ; ///< number of conditional mean parameters
		uint		mvNVarParam ; ///< number of conditional variance parameters
		uint		mvNDistrParameter ; ///< number of conditional distribution parameters
	public :
		cDVector*	mGradMt	; ///< matrix of past gradients of conditional mean.
		cDVector*	mGradHt	; ///< matrix of past gradients of conditional variance.
		cDVector*	mGradEpst ; ///< matrix of past gradients of standardized residuals.
		cDVector	mCurrentGradVar ; ///< Current gradient of conditional variance
		cDVector	mCurrentGradSigma ; ///< Current gradient of conditional standard deviation
		cDVector	mCurrentGradMu ; ///< Current gradient of conditional mean
		cDVector	mCurrentGradDens ; ///< Currend gradient of conditional log density. 
		cDVector	mCurrentGradEps ; // Current gradient of conditional standardized residuals
		double		mCurrentDiffLogDensity; // Current derivative of the log density with respect of x
	public :
		cRegArchGradient(uint theNPast = 0, uint theNMeanParam = 0, uint theNVarParam = 0, uint theNDistrParameter = 0) ; ///< a creator
		cRegArchGradient(cRegArchModel* theParam) ; ///< a creator
		void Delete(void) ; ///< delete
		void ReAlloc(uint theNPast, uint theNMeanParam, uint theNVarParam, uint theNDistrParameter) ;
		void ReAlloc(cRegArchModel* theParam) ;
		virtual ~cRegArchGradient() ;
		void ReInitialize(void);
		uint GetNPast(void) ; ///< return the number of lags
		uint GetNParam(void) ;///< return the size of the gradient vector
		uint GetNMeanParam(void) ;///< return the number of mean parameters
		uint GetNVarParam(void) ;///< return the number of variance parameters
		uint GetNDistrParameter(void) ; ///< return the number of distribution parameters
		void Update(void) ; ///< Update gradient at current date and shift stored values of past gradients towards future (right shift of indices)
	};
}
#endif // _CREGARCHGRADIENT_H_
